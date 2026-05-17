import type { Machine, MachineBody, Project } from '../model/types';
import { getRootBody } from '../model/factory';
import { isComplexMachine, isWriteMachine } from '../model/types';

function escapeLabel(label: string): string {
  return label
    .replace(/\\/g, '\\\\')
    .replace(/"/g, '\\"')
    .replace(/\n/g, ' ')
    .replace(/\r/g, ' ');
}

function machineLabel(m: Machine): string {
  if (isWriteMachine(m)) {
    return `λ(${m.sign})`;
  }
  if (isComplexMachine(m)) {
    return m.name;
  }
  return m.type;
}

function sanitizeAlias(raw: string, usage: Map<string, number>): string {
  let alias = raw.replace(/[^A-Za-z0-9_]/g, '_').replace(/_+/g, '_');
  if (!alias || /^[0-9]/.test(alias)) alias = `s_${alias}`;
  const n = (usage.get(alias) ?? 0) + 1;
  usage.set(alias, n);
  return n > 1 ? `${alias}_${n}` : alias;
}

function exportBody(
  body: MachineBody,
  project: Project,
  indent: string,
  lines: string[],
  aliases: Map<string, string>,
  usage: Map<string, number>,
): void {
  for (const m of body.machines) {
    if (m.type === 'start' || m.type === 'finish') continue;
    const alias = sanitizeAlias(machineLabel(m), usage);
    aliases.set(m.id, alias);
    if (isComplexMachine(m)) {
      const inner = project.bodies.find((b) => b.id === m.innerId);
      lines.push(
        `${indent}state "${escapeLabel(machineLabel(m))}" as ${alias} {`,
      );
      if (inner) {
        exportBody(inner, project, indent + '  ', lines, aliases, usage);
      }
      lines.push(`${indent}}`);
    } else {
      lines.push(
        `${indent}state "${escapeLabel(machineLabel(m))}" as ${alias}`,
      );
    }
  }

  if (body.transitions.length > 0) lines.push('');
  for (const t of body.transitions) {
    const from =
      body.machines.find((m) => m.id === t.startMachineId)?.type === 'start' ||
      body.machines.find((m) => m.id === t.startMachineId)?.type === 'finish'
        ? '[*]'
        : aliases.get(t.startMachineId);
    const toMachine = body.machines.find((m) => m.id === t.finishMachineId);
    const to =
      toMachine?.type === 'start' || toMachine?.type === 'finish'
        ? '[*]'
        : aliases.get(t.finishMachineId);
    if (!from || !to) continue;
    const enabled = t.conditions.enabledSigns;
    const label =
      enabled.length === body.alphabet.length
        ? '*'
        : enabled.length === 0
          ? 'ε'
          : enabled.join('');
    lines.push(`${indent}${from} --> ${to} : ${escapeLabel(label)}`);
  }
}

export function exportPlantUml(project: Project): string {
  const root = getRootBody(project);
  const lines: string[] = [
    '@startuml',
    '!theme plain',
    'hide empty description',
    'skinparam shadowing false',
    '',
    `title ${escapeLabel(root.name)}`,
    '',
  ];
  const aliases = new Map<string, string>();
  const usage = new Map<string, number>();
  exportBody(root, project, '', lines, aliases, usage);
  lines.push('@enduml', '');
  return lines.join('\n');
}
