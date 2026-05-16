#ifndef VMTTHEME_H
#define VMTTHEME_H

#include <QColor>
#include <QString>

class QApplication;
class QWidget;

/** Material Design 2 palette and styles for VTM. */
class VmtTheme
{
public:
    // --- Material palette (Blue primary) ---
    static const QColor& primary();
    static const QColor& primaryDark();
    static const QColor& primaryLight();
    static const QColor& onPrimary();

    static const QColor& background();
    static const QColor& surface();
    static const QColor& onSurface();
    static const QColor& onSurfaceSecondary();

    static const QColor& divider();
    static const QColor& selectionBackground();
    static const QColor& onSelection();

    static const QColor& error();
    static const QColor& onError();

    // Diagram canvas
    static const QColor& diagramBackground();
    static const QColor& diagramGrid();
    static const QColor& diagramLine();
    static const QColor& diagramLineSelected();
    static const QColor& diagramNodeFillSelected();
    static const QColor& diagramNodeBorder();
    static const QColor& diagramNodeBorderSelected();

    static void applyApplication(QApplication* app);
    static void polishWidgetTree(QWidget* root);
    /** Dark chrome + button styles for toolbars that use white/light pixmap icons. */
    static void applyIconToolBar(QWidget* toolbarRoot);

    static QString applicationStyleSheet();
    static QString headerChromeStyle();
    static QString contentAreaStyle();
    static QString breadcrumbLinkStyle();
    static QString breadcrumbCurrentStyle();
    static QString toolButtonStyle();
    static QString iconToolButtonStyle();
    static QString toolbarToolButtonStyle();
    static QString alphabetButtonStyle();
    static QString lineEditTapeStyle(bool machineHead);
    static QString primaryButtonStyle();
    static QString accentPanelStyle();
    static QString panelSurfaceStyle();
    static QString listSurfaceStyle();

    static QString colorName(const QColor& color);
};

#endif // VMTTHEME_H
