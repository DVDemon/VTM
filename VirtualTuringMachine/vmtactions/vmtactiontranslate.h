#ifndef VMTACTIONTRANSLATE_H
#define VMTACTIONTRANSLATE_H

#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"

#include <QPoint>
class VMTActionTranslate : public IVMTAction
{
protected:
    IVMTActionController *_controller;

    QPoint _position;
    QPoint _mouse,_shift;
    bool   _is_shift;


public:
    VMTActionTranslate(IVMTActionController *controller);
    void Cancel(IVMTEnvironment *environment) override;
    void Enable(IVMTEnvironment *environment) override;
    bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    void Paint(UICanvas& canvas, const QRect &rect) override;
};

#endif // VMTACTIONTRANSLATE_H
