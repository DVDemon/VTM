#ifndef FORMCALCULATOR_H
#define FORMCALCULATOR_H

#include <QWidget>
#include <QDebug>
#include "interfaces/IVMTMachine.h"
#include "interfaces/IVMTEnvironment.h"
#include <memory>

namespace Ui {
class FormCalculator;
}

class FormCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit FormCalculator(std::shared_ptr<IVMTMachine> machine,IVMTEnvironment *environment,QWidget *parent = 0);
    ~FormCalculator();

private slots:
    void on_text_power_textChanged(const QString &arg1);

    void on_button_plus_clicked();

    void on_button_minus_clicked();

    void on__plus_clicked();

    void on__minus_clicked();

private:
    Ui::FormCalculator *ui;
    std::shared_ptr<IVMTMachine> _machine;
    IVMTEnvironment              *_environment;

    void Check();
};

#endif // FORMCALCULATOR_H
