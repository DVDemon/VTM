/********************************************************************************
** Form generated from reading UI file 'formexercises.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMEXERCISES_H
#define UI_FORMEXERCISES_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormExercises
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QToolButton *_ok;
    QToolButton *_open;
    QFrame *frame_3;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_3;
    QListView *_list;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *en;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *_text;
    QWidget *ru;
    QVBoxLayout *verticalLayout_5;
    QTextEdit *_text_ru;

    void setupUi(QWidget *FormExercises)
    {
        if (FormExercises->objectName().isEmpty())
            FormExercises->setObjectName(QString::fromUtf8("FormExercises"));
        FormExercises->resize(478, 300);
        FormExercises->setStyleSheet(QString::fromUtf8("background:white"));
        horizontalLayout = new QHBoxLayout(FormExercises);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(FormExercises);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setStyleSheet(QString::fromUtf8("background:#01547a"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _ok = new QToolButton(frame);
        _ok->setObjectName(QString::fromUtf8("_ok"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(_ok->sizePolicy().hasHeightForWidth());
        _ok->setSizePolicy(sizePolicy1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Files/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        _ok->setIcon(icon);
        _ok->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(_ok);

        _open = new QToolButton(frame);
        _open->setObjectName(QString::fromUtf8("_open"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Files/images/toolbars/main/icon_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        _open->setIcon(icon1);
        _open->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(_open);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame_3);


        horizontalLayout->addWidget(frame);

        frame_4 = new QFrame(FormExercises);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMinimumSize(QSize(100, 0));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        _list = new QListView(frame_4);
        _list->setObjectName(QString::fromUtf8("_list"));
        _list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _list->setProperty("showDropIndicator", QVariant(true));
        _list->setDragDropMode(QAbstractItemView::DragDrop);
        _list->setDefaultDropAction(Qt::CopyAction);
        _list->setIconSize(QSize(32, 32));
        _list->setMovement(QListView::Snap);
        _list->setProperty("isWrapping", QVariant(true));
        _list->setViewMode(QListView::IconMode);
        _list->setSelectionRectVisible(true);

        verticalLayout_3->addWidget(_list);


        horizontalLayout->addWidget(frame_4);

        frame_2 = new QFrame(FormExercises);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(12, 12, 12, 12);
        tabWidget = new QTabWidget(frame_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        tabWidget->setTabShape(QTabWidget::Rounded);
        en = new QWidget();
        en->setObjectName(QString::fromUtf8("en"));
        verticalLayout_4 = new QVBoxLayout(en);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        _text = new QTextEdit(en);
        _text->setObjectName(QString::fromUtf8("_text"));
        _text->setFrameShape(QFrame::NoFrame);
        _text->setReadOnly(true);

        verticalLayout_4->addWidget(_text);

        tabWidget->addTab(en, QString());
        ru = new QWidget();
        ru->setObjectName(QString::fromUtf8("ru"));
        verticalLayout_5 = new QVBoxLayout(ru);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        _text_ru = new QTextEdit(ru);
        _text_ru->setObjectName(QString::fromUtf8("_text_ru"));
        _text_ru->setFrameShape(QFrame::NoFrame);

        verticalLayout_5->addWidget(_text_ru);

        tabWidget->addTab(ru, QString());

        verticalLayout_2->addWidget(tabWidget);


        horizontalLayout->addWidget(frame_2);


        retranslateUi(FormExercises);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FormExercises);
    } // setupUi

    void retranslateUi(QWidget *FormExercises)
    {
        FormExercises->setWindowTitle(QCoreApplication::translate("FormExercises", "Form", nullptr));
        _ok->setText(QString());
        _open->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(en), QCoreApplication::translate("FormExercises", "En", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ru), QCoreApplication::translate("FormExercises", "Ru", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormExercises: public Ui_FormExercises {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMEXERCISES_H
