/********************************************************************************
** Form generated from reading UI file 'formwarning.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMWARNING_H
#define UI_FORMWARNING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            FormWarning->setObjectName(QStringLiteral("FormWarning"));
        FormWarning->resize(400, 300);
        verticalLayout = new QVBoxLayout(FormWarning);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(FormWarning);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/Files/images/warning.png")));

        horizontalLayout_2->addWidget(label_2);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(FormWarning);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        button_ok = new QPushButton(frame_2);
        button_ok->setObjectName(QStringLiteral("button_ok"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_ok->setIcon(icon);

        horizontalLayout->addWidget(button_ok);

        button_cancel = new QPushButton(frame_2);
        button_cancel->setObjectName(QStringLiteral("button_cancel"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_cancel->setIcon(icon1);

        horizontalLayout->addWidget(button_cancel);


        verticalLayout->addWidget(frame_2);


        retranslateUi(FormWarning);

        QMetaObject::connectSlotsByName(FormWarning);
    } // setupUi

    void retranslateUi(QWidget *FormWarning)
    {
        FormWarning->setWindowTitle(QApplication::translate("FormWarning", "Form", Q_NULLPTR));
        label_2->setText(QString());
        label->setText(QApplication::translate("FormWarning", "Are you sure?", Q_NULLPTR));
        button_ok->setText(QApplication::translate("FormWarning", "Ok", Q_NULLPTR));
        button_cancel->setText(QApplication::translate("FormWarning", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormWarning: public Ui_FormWarning {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMWARNING_H
