#include "vmtactioncreateandconnect.h"
#include "VMTTransitionImpl.h"

VMTActionCreateAndConnect::VMTActionCreateAndConnect(IVMTActionController *controller,
                                                     std::shared_ptr<IVMTMachine> machine_start,
                                                     std::shared_ptr<IVMTMachine> machine) :
    _controller(controller),_machine_start(machine_start),_machine(machine)
{

}

void VMTActionCreateAndConnect::Cancel(IVMTEnvironment *environment){
   // environment->EnableAnimation(false);
    if(_machine->IsAlphabit())
        environment->DisableAlphabit();
    else environment->DisableActionHint();

    if(_transition) {
        _transition->RemoveChilds();
    }


    if(_machine_start) _machine_start->SetSelected(false);
    if(_machine) _machine->SetSelected(false);


    environment->Repaint(QRect());

}

void VMTActionCreateAndConnect::Enable(IVMTEnvironment *environment){
    if(!environment) return;

    _transition = std::shared_ptr<IVMTTransition>(new VMTTransitionImpl(environment->GetMachine()));

    _transition->SetStart(environment,_machine_start);
    _transition->SetFinish(environment,_machine);
    _machine_start->AddOutgoingTransition(std::weak_ptr<IVMTTransition>(_transition));
    _machine->AddIncomingTransition(std::weak_ptr<IVMTTransition>(_transition));

    _machine->Move(_machine_start->GetCenter(),environment);
    _machine->Update(environment);
     //   _machine->Update(environment)

    if(_machine->IsAlphabit())
        environment->EnableAlphabit(_machine->GetAlphabitSource(),"Select sign for \"Write\" machine",*environment->GetGraphics().GetImage(_machine->GetImageType()));
    else {
        QString str = "Click on the editor field to add \"";
        str.append(_machine->GetName());
        str.append("\" machine to diagram.");
        environment->EnableActionHint(str,*environment->GetGraphics().GetImage(_machine->GetImageType()));
    }
   // environment->EnableAnimation(true);
}

bool VMTActionCreateAndConnect::OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){
    if(!environment) return false;

    QRect old_bounds = _transition->GetBounds();
    old_bounds = old_bounds.united(_machine->GetBounds());
    old_bounds = old_bounds.united(_machine_start->GetBounds());

    _machine->Move(real,environment);
    _machine->Update(environment);

    old_bounds = old_bounds.united(_machine->GetBounds());
    old_bounds = old_bounds.united(_machine_start->GetBounds());
    old_bounds = old_bounds.united(_transition->GetBounds());

    old_bounds.setWidth(old_bounds.width()+32);
    old_bounds.setHeight(old_bounds.height()+32);
    old_bounds.moveLeft(old_bounds.left()-16);
    old_bounds.moveTop(old_bounds.top()-16);
    environment->Repaint(old_bounds);

    return true;
}

#include "vmtundoelementeditor.h"
#include "vmtproject.h"
#include "vmtundoelementmachine.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "vmtactionpointer.h"

bool VMTActionCreateAndConnect::OnMousePressed([[maybe_unused]] IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){

    if(auto complex_machine = environment->GetMachine().lock()){
        VMTProject::GetInstance().GetUndoManager()->Remember(
                    std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));

        QPoint center = real;
        environment->GetGraphics().SnapToGris(center);
        _machine->Move(center,environment);

        complex_machine->AddMachine(_machine);
        complex_machine->AddTransition(_transition);
        _machine->Update(environment);
        environment->Repaint(QRect());

        _transition = nullptr;
       _controller->SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(_controller,_machine,true)));

    }
    return true;
}

bool VMTActionCreateAndConnect::OnMouseReleased([[maybe_unused]] IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){

    return true;
}

void VMTActionCreateAndConnect::Paint(UICanvas& canvas, const QRect &rect){
    if(_transition)
        _transition->Paint(canvas,rect);
    if(_machine)
        _machine->Paint(canvas,rect);
}
