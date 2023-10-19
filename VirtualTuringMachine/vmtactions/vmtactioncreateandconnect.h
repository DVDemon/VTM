#ifndef VMTACTIONCREATEANDCONNECT_H
#define VMTACTIONCREATEANDCONNECT_H


#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTMachine.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"
#include "interfaces/IVMTTransition.h"

#include <QPoint>
class VMTActionCreateAndConnect : public IVMTAction
{
protected:
    IVMTActionController *_controller;

    std::shared_ptr<IVMTTransition> _transition;

    std::shared_ptr<IVMTMachine> _machine_start;
    std::shared_ptr<IVMTMachine> _machine;


public:
    VMTActionCreateAndConnect(IVMTActionController *controller,std::shared_ptr<IVMTMachine> machine_start,std::shared_ptr<IVMTMachine> machine);
    void Cancel(IVMTEnvironment *environment) override;
    void Enable(IVMTEnvironment *environment) override;
    bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    void Paint(UICanvas& canvas, const QRect &rect) override;
};


#endif // VMTACTIONCREATEANDCONNECT_H
