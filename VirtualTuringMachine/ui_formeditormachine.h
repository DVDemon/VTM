/********************************************************************************
** Form generated from reading UI file 'formeditormachine.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMEDITORMACHINE_H
#define UI_FORMEDITORMACHINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "formeditorwidget.h"

QT_BEGIN_NAMESPACE

class Ui_FormEditorMachine
{
public:
    QVBoxLayout *verticalLayout_6;
    QSplitter *splitter;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QFrame *tool_frame;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_3;
    QGridLayout *gridLayout;
    QToolButton *tool_pointer;
    QToolButton *tool_machine_right;
    QToolButton *tool_machine_left_word;
    QToolButton *tool_translate;
    QToolButton *tool_machine_copy;
    QToolButton *tool_machine_lambda;
    QToolButton *tool_machine_finish;
    QToolButton *tool_trash;
    QToolButton *tool_machine_left;
    QToolButton *tool_machine_right_word;
    QToolButton *tool_machine_start;
    QToolButton *tool_link;
    QSpacerItem *verticalSpacer;
    QFrame *frame_6;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_8;
    QFrame *frame_navi;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *tool_complex;
    QToolButton *_navi;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_4;
    FormEditorWidget *widget_editor;
    QFrame *properties_frame;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_7;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label_image;
    QLabel *label_title;
    QFrame *properties;

    void setupUi(QWidget *FormEditorMachine)
    {
        if (FormEditorMachine->objectName().isEmpty())
            FormEditorMachine->setObjectName(QStringLiteral("FormEditorMachine"));
        FormEditorMachine->resize(600, 440);
        verticalLayout_6 = new QVBoxLayout(FormEditorMachine);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(FormEditorMachine);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        frame = new QFrame(splitter);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tool_frame = new QFrame(frame);
        tool_frame->setObjectName(QStringLiteral("tool_frame"));
        tool_frame->setStyleSheet(QStringLiteral("color:white;background:#01547a;"));
        tool_frame->setFrameShape(QFrame::NoFrame);
        tool_frame->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(tool_frame);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        frame_3 = new QFrame(tool_frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_3->setLineWidth(0);
        gridLayout = new QGridLayout(frame_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(4, 4, 4, 4);
        tool_pointer = new QToolButton(frame_3);
        tool_pointer->setObjectName(QStringLiteral("tool_pointer"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tool_pointer->sizePolicy().hasHeightForWidth());
        tool_pointer->setSizePolicy(sizePolicy1);
        tool_pointer->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_arrow.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_pointer->setIcon(icon);
        tool_pointer->setIconSize(QSize(24, 24));
        tool_pointer->setCheckable(true);
        tool_pointer->setPopupMode(QToolButton::DelayedPopup);
        tool_pointer->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tool_pointer->setAutoRaise(false);

        gridLayout->addWidget(tool_pointer, 0, 0, 1, 1);

        tool_machine_right = new QToolButton(frame_3);
        tool_machine_right->setObjectName(QStringLiteral("tool_machine_right"));
        sizePolicy1.setHeightForWidth(tool_machine_right->sizePolicy().hasHeightForWidth());
        tool_machine_right->setSizePolicy(sizePolicy1);
        tool_machine_right->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_r_small.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_right->setIcon(icon1);
        tool_machine_right->setIconSize(QSize(24, 24));
        tool_machine_right->setCheckable(true);
        tool_machine_right->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_right, 4, 1, 1, 1);

        tool_machine_left_word = new QToolButton(frame_3);
        tool_machine_left_word->setObjectName(QStringLiteral("tool_machine_left_word"));
        sizePolicy1.setHeightForWidth(tool_machine_left_word->sizePolicy().hasHeightForWidth());
        tool_machine_left_word->setSizePolicy(sizePolicy1);
        tool_machine_left_word->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_l_big.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_left_word->setIcon(icon2);
        tool_machine_left_word->setIconSize(QSize(24, 24));
        tool_machine_left_word->setCheckable(true);
        tool_machine_left_word->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_left_word, 5, 0, 1, 1);

        tool_translate = new QToolButton(frame_3);
        tool_translate->setObjectName(QStringLiteral("tool_translate"));
        sizePolicy1.setHeightForWidth(tool_translate->sizePolicy().hasHeightForWidth());
        tool_translate->setSizePolicy(sizePolicy1);
        tool_translate->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_hand.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_translate->setIcon(icon3);
        tool_translate->setIconSize(QSize(24, 24));
        tool_translate->setCheckable(true);
        tool_translate->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_translate, 1, 0, 1, 1);

        tool_machine_copy = new QToolButton(frame_3);
        tool_machine_copy->setObjectName(QStringLiteral("tool_machine_copy"));
        sizePolicy1.setHeightForWidth(tool_machine_copy->sizePolicy().hasHeightForWidth());
        tool_machine_copy->setSizePolicy(sizePolicy1);
        tool_machine_copy->setStyleSheet(QStringLiteral(""));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_k.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_copy->setIcon(icon4);
        tool_machine_copy->setIconSize(QSize(24, 24));
        tool_machine_copy->setCheckable(true);
        tool_machine_copy->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_copy, 6, 0, 1, 1);

        tool_machine_lambda = new QToolButton(frame_3);
        tool_machine_lambda->setObjectName(QStringLiteral("tool_machine_lambda"));
        sizePolicy1.setHeightForWidth(tool_machine_lambda->sizePolicy().hasHeightForWidth());
        tool_machine_lambda->setSizePolicy(sizePolicy1);
        tool_machine_lambda->setStyleSheet(QStringLiteral(""));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_lambda.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_lambda->setIcon(icon5);
        tool_machine_lambda->setIconSize(QSize(24, 24));
        tool_machine_lambda->setCheckable(true);
        tool_machine_lambda->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_lambda, 6, 1, 1, 1);

        tool_machine_finish = new QToolButton(frame_3);
        tool_machine_finish->setObjectName(QStringLiteral("tool_machine_finish"));
        sizePolicy1.setHeightForWidth(tool_machine_finish->sizePolicy().hasHeightForWidth());
        tool_machine_finish->setSizePolicy(sizePolicy1);
        tool_machine_finish->setStyleSheet(QStringLiteral(""));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/toolbars/tools/icon_finish.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_finish->setIcon(icon6);
        tool_machine_finish->setIconSize(QSize(24, 24));
        tool_machine_finish->setCheckable(true);
        tool_machine_finish->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_finish, 3, 1, 1, 1);

        tool_trash = new QToolButton(frame_3);
        tool_trash->setObjectName(QStringLiteral("tool_trash"));
        sizePolicy1.setHeightForWidth(tool_trash->sizePolicy().hasHeightForWidth());
        tool_trash->setSizePolicy(sizePolicy1);
        tool_trash->setStyleSheet(QStringLiteral(""));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_delete.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_trash->setIcon(icon7);
        tool_trash->setIconSize(QSize(24, 24));
        tool_trash->setCheckable(true);
        tool_trash->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_trash, 1, 1, 1, 1);

        tool_machine_left = new QToolButton(frame_3);
        tool_machine_left->setObjectName(QStringLiteral("tool_machine_left"));
        sizePolicy1.setHeightForWidth(tool_machine_left->sizePolicy().hasHeightForWidth());
        tool_machine_left->setSizePolicy(sizePolicy1);
        tool_machine_left->setStyleSheet(QStringLiteral(""));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_l_small.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_left->setIcon(icon8);
        tool_machine_left->setIconSize(QSize(24, 24));
        tool_machine_left->setCheckable(true);
        tool_machine_left->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_left, 4, 0, 1, 1);

        tool_machine_right_word = new QToolButton(frame_3);
        tool_machine_right_word->setObjectName(QStringLiteral("tool_machine_right_word"));
        sizePolicy1.setHeightForWidth(tool_machine_right_word->sizePolicy().hasHeightForWidth());
        tool_machine_right_word->setSizePolicy(sizePolicy1);
        tool_machine_right_word->setStyleSheet(QStringLiteral(""));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_r_big.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_right_word->setIcon(icon9);
        tool_machine_right_word->setIconSize(QSize(24, 24));
        tool_machine_right_word->setCheckable(true);
        tool_machine_right_word->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_right_word, 5, 1, 1, 1);

        tool_machine_start = new QToolButton(frame_3);
        tool_machine_start->setObjectName(QStringLiteral("tool_machine_start"));
        sizePolicy1.setHeightForWidth(tool_machine_start->sizePolicy().hasHeightForWidth());
        tool_machine_start->setSizePolicy(sizePolicy1);
        tool_machine_start->setStyleSheet(QStringLiteral(""));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/toolbars/tools/icon_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        tool_machine_start->setIcon(icon10);
        tool_machine_start->setIconSize(QSize(24, 24));
        tool_machine_start->setCheckable(true);
        tool_machine_start->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_machine_start, 3, 0, 1, 1);

        tool_link = new QToolButton(frame_3);
        tool_link->setObjectName(QStringLiteral("tool_link"));
        sizePolicy1.setHeightForWidth(tool_link->sizePolicy().hasHeightForWidth());
        tool_link->setSizePolicy(sizePolicy1);
        tool_link->setStyleSheet(QStringLiteral(""));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_link.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_link->setIcon(icon11);
        tool_link->setIconSize(QSize(24, 24));
        tool_link->setCheckable(true);
        tool_link->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(tool_link, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(12, 12, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);


        verticalLayout_5->addWidget(frame_3);

        frame_6 = new QFrame(tool_frame);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy2);
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Raised);

        verticalLayout_5->addWidget(frame_6);

        frame_7 = new QFrame(tool_frame);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy3);
        frame_7->setFrameShape(QFrame::NoFrame);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_7);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(4, 4, 4, 4);
        frame_navi = new QFrame(frame_7);
        frame_navi->setObjectName(QStringLiteral("frame_navi"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame_navi->sizePolicy().hasHeightForWidth());
        frame_navi->setSizePolicy(sizePolicy4);
        frame_navi->setMinimumSize(QSize(0, 0));
        frame_navi->setFrameShape(QFrame::NoFrame);
        frame_navi->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_navi);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(4, 4, 4, 4);
        tool_complex = new QToolButton(frame_navi);
        tool_complex->setObjectName(QStringLiteral("tool_complex"));
        sizePolicy1.setHeightForWidth(tool_complex->sizePolicy().hasHeightForWidth());
        tool_complex->setSizePolicy(sizePolicy1);
        tool_complex->setStyleSheet(QStringLiteral(""));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/Files/images/toolbars/tools/icon_complex.gif"), QSize(), QIcon::Normal, QIcon::Off);
        tool_complex->setIcon(icon12);
        tool_complex->setIconSize(QSize(24, 24));
        tool_complex->setCheckable(false);
        tool_complex->setToolButtonStyle(Qt::ToolButtonIconOnly);

        horizontalLayout_2->addWidget(tool_complex);

        _navi = new QToolButton(frame_navi);
        _navi->setObjectName(QStringLiteral("_navi"));
        sizePolicy1.setHeightForWidth(_navi->sizePolicy().hasHeightForWidth());
        _navi->setSizePolicy(sizePolicy1);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/Files/images/nav.png"), QSize(), QIcon::Normal, QIcon::Off);
        _navi->setIcon(icon13);
        _navi->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(_navi);


        verticalLayout_8->addWidget(frame_navi);


        verticalLayout_5->addWidget(frame_7);


        verticalLayout_2->addWidget(tool_frame);

        splitter->addWidget(frame);
        frame_4 = new QFrame(splitter);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(9);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy5);
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(frame_4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_5 = new QWidget(frame_4);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(9);
        sizePolicy6.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy6);
        verticalLayout_4 = new QVBoxLayout(widget_5);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        widget_editor = new FormEditorWidget(widget_5);
        widget_editor->setObjectName(QStringLiteral("widget_editor"));
        sizePolicy2.setHeightForWidth(widget_editor->sizePolicy().hasHeightForWidth());
        widget_editor->setSizePolicy(sizePolicy2);
        widget_editor->setMinimumSize(QSize(128, 128));

        verticalLayout_4->addWidget(widget_editor);


        verticalLayout->addWidget(widget_5);

        properties_frame = new QFrame(frame_4);
        properties_frame->setObjectName(QStringLiteral("properties_frame"));
        sizePolicy4.setHeightForWidth(properties_frame->sizePolicy().hasHeightForWidth());
        properties_frame->setSizePolicy(sizePolicy4);
        properties_frame->setMinimumSize(QSize(0, 0));
        properties_frame->setFrameShape(QFrame::NoFrame);
        properties_frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(properties_frame);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame_2 = new QFrame(properties_frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        sizePolicy4.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy4);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame_2);
#ifndef Q_OS_MAC
        verticalLayout_7->setSpacing(-1);
#endif
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 12);
        frame_5 = new QFrame(frame_2);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        sizePolicy4.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy4);
        frame_5->setAutoFillBackground(false);
        frame_5->setStyleSheet(QStringLiteral("color:white;background:#0095da"));
        frame_5->setFrameShape(QFrame::VLine);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_5->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(frame_5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_image = new QLabel(frame_5);
        label_image->setObjectName(QStringLiteral("label_image"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(label_image->sizePolicy().hasHeightForWidth());
        label_image->setSizePolicy(sizePolicy7);
        QFont font;
        font.setPointSize(10);
        label_image->setFont(font);

        horizontalLayout->addWidget(label_image);

        label_title = new QLabel(frame_5);
        label_title->setObjectName(QStringLiteral("label_title"));
        sizePolicy3.setHeightForWidth(label_title->sizePolicy().hasHeightForWidth());
        label_title->setSizePolicy(sizePolicy3);
        label_title->setFont(font);

        horizontalLayout->addWidget(label_title);


        verticalLayout_7->addWidget(frame_5);

        properties = new QFrame(frame_2);
        properties->setObjectName(QStringLiteral("properties"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(properties->sizePolicy().hasHeightForWidth());
        properties->setSizePolicy(sizePolicy8);
        properties->setFrameShape(QFrame::NoFrame);
        properties->setFrameShadow(QFrame::Raised);
        properties->setLineWidth(0);

        verticalLayout_7->addWidget(properties);


        verticalLayout_3->addWidget(frame_2);


        verticalLayout->addWidget(properties_frame);

        splitter->addWidget(frame_4);

        verticalLayout_6->addWidget(splitter);


        retranslateUi(FormEditorMachine);

        QMetaObject::connectSlotsByName(FormEditorMachine);
    } // setupUi

    void retranslateUi(QWidget *FormEditorMachine)
    {
        FormEditorMachine->setWindowTitle(QApplication::translate("FormEditorMachine", "Form", Q_NULLPTR));
        tool_pointer->setText(QString());
        tool_machine_right->setText(QString());
        tool_machine_left_word->setText(QString());
        tool_translate->setText(QString());
        tool_machine_copy->setText(QString());
        tool_machine_lambda->setText(QString());
        tool_machine_finish->setText(QString());
        tool_trash->setText(QString());
        tool_machine_left->setText(QString());
        tool_machine_right_word->setText(QString());
        tool_machine_start->setText(QString());
        tool_link->setText(QString());
        tool_complex->setText(QString());
        _navi->setText(QString());
        label_image->setText(QApplication::translate("FormEditorMachine", "TextLabel", Q_NULLPTR));
        label_title->setText(QApplication::translate("FormEditorMachine", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormEditorMachine: public Ui_FormEditorMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMEDITORMACHINE_H
