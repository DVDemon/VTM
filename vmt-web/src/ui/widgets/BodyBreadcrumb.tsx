import { findBody, getRootBody } from '@core/model/factory';
import type { Project } from '@core/model/types';

interface BodyBreadcrumbProps {
  project: Project;
  editingBodyId: string;
  onNavigate: (bodyId: string) => void;
}

export function BodyBreadcrumb({
  project,
  editingBodyId,
  onNavigate,
}: BodyBreadcrumbProps) {
  const root = getRootBody(project);
  const current = findBody(project, editingBodyId) ?? root;

  return (
    <nav className="body-breadcrumb" aria-label="Diagram navigation">
      <button
        type="button"
        className={editingBodyId === root.id ? 'active' : ''}
        onClick={() => onNavigate(root.id)}
      >
        {root.name}
      </button>
      {editingBodyId !== root.id && (
        <>
          <span className="body-breadcrumb-sep">›</span>
          <span className="body-breadcrumb-current">{current.name}</span>
        </>
      )}
    </nav>
  );
}
