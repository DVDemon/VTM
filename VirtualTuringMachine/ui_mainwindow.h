/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
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
    QToolButton *button_export_plantuml;
    QToolButton *button_run;
    QToolButton *button_configure;
    QToolButton *button_theme;
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
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(816, 426);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setBaseSize(QSize(800, 600));
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setDocumentMode(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setStyleSheet(QString::fromUtf8("background:#012C40"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame_header = new QFrame(centralWidget);
        frame_header->setObjectName(QString::fromUtf8("frame_header"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_header->sizePolicy().hasHeightForWidth());
        frame_header->setSizePolicy(sizePolicy2);
        frame_header->setAutoFillBackground(false);
        frame_header->setStyleSheet(QString::fromUtf8(""));
        frame_header->setFrameShape(QFrame::NoFrame);
        frame_header->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame_header);
        horizontalLayout_2->setSpacing(-1);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(4, 4, 4, 4);
        button_back = new QToolButton(frame_header);
        button_back->setObjectName(QString::fromUtf8("button_back"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(button_back->sizePolicy().hasHeightForWidth());
        button_back->setSizePolicy(sizePolicy3);
        button_back->setAutoFillBackground(false);
        button_back->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Files/images/toolbars/main/undo.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_back->setIcon(icon);
        button_back->setIconSize(QSize(24, 24));
        button_back->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_back->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_back);

        button_debug_back = new QToolButton(frame_header);
        button_debug_back->setObjectName(QString::fromUtf8("button_debug_back"));
        button_debug_back->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/toolbars/debug/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_back->setIcon(icon1);
        button_debug_back->setIconSize(QSize(24, 24));
        button_debug_back->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_debug_back->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_debug_back);

        widgetLabel = new QWidget(frame_header);
        widgetLabel->setObjectName(QString::fromUtf8("widgetLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widgetLabel->sizePolicy().hasHeightForWidth());
        widgetLabel->setSizePolicy(sizePolicy4);
        horizontalLayout = new QHBoxLayout(widgetLabel);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widgetLabel);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QString::fromUtf8("background:transparent;color:white"));

        horizontalLayout->addWidget(label);


        horizontalLayout_2->addWidget(widgetLabel);

        button_ok = new QToolButton(frame_header);
        button_ok->setObjectName(QString::fromUtf8("button_ok"));
        button_ok->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok->setIcon(icon2);
        button_ok->setIconSize(QSize(24, 24));
        button_ok->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_ok);

        button_cancel = new QToolButton(frame_header);
        button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
        button_cancel->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Files/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_cancel->setIcon(icon3);
        button_cancel->setIconSize(QSize(24, 24));
        button_cancel->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_cancel);

        button_4th = new QToolButton(frame_header);
        button_4th->setObjectName(QString::fromUtf8("button_4th"));
        button_4th->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Files/images/toolbars/tools/e4.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_4th->setIcon(icon4);
        button_4th->setIconSize(QSize(24, 24));
        button_4th->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_4th);

        button_exercise = new QToolButton(frame_header);
        button_exercise->setObjectName(QString::fromUtf8("button_exercise"));
        button_exercise->setAutoFillBackground(false);
        button_exercise->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Files/images/exercise.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_exercise->setIcon(icon5);
        button_exercise->setIconSize(QSize(24, 24));
        button_exercise->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_exercise->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_exercise);

        button_new_machine = new QToolButton(frame_header);
        button_new_machine->setObjectName(QString::fromUtf8("button_new_machine"));
        button_new_machine->setAutoFillBackground(false);
        button_new_machine->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_new_machine.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_new_machine->setIcon(icon6);
        button_new_machine->setIconSize(QSize(24, 24));
        button_new_machine->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_new_machine->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_new_machine);

        button_open_machine = new QToolButton(frame_header);
        button_open_machine->setObjectName(QString::fromUtf8("button_open_machine"));
        button_open_machine->setAutoFillBackground(false);
        button_open_machine->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_open_machine->setIcon(icon7);
        button_open_machine->setIconSize(QSize(24, 24));
        button_open_machine->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_open_machine->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_open_machine);

        button_save = new QToolButton(frame_header);
        button_save->setObjectName(QString::fromUtf8("button_save"));
        button_save->setAutoFillBackground(false);
        button_save->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_save.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_save->setIcon(icon8);
        button_save->setIconSize(QSize(24, 24));
        button_save->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_save->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_save);

        button_export = new QToolButton(frame_header);
        button_export->setObjectName(QString::fromUtf8("button_export"));
        button_export->setAutoFillBackground(false);
        button_export->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_export.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_export->setIcon(icon9);
        button_export->setIconSize(QSize(24, 24));
        button_export->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_export->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_export);

        button_export_plantuml = new QToolButton(frame_header);
        button_export_plantuml->setObjectName(QString::fromUtf8("button_export_plantuml"));
        button_export_plantuml->setIcon(icon9);
        button_export_plantuml->setIconSize(QSize(24, 24));
        button_export_plantuml->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button_export_plantuml->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_export_plantuml);

        button_run = new QToolButton(frame_header);
        button_run->setObjectName(QString::fromUtf8("button_run"));
        button_run->setAutoFillBackground(false);
        button_run->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_run.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_run->setIcon(icon10);
        button_run->setIconSize(QSize(24, 24));
        button_run->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_run->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_run);

        button_configure = new QToolButton(frame_header);
        button_configure->setObjectName(QString::fromUtf8("button_configure"));
        button_configure->setAutoFillBackground(false);
        button_configure->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_configure.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_configure->setIcon(icon11);
        button_configure->setIconSize(QSize(24, 24));
        button_configure->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button_configure->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_configure);

        button_theme = new QToolButton(frame_header);
        button_theme->setObjectName(QString::fromUtf8("button_theme"));
        button_theme->setCheckable(true);
        button_theme->setIconSize(QSize(24, 24));
        button_theme->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button_theme->setAutoRaise(true);

        horizontalLayout_2->addWidget(button_theme);

        button_debug_run = new QToolButton(frame_header);
        button_debug_run->setObjectName(QString::fromUtf8("button_debug_run"));
        button_debug_run->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Files/images/toolbars/debug/run.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_run->setIcon(icon12);
        button_debug_run->setIconSize(QSize(24, 24));
        button_debug_run->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_run);

        button_debug_pause = new QToolButton(frame_header);
        button_debug_pause->setObjectName(QString::fromUtf8("button_debug_pause"));
        button_debug_pause->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/Files/images/toolbars/debug/pause.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_pause->setIcon(icon13);
        button_debug_pause->setIconSize(QSize(24, 24));
        button_debug_pause->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_pause);

        button_debug_stop = new QToolButton(frame_header);
        button_debug_stop->setObjectName(QString::fromUtf8("button_debug_stop"));
        button_debug_stop->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/Files/images/toolbars/debug/stop.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_stop->setIcon(icon14);
        button_debug_stop->setIconSize(QSize(24, 24));
        button_debug_stop->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_stop);

        button_debug_over = new QToolButton(frame_header);
        button_debug_over->setObjectName(QString::fromUtf8("button_debug_over"));
        button_debug_over->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/Files/images/toolbars/debug/step_over.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_over->setIcon(icon15);
        button_debug_over->setIconSize(QSize(24, 24));
        button_debug_over->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_over);

        button_debug_step = new QToolButton(frame_header);
        button_debug_step->setObjectName(QString::fromUtf8("button_debug_step"));
        button_debug_step->setStyleSheet(QString::fromUtf8("background:#012C40"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/Files/images/toolbars/debug/step.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_debug_step->setIcon(icon16);
        button_debug_step->setIconSize(QSize(24, 24));
        button_debug_step->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(button_debug_step);


        verticalLayout->addWidget(frame_header);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy5);
        frame->setStyleSheet(QString::fromUtf8("background:white"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget_container = new QWidget(frame);
        widget_container->setObjectName(QString::fromUtf8("widget_container"));
        sizePolicy5.setHeightForWidth(widget_container->sizePolicy().hasHeightForWidth());
        widget_container->setSizePolicy(sizePolicy5);
        widget_container->setMaximumSize(QSize(16777215, 16777215));
        widget_container->setAutoFillBackground(false);
        widget_container->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(widget_container);

        frame_zoom = new QFrame(frame);
        frame_zoom->setObjectName(QString::fromUtf8("frame_zoom"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(frame_zoom->sizePolicy().hasHeightForWidth());
        frame_zoom->setSizePolicy(sizePolicy6);
        frame_zoom->setMinimumSize(QSize(0, 0));
        frame_zoom->setMaximumSize(QSize(16777215, 16777215));
        frame_zoom->setFrameShape(QFrame::NoFrame);
        frame_zoom->setFrameShadow(QFrame::Plain);
        verticalLayout_2 = new QVBoxLayout(frame_zoom);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(12, 12, 12, 12);
        slider_zoom = new QSlider(frame_zoom);
        slider_zoom->setObjectName(QString::fromUtf8("slider_zoom"));
        sizePolicy6.setHeightForWidth(slider_zoom->sizePolicy().hasHeightForWidth());
        slider_zoom->setSizePolicy(sizePolicy6);
        slider_zoom->setMinimumSize(QSize(0, 0));
        slider_zoom->setMaximumSize(QSize(16, 16777215));
        slider_zoom->setMinimum(1);
        slider_zoom->setMaximum(200);
        slider_zoom->setValue(100);
        slider_zoom->setTracking(true);
        slider_zoom->setOrientation(Qt::Vertical);
        slider_zoom->setTickPosition(QSlider::TicksAbove);
        slider_zoom->setTickInterval(10);

        verticalLayout_2->addWidget(slider_zoom);

        label_2 = new QLabel(frame_zoom);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy7);
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(16, 16));
        label_2->setText(QString::fromUtf8(""));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Files/images/zoom.png")));
        label_2->setScaledContents(true);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_3->addWidget(frame_zoom);


        verticalLayout->addWidget(frame);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Virtual Turing Machine", nullptr));
        button_back->setText(QString());
        button_debug_back->setText(QString());
        button_debug_back->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_back", nullptr)));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        button_ok->setText(QCoreApplication::translate("MainWindow", "OK", nullptr));
        button_ok->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_ok", nullptr)));
        button_cancel->setText(QString());
        button_cancel->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_cancel", nullptr)));
        button_4th->setText(QString());
        button_4th->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_4th", nullptr)));
        button_exercise->setText(QString());
        button_exercise->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_exercise", nullptr)));
        button_new_machine->setText(QString());
        button_new_machine->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_new_machine", nullptr)));
        button_open_machine->setText(QString());
        button_open_machine->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_open_machine", nullptr)));
        button_save->setText(QString());
        button_save->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_save", nullptr)));
        button_export->setText(QString());
        button_export->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_export", nullptr)));
#if QT_CONFIG(tooltip)
        button_export_plantuml->setToolTip(QCoreApplication::translate("MainWindow", "Export diagram to PlantUML (.puml)", nullptr));
#endif // QT_CONFIG(tooltip)
        button_export_plantuml->setText(QCoreApplication::translate("MainWindow", "PUML", nullptr));
        button_export_plantuml->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_export_plantuml", nullptr)));
        button_run->setText(QString());
        button_run->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_run", nullptr)));
        button_configure->setText(QString());
        button_configure->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_configure", nullptr)));
#if QT_CONFIG(tooltip)
        button_theme->setToolTip(QCoreApplication::translate("MainWindow", "Dark theme", nullptr));
#endif // QT_CONFIG(tooltip)
        button_theme->setText(QCoreApplication::translate("MainWindow", "\360\237\214\231", nullptr));
        button_theme->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_theme", nullptr)));
        button_debug_run->setText(QString());
        button_debug_run->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_debug_run", nullptr)));
        button_debug_pause->setText(QString());
        button_debug_pause->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_debug_pause", nullptr)));
        button_debug_stop->setText(QString());
        button_debug_stop->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_debug_stop", nullptr)));
        button_debug_over->setText(QString());
        button_debug_over->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_debug_over", nullptr)));
        button_debug_step->setText(QString());
        button_debug_step->setProperty("name", QVariant(QCoreApplication::translate("MainWindow", "button_debug_step", nullptr)));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
