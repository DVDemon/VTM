import type { MachineBody, Project } from '../model/types';
import { isComplexMachine } from '../model/types';
import { getRootBody } from '../model/factory';
import { transitionConditionSigns } from '../model/transitionLabel';

export interface CompilerError {
  readonly bodyId: string;
  readonly machineId?: string;
  readonly message: string;
}

function checkBody(body: MachineBody, alphabet: string): CompilerError[] {
  const errors: CompilerError[] = [];
  const starts = body.machines.filter((m) => m.type === 'start');
  const finishes = body.machines.filter((m) => m.type === 'finish');

  if (starts.length === 0) {
    errors.push({
      bodyId: body.id,
      message: `No start machine found in ${body.name}`,
    });
  }
  if (starts.length > 1) {
    errors.push({
      bodyId: body.id,
      message: `Only one start machine allowed in ${body.name}, found ${starts.length}`,
    });
  }
  if (finishes.length === 0) {
    errors.push({
      bodyId: body.id,
      message: `No finish machine found in ${body.name}`,
    });
  }

  for (const m of body.machines) {
    const incoming = body.transitions.filter((t) => t.finishMachineId === m.id);
    const outgoing = body.transitions.filter((t) => t.startMachineId === m.id);

    if (m.type === 'start' && incoming.length > 0) {
      errors.push({
        bodyId: body.id,
        machineId: m.id,
        message: `Start machine must not have incoming transitions in ${body.name}`,
      });
    } else if (m.type !== 'start' && incoming.length === 0) {
      errors.push({
        bodyId: body.id,
        machineId: m.id,
        message: `No incoming transition for ${m.type} in ${body.name}`,
      });
    }

    if (m.type === 'finish' && outgoing.length > 0) {
      errors.push({
        bodyId: body.id,
        machineId: m.id,
        message: `Finish machine must not have outgoing transitions in ${body.name}`,
      });
    } else if (m.type !== 'finish' && outgoing.length === 0) {
      errors.push({
        bodyId: body.id,
        machineId: m.id,
        message: `No outgoing transition for ${m.type} in ${body.name}`,
      });
    }

    if (m.type !== 'finish') {
      let uncovered = alphabet;
      const seen = new Set<string>();
      for (const t of outgoing) {
        const covered = transitionConditionSigns(alphabet, t.conditions);
        for (const ch of covered) {
          if (seen.has(ch)) {
            errors.push({
              bodyId: body.id,
              machineId: m.id,
              message: `Duplicate transition for sign '${ch}' in ${body.name}`,
            });
          }
          seen.add(ch);
          uncovered = uncovered.split('').filter((c) => c !== ch).join('');
        }
      }
      if (uncovered.length > 0) {
        errors.push({
          bodyId: body.id,
          machineId: m.id,
          message: `No outgoing transition for signs '${uncovered}' in ${body.name}`,
        });
      }
    }
  }

  return errors;
}

export function runCompilerChecks(project: Project): CompilerError[] {
  const errors: CompilerError[] = [];
  const visited = new Set<string>();

  function walk(body: MachineBody): void {
    if (visited.has(body.id)) return;
    visited.add(body.id);
    errors.push(...checkBody(body, body.alphabet || project.alphabet));
    for (const m of body.machines) {
      if (isComplexMachine(m)) {
        const inner = project.bodies.find((b) => b.id === m.innerId);
        if (inner) walk(inner);
      }
    }
  }

  walk(getRootBody(project));
  return errors;
}
