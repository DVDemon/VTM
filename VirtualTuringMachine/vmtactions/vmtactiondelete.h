#ifndef VMTACTIONDELETE_H
#define VMTACTIONDELETE_H

#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"
#include "interfaces/IVMTAlphabitSource.h"
#include "interfaces/IVMTTransition.h"

class VMTActionDelete: public IVMTAction
{
protected:
    IVMTActionController *_controller;
    std::weak_ptr<IVMTMachine> _machine;
    std::weak_ptr<IVMTTransition> _transition;
public:
    VMTActionDelete(IVMTActionController *controller);
    void Cancel(IVMTEnvironment *environment) override;
    void Enable(IVMTEnvironment *environment) override;
    bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    void Paint(UICanvas& canvas, const QRect &rect) override;

};

#endif // VMTACTIONDELETE_H
