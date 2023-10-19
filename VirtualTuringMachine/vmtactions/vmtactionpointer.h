#ifndef VMTACTIONPOINTER_H
#define VMTACTIONPOINTER_H


#include "interfaces/IVMTAction.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"
#include "interfaces/IVMTAlphabitSource.h"
#include "interfaces/IVMTTransition.h"

#include <QPoint>
#include <functional>
#include <vector>

struct VMTActionPointerControl{
    QRect   _bounds;
    ImageType _image;
    std::function<void (void)> _handler;
    IVMTMachine::MachineType _machine_type;
    VMTActionPointerControl(QRect bounds,ImageType image,IVMTMachine::MachineType machine_type) :
    _bounds(bounds),_image(image),_machine_type(machine_type){}
};

class VMTActionPointer : public IVMTAction
{
protected:
    std::vector<VMTActionPointerControl> _controls;

    IVMTActionController *_controller;
    VMTAlphabit           _alphabit_dummy;

    QRect  _action_rect;
    QPoint _mouse,_shift,_screen_mouse;
    QSize  _control_size;
    bool   _is_shift;
    bool   _navi;

    std::weak_ptr<IVMTMachine> _machine;
    std::weak_ptr<IVMTTransition> _transition;
    void GetMachineRect(std::shared_ptr<IVMTMachine> machine,QRect &rect);
    void FillControls(IVMTEnvironment *environment);
    bool CheckControlClick(IVMTEnvironment *environment,const QPoint& real);
    void Select(std::weak_ptr<IVMTMachine> machine,IVMTEnvironment *environment);
    void GetActionRect(std::shared_ptr<IVMTMachine> machine,QRect& rect,IVMTEnvironment *environment);
public:
    VMTActionPointer(IVMTActionController *controller,bool navi);
    VMTActionPointer(IVMTActionController *controller,std::shared_ptr<IVMTMachine> select,bool navi);
    void Cancel(IVMTEnvironment *environment) override;
    void Enable(IVMTEnvironment *environment) override;
    bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) override;
    void Paint(UICanvas& canvas, const QRect &rect) override;

};
#endif // VMTACTIONPOINTER_H
