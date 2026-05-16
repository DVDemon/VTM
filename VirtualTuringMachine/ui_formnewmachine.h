/********************************************************************************
** Form generated from reading UI file 'formnewmachine.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMNEWMACHINE_H
#define UI_FORMNEWMACHINE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormNewMachine
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QToolButton *button_ok_2;
    QToolButton *button_delete_2;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *text_machine_name;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_2;
    QListView *list_machines;

    void setupUi(QWidget *FormNewMachine)
    {
        if (FormNewMachine->objectName().isEmpty())
            FormNewMachine->setObjectName(QString::fromUtf8("FormNewMachine"));
        FormNewMachine->resize(610, 515);
        FormNewMachine->setStyleSheet(QString::fromUtf8("background:white;\n"
"color:black"));
        horizontalLayout = new QHBoxLayout(FormNewMachine);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame_2 = new QFrame(FormNewMachine);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(0, 0));
        frame_2->setStyleSheet(QString::fromUtf8("background:#01547a"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        button_ok_2 = new QToolButton(frame_2);
        button_ok_2->setObjectName(QString::fromUtf8("button_ok_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(button_ok_2->sizePolicy().hasHeightForWidth());
        button_ok_2->setSizePolicy(sizePolicy);
        button_ok_2->setAutoFillBackground(false);
        button_ok_2->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok_2->setIcon(icon);
        button_ok_2->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_ok_2);

        button_delete_2 = new QToolButton(frame_2);
        button_delete_2->setObjectName(QString::fromUtf8("button_delete_2"));
        sizePolicy.setHeightForWidth(button_delete_2->sizePolicy().hasHeightForWidth());
        button_delete_2->setSizePolicy(sizePolicy);
        button_delete_2->setAutoFillBackground(false);
        button_delete_2->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Files/images/toolbars/main/bin.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_delete_2->setIcon(icon1);
        button_delete_2->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_delete_2);

        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy1);
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame_6);


        horizontalLayout->addWidget(frame_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 12, 12, 12);
        label = new QLabel(FormNewMachine);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        QFont font;
        font.setBold(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        text_machine_name = new QLineEdit(FormNewMachine);
        text_machine_name->setObjectName(QString::fromUtf8("text_machine_name"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(text_machine_name->sizePolicy().hasHeightForWidth());
        text_machine_name->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(text_machine_name);

        frame = new QFrame(FormNewMachine);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy4);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(label_2);

        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMinimumSize(QSize(50, 50));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        list_machines = new QListView(frame_4);
        list_machines->setObjectName(QString::fromUtf8("list_machines"));

        horizontalLayout_2->addWidget(list_machines);


        verticalLayout_3->addWidget(frame_4);


        verticalLayout->addWidget(frame);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(FormNewMachine);

        QMetaObject::connectSlotsByName(FormNewMachine);
    } // setupUi

    void retranslateUi(QWidget *FormNewMachine)
    {
        FormNewMachine->setWindowTitle(QCoreApplication::translate("FormNewMachine", "Form", nullptr));
        button_ok_2->setText(QString());
#if QT_CONFIG(shortcut)
        button_ok_2->setShortcut(QString());
#endif // QT_CONFIG(shortcut)
        button_delete_2->setText(QString());
        label->setText(QCoreApplication::translate("FormNewMachine", "Machine Name", nullptr));
        label_2->setText(QCoreApplication::translate("FormNewMachine", "Existing project machines", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormNewMachine: public Ui_FormNewMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMNEWMACHINE_H
