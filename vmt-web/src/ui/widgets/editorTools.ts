import type { MachineType } from '@core/model/types';

/** Режимы панели инструментов редактора (порт tool_* из formeditormachine.ui). */
export type EditorTool =
  | 'pointer'
  | 'link'
  | 'pan'
  | 'delete'
  | 'start'
  | 'finish'
  | 'left'
  | 'right'
  | 'leftWord'
  | 'rightWord'
  | 'write'
  | 'copy'
  | 'complex';

export interface EditorToolDef {
  readonly id: EditorTool;
  readonly label: string;
  readonly title: string;
  readonly machineType?: MachineType;
}

export const EDITOR_TOOL_GROUPS: readonly {
  readonly title: string;
  readonly tools: readonly EditorToolDef[];
}[] = [
  {
    title: 'Selection',
    tools: [
      { id: 'pointer', label: '↖', title: 'Select / move' },
      { id: 'link', label: '⟷', title: 'Create link' },
      { id: 'pan', label: '✥', title: 'Pan canvas' },
      { id: 'delete', label: '🗑', title: 'Delete selection' },
    ],
  },
  {
    title: 'Machines',
    tools: [
      { id: 'start', label: 'S', title: 'Start', machineType: 'start' },
      { id: 'finish', label: 'F', title: 'Finish', machineType: 'finish' },
      { id: 'left', label: '←', title: 'Move left', machineType: 'left' },
      { id: 'right', label: '→', title: 'Move right', machineType: 'right' },
      {
        id: 'leftWord',
        label: '⇐',
        title: 'Move left (word)',
        machineType: 'leftWord',
      },
      {
        id: 'rightWord',
        label: '⇒',
        title: 'Move right (word)',
        machineType: 'rightWord',
      },
      { id: 'write', label: 'λ', title: 'Write symbol', machineType: 'write' },
      { id: 'copy', label: 'C', title: 'Copy', machineType: 'copy' },
      { id: 'complex', label: '⊞', title: 'Complex machine', machineType: 'complex' },
    ],
  },
];

export function isPlacementTool(tool: EditorTool): tool is EditorTool & {
  machineType: MachineType;
} {
  const def = EDITOR_TOOL_GROUPS.flatMap((g) => g.tools).find((t) => t.id === tool);
  return def?.machineType !== undefined;
}

export function machineTypeForTool(tool: EditorTool): MachineType | null {
  const def = EDITOR_TOOL_GROUPS.flatMap((g) => g.tools).find((t) => t.id === tool);
  return def?.machineType ?? null;
}
