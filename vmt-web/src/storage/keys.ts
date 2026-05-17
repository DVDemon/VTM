export const STORAGE_PREFIX = 'vmt:';

export const KEYS = {
  draft: `${STORAGE_PREFIX}draft`,
  recent: `${STORAGE_PREFIX}recent`,
  settings: `${STORAGE_PREFIX}settings`,
  exerciseProgress: `${STORAGE_PREFIX}exercise-progress`,
} as const;
