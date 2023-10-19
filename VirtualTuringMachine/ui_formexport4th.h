/********************************************************************************
** Form generated from reading UI file 'formexport4th.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMEXPORT4TH_H
#define UI_FORMEXPORT4TH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormExport4th
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_5;
    QToolButton *_export;
    QToolButton *_copy;
    QFrame *frame_5;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *_text;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLineEdit *_delimiter;
    QFrame *frame_11;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *_left;
    QFrame *frame_12;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLineEdit *_right;
    QFrame *frame_13;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QLineEdit *_stop;
    QFrame *frame_14;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QLineEdit *_quotes;
    QFrame *frame_15;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *information;

    void setupUi(QWidget *FormExport4th)
    {
        if (FormExport4th->objectName().isEmpty())
            FormExport4th->setObjectName(QStringLiteral("FormExport4th"));
        FormExport4th->resize(683, 439);
        horizontalLayout = new QHBoxLayout(FormExport4th);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame_4 = new QFrame(FormExport4th);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setStyleSheet(QStringLiteral("background:#01547a"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_4);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        _export = new QToolButton(frame_4);
        _export->setObjectName(QStringLiteral("_export"));
        _export->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Files/images/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        _export->setIcon(icon);
        _export->setIconSize(QSize(32, 32));

        verticalLayout_5->addWidget(_export);

        _copy = new QToolButton(frame_4);
        _copy->setObjectName(QStringLiteral("_copy"));
        _copy->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Files/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        _copy->setIcon(icon1);
        _copy->setIconSize(QSize(32, 32));

        verticalLayout_5->addWidget(_copy);

        frame_5 = new QFrame(frame_4);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::NoFrame);
        frame_5->setFrameShadow(QFrame::Raised);

        verticalLayout_5->addWidget(frame_5);


        horizontalLayout->addWidget(frame_4);

        frame = new QFrame(FormExport4th);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setMinimumSize(QSize(0, 50));
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        _text = new QTextEdit(frame_6);
        _text->setObjectName(QStringLiteral("_text"));

        horizontalLayout_3->addWidget(_text);

        frame_7 = new QFrame(frame_6);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy1);
        frame_7->setMinimumSize(QSize(300, 0));
        frame_7->setFrameShape(QFrame::NoFrame);
        frame_7->setFrameShadow(QFrame::Plain);
        verticalLayout_4 = new QVBoxLayout(frame_7);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        frame_10 = new QFrame(frame_7);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setFrameShape(QFrame::NoFrame);
        frame_10->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_10);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(frame_10);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_2);

        _delimiter = new QLineEdit(frame_10);
        _delimiter->setObjectName(QStringLiteral("_delimiter"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(_delimiter->sizePolicy().hasHeightForWidth());
        _delimiter->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(_delimiter);


        verticalLayout_4->addWidget(frame_10);

        frame_11 = new QFrame(frame_7);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setFrameShape(QFrame::NoFrame);
        frame_11->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(frame_11);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_3);

        _left = new QLineEdit(frame_11);
        _left->setObjectName(QStringLiteral("_left"));
        sizePolicy2.setHeightForWidth(_left->sizePolicy().hasHeightForWidth());
        _left->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(_left);


        verticalLayout_4->addWidget(frame_11);

        frame_12 = new QFrame(frame_7);
        frame_12->setObjectName(QStringLiteral("frame_12"));
        frame_12->setFrameShape(QFrame::NoFrame);
        frame_12->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_12);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(frame_12);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(label_4);

        _right = new QLineEdit(frame_12);
        _right->setObjectName(QStringLiteral("_right"));
        sizePolicy2.setHeightForWidth(_right->sizePolicy().hasHeightForWidth());
        _right->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(_right);


        verticalLayout_4->addWidget(frame_12);

        frame_13 = new QFrame(frame_7);
        frame_13->setObjectName(QStringLiteral("frame_13"));
        frame_13->setFrameShape(QFrame::NoFrame);
        frame_13->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_13);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(frame_13);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(label_5);

        _stop = new QLineEdit(frame_13);
        _stop->setObjectName(QStringLiteral("_stop"));
        sizePolicy2.setHeightForWidth(_stop->sizePolicy().hasHeightForWidth());
        _stop->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(_stop);


        verticalLayout_4->addWidget(frame_13);

        frame_14 = new QFrame(frame_7);
        frame_14->setObjectName(QStringLiteral("frame_14"));
        frame_14->setFrameShape(QFrame::NoFrame);
        frame_14->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frame_14);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(frame_14);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(label_6);

        _quotes = new QLineEdit(frame_14);
        _quotes->setObjectName(QStringLiteral("_quotes"));
        sizePolicy2.setHeightForWidth(_quotes->sizePolicy().hasHeightForWidth());
        _quotes->setSizePolicy(sizePolicy2);

        horizontalLayout_9->addWidget(_quotes);


        verticalLayout_4->addWidget(frame_14);

        frame_15 = new QFrame(frame_7);
        frame_15->setObjectName(QStringLiteral("frame_15"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_15->sizePolicy().hasHeightForWidth());
        frame_15->setSizePolicy(sizePolicy3);
        frame_15->setFrameShape(QFrame::NoFrame);
        frame_15->setFrameShadow(QFrame::Raised);

        verticalLayout_4->addWidget(frame_15);


        horizontalLayout_3->addWidget(frame_7);


        verticalLayout_3->addWidget(frame_6);


        verticalLayout->addWidget(frame_2);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        sizePolicy.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy);
        frame_3->setMinimumSize(QSize(0, 0));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        information = new QLabel(frame_3);
        information->setObjectName(QStringLiteral("information"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(information->sizePolicy().hasHeightForWidth());
        information->setSizePolicy(sizePolicy4);
        QFont font;
        font.setItalic(true);
        information->setFont(font);
        information->setTextFormat(Qt::AutoText);
        information->setScaledContents(false);
        information->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(information);


        verticalLayout->addWidget(frame_3);


        horizontalLayout->addWidget(frame);


        retranslateUi(FormExport4th);

        QMetaObject::connectSlotsByName(FormExport4th);
    } // setupUi

    void retranslateUi(QWidget *FormExport4th)
    {
        FormExport4th->setWindowTitle(QApplication::translate("FormExport4th", "Form", Q_NULLPTR));
        _export->setText(QString());
        _copy->setText(QString());
        label_2->setText(QApplication::translate("FormExport4th", "Inline delimiter", Q_NULLPTR));
        _delimiter->setText(QApplication::translate("FormExport4th", ",", Q_NULLPTR));
        label_3->setText(QApplication::translate("FormExport4th", "Left", Q_NULLPTR));
        _left->setText(QApplication::translate("FormExport4th", "<", Q_NULLPTR));
        label_4->setText(QApplication::translate("FormExport4th", "Right", Q_NULLPTR));
        _right->setText(QApplication::translate("FormExport4th", ">", Q_NULLPTR));
        label_5->setText(QApplication::translate("FormExport4th", "Stop", Q_NULLPTR));
        _stop->setText(QApplication::translate("FormExport4th", "#", Q_NULLPTR));
        label_6->setText(QApplication::translate("FormExport4th", "Write quotes", Q_NULLPTR));
        _quotes->setText(QString());
        information->setText(QApplication::translate("FormExport4th", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FormExport4th: public Ui_FormExport4th {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMEXPORT4TH_H
