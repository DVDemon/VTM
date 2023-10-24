/********************************************************************************
** Form generated from reading UI file 'formdebugermachine.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMDEBUGERMACHINE_H
#define UI_FORMDEBUGERMACHINE_H

#include <QtCore/QVariant>
#include <QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "formeditorwidget.h"
#include "formlinewidget.h"

QT_BEGIN_NAMESPACE

class Ui_FormDebugerMachine
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *button_left_page;
    QToolButton *button_left;
    FormLineWidget *widget_line;
    QToolButton *button_right;
    QToolButton *button_right_page;
    QWidget *left_panel;
    QHBoxLayout *horizontalLayout_2;
    QFrame *_tests_frame;
    QVBoxLayout *verticalLayout_2;
    QListView *_test_list;
    FormEditorWidget *widget_editor;

    void setupUi(QWidget *FormDebugerMachine)
    {
        if (FormDebugerMachine->objectName().isEmpty())
            FormDebugerMachine->setObjectName(QStringLiteral("FormDebugerMachine"));
        FormDebugerMachine->resize(583, 452);
        verticalLayout = new QVBoxLayout(FormDebugerMachine);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(FormDebugerMachine);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setStyleSheet(QStringLiteral("color:black;background:white"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        button_left_page = new QToolButton(widget);
        button_left_page->setObjectName(QStringLiteral("button_left_page"));
        button_left_page->setStyleSheet(QStringLiteral("background:white;"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/left_page.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_left_page->setIcon(icon);
        button_left_page->setIconSize(QSize(32, 32));
        button_left_page->setAutoRaise(true);

        horizontalLayout->addWidget(button_left_page);

        button_left = new QToolButton(widget);
        button_left->setObjectName(QStringLiteral("button_left"));
        button_left->setStyleSheet(QStringLiteral("background:white;"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_left->setIcon(icon1);
        button_left->setIconSize(QSize(32, 32));
        button_left->setAutoRaise(true);

        horizontalLayout->addWidget(button_left);

        widget_line = new FormLineWidget(widget);
        widget_line->setObjectName(QStringLiteral("widget_line"));

        horizontalLayout->addWidget(widget_line);

        button_right = new QToolButton(widget);
        button_right->setObjectName(QStringLiteral("button_right"));
        button_right->setStyleSheet(QStringLiteral("background:white;"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Files/images/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_right->setIcon(icon2);
        button_right->setIconSize(QSize(32, 32));
        button_right->setAutoRaise(true);

        horizontalLayout->addWidget(button_right);

        button_right_page = new QToolButton(widget);
        button_right_page->setObjectName(QStringLiteral("button_right_page"));
        button_right_page->setStyleSheet(QStringLiteral("background:white;"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Files/images/right_page.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_right_page->setIcon(icon3);
        button_right_page->setIconSize(QSize(32, 32));
        button_right_page->setAutoRaise(true);

        horizontalLayout->addWidget(button_right_page);


        verticalLayout->addWidget(widget);

        left_panel = new QWidget(FormDebugerMachine);
        left_panel->setObjectName(QStringLiteral("left_panel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(left_panel->sizePolicy().hasHeightForWidth());
        left_panel->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(left_panel);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        _tests_frame = new QFrame(left_panel);
        _tests_frame->setObjectName(QStringLiteral("_tests_frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(_tests_frame->sizePolicy().hasHeightForWidth());
        _tests_frame->setSizePolicy(sizePolicy2);
        _tests_frame->setFrameShape(QFrame::NoFrame);
        _tests_frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(_tests_frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        _test_list = new QListView(_tests_frame);
        _test_list->setObjectName(QStringLiteral("_test_list"));
        sizePolicy1.setHeightForWidth(_test_list->sizePolicy().hasHeightForWidth());
        _test_list->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(_test_list);


        horizontalLayout_2->addWidget(_tests_frame, 0, Qt::AlignLeft);

        widget_editor = new FormEditorWidget(left_panel);
        widget_editor->setObjectName(QStringLiteral("widget_editor"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(9);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_editor->sizePolicy().hasHeightForWidth());
        widget_editor->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(widget_editor);


        verticalLayout->addWidget(left_panel);


        retranslateUi(FormDebugerMachine);

        QMetaObject::connectSlotsByName(FormDebugerMachine);
    } // setupUi

    void retranslateUi(QWidget *FormDebugerMachine)
    {
        FormDebugerMachine->setWindowTitle(QApplication::translate("FormDebugerMachine", "Form", Q_NULLPTR));
        button_left_page->setText(QString());
        button_left->setText(QString());
        button_right->setText(QString());
        button_right_page->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FormDebugerMachine: public Ui_FormDebugerMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMDEBUGERMACHINE_H
