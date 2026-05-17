#include "vmttheme.h"

#include "formeditorwidget.h"
#include "formlinewidget.h"
#include "vmticons.h"

#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>

namespace {

QColor gPrimary(25, 118, 210);           // #1976D2
QColor gPrimaryDark(1, 44, 64);          // #012C40 toolbar chrome
QColor gPrimaryLight(227, 242, 253);     // #E3F2FD
QColor gOnPrimary(255, 255, 255);

QColor gBackground(245, 245, 245);       // #F5F5F5 app bg
QColor gSurface(255, 255, 255);
QColor gOnSurface(33, 33, 33);           // #212121
QColor gOnSurfaceSecondary(97, 97, 97);  // #616161

QColor gDivider(189, 189, 189);          // #BDBDBD
QColor gSelectionBackground(187, 222, 251);
QColor gOnSelection(21, 101, 192);

QColor gError(211, 47, 47);              // #D32F2F
QColor gOnError(255, 255, 255);

QColor gCanvasWorkspace(236, 239, 241);  // #ECEFF1 editor wrap
QColor gDiagramBackground(250, 250, 250);  // #FAFAFA stage
QColor gDiagramGrid(224, 224, 224);        // #E0E0E0
QColor gDiagramLine(33, 33, 33);           // #212121
QColor gDiagramLineSelected(25, 118, 210);
QColor gMachineFill(255, 255, 255);
QColor gMachineFillSelected(187, 222, 251); // #BBDEFB
QColor gDiagramNodeFillSelected(187, 222, 251);
QColor gDiagramNodeBorder(33, 33, 33);
QColor gDiagramNodeBorderSelected(25, 118, 210);

bool gDarkMode = false;

void applyLightPalette()
{
    gPrimary = QColor(25, 118, 210);
    gPrimaryDark = QColor(1, 44, 64);
    gPrimaryLight = QColor(227, 242, 253);
    gOnPrimary = QColor(255, 255, 255);
    gBackground = QColor(245, 245, 245);
    gSurface = QColor(255, 255, 255);
    gOnSurface = QColor(33, 33, 33);
    gOnSurfaceSecondary = QColor(97, 97, 97);
    gDivider = QColor(189, 189, 189);
    gSelectionBackground = QColor(187, 222, 251);
    gOnSelection = QColor(21, 101, 192);
    gError = QColor(211, 47, 47);
    gOnError = QColor(255, 255, 255);
    gCanvasWorkspace = QColor(236, 239, 241);
    gDiagramBackground = QColor(250, 250, 250);
    gDiagramGrid = QColor(224, 224, 224);
    gDiagramLine = QColor(33, 33, 33);
    gDiagramLineSelected = QColor(25, 118, 210);
    gMachineFill = QColor(255, 255, 255);
    gMachineFillSelected = QColor(187, 222, 251);
    gDiagramNodeFillSelected = gMachineFillSelected;
    gDiagramNodeBorder = QColor(33, 33, 33);
    gDiagramNodeBorderSelected = QColor(25, 118, 210);
}

void applyDarkPalette()
{
    gPrimary = QColor(66, 165, 245);
    gPrimaryDark = QColor(1, 44, 64);
    gPrimaryLight = QColor(38, 50, 56);
    gOnPrimary = QColor(255, 255, 255);
    gBackground = QColor(18, 18, 18);
    gSurface = QColor(38, 50, 56);
    gOnSurface = QColor(224, 224, 224);
    gOnSurfaceSecondary = QColor(158, 158, 158);
    gDivider = QColor(69, 90, 100);
    gSelectionBackground = QColor(55, 71, 79);
    gOnSelection = QColor(144, 202, 249);
    gError = QColor(239, 83, 80);
    gOnError = QColor(255, 255, 255);
    gCanvasWorkspace = QColor(30, 30, 30);
    gDiagramBackground = QColor(38, 50, 56);
    gDiagramGrid = QColor(69, 90, 100);
    gDiagramLine = QColor(236, 239, 241);
    gDiagramLineSelected = QColor(66, 165, 245);
    gMachineFill = QColor(55, 71, 79);
    gMachineFillSelected = QColor(21, 101, 192);
    gDiagramNodeFillSelected = gMachineFillSelected;
    gDiagramNodeBorder = QColor(236, 239, 241);
    gDiagramNodeBorderSelected = QColor(66, 165, 245);
}

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

bool shouldClearWidgetStyle(const QString& styleSheet)
{
    if (styleSheet.isEmpty()) {
        return false;
    }
    if (isLegacyStyleSheet(styleSheet)) {
        return true;
    }
    return styleSheet.contains(QStringLiteral("background:white"), Qt::CaseInsensitive)
           || styleSheet.contains(QStringLiteral("background-color:white"), Qt::CaseInsensitive)
           || styleSheet.contains(QStringLiteral("background-color: white"), Qt::CaseInsensitive)
           || styleSheet.contains(QStringLiteral("background: white"), Qt::CaseInsensitive);
}

void applyThemedWidget(QWidget* widget)
{
    if (!widget) {
        return;
    }

    if (shouldClearWidgetStyle(widget->styleSheet())) {
        widget->setStyleSheet(QString());
    }

    if (auto* lineEdit = qobject_cast<QLineEdit*>(widget)) {
        lineEdit->setStyleSheet(VmtTheme::inputStyle());
    } else if (auto* textEdit = qobject_cast<QTextEdit*>(widget)) {
        const QString name = textEdit->objectName();
        if (name == QLatin1String("_text") || name == QLatin1String("_text_ru")) {
            textEdit->setStyleSheet(VmtTheme::accentPanelStyle());
        } else {
            textEdit->setStyleSheet(VmtTheme::inputStyle());
        }
    } else if (auto* plain = qobject_cast<QPlainTextEdit*>(widget)) {
        plain->setStyleSheet(VmtTheme::inputStyle());
    } else if (auto* combo = qobject_cast<QComboBox*>(widget)) {
        combo->setStyleSheet(VmtTheme::inputStyle());
    } else if (auto* spin = qobject_cast<QSpinBox*>(widget)) {
        spin->setStyleSheet(VmtTheme::inputStyle());
    } else if (auto* list = qobject_cast<QListWidget*>(widget)) {
        list->setStyleSheet(VmtTheme::listSurfaceStyle());
    } else if (auto* tool = qobject_cast<QToolButton*>(widget)) {
        if (tool->objectName().startsWith(QLatin1String("button_z"))) {
            tool->setStyleSheet(VmtTheme::surfaceToolButtonStyle());
        } else if (widget->parentWidget()
                   && QLatin1String(widget->parentWidget()->metaObject()->className())
                          == QLatin1String("FormAlphabit")) {
            tool->setStyleSheet(VmtTheme::alphabetButtonStyle());
        }
    }

    for (QObject* child : widget->children()) {
        if (auto* childWidget = qobject_cast<QWidget*>(child)) {
            applyThemedWidget(childWidget);
        }
    }
}

void polishWidgetTreeImpl(QWidget* root)
{
    if (!root) {
        return;
    }

    if (shouldClearWidgetStyle(root->styleSheet())) {
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

const QColor& VmtTheme::canvasWorkspace() { return gCanvasWorkspace; }
const QColor& VmtTheme::diagramBackground() { return gDiagramBackground; }
const QColor& VmtTheme::diagramGrid() { return gDiagramGrid; }
const QColor& VmtTheme::diagramLine() { return gDiagramLine; }
const QColor& VmtTheme::diagramLineSelected() { return gDiagramLineSelected; }
const QColor& VmtTheme::machineFill() { return gMachineFill; }
const QColor& VmtTheme::machineFillSelected() { return gMachineFillSelected; }
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

QString VmtTheme::inputStyle()
{
    return QString(
        "QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QComboBox {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  selection-background-color: %4;"
        "  selection-color: %5;"
        "}")
        .arg(colorName(gSurface),
             colorName(gOnSurface),
             colorName(gDivider),
             colorName(gSelectionBackground),
             colorName(gOnSelection));
}

QString VmtTheme::surfaceToolButtonStyle()
{
    return QString(
        "QToolButton {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 4px;"
        "}")
        .arg(colorName(gSurface), colorName(gDivider));
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

void VmtTheme::applyIconToolBarsInTree(QWidget* root)
{
    if (!root) {
        return;
    }
    static const QStringList toolbarFrameNames = {
        QStringLiteral("frame_header"),
        QStringLiteral("frame"),
        QStringLiteral("tool_frame"),
        QStringLiteral("frame_navi"),
        QStringLiteral("frame_2"),
        QStringLiteral("frame_4"),
    };
    for (QWidget* widget : root->findChildren<QWidget*>()) {
        if (toolbarFrameNames.contains(widget->objectName())) {
            applyIconToolBar(widget);
        }
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

bool VmtTheme::isDarkMode()
{
    return gDarkMode;
}

void VmtTheme::setDarkMode(bool dark, QApplication* app)
{
    gDarkMode = dark;
    if (dark) {
        applyDarkPalette();
    } else {
        applyLightPalette();
    }
    if (app) {
        applyApplication(app);
    }
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

void VmtTheme::applyThemedWidgets(QWidget* root)
{
    if (!root) {
        return;
    }
    applyThemedWidget(root);
    refreshDiagramViews(root);
    VmtIcons::refreshInterfaceIcons(root);
    for (FormLineWidget* tape : root->findChildren<FormLineWidget*>()) {
        tape->applyTheme();
    }
}

void VmtTheme::refreshDiagramViews(QWidget* root)
{
    if (!root) {
        return;
    }
    for (FormEditorWidget* editor : root->findChildren<FormEditorWidget*>()) {
        editor->applyTheme();
    }
}
