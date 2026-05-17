export const SCHEMA_VERSION = 1 as const;

export type MachineType =
  | 'start'
  | 'finish'
  | 'left'
  | 'right'
  | 'leftWord'
  | 'rightWord'
  | 'write'
  | 'copy'
  | 'complex';

export interface Point {
  readonly x: number;
  readonly y: number;
}

export interface TransitionConditions {
  readonly enabledSigns: readonly string[];
}

export interface Transition {
  readonly id: string;
  readonly startMachineId: string;
  readonly finishMachineId: string;
  readonly routingPoints: readonly Point[];
  readonly conditions: TransitionConditions;
  /** Позиция подписи вдоль коннектора 0…1 (сохраняется при перестроении маршрута). */
  readonly conditionsLabelT?: number;
  /** @deprecated мигрируется в conditionsLabelT */
  readonly conditionsAnchor?: Point;
}

export interface MachineBase {
  readonly id: string;
  readonly type: MachineType;
  readonly center: Point;
  readonly size: Point;
  readonly power: number;
  readonly parentBodyId: string | null;
}

export interface WriteMachine extends MachineBase {
  readonly type: 'write';
  readonly sign: string;
}

export interface ComplexMachine extends MachineBase {
  readonly type: 'complex';
  readonly name: string;
  readonly innerId: string;
}

export type Machine = MachineBase | WriteMachine | ComplexMachine;

export interface MachineBody {
  readonly id: string;
  readonly name: string;
  readonly alphabet: string;
  readonly machines: readonly Machine[];
  readonly transitions: readonly Transition[];
}

export interface Project {
  readonly schemaVersion: typeof SCHEMA_VERSION;
  readonly id: string;
  readonly name: string;
  readonly alphabet: string;
  readonly bodies: readonly MachineBody[];
  readonly rootBodyIds: readonly string[];
  readonly createdAt: string;
  readonly updatedAt: string;
}

export function isWriteMachine(m: Machine): m is WriteMachine {
  return m.type === 'write';
}

export function isComplexMachine(m: Machine): m is ComplexMachine {
  return m.type === 'complex';
}

export function getLambda(alphabet: string): string {
  return alphabet.length > 0 ? alphabet[0]! : ' ';
}
