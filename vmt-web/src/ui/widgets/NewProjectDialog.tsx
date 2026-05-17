import { useMemo, useState } from 'react';
import {
  alphabetHint,
  createProject,
  isValidAlphabet,
  normalizeAlphabet,
  type Project,
} from '@core/index';

interface NewProjectDialogProps {
  open: boolean;
  onClose: () => void;
  onCreate: (project: Project) => void;
}

export function NewProjectDialog({
  open,
  onClose,
  onCreate,
}: NewProjectDialogProps) {
  const [projectName, setProjectName] = useState('New Project');
  const [machineName, setMachineName] = useState('Main machine');
  const [alphabetRaw, setAlphabetRaw] = useState(' 01');
  const [error, setError] = useState<string | null>(null);

  const normalized = useMemo(
    () => normalizeAlphabet(alphabetRaw),
    [alphabetRaw],
  );
  const canSubmit =
    projectName.trim().length > 0 &&
    machineName.trim().length > 0 &&
    isValidAlphabet(normalized);

  if (!open) return null;

  const submit = () => {
    try {
      const project = createProject(projectName, alphabetRaw, machineName);
      onCreate(project);
      onClose();
      setError(null);
    } catch (e) {
      setError(e instanceof Error ? e.message : 'Failed to create project');
    }
  };

  return (
    <div
      className="modal-backdrop"
      role="presentation"
      onClick={onClose}
    >
      <div
        className="modal-dialog"
        role="dialog"
        aria-labelledby="new-project-title"
        onClick={(e) => e.stopPropagation()}
      >
        <h2 id="new-project-title">New project</h2>
        <label className="form-field">
          <span>Project name</span>
          <input
            type="text"
            value={projectName}
            onChange={(e) => setProjectName(e.target.value)}
            autoFocus
          />
        </label>
        <label className="form-field">
          <span>Root machine name</span>
          <input
            type="text"
            value={machineName}
            onChange={(e) => setMachineName(e.target.value)}
          />
        </label>
        <label className="form-field">
          <span>Alphabet</span>
          <input
            type="text"
            value={alphabetRaw}
            onChange={(e) => setAlphabetRaw(e.target.value)}
            spellCheck={false}
          />
        </label>
        <p className="form-hint">{alphabetHint()}</p>
        <p className="form-hint">
          Normalized: <code>{normalized || '—'}</code>
          {!isValidAlphabet(normalized) && ' (need ≥2 symbols)'}
        </p>
        {error && <p className="form-error">{error}</p>}
        <div className="modal-actions">
          <button type="button" onClick={onClose}>
            Cancel
          </button>
          <button type="button" disabled={!canSubmit} onClick={submit}>
            Create
          </button>
        </div>
      </div>
    </div>
  );
}
