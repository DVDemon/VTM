/********************************************************************************
** Form generated from reading UI file 'formlinewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMLINEWIDGET_H
#define UI_FORMLINEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormLineWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;

    void setupUi(QWidget *FormLineWidget)
    {
        if (FormLineWidget->objectName().isEmpty())
            FormLineWidget->setObjectName(QString::fromUtf8("FormLineWidget"));
        FormLineWidget->resize(1023, 88);
        horizontalLayout = new QHBoxLayout(FormLineWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(FormLineWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);

        horizontalLayout->addWidget(frame);


        retranslateUi(FormLineWidget);

        QMetaObject::connectSlotsByName(FormLineWidget);
    } // setupUi

    void retranslateUi(QWidget *FormLineWidget)
    {
        FormLineWidget->setWindowTitle(QCoreApplication::translate("FormLineWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormLineWidget: public Ui_FormLineWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMLINEWIDGET_H
