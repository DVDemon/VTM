import type {
  Machine,
  MachineBody,
  MachineType,
  Point,
  Project,
  Transition,
  TransitionConditions,
} from './types';
import { SCHEMA_VERSION } from './types';
import { isValidAlphabet, normalizeAlphabet } from './alphabet';
import { defaultTransitionConditions } from './transitionLabel';

function id(): string {
  return crypto.randomUUID();
}

export function createEmptyBody(name: string, alphabet = ' '): MachineBody {
  const bodyId = id();
  const startId = id();
  const finishId = id();
  return {
    id: bodyId,
    name,
    alphabet,
    machines: [
      {
        id: startId,
        type: 'start',
        center: { x: 200, y: 200 },
        size: { x: 48, y: 48 },
        power: 1,
        parentBodyId: bodyId,
      },
      {
        id: finishId,
        type: 'finish',
        center: { x: 400, y: 200 },
        size: { x: 48, y: 48 },
        power: 1,
        parentBodyId: bodyId,
      },
    ],
    transitions: [],
  };
}

export function createEmptyProject(name = 'New Project', alphabet = ' 01'): Project {
  return createProject(name, alphabet, name);
}

/** Создание проекта с обязательными именем и алфавитом (≥2 символа, λ первым). */
export function createProject(
  projectName: string,
  rawAlphabet: string,
  rootMachineName?: string,
): Project {
  const name = projectName.trim();
  const machineName = (rootMachineName ?? projectName).trim();
  if (!name) throw new Error('Project name is required');
  if (!machineName) throw new Error('Machine name is required');
  const alphabet = normalizeAlphabet(rawAlphabet);
  if (!isValidAlphabet(alphabet)) {
    throw new Error('Alphabet must contain λ and at least one more symbol');
  }
  const now = new Date().toISOString();
  const body = createEmptyBody(machineName, alphabet);
  return {
    schemaVersion: SCHEMA_VERSION,
    id: id(),
    name,
    alphabet,
    bodies: [body],
    rootBodyIds: [body.id],
    createdAt: now,
    updatedAt: now,
  };
}

export function touchProject(project: Project): Project {
  return { ...project, updatedAt: new Date().toISOString() };
}

export function getRootBody(project: Project): MachineBody {
  const rootId = project.rootBodyIds[0];
  const body = project.bodies.find((b) => b.id === rootId);
  if (!body) {
    throw new Error('Project has no root machine body');
  }
  return body;
}

export function findBody(project: Project, bodyId: string): MachineBody | undefined {
  return project.bodies.find((b) => b.id === bodyId);
}

export function findMachine(body: MachineBody, machineId: string) {
  return body.machines.find((m) => m.id === machineId);
}

export function updateBody(project: Project, body: MachineBody): Project {
  return touchProject({
    ...project,
    bodies: project.bodies.map((b) => (b.id === body.id ? body : b)),
  });
}

export function moveMachine(
  project: Project,
  bodyId: string,
  machineId: string,
  center: Point,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  return updateBody(project, {
    ...body,
    machines: body.machines.map((m) =>
      m.id === machineId ? { ...m, center } : m,
    ),
  });
}

function defaultMachineSize(type: MachineType): Point {
  return type === 'complex' ? { x: 56, y: 56 } : { x: 48, y: 48 };
}

function createMachineNode(
  bodyId: string,
  type: MachineType,
  center: Point,
  alphabet: string,
  innerId?: string,
  complexName?: string,
): Machine {
  const base = {
    id: id(),
    center,
    size: defaultMachineSize(type),
    power: 1,
    parentBodyId: bodyId,
  };
  if (type === 'write') {
    const sign = alphabet.length > 1 ? alphabet[1]! : '1';
    return { ...base, type: 'write', sign };
  }
  if (type === 'complex') {
    return {
      ...base,
      type: 'complex',
      name: complexName ?? 'Machine',
      innerId: innerId ?? id(),
    };
  }
  return { ...base, type };
}

export function addMachine(
  project: Project,
  bodyId: string,
  type: MachineType,
  center: Point,
  options?: { complexName?: string },
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;

  const alphabet = body.alphabet || project.alphabet;
  let bodies = project.bodies;
  let innerId: string | undefined;
  let complexName: string | undefined;

  if (type === 'complex') {
    complexName = options?.complexName?.trim();
    if (!complexName) {
      throw new Error('Complex machine name is required');
    }
    const inner = createEmptyBody(complexName, alphabet);
    innerId = inner.id;
    bodies = [...bodies, inner];
  }

  const machine = createMachineNode(
    bodyId,
    type,
    center,
    alphabet,
    innerId,
    complexName,
  );
  return updateBody(
    { ...project, bodies },
    { ...body, machines: [...body.machines, machine] },
  );
}

export function removeMachine(
  project: Project,
  bodyId: string,
  machineId: string,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  return updateBody(project, {
    ...body,
    machines: body.machines.filter((m) => m.id !== machineId),
    transitions: body.transitions.filter(
      (t) =>
        t.startMachineId !== machineId && t.finishMachineId !== machineId,
    ),
  });
}

export function addTransition(
  project: Project,
  bodyId: string,
  startMachineId: string,
  finishMachineId: string,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  const alphabet = body.alphabet || project.alphabet;
  const transition: Transition = {
    id: id(),
    startMachineId,
    finishMachineId,
    routingPoints: [],
    conditions: defaultTransitionConditions(alphabet),
    conditionsLabelT: 0.5,
  };
  return updateBody(project, {
    ...body,
    transitions: [...body.transitions, transition],
  });
}

export function removeTransition(
  project: Project,
  bodyId: string,
  transitionId: string,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  return updateBody(project, {
    ...body,
    transitions: body.transitions.filter((t) => t.id !== transitionId),
  });
}

export function updateWriteMachineSign(
  project: Project,
  bodyId: string,
  machineId: string,
  sign: string,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  return updateBody(project, {
    ...body,
    machines: body.machines.map((m) =>
      m.id === machineId && m.type === 'write' ? { ...m, sign } : m,
    ),
  });
}

export function updateTransitionConditions(
  project: Project,
  bodyId: string,
  transitionId: string,
  conditions: TransitionConditions,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  return updateBody(project, {
    ...body,
    transitions: body.transitions.map((t) =>
      t.id === transitionId ? { ...t, conditions } : t,
    ),
  });
}

export function updateTransitionLabelT(
  project: Project,
  bodyId: string,
  transitionId: string,
  conditionsLabelT: number,
): Project {
  const body = findBody(project, bodyId);
  if (!body) return project;
  const t = Math.max(0, Math.min(1, conditionsLabelT));
  return updateBody(project, {
    ...body,
    transitions: body.transitions.map((tr) =>
      tr.id === transitionId
        ? { ...tr, conditionsLabelT: t, conditionsAnchor: undefined }
        : tr,
    ),
  });
}

