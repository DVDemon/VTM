import { useEffect, useState } from 'react';

/** Load a URL into an `HTMLImageElement` for Konva `Image`. */
export function useHtmlImage(src: string | undefined): HTMLImageElement | null {
  const [image, setImage] = useState<HTMLImageElement | null>(null);

  useEffect(() => {
    if (!src) {
      setImage(null);
      return;
    }
    const img = new window.Image();
    img.decoding = 'async';
    const onLoad = () => setImage(img);
    const onError = () => setImage(null);
    img.addEventListener('load', onLoad);
    img.addEventListener('error', onError);
    img.src = src;
    return () => {
      img.removeEventListener('load', onLoad);
      img.removeEventListener('error', onError);
    };
  }, [src]);

  return image;
}
