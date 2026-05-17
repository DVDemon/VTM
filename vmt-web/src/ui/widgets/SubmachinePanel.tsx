import { listAllBodies } from '@core/model/factory';
import type { Project } from '@core/model/types';

export type SubmachinePlaceMode = 'new' | 'existing';

interface SubmachinePanelProps {
  project: Project;
  currentBodyId: string;
  mode: SubmachinePlaceMode;
  onModeChange: (mode: SubmachinePlaceMode) => void;
  newName: string;
  onNewNameChange: (name: string) => void;
  selectedInnerBodyId: string;
  onSelectInnerBodyId: (id: string) => void;
  onCreateSubmachine: () => void;
}

/** Place MT_COMPLEX: new submachine or reference an existing body (desktop FormComplexMachines). */
export function SubmachinePanel({
  project,
  currentBodyId,
  mode,
  onModeChange,
  newName,
  onNewNameChange,
  selectedInnerBodyId,
  onSelectInnerBodyId,
  onCreateSubmachine,
}: SubmachinePanelProps) {
  const bodies = listAllBodies(project);

  return (
    <aside className="editor-inspector submachine-panel" aria-label="Submachine">
      <h3>Submachine</h3>
      <p className="editor-inspector-hint">
        Create or pick a submachine, then click the canvas. Double-click a placed
        node to edit its diagram.
      </p>

      <div className="submachine-mode">
        <label>
          <input
            type="radio"
            name="submachine-mode"
            checked={mode === 'new'}
            onChange={() => onModeChange('new')}
          />
          New
        </label>
        <label>
          <input
            type="radio"
            name="submachine-mode"
            checked={mode === 'existing'}
            onChange={() => onModeChange('existing')}
          />
          Existing
        </label>
      </div>

      {mode === 'new' ? (
        <label className="form-field">
          <span>Name</span>
          <input
            type="text"
            value={newName}
            onChange={(e) => onNewNameChange(e.target.value)}
            autoFocus
          />
        </label>
      ) : (
        <label className="form-field">
          <span>Submachine</span>
          <select
            value={selectedInnerBodyId}
            onChange={(e) => onSelectInnerBodyId(e.target.value)}
          >
            <option value="">— select —</option>
            {bodies.map((b) => (
              <option key={b.id} value={b.id}>
                {b.name}
                {b.id === currentBodyId ? ' (current)' : ''}
              </option>
            ))}
          </select>
        </label>
      )}

      <div className="submachine-list">
        <div className="submachine-list-header">
          <strong>All machines</strong>
          <button type="button" onClick={onCreateSubmachine}>
            + New
          </button>
        </div>
        <ul>
          {bodies.map((b) => (
            <li key={b.id}>
              <button
                type="button"
                className={
                  selectedInnerBodyId === b.id && mode === 'existing'
                    ? 'submachine-list-item active'
                    : 'submachine-list-item'
                }
                onClick={() => {
                  onModeChange('existing');
                  onSelectInnerBodyId(b.id);
                }}
              >
                {b.name}
                <span className="submachine-list-meta">
                  {b.machines.length} nodes
                </span>
              </button>
            </li>
          ))}
        </ul>
      </div>
    </aside>
  );
}
