import { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import { useElementSize } from '../hooks/useElementSize';
import {
  alignTapeViewToHead,
  createDebugger,
  defaultTapeViewStart,
  findBody,
  formatDebuggerCallStack,
  stepDebugger,
  tapeCellCountForWidth,
  type DebuggerState,
  type Point,
} from '@core/index';
import { useProject } from '../../storage/ProjectContext';
import { DiagramStage } from '../canvas/DiagramStage';
import { debugIcons } from '../icons/vmtIcons';
import { ToolbarIcon } from '../icons/ToolbarIcon';
import { TapeView } from '../widgets/TapeView';

export function DebuggerScreen() {
  const { project } = useProject();
  const [dbg, setDbg] = useState<DebuggerState>(() => createDebugger(project));
  const [running, setRunning] = useState(false);
  const [viewOffset, setViewOffset] = useState<Point>({ x: 0, y: 0 });
  const [tapeViewStart, setTapeViewStart] = useState(() =>
    defaultTapeViewStart(0),
  );

  const tapeWrapRef = useRef<HTMLDivElement>(null);
  const tapeWrapSize = useElementSize(tapeWrapRef, 320, 40);
  const cellCount = useMemo(
    () => tapeCellCountForWidth(tapeWrapSize.width),
    [tapeWrapSize.width],
  );
  const tapePage = cellCount;

  const refreshDbg = useCallback(() => {
    setDbg((d) => ({ ...d }));
  }, []);

  useEffect(() => {
    const head = dbg.tape.getHeadIndex();
    setTapeViewStart((start) => alignTapeViewToHead(start, head, cellCount));
  }, [dbg, cellCount]);

  const step = () => setDbg((s) => stepDebugger(s));
  const reset = () => {
    setDbg(createDebugger(project));
    setTapeViewStart(defaultTapeViewStart(0, cellCount));
  };

  const centerTapeOnHead = () => {
    setTapeViewStart(defaultTapeViewStart(dbg.tape.getHeadIndex(), cellCount));
  };

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
  const activeBody = findBody(project, dbg.bodyId);
  const callStack = useMemo(() => formatDebuggerCallStack(dbg), [dbg]);
  const canvasWrapRef = useRef<HTMLDivElement>(null);
  const canvasSize = useElementSize(canvasWrapRef, 200, 120);

  return (
    <div className="screen debugger-screen">
      <div className="debugger-chrome">
        <div className="debugger-controls">
          <button
            type="button"
            className="debugger-icon-btn"
            onClick={step}
            disabled={running || dbg.finished}
            title="Single step"
            aria-label="Step"
          >
            <ToolbarIcon src={debugIcons.step} alt="Step" />
          </button>
          <button
            type="button"
            className="debugger-icon-btn"
            onClick={run}
            disabled={running || dbg.finished}
            title="Run"
            aria-label="Run"
          >
            <ToolbarIcon src={debugIcons.run} alt="Run" />
          </button>
          <button
            type="button"
            className="debugger-icon-btn"
            onClick={stop}
            disabled={!running}
            title="Stop"
            aria-label="Stop"
          >
            <ToolbarIcon src={debugIcons.stop} alt="Stop" />
          </button>
          <button type="button" onClick={reset} title="Reset to start">
            Reset
          </button>
          <span>
            {activeBody?.name ?? '—'} · {selectedId.slice(0, 8)}…
            {dbg.finished ? ' · FINISHED' : ''}
          </span>
        </div>
        <div className="debugger-call-stack" aria-label="Call stack">
          <strong>Stack</strong>
          <ol>
            {callStack.map((entry) => (
              <li
                key={entry.depth}
                className={
                  entry.depth === callStack.length - 1
                    ? 'debugger-stack-frame active'
                    : 'debugger-stack-frame'
                }
              >
                {entry.bodyName} → {entry.machineLabel}
              </li>
            ))}
          </ol>
        </div>
        <div className="tape-controls">
          <button
            type="button"
            className="tape-nav-btn"
            title="Страница влево"
            aria-label="Страница влево"
            onClick={() => setTapeViewStart((s) => s - tapePage)}
          >
            «
          </button>
          <button
            type="button"
            className="tape-nav-btn"
            title="Ячейка влево"
            aria-label="Ячейка влево"
            onClick={() => setTapeViewStart((s) => s - 1)}
          >
            ‹
          </button>
          <button
            type="button"
            className="tape-nav-btn"
            title="К головке (начальное положение окна)"
            aria-label="К головке"
            onClick={centerTapeOnHead}
          >
            ⌂
          </button>
          <button
            type="button"
            className="tape-nav-btn"
            title="Ячейка вправо"
            aria-label="Ячейка вправо"
            onClick={() => setTapeViewStart((s) => s + 1)}
          >
            ›
          </button>
          <button
            type="button"
            className="tape-nav-btn"
            title="Страница вправо"
            aria-label="Страница вправо"
            onClick={() => setTapeViewStart((s) => s + tapePage)}
          >
            »
          </button>
        </div>
        <div ref={tapeWrapRef} className="tape-view-wrap">
          <TapeView
            tape={dbg.tape}
            alphabet={project.alphabet}
            viewStart={tapeViewStart}
            cellCount={cellCount}
            disabled={running}
            onChange={refreshDbg}
          />
        </div>
      </div>
      <div ref={canvasWrapRef} className="editor-canvas-wrap debugger-canvas">
        <DiagramStage
          project={project}
          bodyId={dbg.bodyId}
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
