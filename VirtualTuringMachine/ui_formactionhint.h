/********************************************************************************
** Form generated from reading UI file 'formactionhint.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMACTIONHINT_H
#define UI_FORMACTIONHINT_H

#include <QtCore/QVariant>
#include <QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormActionHint
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label_img;
    QLabel *label_text;

    void setupUi(QWidget *FormActionHint)
    {
        if (FormActionHint->objectName().isEmpty())
            FormActionHint->setObjectName(QStringLiteral("FormActionHint"));
        FormActionHint->resize(400, 46);
        horizontalLayout = new QHBoxLayout(FormActionHint);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_img = new QLabel(FormActionHint);
        label_img->setObjectName(QStringLiteral("label_img"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_img->sizePolicy().hasHeightForWidth());
        label_img->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_img);

        label_text = new QLabel(FormActionHint);
        label_text->setObjectName(QStringLiteral("label_text"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_text->sizePolicy().hasHeightForWidth());
        label_text->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label_text);


        retranslateUi(FormActionHint);

        QMetaObject::connectSlotsByName(FormActionHint);
    } // setupUi

    void retranslateUi(QWidget *FormActionHint)
    {
        FormActionHint->setWindowTitle(QApplication::translate("FormActionHint", "Form", Q_NULLPTR));
        label_img->setText(QApplication::translate("FormActionHint", "TextLabel", Q_NULLPTR));
        label_text->setText(QApplication::translate("FormActionHint", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormActionHint: public Ui_FormActionHint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMACTIONHINT_H
