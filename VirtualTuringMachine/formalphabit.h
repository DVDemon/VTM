#ifndef FORMALPHABIT_H
#define FORMALPHABIT_H

#include <QWidget>
#include "interfaces/IVMTAlphabitSource.h"
#include <vector>
#include <QToolButton>
#include <QButtonGroup>
#include "flowlayout.h"
class IVMTEnvironment;

namespace Ui {
class FormAlphabit;
}

class FormAlphabit : public QWidget
{
    Q_OBJECT

public:
    explicit FormAlphabit(IVMTAlphabitSource *alphabit_source,IVMTEnvironment *environment,QWidget *parent = 0);

    ~FormAlphabit();

private:
    Ui::FormAlphabit *ui;
    IVMTAlphabitSource *_alphabit_source;
    IVMTEnvironment *_environment;
    //std::vector<QPushButton*> _buttons;
    std::vector<QString> _names;
    FlowLayout _layout;
    QToolButton _buttons[256];
    QButtonGroup _group;
private slots:
    void onRefClicked();
};

#endif // FORMALPHABIT_H
