#include "formalphabit.h"
#include "ui_formalphabit.h"
#include <QHBoxLayout>
#include <QStringRef>
#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QDebug>
#include <QToolButton>
#include "flowlayout.h"
#include "interfaces/IVMTEnvironment.h"
#include "screentools.h"

FormAlphabit::FormAlphabit(IVMTAlphabitSource *alphabit_source,IVMTEnvironment *environment,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAlphabit),
    _alphabit_source(alphabit_source),
    _environment(environment)
{
    ui->setupUi(this);

//    QButtonGroup *_group = nullptr;
//    if(_alphabit_source->IsSingleChar())
//         _group = new QButtonGroup();

    if(!_alphabit_source->GetAlphabit()) qDebug() << "FormAlphabit::FormAlphabit Oppps 2!";
    QString str = _alphabit_source->GetAlphabit()->ToString();
    //_layout.setSpacing(10);

    ui->frame->setLayout(&_layout);
    ui->frame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    _layout.setContentsMargins(5,5,5,5);

    ScreenTools st;
    QString style = "QToolButton:checked{ background-color: rgb(232,143,12); border: none; font-size:20px;}";

    for(int i=0;(i<str.size())&&(i<256);i++){

        QToolButton* label = &_buttons[i];
        //label->setFlat(true);
        label->setProperty("sign",QString(QChar(str[i])));
        label->setText(QString(QChar(str[i])));
        label->setCheckable(true);
        label->setStyleSheet(style);

        st.ResizeButton(label);
        if(_alphabit_source->IsEnabled(str[i].toLatin1()))
            label->setChecked(true);
        else label->setChecked(false);

        QObject::connect(label, SIGNAL(clicked()),
                          this, SLOT(onRefClicked()));
        _layout.addWidget(label);
        if(_alphabit_source->IsSingleChar()) _group.addButton(label);
    }

}

void FormAlphabit::onRefClicked(){
    QToolButton *button = dynamic_cast<QToolButton*> (sender());
    if(button)
    {
        QVariant var = button->property("sign");
        QString  sign = var.toString();
        qDebug() << "Click";
        if(_alphabit_source)
        {
            if(ctrlPressed) {
                qDebug() << "Disable all exclude this";
                _alphabit_source->SetDisableAllExclude(sign[0].toLatin1(),_environment);
                _environment->Repaint(QRect());
            } else {
                _alphabit_source->SetEnable(sign[0].toLatin1(),button->isChecked(),_environment);
                _environment->Repaint(QRect());
            }
        }
    }
}


FormAlphabit::~FormAlphabit()
{
    delete ui;
}
