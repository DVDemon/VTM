/********************************************************************************
** Form generated from reading UI file 'formnewproject.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMNEWPROJECT_H
#define UI_FORMNEWPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormNewProject
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QToolButton *button_create;
    QFrame *frame_2;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *text_project_name;
    QLabel *label_3;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *text_project_location;
    QToolButton *button_project_location;
    QLabel *label_4;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_6;
    QToolButton *button_z2;
    QToolButton *button_z3;
    QToolButton *button_z4;
    QToolButton *button_z10;
    QToolButton *button_z16;
    QToolButton *button_z80;
    QWidget *widget_6;
    QTextEdit *text_alphabit;

    void setupUi(QWidget *FormNewProject)
    {
        if (FormNewProject->objectName().isEmpty())
            FormNewProject->setObjectName(QStringLiteral("FormNewProject"));
        FormNewProject->resize(549, 571);
        horizontalLayout = new QHBoxLayout(FormNewProject);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(FormNewProject);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 549, 571));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(scrollAreaWidgetContents);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setStyleSheet(QStringLiteral("background:#01547a"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        button_create = new QToolButton(frame);
        button_create->setObjectName(QStringLiteral("button_create"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button_create->sizePolicy().hasHeightForWidth());
        button_create->setSizePolicy(sizePolicy1);
        button_create->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_create->setIcon(icon);
        button_create->setIconSize(QSize(32, 32));
        button_create->setAutoRaise(true);

        verticalLayout->addWidget(button_create);

        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame_2);


        horizontalLayout_2->addWidget(frame);

        frame_5 = new QFrame(scrollAreaWidgetContents);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy3);
        frame_5->setFrameShape(QFrame::NoFrame);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_5);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(frame_5);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy4);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        text_project_name = new QLineEdit(frame_5);
        text_project_name->setObjectName(QStringLiteral("text_project_name"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(text_project_name->sizePolicy().hasHeightForWidth());
        text_project_name->setSizePolicy(sizePolicy5);

        verticalLayout_2->addWidget(text_project_name);

        label_3 = new QLabel(frame_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy6);
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        frame_3 = new QFrame(frame_5);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        text_project_location = new QLineEdit(frame_3);
        text_project_location->setObjectName(QStringLiteral("text_project_location"));

        horizontalLayout_3->addWidget(text_project_location);

        button_project_location = new QToolButton(frame_3);
        button_project_location->setObjectName(QStringLiteral("button_project_location"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(button_project_location->sizePolicy().hasHeightForWidth());
        button_project_location->setSizePolicy(sizePolicy7);

        horizontalLayout_3->addWidget(button_project_location);


        verticalLayout_2->addWidget(frame_3);

        label_4 = new QLabel(frame_5);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);

        verticalLayout_2->addWidget(label_4);

        widget_4 = new QWidget(frame_5);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setEnabled(true);
        sizePolicy6.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy6);
        widget_4->setLayoutDirection(Qt::LeftToRight);
        widget_4->setStyleSheet(QStringLiteral(""));
        horizontalLayout_6 = new QHBoxLayout(widget_4);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        button_z2 = new QToolButton(widget_4);
        button_z2->setObjectName(QStringLiteral("button_z2"));
        button_z2->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/alphabet/z2.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z2->setIcon(icon1);
        button_z2->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z2);

        button_z3 = new QToolButton(widget_4);
        button_z3->setObjectName(QStringLiteral("button_z3"));
        button_z3->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Files/images/alphabet/z3.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z3->setIcon(icon2);
        button_z3->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z3);

        button_z4 = new QToolButton(widget_4);
        button_z4->setObjectName(QStringLiteral("button_z4"));
        button_z4->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Files/images/alphabet/z8.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z4->setIcon(icon3);
        button_z4->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z4);

        button_z10 = new QToolButton(widget_4);
        button_z10->setObjectName(QStringLiteral("button_z10"));
        button_z10->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Files/images/alphabet/z10.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z10->setIcon(icon4);
        button_z10->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z10);

        button_z16 = new QToolButton(widget_4);
        button_z16->setObjectName(QStringLiteral("button_z16"));
        button_z16->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Files/images/alphabet/z16.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z16->setIcon(icon5);
        button_z16->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z16);

        button_z80 = new QToolButton(widget_4);
        button_z80->setObjectName(QStringLiteral("button_z80"));
        button_z80->setStyleSheet(QStringLiteral("background:white"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Files/images/alphabet/z80.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_z80->setIcon(icon6);
        button_z80->setIconSize(QSize(32, 32));

        horizontalLayout_6->addWidget(button_z80);

        widget_6 = new QWidget(widget_4);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy8);

        horizontalLayout_6->addWidget(widget_6);


        verticalLayout_2->addWidget(widget_4);

        text_alphabit = new QTextEdit(frame_5);
        text_alphabit->setObjectName(QStringLiteral("text_alphabit"));
        text_alphabit->setLayoutDirection(Qt::LeftToRight);

        verticalLayout_2->addWidget(text_alphabit);


        horizontalLayout_2->addWidget(frame_5);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(FormNewProject);

        QMetaObject::connectSlotsByName(FormNewProject);
    } // setupUi

    void retranslateUi(QWidget *FormNewProject)
    {
        FormNewProject->setWindowTitle(QApplication::translate("FormNewProject", "Form", Q_NULLPTR));
        button_create->setText(QString());
        label->setText(QApplication::translate("FormNewProject", "Project Name", Q_NULLPTR));
        label_3->setText(QApplication::translate("FormNewProject", "Location", Q_NULLPTR));
        button_project_location->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        label_4->setText(QApplication::translate("FormNewProject", "Alphabet", Q_NULLPTR));
        button_z2->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        button_z3->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        button_z4->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        button_z10->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        button_z16->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
        button_z80->setText(QApplication::translate("FormNewProject", "...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormNewProject: public Ui_FormNewProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMNEWPROJECT_H
