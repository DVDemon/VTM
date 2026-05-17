interface EditorActionBarProps {
  zoom: number;
  canUndo: boolean;
  onUndo: () => void;
  onExportPng: () => void;
  onExportPuml: () => void;
  onRun: () => void;
  onExport4th: () => void;
  onZoomChange: (zoom: number) => void;
}

export function EditorActionBar({
  zoom,
  canUndo,
  onUndo,
  onExportPng,
  onExportPuml,
  onRun,
  onExport4th,
  onZoomChange,
}: EditorActionBarProps) {
  return (
    <div className="editor-action-bar" role="toolbar" aria-label="Editor actions">
      <button type="button" disabled={!canUndo} onClick={onUndo} title="Undo">
        Undo
      </button>
      <span className="editor-action-sep" aria-hidden />
      <button type="button" onClick={onExportPng} title="Export diagram as PNG">
        PNG
      </button>
      <button type="button" onClick={onExportPuml} title="Export PlantUML">
        PUML
      </button>
      <button type="button" onClick={onRun} title="Check and run">
        Run
      </button>
      <button type="button" onClick={onExport4th} title="Export fourths">
        4th
      </button>
      <span className="editor-action-sep" aria-hidden />
      <label className="editor-zoom">
        <span>Zoom</span>
        <input
          type="range"
          min={25}
          max={200}
          step={5}
          value={zoom}
          onChange={(e) => onZoomChange(Number(e.target.value))}
          aria-valuemin={25}
          aria-valuemax={200}
          aria-valuenow={zoom}
        />
        <span className="editor-zoom-value">{zoom}%</span>
      </label>
    </div>
  );
}
