#ifndef FORMWARNING_H
#define FORMWARNING_H

#include <QWidget>
#include "statecontroller.h"

namespace Ui {
class FormWarning;
}

class FormWarning : public QWidget
{
    Q_OBJECT

public:
    explicit FormWarning(StateController *controller,QWidget *parent = 0);
    ~FormWarning();

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::FormWarning *ui;
    StateController *_controller;
};

#endif // FORMWARNING_H
