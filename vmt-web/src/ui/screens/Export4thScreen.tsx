import { useCallback, useMemo, useState } from 'react';
import {
  DEFAULT_EXPORT_FOURTH_OPTIONS,
  exportFourth,
  type ExportFourthOptions,
} from '@core/index';
import { useProject } from '../../storage/ProjectContext';

export function Export4thScreen() {
  const { project } = useProject();
  const [options, setOptions] = useState<ExportFourthOptions>(
    DEFAULT_EXPORT_FOURTH_OPTIONS,
  );
  const [result, setResult] = useState<ReturnType<typeof exportFourth> | null>(
    null,
  );
  const [copyHint, setCopyHint] = useState<string | null>(null);

  const rootName = useMemo(
    () =>
      project.bodies.find((b) => b.id === project.rootBodyIds[0])?.name ??
      project.name,
    [project],
  );

  const runExport = useCallback(() => {
    setCopyHint(null);
    setResult(exportFourth(project, options));
  }, [project, options]);

  const copyOutput = useCallback(async () => {
    if (!result?.text) return;
    try {
      await navigator.clipboard.writeText(result.text);
      setCopyHint('Copied to clipboard');
    } catch {
      setCopyHint('Could not access clipboard');
    }
  }, [result]);

  const setField = (key: keyof ExportFourthOptions, value: string) => {
    setOptions((prev) => ({ ...prev, [key]: value }));
  };

  return (
    <div className="screen export-fourth-screen">
      <h1>Export to fourth notation</h1>
      <p className="hint">
        Machine: <strong>{rootName}</strong> — tabular transitions (
        <code>state | symbol | command | next</code>), port of desktop{' '}
        <code>VMTExport4th</code>.
      </p>

      <div className="export-fourth-options">
        <label className="form-field">
          <span>Delimiter</span>
          <input
            type="text"
            value={options.delimiter}
            onChange={(e) => setField('delimiter', e.target.value)}
          />
        </label>
        <label className="form-field">
          <span>Left</span>
          <input
            type="text"
            value={options.left}
            onChange={(e) => setField('left', e.target.value)}
          />
        </label>
        <label className="form-field">
          <span>Right</span>
          <input
            type="text"
            value={options.right}
            onChange={(e) => setField('right', e.target.value)}
          />
        </label>
        <label className="form-field">
          <span>Stop</span>
          <input
            type="text"
            value={options.stop}
            onChange={(e) => setField('stop', e.target.value)}
          />
        </label>
        <label className="form-field">
          <span>Quotes</span>
          <input
            type="text"
            value={options.quotes}
            onChange={(e) => setField('quotes', e.target.value)}
          />
        </label>
      </div>

      <div className="export-fourth-actions">
        <button type="button" onClick={runExport}>
          Export
        </button>
        <button
          type="button"
          onClick={copyOutput}
          disabled={!result?.text}
        >
          Copy to clipboard
        </button>
        {copyHint ? <span className="hint">{copyHint}</span> : null}
      </div>

      {result ? (
        <>
          <pre
            className={
              result.error
                ? 'export-fourth-log export-fourth-error'
                : 'export-fourth-log'
            }
          >
            {result.information}
            {result.error ? result.errorMessage : ''}
          </pre>
          {!result.error ? (
            <textarea
              className="export-fourth-output"
              readOnly
              value={result.text}
              spellCheck={false}
            />
          ) : null}
        </>
      ) : null}
    </div>
  );
}
