#include "formwarning.h"
#include "ui_formwarning.h"

FormWarning::FormWarning(StateController *controller,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWarning),
    _controller(controller)
{
    ui->setupUi(this);
}



FormWarning::~FormWarning()
{
    delete ui;
}

void FormWarning::on_button_ok_clicked()
{
    _controller->HideWarning(true);
}

void FormWarning::on_button_cancel_clicked()
{
   _controller->HideWarning(false);
}
