import { useRef } from 'react';
import { getLambda } from '@core/model/types';
import type { Tape } from '@core/tape/Tape';
import {
  displayTapeSign,
  parseTapeCellInput,
  tapeCellIndices,
} from '@core/tape/tapeViewport';

type TapeViewProps = {
  tape: Tape;
  alphabet: string;
  viewStart: number;
  cellCount: number;
  disabled?: boolean;
  onChange: () => void;
};

export function TapeView({
  tape,
  alphabet,
  viewStart,
  cellCount,
  disabled,
  onChange,
}: TapeViewProps) {
  const lambda = getLambda(alphabet);
  const head = tape.getHeadIndex();
  const indices = tapeCellIndices(viewStart, cellCount);
  const inputRefs = useRef<Map<number, HTMLInputElement>>(new Map());

  const commitCell = (index: number, raw: string) => {
    const parsed = parseTapeCellInput(raw, alphabet, lambda);
    if (parsed === null) return false;
    tape.setValueAt(index, parsed);
    onChange();
    return true;
  };

  const selectHead = (index: number) => {
    if (disabled) return;
    tape.setHeadIndex(index);
    onChange();
  };

  return (
    <div className="tape-grid" role="group" aria-label="Tape">
      {indices.map((index) => {
        const isHead = index === head;
        const value = tape.getValueAt(index);
        return (
          <div key={index} className="tape-cell">
            <input
              ref={(el) => {
                if (el) inputRefs.current.set(index, el);
                else inputRefs.current.delete(index);
              }}
              type="text"
              inputMode="text"
              className={`tape-cell-input${isHead ? ' tape-cell-input--head' : ''}`}
              value={displayTapeSign(value, lambda)}
              disabled={disabled}
              maxLength={2}
              aria-label={`Cell ${index}${isHead ? ', head' : ''}`}
              onMouseDown={() => selectHead(index)}
              onChange={(e) => {
                if (!commitCell(index, e.target.value)) {
                  e.target.value = displayTapeSign(tape.getValueAt(index), lambda);
                }
              }}
              onKeyDown={(e) => {
                if (e.key === 'Enter' || e.key === 'Tab') {
                  e.preventDefault();
                  const pos = indices.indexOf(index);
                  const next = indices[pos + 1];
                  if (next !== undefined) {
                    tape.setHeadIndex(next);
                    onChange();
                    inputRefs.current.get(next)?.focus();
                  }
                }
              }}
            />
            <span className="tape-cell-index">{index}</span>
          </div>
        );
      })}
    </div>
  );
}
