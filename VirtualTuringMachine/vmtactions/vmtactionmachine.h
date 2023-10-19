#ifndef VMACTIONMACHINE_H
#define VMACTIONMACHINE_H


#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"

#include <QPoint>

class VMTActionMachine : public IVMTAction
{
protected:
    IVMTActionController *_controller;

    std::shared_ptr<IVMTMachine> _machine;
    bool _navi;
public:
    VMTActionMachine(IVMTActionController *controller,std::shared_ptr<IVMTMachine> machine,bool navi);
    void Cancel(IVMTEnvironment *environment) override;
    void Enable(IVMTEnvironment *environment) override;
    bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    void Paint(UICanvas& canvas, const QRect &rect) override;
};

#endif // VMACTIONMACHINE_H
