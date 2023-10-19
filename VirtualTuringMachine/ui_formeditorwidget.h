/********************************************************************************
** Form generated from reading UI file 'formeditorwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMEDITORWIDGET_H
#define UI_FORMEDITORWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormEditorWidget
{
public:

    void setupUi(QWidget *FormEditorWidget)
    {
        if (FormEditorWidget->objectName().isEmpty())
            FormEditorWidget->setObjectName(QStringLiteral("FormEditorWidget"));
        FormEditorWidget->resize(400, 300);

        retranslateUi(FormEditorWidget);

        QMetaObject::connectSlotsByName(FormEditorWidget);
    } // setupUi

    void retranslateUi(QWidget *FormEditorWidget)
    {
        FormEditorWidget->setWindowTitle(QApplication::translate("FormEditorWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormEditorWidget: public Ui_FormEditorWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMEDITORWIDGET_H
