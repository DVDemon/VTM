import type { Machine, MachineBody, Project } from '../model/types';
import { getRootBody } from '../model/factory';
import { isComplexMachine, isWriteMachine } from '../model/types';
import { transitionConditionSigns } from '../model/transitionLabel';

export interface ExportFourthOptions {
  readonly delimiter: string;
  readonly left: string;
  readonly right: string;
  readonly stop: string;
  readonly quotes: string;
}

export const DEFAULT_EXPORT_FOURTH_OPTIONS: ExportFourthOptions = {
  delimiter: ',',
  left: '<',
  right: '>',
  stop: '#',
  quotes: '',
};

export interface ExportFourthResult {
  readonly lines: readonly string[];
  readonly text: string;
  readonly information: string;
  readonly error: boolean;
  readonly errorMessage: string;
}

type MoveCommand = 'left' | 'right' | 'stop';

interface ExportContext {
  readonly options: ExportFourthOptions;
  readonly lines: string[];
  readonly information: string[];
  error: boolean;
  errorMessage: string;
  readonly stateIndex: Map<string, number>;
}

function getFullName(prefix: string, machineId: string): string {
  return prefix ? `${prefix}:${machineId}` : machineId;
}

function formatMoveRow(
  options: ExportFourthOptions,
  state: number,
  ch: string,
  cmd: MoveCommand,
  state2: number,
): string {
  const cmdStr =
    cmd === 'left' ? options.left : cmd === 'right' ? options.right : options.stop;
  const q = options.quotes;
  return `${state}${options.delimiter}${q}${ch}${q}${options.delimiter}${cmdStr}${options.delimiter}${state2}`;
}

function formatWriteRow(
  options: ExportFourthOptions,
  state: number,
  ch: string,
  write: string,
  state2: number,
): string {
  const q = options.quotes;
  return `${state}${options.delimiter}${q}${ch}${q}${options.delimiter}${q}${write}${q}${options.delimiter}${state2}`;
}

function formatIdentityRow(
  options: ExportFourthOptions,
  state: number,
  ch: string,
  state2: number,
): string {
  const q = options.quotes;
  return `${state}${options.delimiter}${q}${ch}${q}${options.delimiter}${q}${ch}${q}${options.delimiter}${state2}`;
}

function checkRecursiveComplex(
  project: Project,
  body: MachineBody,
  stack: Set<string>,
): string | null {
  for (const m of body.machines) {
    if (!isComplexMachine(m)) continue;
    if (stack.has(m.name)) {
      return `Recursive machine call to ${m.name} detected in ${body.name}`;
    }
    stack.add(m.name);
    const inner = project.bodies.find((b) => b.id === m.innerId);
    if (inner) {
      const nested = checkRecursiveComplex(project, inner, stack);
      if (nested) return nested;
    }
    stack.delete(m.name);
  }
  return null;
}

function mapState(
  ctx: ExportContext,
  indexRef: { index: number },
  prefix: string,
  body: MachineBody,
  project: Project,
): void {
  for (const m of body.machines) {
    const fullName = getFullName(prefix, m.id);
    ctx.stateIndex.set(fullName, indexRef.index++);
    if (isComplexMachine(m)) {
      const inner = project.bodies.find((b) => b.id === m.innerId);
      if (inner) {
        mapState(ctx, indexRef, getFullName(prefix, m.id), inner, project);
      }
    }
  }
}

function getIncoming(body: MachineBody, machineId: string) {
  return body.transitions.filter((t) => t.finishMachineId === machineId);
}

function getOutgoing(body: MachineBody, machineId: string) {
  return body.transitions.filter((t) => t.startMachineId === machineId);
}

function resolveIndexStart(
  ctx: ExportContext,
  parentIndex: number,
  prefix: string,
  fromMachine: Machine,
  project: Project,
): number | undefined {
  let indexStart = ctx.stateIndex.get(getFullName(prefix, fromMachine.id));
  if (indexStart === undefined) return undefined;

  switch (fromMachine.type) {
    case 'start':
      if (parentIndex) indexStart = parentIndex;
      break;
    case 'complex':
      if (isComplexMachine(fromMachine)) {
        const inner = project.bodies.find((b) => b.id === fromMachine.innerId);
        if (inner) {
          const finish = inner.machines.find((m) => m.type === 'finish');
          if (finish) {
            const complexPrefix = getFullName(prefix, fromMachine.id);
            indexStart =
              ctx.stateIndex.get(getFullName(complexPrefix, finish.id)) ??
              indexStart;
          }
        }
      }
      break;
    case 'leftWord':
    case 'rightWord':
    case 'copy':
      break;
    default:
      break;
  }
  return indexStart;
}

function emitForTransition(
  ctx: ExportContext,
  parentIndex: number,
  indexStart: number,
  index: number,
  condition: string,
  current: Machine,
  alphabet: string,
): void {
  const { options } = ctx;

  if (current.type === 'start') {
    for (const sym of alphabet) {
      ctx.lines.push(formatIdentityRow(options, indexStart, sym, index));
    }
    return;
  }

  for (const ch of condition) {
    switch (current.type) {
      case 'left':
        ctx.lines.push(formatMoveRow(options, indexStart, ch, 'left', index));
        break;
      case 'right':
        ctx.lines.push(formatMoveRow(options, indexStart, ch, 'right', index));
        break;
      case 'leftWord':
      case 'rightWord':
      case 'copy':
      case 'complex':
        ctx.lines.push(formatIdentityRow(options, indexStart, ch, index));
        break;
      case 'write':
        if (isWriteMachine(current)) {
          ctx.lines.push(
            formatWriteRow(options, indexStart, ch, current.sign, index),
          );
        }
        break;
      case 'finish':
        if (parentIndex) {
          ctx.lines.push(formatIdentityRow(options, indexStart, ch, index));
        } else {
          ctx.lines.push(formatMoveRow(options, indexStart, ch, 'stop', index));
        }
        break;
      default:
        break;
    }
  }
}

function process(
  ctx: ExportContext,
  parentIndex: number,
  prefix: string,
  body: MachineBody,
  callStack: string[],
  current: Machine,
  visited: Set<string>,
  project: Project,
): void {
  if (visited.has(current.id)) return;
  visited.add(current.id);

  const fullName = getFullName(prefix, current.id);
  const index = ctx.stateIndex.get(fullName);
  if (index === undefined) return;

  const alphabet = body.alphabet;

  for (const t of getIncoming(body, current.id)) {
    const from = body.machines.find((m) => m.id === t.startMachineId);
    if (!from) continue;
    const indexStart = resolveIndexStart(
      ctx,
      parentIndex,
      prefix,
      from,
      project,
    );
    if (indexStart === undefined) continue;
    const condition = transitionConditionSigns(alphabet, t.conditions);
    emitForTransition(
      ctx,
      parentIndex,
      indexStart,
      index,
      condition,
      current,
      alphabet,
    );
  }

  if (isComplexMachine(current)) {
    const inner = project.bodies.find((b) => b.id === current.innerId);
    if (inner) {
      processComplex(ctx, index, prefix, current, inner, callStack, project);
    }
  }

  for (const t of getOutgoing(body, current.id)) {
    const next = body.machines.find((m) => m.id === t.finishMachineId);
    if (next && !visited.has(next.id)) {
      process(ctx, parentIndex, prefix, body, callStack, next, visited, project);
    }
  }
}

function processComplex(
  ctx: ExportContext,
  parentIndex: number,
  prefix: string,
  complexMachine: Machine | null,
  body: MachineBody,
  callStack: string[],
  project: Project,
): void {
  const start = body.machines.find((m) => m.type === 'start');
  if (!start) return;

  const complexKey = complexMachine?.id ?? body.id;
  callStack.push(complexKey);

  const complexPrefix = complexMachine
    ? getFullName(prefix, complexMachine.id)
    : prefix;

  process(
    ctx,
    parentIndex,
    complexPrefix,
    body,
    callStack,
    start,
    new Set(),
    project,
  );

  if (callStack.length === 1) {
    const finish = body.machines.find((m) => m.type === 'finish');
    if (finish) {
      const finishIndex = ctx.stateIndex.get(
        getFullName(complexPrefix, finish.id),
      );
      if (finishIndex !== undefined) {
        for (const ch of body.alphabet) {
          ctx.lines.push(
            formatIdentityRow(
              ctx.options,
              finishIndex,
              ch,
              finishIndex,
            ),
          );
        }
      }
    }
  }

  callStack.pop();
}

function runExport(body: MachineBody, project: Project, options: ExportFourthOptions): ExportFourthResult {
  const ctx: ExportContext = {
    options,
    lines: [],
    information: ['Checking for recursive calls ...\n'],
    error: false,
    errorMessage: '',
    stateIndex: new Map(),
  };

  const recursion = checkRecursiveComplex(project, body, new Set());
  if (recursion) {
    ctx.error = true;
    ctx.errorMessage = recursion;
    ctx.information.push("Export doesn't support recursive calls\n");
    return buildResult(ctx);
  }

  const indexRef = { index: 0 };
  mapState(ctx, indexRef, '', body, project);
  processComplex(ctx, 0, '', null, body, [], project);
  ctx.information.push('Export complete\n');

  return buildResult(ctx);
}

function buildResult(ctx: ExportContext): ExportFourthResult {
  return {
    lines: ctx.lines,
    text: ctx.lines.join('\n'),
    information: ctx.information.join(''),
    error: ctx.error,
    errorMessage: ctx.errorMessage,
  };
}

/** Порт `VMTExport4th` для корневой (текущей) машины проекта. */
export function exportFourth(
  project: Project,
  options: Partial<ExportFourthOptions> = {},
  bodyId?: string,
): ExportFourthResult {
  const body = bodyId
    ? project.bodies.find((b) => b.id === bodyId) ?? getRootBody(project)
    : getRootBody(project);
  const merged: ExportFourthOptions = {
    ...DEFAULT_EXPORT_FOURTH_OPTIONS,
    ...options,
  };
  return runExport(body, project, merged);
}
