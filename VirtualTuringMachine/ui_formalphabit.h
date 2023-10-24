/********************************************************************************
** Form generated from reading UI file 'formalphabit.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMALPHABIT_H
#define UI_FORMALPHABIT_H

#include <QtCore/QVariant>
#include <QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormAlphabit
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;

    void setupUi(QWidget *FormAlphabit)
    {
        if (FormAlphabit->objectName().isEmpty())
            FormAlphabit->setObjectName(QStringLiteral("FormAlphabit"));
        FormAlphabit->resize(413, 174);
        horizontalLayout = new QHBoxLayout(FormAlphabit);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(FormAlphabit);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);

        horizontalLayout->addWidget(frame);


        retranslateUi(FormAlphabit);

        QMetaObject::connectSlotsByName(FormAlphabit);
    } // setupUi

    void retranslateUi(QWidget *FormAlphabit)
    {
        FormAlphabit->setWindowTitle(QApplication::translate("FormAlphabit", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormAlphabit: public Ui_FormAlphabit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMALPHABIT_H
