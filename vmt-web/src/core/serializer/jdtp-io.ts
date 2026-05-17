import type { Project } from '../model/types';
import { createEmptyProject } from '../model/factory';
import { deserializeProject, serializeProject } from './json';

/**
 * Binary .jdtp (QDataStream) codec — full port from VMTSerializer is pending.
 * Until then, round-trip through JSON for development; desktop binary import throws.
 */
export function decodeJdtp(buffer: ArrayBuffer): Project {
  if (buffer.byteLength < 2) {
    throw new Error('File is too small to be a valid .jdtp project');
  }
  const text = new TextDecoder().decode(buffer);
  if (text.trimStart().startsWith('{')) {
    return deserializeProject(text);
  }
  throw new Error(
    'Binary .jdtp import is not yet implemented in the web build. ' +
      'Save from desktop or use Export as .vmt.json.',
  );
}

export function encodeJdtp(project: Project): ArrayBuffer {
  const json = serializeProject(project);
  return new TextEncoder().encode(json).buffer;
}

export function createDefaultJdtpDownload(project: Project): Blob {
  return new Blob([encodeJdtp(project)], { type: 'application/octet-stream' });
}

export function isEmptyProject(project: Project): boolean {
  return project.bodies.length === 0;
}

export function ensureProject(project: Project | null): Project {
  return project ?? createEmptyProject();
}
