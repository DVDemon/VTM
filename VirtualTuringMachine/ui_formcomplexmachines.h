/********************************************************************************
** Form generated from reading UI file 'formcomplexmachines.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMCOMPLEXMACHINES_H
#define UI_FORMCOMPLEXMACHINES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormComplexMachines
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *_frame;

    void setupUi(QWidget *FormComplexMachines)
    {
        if (FormComplexMachines->objectName().isEmpty())
            FormComplexMachines->setObjectName(QStringLiteral("FormComplexMachines"));
        FormComplexMachines->resize(400, 108);
        horizontalLayout = new QHBoxLayout(FormComplexMachines);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        _frame = new QFrame(FormComplexMachines);
        _frame->setObjectName(QStringLiteral("_frame"));
        _frame->setFrameShape(QFrame::NoFrame);
        _frame->setFrameShadow(QFrame::Raised);
        _frame->setLineWidth(0);

        horizontalLayout->addWidget(_frame);


        retranslateUi(FormComplexMachines);

        QMetaObject::connectSlotsByName(FormComplexMachines);
    } // setupUi

    void retranslateUi(QWidget *FormComplexMachines)
    {
        FormComplexMachines->setWindowTitle(QApplication::translate("FormComplexMachines", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormComplexMachines: public Ui_FormComplexMachines {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMCOMPLEXMACHINES_H
