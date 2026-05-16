#include "vmttheme.h"

#include <QApplication>
#include <QToolButton>
#include <QWidget>

namespace {

QColor gPrimary(21, 101, 192);           // Blue 700
QColor gPrimaryDark(13, 71, 161);       // Blue 900 — higher contrast with white text
QColor gPrimaryLight(227, 242, 253);    // Blue 50
QColor gOnPrimary(255, 255, 255);

QColor gBackground(250, 250, 250);      // Grey 50
QColor gSurface(255, 255, 255);
QColor gOnSurface(33, 33, 33);          // Grey 900
QColor gOnSurfaceSecondary(97, 97, 97); // Grey 600

QColor gDivider(224, 224, 224);         // Grey 300
QColor gSelectionBackground(227, 242, 253);
QColor gOnSelection(13, 71, 161);

QColor gError(211, 47, 47);             // Red 700
QColor gOnError(255, 255, 255);

QColor gDiagramBackground(255, 255, 255);
QColor gDiagramGrid(224, 224, 224);
QColor gDiagramLine(66, 66, 66);
QColor gDiagramLineSelected(21, 101, 192);
QColor gDiagramNodeFillSelected(227, 242, 253);
QColor gDiagramNodeBorder(158, 158, 158);
QColor gDiagramNodeBorderSelected(21, 101, 192);

bool isLegacyStyleSheet(const QString& styleSheet)
{
    return styleSheet.contains("#012C40", Qt::CaseInsensitive)
           || styleSheet.contains("#01547a", Qt::CaseInsensitive)
           || styleSheet.contains("rgb(232,143,12)", Qt::CaseInsensitive)
           || styleSheet.contains("rgb(232, 143, 12)", Qt::CaseInsensitive)
           || styleSheet.contains("background-color:gray", Qt::CaseInsensitive)
           || styleSheet.contains("color:darkgray", Qt::CaseInsensitive)
           || styleSheet.contains("rgb(80, 80, 80)", Qt::CaseInsensitive);
}

void polishWidgetTreeImpl(QWidget* root)
{
    if (!root) {
        return;
    }

    if (isLegacyStyleSheet(root->styleSheet())) {
        root->setStyleSheet(QString());
    }

    for (QObject* child : root->children()) {
        if (auto* widget = qobject_cast<QWidget*>(child)) {
            polishWidgetTreeImpl(widget);
        }
    }
}

} // namespace

const QColor& VmtTheme::primary() { return gPrimary; }
const QColor& VmtTheme::primaryDark() { return gPrimaryDark; }
const QColor& VmtTheme::primaryLight() { return gPrimaryLight; }
const QColor& VmtTheme::onPrimary() { return gOnPrimary; }

const QColor& VmtTheme::background() { return gBackground; }
const QColor& VmtTheme::surface() { return gSurface; }
const QColor& VmtTheme::onSurface() { return gOnSurface; }
const QColor& VmtTheme::onSurfaceSecondary() { return gOnSurfaceSecondary; }

const QColor& VmtTheme::divider() { return gDivider; }
const QColor& VmtTheme::selectionBackground() { return gSelectionBackground; }
const QColor& VmtTheme::onSelection() { return gOnSelection; }

const QColor& VmtTheme::error() { return gError; }
const QColor& VmtTheme::onError() { return gOnError; }

const QColor& VmtTheme::diagramBackground() { return gDiagramBackground; }
const QColor& VmtTheme::diagramGrid() { return gDiagramGrid; }
const QColor& VmtTheme::diagramLine() { return gDiagramLine; }
const QColor& VmtTheme::diagramLineSelected() { return gDiagramLineSelected; }
const QColor& VmtTheme::diagramNodeFillSelected() { return gDiagramNodeFillSelected; }
const QColor& VmtTheme::diagramNodeBorder() { return gDiagramNodeBorder; }
const QColor& VmtTheme::diagramNodeBorderSelected() { return gDiagramNodeBorderSelected; }

QString VmtTheme::colorName(const QColor& color)
{
    return color.name(QColor::HexRgb);
}

QString VmtTheme::applicationStyleSheet()
{
    const QString primary = colorName(gPrimary);
    const QString primaryDark = colorName(gPrimaryDark);
    const QString onPrimary = colorName(gOnPrimary);
    const QString background = colorName(gBackground);
    const QString surface = colorName(gSurface);
    const QString onSurface = colorName(gOnSurface);
    const QString onSurfaceSecondary = colorName(gOnSurfaceSecondary);
    const QString divider = colorName(gDivider);
    const QString selectionBg = colorName(gSelectionBackground);
    const QString onSelection = colorName(gOnSelection);
    const QString error = colorName(gError);

    return QString(
        "QWidget {"
        "  background-color: %1;"
        "  color: %2;"
        "}"
        "QMainWindow, QDialog {"
        "  background-color: %1;"
        "}"
        "QFrame, QGroupBox {"
        "  background-color: %3;"
        "  color: %2;"
        "  border: none;"
        "}"
        "QLabel {"
        "  background-color: transparent;"
        "  color: %2;"
        "}"
        "QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QComboBox {"
        "  background-color: %3;"
        "  color: %2;"
        "  border: 1px solid %4;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  selection-background-color: %5;"
        "  selection-color: %6;"
        "}"
        "QListWidget, QListView, QTreeView, QTableView {"
        "  background-color: %3;"
        "  color: %2;"
        "  border: 1px solid %4;"
        "  selection-background-color: %5;"
        "  selection-color: %6;"
        "}"
        "QPushButton {"
        "  background-color: %3;"
        "  color: %2;"
        "  border: 1px solid %4;"
        "  border-radius: 4px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %5;"
        "  color: %6;"
        "}"
        "QPushButton:pressed {"
        "  background-color: %7;"
        "  color: %8;"
        "}"
        "QSlider::groove:horizontal {"
        "  background: %4;"
        "  height: 4px;"
        "  border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "  background: %7;"
        "  width: 14px;"
        "  margin: -5px 0;"
        "  border-radius: 7px;"
        "}"
        "QScrollBar:vertical {"
        "  background: %1;"
        "  width: 10px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: %4;"
        "  border-radius: 5px;"
        "  min-height: 24px;"
        "}"
        )
        .arg(background, onSurface, surface, divider, selectionBg, onSelection, primaryDark, onPrimary);
}

QString VmtTheme::headerChromeStyle()
{
    return QString("QWidget { background-color: %1; color: %2; }")
        .arg(colorName(gPrimaryDark), colorName(gOnPrimary));
}

QString VmtTheme::contentAreaStyle()
{
    return QString("QWidget { background-color: %1; color: %2; }")
        .arg(colorName(gBackground), colorName(gOnSurface));
}

QString VmtTheme::breadcrumbLinkStyle()
{
    return QString(
        "QLabel { color: #BBDEFB; background: transparent; text-decoration: underline; }");
}

QString VmtTheme::breadcrumbCurrentStyle()
{
    return QString("QLabel { color: %1; background: transparent; font-weight: bold; }")
        .arg(colorName(gOnPrimary));
}

QString VmtTheme::toolButtonStyle()
{
    return QString(
        "QToolButton { background-color: transparent; color: %1; border: none; border-radius: 4px; }"
        "QToolButton:hover { background-color: %2; color: %3; }"
        "QToolButton:checked { background-color: %4; color: %5; }")
        .arg(colorName(gOnSurface),
             colorName(gSelectionBackground),
             colorName(gOnSelection),
             colorName(gPrimaryDark),
             colorName(gOnPrimary));
}

QString VmtTheme::iconToolButtonStyle()
{
    return QString(
        "QToolButton {"
        "  background-color: transparent;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "}"
        "QToolButton:hover {"
        "  background-color: %1;"
        "}"
        "QToolButton:pressed {"
        "  background-color: %2;"
        "}"
        "QToolButton:checked {"
        "  background-color: %1;"
        "  border: 1px solid %3;"
        "}"
        "QToolButton:disabled {"
        "  background-color: transparent;"
        "  opacity: 0.45;"
        "}")
        .arg(colorName(gPrimary),
             colorName(gPrimaryDark),
             colorName(gOnPrimary));
}

QString VmtTheme::toolbarToolButtonStyle()
{
    return iconToolButtonStyle();
}

QString VmtTheme::alphabetButtonStyle()
{
    return QString(
        "QToolButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-radius: 4px;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QToolButton:hover {"
        "  background-color: %4;"
        "  color: %5;"
        "}"
        "QToolButton:checked {"
        "  background-color: %6;"
        "  color: %7;"
        "  border: 1px solid %6;"
        "}")
        .arg(colorName(gSurface),
             colorName(gOnSurface),
             colorName(gDivider),
             colorName(gSelectionBackground),
             colorName(gOnSelection),
             colorName(gPrimaryDark),
             colorName(gOnPrimary));
}

QString VmtTheme::primaryButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %3;"
        "  color: %2;"
        "}"
        "QPushButton:pressed {"
        "  background-color: %4;"
        "  color: %2;"
        "}")
        .arg(colorName(gPrimary),
             colorName(gOnPrimary),
             colorName(gPrimaryDark),
             colorName(gPrimaryDark));
}

void VmtTheme::applyIconToolBar(QWidget* toolbarRoot)
{
    if (!toolbarRoot) {
        return;
    }

    toolbarRoot->setStyleSheet(headerChromeStyle());
    const QString buttonStyle = iconToolButtonStyle();
    const auto buttons = toolbarRoot->findChildren<QToolButton*>();
    for (QToolButton* button : buttons) {
        button->setStyleSheet(buttonStyle);
    }
}

QString VmtTheme::accentPanelStyle()
{
    return QString(
        "QTextEdit, QTextBrowser {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-radius: 4px;"
        "}")
        .arg(colorName(gPrimaryLight),
             colorName(gOnSelection),
             colorName(gPrimary));
}

QString VmtTheme::lineEditTapeStyle(bool machineHead)
{
    if (machineHead) {
        return QString(
            "QLineEdit {"
            "  background-color: %1;"
            "  color: %2;"
            "  selection-background-color: %2;"
            "  selection-color: %1;"
            "  border: 1px solid %1;"
            "  font-weight: bold;"
            "}")
            .arg(colorName(gPrimaryDark), colorName(gOnPrimary));
    }

    return QString(
        "QLineEdit {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  selection-background-color: %4;"
        "  selection-color: %5;"
        "}")
        .arg(colorName(gSurface),
             colorName(gOnSurface),
             colorName(gDivider),
             colorName(gSelectionBackground),
             colorName(gOnSelection));
}

QString VmtTheme::panelSurfaceStyle()
{
    return QString("QWidget { background-color: %1; color: %2; }")
        .arg(colorName(gSurface), colorName(gOnSurface));
}

QString VmtTheme::listSurfaceStyle()
{
    return QString(
        "QListWidget { background-color: %1; color: %2; border: 1px solid %3; }"
        "QListWidget::item:selected { background-color: %4; color: %5; }")
        .arg(colorName(gSurface),
             colorName(gOnSurface),
             colorName(gDivider),
             colorName(gPrimaryDark),
             colorName(gOnPrimary));
}

void VmtTheme::applyApplication(QApplication* app)
{
    if (!app) {
        return;
    }
    app->setStyleSheet(applicationStyleSheet());
}

void VmtTheme::polishWidgetTree(QWidget* root)
{
    polishWidgetTreeImpl(root);
}
