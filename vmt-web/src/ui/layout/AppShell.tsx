import { Link, Outlet, useLocation } from 'react-router-dom';
import { useProject } from '../../storage/ProjectContext';
import {
  openProjectFile,
  saveProjectAsJdtp,
  saveProjectAsJson,
} from '../files/projectFiles';
import { vmtTheme } from '../theme/vmtTheme';

const nav = [
  { to: '/', label: 'Home' },
  { to: '/editor', label: 'Editor' },
  { to: '/compiler', label: 'Check' },
  { to: '/debugger', label: 'Debug' },
  { to: '/exercises', label: 'Exercises' },
  { to: '/export-4th', label: '4th' },
] as const;

export function AppShell() {
  const { project, dirty, replaceProject, persistDraft } = useProject();
  const location = useLocation();

  return (
    <div className="app" style={{ display: 'flex', flexDirection: 'column', minHeight: '100vh' }}>
      <header className="toolbar" style={{ background: vmtTheme.background }}>
        <strong style={{ color: vmtTheme.textOnDark, marginRight: 16 }}>
          VMT Web
        </strong>
        {nav.map(({ to, label }) => (
          <Link
            key={to}
            to={to}
            className={location.pathname === to ? 'active' : ''}
          >
            {label}
          </Link>
        ))}
        <span className="spacer" />
        <button
          type="button"
          onClick={async () => {
            const p = await openProjectFile();
            if (p) replaceProject(p);
          }}
        >
          Open
        </button>
        <button type="button" onClick={() => saveProjectAsJdtp(project)}>
          Save .jdtp
        </button>
        <button type="button" onClick={() => saveProjectAsJson(project)}>
          Save .json
        </button>
        <button type="button" onClick={() => persistDraft()}>
          Draft{dirty ? '*' : ''}
        </button>
      </header>
      <main className="main">
        <Outlet />
      </main>
    </div>
  );
}
