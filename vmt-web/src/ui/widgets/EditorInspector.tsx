import {
  formatTransitionConditions,
  getLambda,
  isWriteMachine,
  transitionConditionSigns,
  type Machine,
  type Project,
  type Transition,
  type TransitionConditions,
} from '@core/index';
import { getRootBody } from '@core/index';

interface EditorInspectorProps {
  project: Project;
  selectedMachine: Machine | null;
  selectedTransition: Transition | null;
  onWriteSign: (sign: string) => void;
  onTransitionConditions: (conditions: TransitionConditions) => void;
}

export function EditorInspector({
  project,
  selectedMachine,
  selectedTransition,
  onWriteSign,
  onTransitionConditions,
}: EditorInspectorProps) {
  const alphabet = getRootBody(project).alphabet || project.alphabet;
  const lambda = getLambda(alphabet);

  if (selectedMachine && isWriteMachine(selectedMachine)) {
    return (
      <aside className="editor-inspector" aria-label="Write machine sign">
        <h3>Write (λ)</h3>
        <p className="editor-inspector-hint">Choose symbol from alphabet</p>
        <div className="alphabet-chips">
          {alphabet.split('').map((sign) => (
            <button
              key={`${selectedMachine.id}-${sign}`}
              type="button"
              className={
                selectedMachine.sign === sign
                  ? 'chip active'
                  : 'chip'
              }
              onClick={() => onWriteSign(sign)}
            >
              {sign === lambda ? 'λ' : sign}
            </button>
          ))}
        </div>
      </aside>
    );
  }

  if (selectedTransition) {
    const track = alphabet.split('').filter((c) => c !== lambda);
    const enabled = new Set(selectedTransition.conditions.enabledSigns);
    const label = formatTransitionConditions(
      alphabet,
      selectedTransition.conditions,
    );
    const allSigns =
      transitionConditionSigns(alphabet, selectedTransition.conditions) ===
      alphabet;

    const toggle = (sign: string) => {
      const next = new Set(enabled);
      if (next.has(sign)) next.delete(sign);
      else next.add(sign);
      onTransitionConditions({
        enabledSigns: [...next],
      });
    };

    return (
      <aside className="editor-inspector" aria-label="Transition conditions">
        <h3>Transition</h3>
        <p className="editor-inspector-label">Label: {label}</p>
        <p className="editor-inspector-hint">
          Toggle symbols (drag label on the line)
        </p>
        <div className="alphabet-chips">
          {track.map((sign) => (
            <button
              key={`${selectedTransition.id}-${sign}`}
              type="button"
              className={enabled.has(sign) ? 'chip active' : 'chip'}
              onClick={() => toggle(sign)}
            >
              {sign}
            </button>
          ))}
          <button
            type="button"
            className={allSigns ? 'chip active' : 'chip'}
            onClick={() =>
              onTransitionConditions({ enabledSigns: alphabet.split('') })
            }
          >
            *
          </button>
        </div>
      </aside>
    );
  }

  return null;
}
