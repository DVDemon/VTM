/********************************************************************************
** Form generated from reading UI file 'formmain.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMMAIN_H
#define UI_FORMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormMain
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QToolButton *button_new;
    QToolButton *button_open;
    QToolButton *toolButton;
    QFrame *frame_3;
    QToolButton *toolButton_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QListWidget *listWidget;

    void setupUi(QWidget *FormMain)
    {
        if (FormMain->objectName().isEmpty())
            FormMain->setObjectName(QStringLiteral("FormMain"));
        FormMain->resize(755, 462);
        FormMain->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(FormMain);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(FormMain);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setMinimumSize(QSize(500, 0));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 755, 462));
        scrollAreaWidgetContents->setAutoFillBackground(true);
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(scrollAreaWidgetContents);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setLayoutDirection(Qt::LeftToRight);
        frame->setStyleSheet(QStringLiteral("background:#01547a"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(24);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(12, 12, 12, -1);
        button_new = new QToolButton(frame);
        button_new->setObjectName(QStringLiteral("button_new"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button_new->sizePolicy().hasHeightForWidth());
        button_new->setSizePolicy(sizePolicy1);
        button_new->setStyleSheet(QStringLiteral("background:#01547a"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_new.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_new->setIcon(icon);
        button_new->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_new);

        button_open = new QToolButton(frame);
        button_open->setObjectName(QStringLiteral("button_open"));
        sizePolicy1.setHeightForWidth(button_open->sizePolicy().hasHeightForWidth());
        button_open->setSizePolicy(sizePolicy1);
        button_open->setStyleSheet(QStringLiteral("background:#01547a"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/toolbars/main/icon_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_open->setIcon(icon1);
        button_open->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_open);

        toolButton = new QToolButton(frame);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        sizePolicy1.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
        toolButton->setSizePolicy(sizePolicy1);
        toolButton->setStyleSheet(QStringLiteral("background:#01547a"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Files/images/exercise.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon2);
        toolButton->setIconSize(QSize(32, 32));

        verticalLayout_2->addWidget(toolButton);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy2);
        frame_3->setLineWidth(0);

        verticalLayout_2->addWidget(frame_3);

        toolButton_2 = new QToolButton(frame);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy3);
        toolButton_2->setStyleSheet(QStringLiteral("background:#01547a"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Files/images/emblema_mai.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon3);
        toolButton_2->setIconSize(QSize(64, 64));

        verticalLayout_2->addWidget(toolButton_2);


        horizontalLayout->addWidget(frame);

        frame_2 = new QFrame(scrollAreaWidgetContents);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(12, 12, 12, -1);
        label = new QLabel(frame_2);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setWordWrap(true);

        verticalLayout_3->addWidget(label);

        listWidget = new QListWidget(frame_2);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setStyleSheet(QStringLiteral("background:white"));

        verticalLayout_3->addWidget(listWidget);


        horizontalLayout->addWidget(frame_2);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(FormMain);

        QMetaObject::connectSlotsByName(FormMain);
    } // setupUi

    void retranslateUi(QWidget *FormMain)
    {
        FormMain->setWindowTitle(QApplication::translate("FormMain", "Form", Q_NULLPTR));
        button_new->setText(QString());
        button_open->setText(QString());
        toolButton->setText(QString());
        toolButton_2->setText(QString());
        label->setText(QApplication::translate("FormMain", "Recent projects", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormMain: public Ui_FormMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMAIN_H
