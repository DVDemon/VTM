import { KEYS, STORAGE_PREFIX } from './keys';

const MAX_DRAFT_BYTES = 4 * 1024 * 1024;

export class StorageQuotaError extends Error {
  constructor(message: string) {
    super(message);
    this.name = 'StorageQuotaError';
  }
}

export function saveDraft(json: string): void {
  const bytes = new TextEncoder().encode(json).byteLength;
  if (bytes > MAX_DRAFT_BYTES) {
    throw new StorageQuotaError(
      'Draft is too large for browser storage. Save the project as a file on disk.',
    );
  }
  try {
    localStorage.setItem(KEYS.draft, json);
  } catch (e) {
    if (e instanceof DOMException && e.name === 'QuotaExceededError') {
      throw new StorageQuotaError(
        'Browser storage quota exceeded. Save the project as a file on disk.',
      );
    }
    throw e;
  }
}

export function loadDraft(): string | null {
  return localStorage.getItem(KEYS.draft);
}

export function clearDraft(): void {
  localStorage.removeItem(KEYS.draft);
}

export interface RecentEntry {
  readonly id: string;
  readonly name: string;
  readonly updatedAt: string;
}

export function loadRecent(): RecentEntry[] {
  const raw = localStorage.getItem(KEYS.recent);
  if (!raw) return [];
  try {
    return JSON.parse(raw) as RecentEntry[];
  } catch {
    return [];
  }
}

export function pushRecent(entry: RecentEntry): void {
  const list = loadRecent().filter((r) => r.id !== entry.id);
  list.unshift(entry);
  localStorage.setItem(KEYS.recent, JSON.stringify(list.slice(0, 10)));
}

export function assertVmtKey(key: string): void {
  if (!key.startsWith(STORAGE_PREFIX)) {
    throw new Error(`Storage key must start with ${STORAGE_PREFIX}`);
  }
}
