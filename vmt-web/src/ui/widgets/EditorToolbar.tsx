import { EDITOR_TOOL_GROUPS, type EditorTool } from './editorTools';

interface EditorToolbarProps {
  activeTool: EditorTool;
  onToolChange: (tool: EditorTool) => void;
}

export function EditorToolbar({ activeTool, onToolChange }: EditorToolbarProps) {
  return (
    <aside
      className="editor-toolbar-panel"
      role="toolbar"
      aria-label="Diagram tools"
    >
      {EDITOR_TOOL_GROUPS.map((group) => (
        <div
          key={group.title}
          className="editor-toolbar-group"
          role="group"
          aria-label={group.title}
        >
          {group.tools.map((tool) => (
            <button
              key={tool.id}
              type="button"
              className={
                activeTool === tool.id
                  ? 'editor-tool-btn active'
                  : 'editor-tool-btn'
              }
              title={tool.title}
              aria-label={tool.title}
              aria-pressed={activeTool === tool.id}
              onClick={() => onToolChange(tool.id)}
            >
              <span className="editor-tool-label">{tool.label}</span>
            </button>
          ))}
        </div>
      ))}
    </aside>
  );
}
