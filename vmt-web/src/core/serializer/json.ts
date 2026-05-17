import type { MachineBody, Project } from '../model/types';
import { SCHEMA_VERSION } from '../model/types';
import { createEmptyProject } from '../model/factory';
import { normalizeAlphabet } from '../model/alphabet';

export function serializeProject(project: Project): string {
  return JSON.stringify(project, null, 2);
}

export function deserializeProject(raw: string): Project {
  const data = JSON.parse(raw) as Partial<Project>;
  return migrateProject(data);
}

function migrateBody(
  body: Partial<MachineBody>,
  projectAlphabet: string,
): MachineBody {
  const alphabet = normalizeAlphabet(
    typeof body.alphabet === 'string' ? body.alphabet : projectAlphabet,
  );
  return {
    id: body.id!,
    name: body.name ?? 'Machine',
    alphabet,
    machines: body.machines ?? [],
    transitions: body.transitions ?? [],
  };
}

function migrateProject(data: Partial<Project>): Project {
  if (!data || typeof data !== 'object') {
    return createEmptyProject();
  }
  const version = data.schemaVersion ?? 1;
  if (version !== SCHEMA_VERSION) {
    throw new Error(`Unsupported schema version: ${version}`);
  }
  if (!data.id || !data.name || !Array.isArray(data.bodies)) {
    return createEmptyProject(data.name ?? 'Recovered');
  }
  const alphabet = normalizeAlphabet(
    typeof data.alphabet === 'string' ? data.alphabet : ' 01',
  );
  const bodies = data.bodies.map((b) => migrateBody(b, alphabet));
  return {
    schemaVersion: SCHEMA_VERSION,
    id: data.id,
    name: data.name,
    alphabet,
    bodies,
    rootBodyIds: data.rootBodyIds ?? [bodies[0]!.id],
    createdAt: data.createdAt ?? new Date().toISOString(),
    updatedAt: data.updatedAt ?? new Date().toISOString(),
  };
}
