/********************************************************************************
** Form generated from reading UI file 'formcompilermachine.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMCOMPILERMACHINE_H
#define UI_FORMCOMPILERMACHINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "formeditorwidget.h"

QT_BEGIN_NAMESPACE

class Ui_FormCompilerMachine
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    FormEditorWidget *widget_editor;
    QToolBox *toolBox;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QListWidget *list_errors;

    void setupUi(QWidget *FormCompilerMachine)
    {
        if (FormCompilerMachine->objectName().isEmpty())
            FormCompilerMachine->setObjectName(QStringLiteral("FormCompilerMachine"));
        FormCompilerMachine->resize(542, 434);
        verticalLayout = new QVBoxLayout(FormCompilerMachine);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(FormCompilerMachine);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        widget_editor = new FormEditorWidget(splitter);
        widget_editor->setObjectName(QStringLiteral("widget_editor"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(9);
        sizePolicy.setHeightForWidth(widget_editor->sizePolicy().hasHeightForWidth());
        widget_editor->setSizePolicy(sizePolicy);
        splitter->addWidget(widget_editor);
        toolBox = new QToolBox(splitter);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy1);
        toolBox->setFrameShadow(QFrame::Plain);
        toolBox->setLineWidth(0);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 542, 166));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 6, 6);
        list_errors = new QListWidget(page_2);
        list_errors->setObjectName(QStringLiteral("list_errors"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(list_errors->sizePolicy().hasHeightForWidth());
        list_errors->setSizePolicy(sizePolicy2);
        list_errors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        list_errors->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

        verticalLayout_3->addWidget(list_errors);

        toolBox->addItem(page_2, QStringLiteral("Messages"));
        splitter->addWidget(toolBox);

        verticalLayout->addWidget(splitter);


        retranslateUi(FormCompilerMachine);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FormCompilerMachine);
    } // setupUi

    void retranslateUi(QWidget *FormCompilerMachine)
    {
        FormCompilerMachine->setWindowTitle(QApplication::translate("FormCompilerMachine", "Form", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("FormCompilerMachine", "Messages", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormCompilerMachine: public Ui_FormCompilerMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMCOMPILERMACHINE_H
