import { getLambda } from './types';

/** Нормализует алфавит: λ первым, без дубликатов (как FormNewProject). */
export function normalizeAlphabet(raw: string): string {
  const lambda = getLambda(' ');
  const seen = new Set<string>();
  const rest: string[] = [];
  for (const ch of raw) {
    if (ch === lambda || seen.has(ch)) continue;
    seen.add(ch);
    rest.push(ch);
  }
  return lambda + rest.join('');
}

export function isValidAlphabet(alphabet: string): boolean {
  return normalizeAlphabet(alphabet).length >= 2;
}

export function alphabetHint(): string {
  return 'First symbol is λ (empty tape cell). Enter at least one more symbol, e.g. " 01".';
}
