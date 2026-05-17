import type { Project } from '../model/types';
import { SCHEMA_VERSION } from '../model/types';
import { createEmptyProject } from '../model/factory';

export function serializeProject(project: Project): string {
  return JSON.stringify(project, null, 2);
}

export function deserializeProject(raw: string): Project {
  const data = JSON.parse(raw) as Partial<Project>;
  return migrateProject(data);
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
  return {
    schemaVersion: SCHEMA_VERSION,
    id: data.id,
    name: data.name,
    alphabet: data.alphabet ?? ' ',
    bodies: data.bodies,
    rootBodyIds: data.rootBodyIds ?? [data.bodies[0]!.id],
    createdAt: data.createdAt ?? new Date().toISOString(),
    updatedAt: data.updatedAt ?? new Date().toISOString(),
  };
}
