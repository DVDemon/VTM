/********************************************************************************
** Form generated from reading UI file 'formcalculator.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMCALCULATOR_H
#define UI_FORMCALCULATOR_H

#include <QtCore/QVariant>
#include <QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormCalculator
{
public:
    QHBoxLayout *horizontalLayout;
    QLineEdit *text_power;
    QToolButton *_plus;
    QToolButton *_minus;

    void setupUi(QWidget *FormCalculator)
    {
        if (FormCalculator->objectName().isEmpty())
            FormCalculator->setObjectName(QStringLiteral("FormCalculator"));
        FormCalculator->resize(618, 26);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FormCalculator->sizePolicy().hasHeightForWidth());
        FormCalculator->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(FormCalculator);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        text_power = new QLineEdit(FormCalculator);
        text_power->setObjectName(QStringLiteral("text_power"));

        horizontalLayout->addWidget(text_power);

        _plus = new QToolButton(FormCalculator);
        _plus->setObjectName(QStringLiteral("_plus"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/plus.png"), QSize(), QIcon::Normal, QIcon::Off);
        _plus->setIcon(icon);

        horizontalLayout->addWidget(_plus);

        _minus = new QToolButton(FormCalculator);
        _minus->setObjectName(QStringLiteral("_minus"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        _minus->setIcon(icon1);

        horizontalLayout->addWidget(_minus);


        retranslateUi(FormCalculator);

        QMetaObject::connectSlotsByName(FormCalculator);
    } // setupUi

    void retranslateUi(QWidget *FormCalculator)
    {
        FormCalculator->setWindowTitle(QApplication::translate("FormCalculator", "Form", Q_NULLPTR));
        _plus->setText(QString());
        _minus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FormCalculator: public Ui_FormCalculator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMCALCULATOR_H
