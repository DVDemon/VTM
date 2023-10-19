#ifndef FORMCOMPLEXMACHINES_H
#define FORMCOMPLEXMACHINES_H

#include <QWidget>
#include <QButtonGroup>
#include <memory>

class IVMTActionController;


namespace Ui {
class FormComplexMachines;
}

class FormComplexMachines : public QWidget
{
    Q_OBJECT

public:
    explicit FormComplexMachines(IVMTActionController *controller, QWidget *parent,bool navi);
    ~FormComplexMachines();

private slots:
    void on_complex_machine_clicked();

private:
    Ui::FormComplexMachines *ui;
    std::shared_ptr<QButtonGroup> _tools_group;
    IVMTActionController *_controller;
    bool _navi;

};

#endif // FORMCOMPLEXMACHINES_H
