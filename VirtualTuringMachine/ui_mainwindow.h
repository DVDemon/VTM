/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame_header;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *button_back;
    QToolButton *button_debug_back;
    QWidget *widgetLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QToolButton *button_ok;
    QToolButton *button_cancel;
    QToolButton *button_4th;
    QToolButton *button_exercise;
    QToolButton *button_new_machine;
    QToolButton *button_open_machine;
    QToolButton *button_save;
    QToolButton *button_export;
    QToolButton *button_run;
    QToolButton *button_configure;
    QToolButton *button_debug_run;
    QToolButton *button_debug_pause;
    QToolButton *button_debug_stop;
    QToolButton *button_debug_over;
    QToolButton *button_debug_step;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_container;
    QFrame *frame_zoom;
    QVBoxLayout *verticalLayout_2;
    QSlider *slider_zoom;
    QLabel *label_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(816, 426);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setBaseSize(QSize(800, 600));
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setDocumentMode(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setStyleSheet(QStringLiteral("background:#012C40"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame_header = new QFrame(centralWidget);
        frame_header->setObjectName(QStringLiteral("frame_header"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_header->sizePolicy().hasHeightForWidth());
        frame_header->setSizePolicy(sizePolicy2);
        frame_header->setAutoFillBackground(false);
        frame_header->setStyleSheet(QStringLiteral(""));
        frame_header->setFrameShape(QFrame::NoFrame);
        frame_header->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame_header);
        horizontalLayout_2->setSpacing(-1);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(4, 4, 4, 4);
        button_back = new QToolButton(frame_header);
        button_back->setObjectName(QStringLiteral("button_back"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(button_back->sizePolicy().hasHeightForWidth());
        button_back->setSizePolicy(sizePolicy3);
        button_back->setAutoFillBackground(false);
        button_back->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/toolbars/main/undo.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_back->setIcon(icon);
        button_back->setIconSize(QSize(24, 24));
        button_back->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_back->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_back);

        button_debug_back = new QToolButton(frame_header);
        button_debug_back->setObjectName(QStringLiteral("button_debug_back"));
        button_debug_back->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/toolbars/debug/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_back->setIcon(icon1);
        button_debug_back->setIconSize(QSize(24, 24));
        button_debug_back->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_debug_back->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_debug_back);

        widgetLabel = new QWidget(frame_header);
        widgetLabel->setObjectName(QStringLiteral("widgetLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widgetLabel->sizePolicy().hasHeightForWidth());
        widgetLabel->setSizePolicy(sizePolicy4);
        horizontalLayout = new QHBoxLayout(widgetLabel);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widgetLabel);
        label->setObjectName(QStringLiteral("label"));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QStringLiteral("background:transparent;color:white"));

        horizontalLayout->addWidget(label);


        horizontalLayout_2->addWidget(widgetLabel);

        button_ok = new QToolButton(frame_header);
        button_ok->setObjectName(QStringLiteral("button_ok"));
        button_ok->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok->setIcon(icon2);
        button_ok->setIconSize(QSize(24, 24));
        button_ok->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_ok);

        button_cancel = new QToolButton(frame_header);
        button_cancel->setObjectName(QStringLiteral("button_cancel"));
        button_cancel->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Files/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_cancel->setIcon(icon3);
        button_cancel->setIconSize(QSize(24, 24));
        button_cancel->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_cancel);

        button_4th = new QToolButton(frame_header);
        button_4th->setObjectName(QStringLiteral("button_4th"));
        button_4th->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Files/images/toolbars/tools/e4.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_4th->setIcon(icon4);
        button_4th->setIconSize(QSize(24, 24));
        button_4th->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_4th);

        button_exercise = new QToolButton(frame_header);
        button_exercise->setObjectName(QStringLiteral("button_exercise"));
        button_exercise->setAutoFillBackground(false);
        button_exercise->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Files/images/exercise.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_exercise->setIcon(icon5);
        button_exercise->setIconSize(QSize(24, 24));
        button_exercise->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_exercise->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_exercise);

        button_new_machine = new QToolButton(frame_header);
        button_new_machine->setObjectName(QStringLiteral("button_new_machine"));
        button_new_machine->setAutoFillBackground(false);
        button_new_machine->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_new_machine.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_new_machine->setIcon(icon6);
        button_new_machine->setIconSize(QSize(24, 24));
        button_new_machine->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_new_machine->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_new_machine);

        button_open_machine = new QToolButton(frame_header);
        button_open_machine->setObjectName(QStringLiteral("button_open_machine"));
        button_open_machine->setAutoFillBackground(false);
        button_open_machine->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_open_machine->setIcon(icon7);
        button_open_machine->setIconSize(QSize(24, 24));
        button_open_machine->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_open_machine->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_open_machine);

        button_save = new QToolButton(frame_header);
        button_save->setObjectName(QStringLiteral("button_save"));
        button_save->setAutoFillBackground(false);
        button_save->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_save.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_save->setIcon(icon8);
        button_save->setIconSize(QSize(24, 24));
        button_save->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_save->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_save);

        button_export = new QToolButton(frame_header);
        button_export->setObjectName(QStringLiteral("button_export"));
        button_export->setAutoFillBackground(false);
        button_export->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_export.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_export->setIcon(icon9);
        button_export->setIconSize(QSize(24, 24));
        button_export->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_export->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_export);

        button_run = new QToolButton(frame_header);
        button_run->setObjectName(QStringLiteral("button_run"));
        button_run->setAutoFillBackground(false);
        button_run->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_run.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_run->setIcon(icon10);
        button_run->setIconSize(QSize(24, 24));
        button_run->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_run->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_run);

        button_configure = new QToolButton(frame_header);
        button_configure->setObjectName(QStringLiteral("button_configure"));
        button_configure->setAutoFillBackground(false);
        button_configure->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_configure.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_configure->setIcon(icon11);
        button_configure->setIconSize(QSize(24, 24));
        button_configure->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_configure->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_configure);

        button_debug_run = new QToolButton(frame_header);
        button_debug_run->setObjectName(QStringLiteral("button_debug_run"));
        button_debug_run->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/Files/images/toolbars/debug/run.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_run->setIcon(icon12);
        button_debug_run->setIconSize(QSize(24, 24));
        button_debug_run->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_run);

        button_debug_pause = new QToolButton(frame_header);
        button_debug_pause->setObjectName(QStringLiteral("button_debug_pause"));
        button_debug_pause->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/Files/images/toolbars/debug/pause.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_pause->setIcon(icon13);
        button_debug_pause->setIconSize(QSize(24, 24));
        button_debug_pause->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_pause);

        button_debug_stop = new QToolButton(frame_header);
        button_debug_stop->setObjectName(QStringLiteral("button_debug_stop"));
        button_debug_stop->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/Files/images/toolbars/debug/stop.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_stop->setIcon(icon14);
        button_debug_stop->setIconSize(QSize(24, 24));
        button_debug_stop->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_stop);

        button_debug_over = new QToolButton(frame_header);
        button_debug_over->setObjectName(QStringLiteral("button_debug_over"));
        button_debug_over->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/Files/images/toolbars/debug/step_over.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_over->setIcon(icon15);
        button_debug_over->setIconSize(QSize(24, 24));
        button_debug_over->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_over);

        button_debug_step = new QToolButton(frame_header);
        button_debug_step->setObjectName(QStringLiteral("button_debug_step"));
        button_debug_step->setStyleSheet(QStringLiteral("background:#012C40"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/Files/images/toolbars/debug/step.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_step->setIcon(icon16);
        button_debug_step->setIconSize(QSize(24, 24));
        button_debug_step->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_step);


        verticalLayout->addWidget(frame_header);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy5);
        frame->setStyleSheet(QStringLiteral("background:white"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget_container = new QWidget(frame);
        widget_container->setObjectName(QStringLiteral("widget_container"));
        sizePolicy5.setHeightForWidth(widget_container->sizePolicy().hasHeightForWidth());
        widget_container->setSizePolicy(sizePolicy5);
        widget_container->setMaximumSize(QSize(16777215, 16777215));
        widget_container->setAutoFillBackground(false);
        widget_container->setStyleSheet(QStringLiteral(""));

        horizontalLayout_3->addWidget(widget_container);

        frame_zoom = new QFrame(frame);
        frame_zoom->setObjectName(QStringLiteral("frame_zoom"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(frame_zoom->sizePolicy().hasHeightForWidth());
        frame_zoom->setSizePolicy(sizePolicy6);
        frame_zoom->setMinimumSize(QSize(0, 20));
        frame_zoom->setFrameShape(QFrame::NoFrame);
        frame_zoom->setFrameShadow(QFrame::Plain);
        verticalLayout_2 = new QVBoxLayout(frame_zoom);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(12, 12, 12, 12);
        slider_zoom = new QSlider(frame_zoom);
        slider_zoom->setObjectName(QStringLiteral("slider_zoom"));
        sizePolicy6.setHeightForWidth(slider_zoom->sizePolicy().hasHeightForWidth());
        slider_zoom->setSizePolicy(sizePolicy6);
        slider_zoom->setMinimumSize(QSize(0, 0));
        slider_zoom->setMinimum(1);
        slider_zoom->setMaximum(500);
        slider_zoom->setValue(100);
        slider_zoom->setOrientation(Qt::Vertical);

        verticalLayout_2->addWidget(slider_zoom);

        label_2 = new QLabel(frame_zoom);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);
        label_2->setMinimumSize(QSize(32, 32));
        label_2->setMaximumSize(QSize(32, 32));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Files/images/zoom.png")));
        label_2->setScaledContents(true);

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_3->addWidget(frame_zoom);


        verticalLayout->addWidget(frame);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Virtual Turing Machine", Q_NULLPTR));
        button_back->setText(QString());
        button_debug_back->setText(QString());
        button_debug_back->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_back", Q_NULLPTR)));
        label->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        button_ok->setText(QApplication::translate("MainWindow", "OK", Q_NULLPTR));
        button_ok->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_ok", Q_NULLPTR)));
        button_cancel->setText(QString());
        button_cancel->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_cancel", Q_NULLPTR)));
        button_4th->setText(QString());
        button_4th->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_4th", Q_NULLPTR)));
        button_exercise->setText(QString());
        button_exercise->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_exercise", Q_NULLPTR)));
        button_new_machine->setText(QString());
        button_new_machine->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_new_machine", Q_NULLPTR)));
        button_open_machine->setText(QString());
        button_open_machine->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_open_machine", Q_NULLPTR)));
        button_save->setText(QString());
        button_save->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_save", Q_NULLPTR)));
        button_export->setText(QString());
        button_export->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_export", Q_NULLPTR)));
        button_run->setText(QString());
        button_run->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_run", Q_NULLPTR)));
        button_configure->setText(QString());
        button_configure->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_configure", Q_NULLPTR)));
        button_debug_run->setText(QString());
        button_debug_run->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_debug_run", Q_NULLPTR)));
        button_debug_pause->setText(QString());
        button_debug_pause->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_debug_pause", Q_NULLPTR)));
        button_debug_stop->setText(QString());
        button_debug_stop->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_debug_stop", Q_NULLPTR)));
        button_debug_over->setText(QString());
        button_debug_over->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_debug_over", Q_NULLPTR)));
        button_debug_step->setText(QString());
        button_debug_step->setProperty("name", QVariant(QApplication::translate("MainWindow", "button_debug_step", Q_NULLPTR)));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
