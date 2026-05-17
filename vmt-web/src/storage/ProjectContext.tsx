import {
  createContext,
  useCallback,
  useContext,
  useMemo,
  useReducer,
  type ReactNode,
} from 'react';
import {
  createEmptyProject,
  deserializeProject,
  serializeProject,
  type Project,
} from '@core/index';
import { loadDraft, pushRecent, saveDraft } from './localStorage';

interface ProjectState {
  project: Project;
  dirty: boolean;
}

type Action =
  | { type: 'set'; project: Project; dirty?: boolean }
  | { type: 'replace'; project: Project };

function reducer(state: ProjectState, action: Action): ProjectState {
  switch (action.type) {
    case 'set':
      return {
        project: action.project,
        dirty: action.dirty ?? true,
      };
    case 'replace':
      return { project: action.project, dirty: false };
    default:
      return state;
  }
}

function loadInitialProject(): Project {
  const draft = loadDraft();
  if (draft) {
    try {
      return deserializeProject(draft);
    } catch {
      /* use empty */
    }
  }
  return createEmptyProject();
}

interface ProjectContextValue {
  project: Project;
  dirty: boolean;
  setProject: (project: Project) => void;
  replaceProject: (project: Project) => void;
  persistDraft: () => void;
}

const ProjectContext = createContext<ProjectContextValue | null>(null);

export function ProjectProvider({ children }: { children: ReactNode }) {
  const [state, dispatch] = useReducer(reducer, undefined, () => ({
    project: loadInitialProject(),
    dirty: false,
  }));

  const setProject = useCallback((project: Project) => {
    dispatch({ type: 'set', project });
  }, []);

  const replaceProject = useCallback((project: Project) => {
    dispatch({ type: 'replace', project });
    pushRecent({
      id: project.id,
      name: project.name,
      updatedAt: project.updatedAt,
    });
  }, []);

  const persistDraft = useCallback(() => {
    const json = serializeProject(state.project);
    saveDraft(json);
    dispatch({ type: 'replace', project: state.project });
  }, [state.project]);

  const value = useMemo(
    () => ({
      project: state.project,
      dirty: state.dirty,
      setProject,
      replaceProject,
      persistDraft,
    }),
    [state, setProject, replaceProject, persistDraft],
  );

  return (
    <ProjectContext.Provider value={value}>{children}</ProjectContext.Provider>
  );
}

export function useProject(): ProjectContextValue {
  const ctx = useContext(ProjectContext);
  if (!ctx) throw new Error('useProject must be used within ProjectProvider');
  return ctx;
}
