import { lazy, Suspense } from 'react';
import { HashRouter, Navigate, Route, Routes } from 'react-router-dom';
import { ProjectProvider } from './storage/ProjectContext';
import { AppShell } from './ui/layout/AppShell';

const MainScreen = lazy(() =>
  import('./ui/screens/MainScreen').then((m) => ({ default: m.MainScreen })),
);
const EditorScreen = lazy(() =>
  import('./ui/screens/EditorScreen').then((m) => ({ default: m.EditorScreen })),
);
const CompilerScreen = lazy(() =>
  import('./ui/screens/CompilerScreen').then((m) => ({
    default: m.CompilerScreen,
  })),
);
const DebuggerScreen = lazy(() =>
  import('./ui/screens/DebuggerScreen').then((m) => ({
    default: m.DebuggerScreen,
  })),
);
const ExercisesScreen = lazy(() =>
  import('./ui/screens/ExercisesScreen').then((m) => ({
    default: m.ExercisesScreen,
  })),
);
const Export4thScreen = lazy(() =>
  import('./ui/screens/Export4thScreen').then((m) => ({
    default: m.Export4thScreen,
  })),
);

export default function App() {
  return (
    <ProjectProvider>
      <HashRouter>
        <Routes>
          <Route path="/" element={<AppShell />}>
            <Route
              index
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <MainScreen />
                </Suspense>
              }
            />
            <Route
              path="editor"
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <EditorScreen />
                </Suspense>
              }
            />
            <Route
              path="compiler"
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <CompilerScreen />
                </Suspense>
              }
            />
            <Route
              path="debugger"
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <DebuggerScreen />
                </Suspense>
              }
            />
            <Route
              path="exercises"
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <ExercisesScreen />
                </Suspense>
              }
            />
            <Route
              path="export-4th"
              element={
                <Suspense fallback={<p>Loading…</p>}>
                  <Export4thScreen />
                </Suspense>
              }
            />
            <Route path="*" element={<Navigate to="/" replace />} />
          </Route>
        </Routes>
      </HashRouter>
    </ProjectProvider>
  );
}
