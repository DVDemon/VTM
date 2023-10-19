#include "formactionhint.h"
#include "ui_formactionhint.h"
#include "screentools.h"

FormActionHint::FormActionHint(const QString &txt,QPixmap& img,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormActionHint)
{
    ui->setupUi(this);

        ScreenTools st;

        ui->label_text->setText(txt);
        ui->label_img->setPixmap(img.scaled(st.GetImageSize(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->label_img->setText("");
}

FormActionHint::~FormActionHint()
{
    delete ui;
}
