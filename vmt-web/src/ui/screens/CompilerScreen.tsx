import { useMemo } from 'react';
import { runCompilerChecks } from '@core/index';
import { useProject } from '../../storage/ProjectContext';

export function CompilerScreen() {
  const { project } = useProject();
  const errors = useMemo(() => runCompilerChecks(project), [project]);

  return (
    <div className="screen">
      <h1>Diagram check</h1>
      {errors.length === 0 ? (
        <p className="ok">No issues found. You can run the debugger.</p>
      ) : (
        <ul className="error-list">
          {errors.map((e, i) => (
            <li key={`${e.bodyId}-${i}`}>{e.message}</li>
          ))}
        </ul>
      )}
    </div>
  );
}
