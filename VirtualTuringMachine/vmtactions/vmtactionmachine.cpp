#include "vmtactionmachine.h"
#include "interfaces/IVMTMachine.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "vmtproject.h"
#include "vmtundomanager.h"
#include "vmtundoelementmachine.h"
#include "vmtactionpointer.h"
#include <QDebug>

VMTActionMachine::VMTActionMachine(IVMTActionController *controller,std::shared_ptr<IVMTMachine> machine,bool navi) :
    _controller(controller), _machine(machine),_navi(navi)
{

}
void VMTActionMachine::Cancel(IVMTEnvironment *environment){
 if(!environment) return;
 if(_machine->IsAlphabit())
     environment->DisableAlphabit();
 else environment->DisableActionHint();
}

void VMTActionMachine::Enable(IVMTEnvironment *environment){
 if(!environment) return;
 if(_machine->IsAlphabit())
     environment->EnableAlphabit(_machine->GetAlphabitSource(),"Select sign for \"Write\" machine",*environment->GetGraphics().GetImage(_machine->GetImageType()));
  else {
     QString str = "Click on the editor field to add \"";
     str.append(_machine->GetName());
     str.append("\" machine to diagram.");
     environment->EnableActionHint(str,*environment->GetGraphics().GetImage(_machine->GetImageType()));
 }
}

bool VMTActionMachine::OnMouseMoved(IVMTEnvironment* environment,[[maybe_unused]]const QPoint &screen,[[maybe_unused]]const QPoint &real){
    if(!environment) return false;

    return true;
}

bool VMTActionMachine::OnMousePressed(IVMTEnvironment* environment,[[maybe_unused]]const QPoint &screen,[[maybe_unused]]const QPoint &real){
    if(!environment) return false;

    if(auto complex_machine = environment->GetMachine().lock()){
        VMTProject::GetInstance().GetUndoManager()->Remember(
                    std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));
        std::shared_ptr<IVMTMachine> machine_add = _machine->Clone(environment);
        QPoint center = real;
        environment->GetGraphics().SnapToGris(center);
        machine_add->Move(center,environment);
        complex_machine->AddMachine(machine_add);
        machine_add->Update(environment);
        environment->Repaint(machine_add->GetBounds());
        if(_navi)
        _controller->SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(_controller,machine_add,_navi)));
    }

    return true;
}

bool VMTActionMachine::OnMouseReleased(IVMTEnvironment* environment,[[maybe_unused]]const QPoint &screen,[[maybe_unused]]const QPoint &real){
    if(!environment) return false;

    return true;
}

void VMTActionMachine::Paint([[maybe_unused]]UICanvas& canvas, [[maybe_unused]]const QRect &rect){

}
