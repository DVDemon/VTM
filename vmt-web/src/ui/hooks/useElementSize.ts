import { useEffect, useState, type RefObject } from 'react';

export interface ElementSize {
  width: number;
  height: number;
}

export function useElementSize(
  ref: RefObject<HTMLElement | null>,
  minWidth = 320,
  minHeight = 200,
): ElementSize {
  const [size, setSize] = useState<ElementSize>({ width: minWidth, height: minHeight });

  useEffect(() => {
    const el = ref.current;
    if (!el) return;

    const measure = () => {
      const rect = el.getBoundingClientRect();
      setSize({
        width: Math.max(minWidth, Math.floor(rect.width)),
        height: Math.max(minHeight, Math.floor(rect.height)),
      });
    };

    measure();
    const observer = new ResizeObserver(() => measure());
    observer.observe(el);
    return () => observer.disconnect();
  }, [ref, minWidth, minHeight]);

  return size;
}
