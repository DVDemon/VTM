#ifndef FORMACTIONHINT_H
#define FORMACTIONHINT_H

#include <QWidget>
#include <QString>
#include <QPixmap>

namespace Ui {
class FormActionHint;
}

class FormActionHint : public QWidget
{
    Q_OBJECT

public:
    explicit FormActionHint(const QString &txt,QPixmap& img,QWidget *parent = 0);
    ~FormActionHint();

private:
    Ui::FormActionHint *ui;
};

#endif // FORMACTIONHINT_H
