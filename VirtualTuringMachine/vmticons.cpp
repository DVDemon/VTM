#include "vmticons.h"

#include "formeditormachine.h"
#include "vmttheme.h"

#include <QFrame>
#include <QHash>
#include <QIcon>
#include <QPixmap>
#include <QToolButton>
#include <QWidget>

#include <vector>

namespace {

QString pathOrEmpty(const char* path)
{
    return path ? QString::fromUtf8(path) : QString();
}

const QHash<QString, const char*>& editorToolChromePaths()
{
    static const QHash<QString, const char*> map = {
        {QStringLiteral("tool_pointer"), ":/Files/images/toolbars/tools/icon_arrow.gif"},
        {QStringLiteral("tool_link"), ":/Files/images/toolbars/tools/icon_link.gif"},
        {QStringLiteral("tool_translate"), ":/Files/images/toolbars/tools/icon_hand.gif"},
        {QStringLiteral("tool_trash"), ":/Files/images/toolbars/tools/icon_delete.gif"},
        {QStringLiteral("tool_machine_start"), ":/images/toolbars/tools/icon_start.png"},
        {QStringLiteral("tool_machine_finish"), ":/images/toolbars/tools/icon_finish.png"},
        {QStringLiteral("tool_machine_left"), ":/Files/images/toolbars/tools/icon_l_small.gif"},
        {QStringLiteral("tool_machine_right"), ":/Files/images/toolbars/tools/icon_r_small.gif"},
        {QStringLiteral("tool_machine_left_word"), ":/Files/images/toolbars/tools/icon_l_big.gif"},
        {QStringLiteral("tool_machine_right_word"), ":/Files/images/toolbars/tools/icon_r_big.gif"},
        {QStringLiteral("tool_machine_lambda"), ":/Files/images/toolbars/tools/icon_lambda.gif"},
        {QStringLiteral("tool_machine_copy"), ":/Files/images/toolbars/tools/icon_k.gif"},
        {QStringLiteral("tool_complex"), ":/Files/images/toolbars/tools/icon_complex.gif"},
        {QStringLiteral("_navi"), ":/Files/images/nav.png"},
    };
    return map;
}

void refreshEditorToolbars(QWidget* root)
{
    for (QFrame* frame : root->findChildren<QFrame*>()) {
        if (frame->objectName() != QLatin1String("tool_frame")) {
            continue;
        }
        for (QToolButton* button : frame->findChildren<QToolButton*>()) {
            const QString path = VmtIcons::editorToolPath(button->objectName());
            if (!path.isEmpty()) {
                button->setIcon(QIcon(path));
            }
        }
    }
}

} // namespace

QString VmtIcons::pick(const char* lightSurfacePath, const char* darkSurfacePath)
{
    return VmtTheme::isDarkMode() ? pathOrEmpty(darkSurfacePath) : pathOrEmpty(lightSurfacePath);
}

QIcon VmtIcons::iconForSurface(const char* lightSurfacePath, const char* darkSurfacePath)
{
    return QIcon(pick(lightSurfacePath, darkSurfacePath));
}

QPixmap VmtIcons::pixmapForSurface(const char* lightSurfacePath, const char* darkSurfacePath)
{
    return QPixmap(pick(lightSurfacePath, darkSurfacePath));
}

QString VmtIcons::toolbarChromePath(const char* path)
{
    return pathOrEmpty(path);
}

QString VmtIcons::machinePath(ImageType type)
{
    switch (type) {
    case MACHINE_LAMBDA:
        return pick(":/Files/images/toolbars/tools/icon_lambda_black.gif",
                    ":/Files/images/toolbars/tools/icon_lambda.gif");
    case MACHINE_START:
        return pick(":/Files/images/toolbars/tools/icon_start_black.png",
                    ":/images/toolbars/tools/icon_start.png");
    case MACHINE_FINISH:
        return pick(":/Files/images/toolbars/tools/icon_finish_black.png",
                    ":/images/toolbars/tools/icon_finish.png");
    case MACHINE_LEFT:
        return pick(":/Files/images/toolbars/tools/icon_l_small_black.gif",
                    ":/Files/images/toolbars/tools/icon_l_small.gif");
    case MACHINE_RIGHT:
        return pick(":/Files/images/toolbars/tools/icon_r_small_black.gif",
                    ":/Files/images/toolbars/tools/icon_r_small.gif");
    case MACHINE_LEFT_WORD:
        return pick(":/Files/images/toolbars/tools/icon_l_big_black.gif",
                    ":/Files/images/toolbars/tools/icon_l_big.gif");
    case MACHINE_RIGHT_WORD:
        return pick(":/Files/images/toolbars/tools/icon_r_big_black.gif",
                    ":/Files/images/toolbars/tools/icon_r_big.gif");
    case MACHINE_COPY:
        return pick(":/Files/images/toolbars/tools/icon_k_black.gif",
                    ":/Files/images/toolbars/tools/icon_k.gif");
    case CANCEL:
        return QStringLiteral(":/Files/images/cancel.png");
    case MACHINE_COMPLEX:
        return pick(":/Files/images/machine_complex.gif",
                    ":/Files/images/toolbars/tools/icon_complex.gif");
    case LINK:
        return pick(":/Files/images/toolbars/tools/icon_link_black.png",
                    ":/Files/images/toolbars/tools/icon_link.gif");
    case NAVIGATION_CIRCLE:
        return QStringLiteral(":/Files/images/circle.png");
    default:
        return QString();
    }
}

QPixmap VmtIcons::machinePixmap(ImageType type)
{
    const QString path = machinePath(type);
    return path.isEmpty() ? QPixmap() : QPixmap(path);
}

QIcon VmtIcons::machineIcon(ImageType type)
{
    return QIcon(machinePath(type));
}

QString VmtIcons::editorToolPath(const QString& objectName)
{
    const auto& map = editorToolChromePaths();
    const auto it = map.find(objectName);
    if (it == map.end()) {
        return QString();
    }
    return toolbarChromePath(it.value());
}

QPixmap VmtIcons::linkPixmap()
{
    return pixmapForSurface(":/Files/images/toolbars/tools/icon_link_black.png",
                          ":/Files/images/toolbars/tools/icon_link.gif");
}

QPixmap VmtIcons::deletePixmap()
{
    return pixmapForSurface(":/Files/images/toolbars/tools/icon_delete_black.png",
                          ":/Files/images/toolbars/tools/icon_delete.gif");
}

QPixmap VmtIcons::handPixmap()
{
    return pixmapForSurface(":/Files/images/toolbars/tools/icon_hand_black.png",
                          ":/Files/images/toolbars/tools/icon_hand.gif");
}

QPixmap VmtIcons::lambdaPixmap()
{
    return machinePixmap(MACHINE_LAMBDA);
}

QPixmap VmtIcons::cancelPixmap()
{
    return QPixmap(QStringLiteral(":/Files/images/cancel.png"));
}

QPixmap VmtIcons::okPixmap()
{
    return pixmapForSurface(":/Files/images/ok_black.png", ":/Files/images/ok.png");
}

void VmtIcons::loadMachineIcons(std::vector<QPixmap*>& out)
{
    for (QPixmap* pixmap : out) {
        delete pixmap;
    }
    out.clear();

    const ImageType types[] = {
        MACHINE_LAMBDA, MACHINE_START, MACHINE_FINISH, MACHINE_LEFT, MACHINE_RIGHT,
        MACHINE_LEFT_WORD, MACHINE_RIGHT_WORD, MACHINE_COPY, CANCEL, MACHINE_COMPLEX,
        LINK, NAVIGATION_CIRCLE,
    };
    for (ImageType type : types) {
        out.push_back(new QPixmap(machinePath(type)));
    }
}

void VmtIcons::refreshInterfaceIcons(QWidget* root)
{
    if (!root) {
        return;
    }
    refreshEditorToolbars(root);
    for (FormEditorMachine* editor : root->findChildren<FormEditorMachine*>()) {
        editor->refreshInterfaceIcons();
    }
}
