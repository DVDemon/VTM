interface ToolbarIconProps {
  readonly src: string;
  readonly alt: string;
  readonly className?: string;
}

export function ToolbarIcon({ src, alt, className }: ToolbarIconProps) {
  return (
    <img
      className={className ?? 'vmt-toolbar-icon'}
      src={src}
      alt=""
      aria-hidden
      draggable={false}
      title={alt}
    />
  );
}
