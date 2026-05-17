import {
  createDefaultJdtpDownload,
  decodeJdtp,
  deserializeProject,
  serializeProject,
  type Project,
} from '@core/index';

export async function openProjectFile(): Promise<Project | null> {
  return new Promise((resolve) => {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.jdtp,.vmt.json,application/json';
    input.onchange = async () => {
      const file = input.files?.[0];
      if (!file) {
        resolve(null);
        return;
      }
      try {
        const buffer = await file.arrayBuffer();
        const name = file.name.toLowerCase();
        if (name.endsWith('.jdtp')) {
          resolve(decodeJdtp(buffer));
        } else {
          const text = new TextDecoder().decode(buffer);
          resolve(deserializeProject(text));
        }
      } catch (e) {
        alert(e instanceof Error ? e.message : 'Failed to open file');
        resolve(null);
      }
    };
    input.click();
  });
}

export function downloadBlob(blob: Blob, filename: string): void {
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = filename;
  a.click();
  URL.revokeObjectURL(url);
}

export function saveProjectAsJdtp(project: Project, filename?: string): void {
  const blob = createDefaultJdtpDownload(project);
  downloadBlob(blob, filename ?? `${sanitizeFilename(project.name)}.jdtp`);
}

export function saveProjectAsJson(project: Project, filename?: string): void {
  const json = serializeProject(project);
  const blob = new Blob([json], { type: 'application/json' });
  downloadBlob(blob, filename ?? `${sanitizeFilename(project.name)}.vmt.json`);
}

export function exportPlantUmlFile(content: string, baseName: string): void {
  const blob = new Blob([content], { type: 'text/plain;charset=utf-8' });
  downloadBlob(blob, `${sanitizeFilename(baseName)}.puml`);
}

function sanitizeFilename(name: string): string {
  return name.replace(/[^\w.-]+/g, '_') || 'project';
}
