#ifndef VMTICONS_H
#define VMTICONS_H

#include <QString>

#include "uicanvas.h"

class QIcon;
class QPixmap;
class QWidget;

/** Theme-aware icon paths (pairs for light / dark surfaces). */
class VmtIcons
{
public:
    /** Dark glyph on light UI; light glyph on dark UI (canvas, panels). */
    static QString pick(const char* lightSurfacePath, const char* darkSurfacePath);

    static QIcon iconForSurface(const char* lightSurfacePath, const char* darkSurfacePath);
    static QPixmap pixmapForSurface(const char* lightSurfacePath, const char* darkSurfacePath);

    /** Colored icons for dark toolbar chrome (#012C40 / #01547a). */
    static QString toolbarChromePath(const char* path);

    static QString machinePath(ImageType type);
    static QPixmap machinePixmap(ImageType type);
    static QIcon machineIcon(ImageType type);

    static QString editorToolPath(const QString& objectName);

    static QPixmap linkPixmap();
    static QPixmap deletePixmap();
    static QPixmap handPixmap();
    static QPixmap lambdaPixmap();
    static QPixmap cancelPixmap();
    static QPixmap okPixmap();

    static void loadMachineIcons(std::vector<QPixmap*>& out);
    static void refreshInterfaceIcons(QWidget* root);
};

#endif // VMTICONS_H
