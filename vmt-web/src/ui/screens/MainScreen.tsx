import { useState } from 'react';
import { Link } from 'react-router-dom';
import { useProject } from '../../storage/ProjectContext';
import { loadRecent } from '../../storage/localStorage';
import { openProjectFile } from '../files/projectFiles';
import { NewProjectDialog } from '../widgets/NewProjectDialog';

export function MainScreen() {
  const { project, replaceProject } = useProject();
  const recent = loadRecent();
  const [newOpen, setNewOpen] = useState(false);

  return (
    <div className="screen">
      <h1>Virtual Turing Machine</h1>
      <p>
        Projects are stored on your device. Use <strong>Open</strong> /{' '}
        <strong>Save .jdtp</strong> in the toolbar — files are not uploaded to
        any server.
      </p>
      <p>
        Current project: <strong>{project.name}</strong>
      </p>
      <div className="actions">
        <button type="button" onClick={() => setNewOpen(true)}>
          New project…
        </button>
        <NewProjectDialog
          open={newOpen}
          onClose={() => setNewOpen(false)}
          onCreate={replaceProject}
        />
        <button
          type="button"
          onClick={async () => {
            const p = await openProjectFile();
            if (p) replaceProject(p);
          }}
        >
          Open file…
        </button>
        <Link to="/editor" className="button-link">
          Open editor
        </Link>
      </div>
      {recent.length > 0 && (
        <section>
          <h2>Recent (metadata only)</h2>
          <ul>
            {recent.map((r) => (
              <li key={r.id}>
                {r.name} — {new Date(r.updatedAt).toLocaleString()}
              </li>
            ))}
          </ul>
        </section>
      )}
      <section className="dev-hint">
        <h2>Local development (NFR-013)</h2>
        <pre>cd vmt-web && npm install && npm run dev</pre>
        <p>Default URL: http://localhost:5173/VTM/</p>
      </section>
    </div>
  );
}
