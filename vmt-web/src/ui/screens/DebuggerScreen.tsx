import { useCallback, useRef, useState } from 'react';
import { useElementSize } from '../hooks/useElementSize';
import {
  createDebugger,
  getRootBody,
  stepDebugger,
  type DebuggerState,
  type Point,
} from '@core/index';
import { useProject } from '../../storage/ProjectContext';
import { DiagramStage } from '../canvas/DiagramStage';

export function DebuggerScreen() {
  const { project } = useProject();
  const [dbg, setDbg] = useState<DebuggerState>(() => createDebugger(project));
  const [running, setRunning] = useState(false);
  const [viewOffset, setViewOffset] = useState<Point>({ x: 0, y: 0 });

  const tapeWindow = useCallback(() => {
    const chars: string[] = [];
    for (let i = dbg.tape.getHeadIndex() - 5; i <= dbg.tape.getHeadIndex() + 5; i++) {
      chars.push(
        i === dbg.tape.getHeadIndex()
          ? `[${dbg.tape.getValueAt(i)}]`
          : dbg.tape.getValueAt(i),
      );
    }
    return chars.join('');
  }, [dbg]);

  const step = () => setDbg((s) => stepDebugger(s));
  const reset = () => setDbg(createDebugger(project));

  const run = () => {
    setRunning(true);
    const id = window.setInterval(() => {
      setDbg((s) => {
        const next = stepDebugger(s);
        if (next.finished) {
          window.clearInterval(id);
          setRunning(false);
        }
        return next;
      });
    }, 50);
  };

  const stop = () => setRunning(false);

  const selectedId = dbg.machineId;
  const root = getRootBody(project);
  const canvasWrapRef = useRef<HTMLDivElement>(null);
  const canvasSize = useElementSize(canvasWrapRef, 200, 120);

  return (
    <div className="screen debugger-screen">
      <div className="debugger-chrome">
        <div className="debugger-controls">
          <button type="button" onClick={step} disabled={running || dbg.finished}>
            Step
          </button>
          <button type="button" onClick={run} disabled={running || dbg.finished}>
            Run
          </button>
          <button type="button" onClick={stop} disabled={!running}>
            Stop
          </button>
          <button type="button" onClick={reset}>
            Reset
          </button>
          <span>
            Body: {root.name} · Machine: {selectedId.slice(0, 8)}…
            {dbg.finished ? ' · FINISHED' : ''}
          </span>
        </div>
        <pre className="tape-view">{tapeWindow()}</pre>
      </div>
      <div ref={canvasWrapRef} className="editor-canvas-wrap debugger-canvas">
        <DiagramStage
          project={project}
          width={canvasSize.width}
          height={canvasSize.height}
          interactionMode="navigate"
          viewOffset={viewOffset}
          onViewOffsetChange={setViewOffset}
          selection={{ kind: 'machine', id: selectedId }}
        />
      </div>
    </div>
  );
}
