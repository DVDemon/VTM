/********************************************************************************
** Form generated from reading UI file 'formwarning.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMWARNING_H
#define UI_FORMWARNING_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormWarning
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *label;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *button_ok;
    QPushButton *button_cancel;

    void setupUi(QWidget *FormWarning)
    {
        if (FormWarning->objectName().isEmpty())
            FormWarning->setObjectName(QString::fromUtf8("FormWarning"));
        FormWarning->resize(400, 300);
        verticalLayout = new QVBoxLayout(FormWarning);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(FormWarning);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Files/images/warning.png")));

        horizontalLayout_2->addWidget(label_2);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(FormWarning);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        button_ok = new QPushButton(frame_2);
        button_ok->setObjectName(QString::fromUtf8("button_ok"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok->setIcon(icon);

        horizontalLayout->addWidget(button_ok);

        button_cancel = new QPushButton(frame_2);
        button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Files/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_cancel->setIcon(icon1);

        horizontalLayout->addWidget(button_cancel);


        verticalLayout->addWidget(frame_2);


        retranslateUi(FormWarning);

        QMetaObject::connectSlotsByName(FormWarning);
    } // setupUi

    void retranslateUi(QWidget *FormWarning)
    {
        FormWarning->setWindowTitle(QCoreApplication::translate("FormWarning", "Form", nullptr));
        label_2->setText(QString());
        label->setText(QCoreApplication::translate("FormWarning", "Are you sure?", nullptr));
        button_ok->setText(QCoreApplication::translate("FormWarning", "Ok", nullptr));
        button_cancel->setText(QCoreApplication::translate("FormWarning", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormWarning: public Ui_FormWarning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMWARNING_H
