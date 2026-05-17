interface ComplexPlacementPanelProps {
  name: string;
  onNameChange: (name: string) => void;
}

/** Запрос имени перед размещением узла MT_COMPLEX на холсте. */
export function ComplexPlacementPanel({
  name,
  onNameChange,
}: ComplexPlacementPanelProps) {
  return (
    <aside className="editor-inspector" aria-label="Complex machine name">
      <h3>Complex machine</h3>
      <p className="editor-inspector-hint">
        Enter a name, then click the canvas to place the node.
      </p>
      <label className="form-field">
        <span>Machine name</span>
        <input
          type="text"
          value={name}
          onChange={(e) => onNameChange(e.target.value)}
          autoFocus
        />
      </label>
    </aside>
  );
}
