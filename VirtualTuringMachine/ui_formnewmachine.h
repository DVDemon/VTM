/********************************************************************************
** Form generated from reading UI file 'formnewmachine.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMNEWMACHINE_H
#define UI_FORMNEWMACHINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            FormNewMachine->setObjectName(QStringLiteral("FormNewMachine"));
        FormNewMachine->resize(610, 515);
        FormNewMachine->setStyleSheet(QStringLiteral("background:white"));
        horizontalLayout = new QHBoxLayout(FormNewMachine);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame_2 = new QFrame(FormNewMachine);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setMinimumSize(QSize(0, 0));
        frame_2->setStyleSheet(QStringLiteral("background:#01547a"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        button_ok_2 = new QToolButton(frame_2);
        button_ok_2->setObjectName(QStringLiteral("button_ok_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(button_ok_2->sizePolicy().hasHeightForWidth());
        button_ok_2->setSizePolicy(sizePolicy);
        button_ok_2->setAutoFillBackground(false);
        button_ok_2->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok_2->setIcon(icon);
        button_ok_2->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_ok_2);

        button_delete_2 = new QToolButton(frame_2);
        button_delete_2->setObjectName(QStringLiteral("button_delete_2"));
        sizePolicy.setHeightForWidth(button_delete_2->sizePolicy().hasHeightForWidth());
        button_delete_2->setSizePolicy(sizePolicy);
        button_delete_2->setAutoFillBackground(false);
        button_delete_2->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/toolbars/main/bin.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_delete_2->setIcon(icon1);
        button_delete_2->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_delete_2);

        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName(QStringLiteral("frame_6"));
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
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 12, 12, 12);
        label = new QLabel(FormNewMachine);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        text_machine_name = new QLineEdit(FormNewMachine);
        text_machine_name->setObjectName(QStringLiteral("text_machine_name"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(text_machine_name->sizePolicy().hasHeightForWidth());
        text_machine_name->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(text_machine_name);

        frame = new QFrame(FormNewMachine);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy4);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout_3->addWidget(label_2);

        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setMinimumSize(QSize(50, 50));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_4);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        list_machines = new QListView(frame_4);
        list_machines->setObjectName(QStringLiteral("list_machines"));

        horizontalLayout_2->addWidget(list_machines);


        verticalLayout_3->addWidget(frame_4);


        verticalLayout->addWidget(frame);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(FormNewMachine);

        QMetaObject::connectSlotsByName(FormNewMachine);
    } // setupUi

    void retranslateUi(QWidget *FormNewMachine)
    {
        FormNewMachine->setWindowTitle(QApplication::translate("FormNewMachine", "Form", Q_NULLPTR));
        button_ok_2->setText(QString());
#ifndef QT_NO_SHORTCUT
        button_ok_2->setShortcut(QString());
#endif // QT_NO_SHORTCUT
        button_delete_2->setText(QString());
        label->setText(QApplication::translate("FormNewMachine", "Machine Name", Q_NULLPTR));
        label_2->setText(QApplication::translate("FormNewMachine", "Existing project machines", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormNewMachine: public Ui_FormNewMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMNEWMACHINE_H
