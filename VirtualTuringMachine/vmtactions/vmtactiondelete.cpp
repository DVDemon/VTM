#include "vmtactiondelete.h"

#include "interfaces/IVMTMachine.h"
#include "vmtmachines/VMTComplexMachine.h"
#include <QDebug>
#include "vmtproject.h"
#include "vmtundomanager.h"
#include "vmtundoelementmachine.h"

VMTActionDelete::VMTActionDelete(IVMTActionController *controller) :
    _controller(controller)
{

}
void VMTActionDelete::Cancel(IVMTEnvironment *environment){
    std::lock_guard<std::mutex> lock(_mutex);

    environment->EnableAnimation(false);
    if(auto ptr = _machine.lock()){
        ptr->SetSelected(false);

        environment->Repaint(ptr->GetBounds());
    }

    environment->DisableActionHint();

}

void VMTActionDelete::Enable(IVMTEnvironment *environment){
    std::lock_guard<std::mutex> lock(_mutex);
    static QPixmap img(":/Files/images/toolbars/tools/icon_delete_black.png");
    environment->EnableActionHint("Click on machine or link to delete it from diagram.",img);
    environment->EnableAnimation(true);
}

bool VMTActionDelete::OnMouseMoved(IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,const QPoint &real){
    std::lock_guard<std::mutex> lock(_mutex);
    if(!environment) return false;

    if(auto ptr = _machine.lock()){
        ptr->SetSelected(false);
        environment->Repaint(ptr->GetBounds());
    }

    if(auto ptr = _transition.lock()){
        ptr->SetSelected(false);
        environment->Repaint(ptr->GetBounds());
    }

    if(auto complex_machine = environment->GetMachine().lock()){

        _machine = complex_machine->FindMachine(environment,real);
        if(auto ptr = _machine.lock()){
            ptr->SetSelected(true);
            environment->Repaint(ptr->GetBounds());
        } else{
            _transition = complex_machine->FindTransition(environment,real);
            if(auto ptr = _transition.lock()){
                ptr->SetSelected(true);
                environment->Repaint(ptr->GetBounds());
            }
        }
    }
    return true;
}

bool VMTActionDelete::OnMousePressed(IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,const QPoint &real){
    std::lock_guard<std::mutex> lock(_mutex);
    if(!environment) return false;

    if(auto complex_machine = environment->GetMachine().lock()){

        _machine = complex_machine->FindMachine(environment,real);
        if(auto ptr = _machine.lock()){
            VMTProject::GetInstance().GetUndoManager()->Remember(
                        std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));

            complex_machine->RemoveMachine(ptr);
            environment->Repaint(QRect());
        } else {
            _transition = complex_machine->FindTransition(environment,real);
            if(auto ptr = _transition.lock()){
                VMTProject::GetInstance().GetUndoManager()->Remember(
                            std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));

                complex_machine->RemoveTransition(ptr);
                if(auto ptr_start=ptr->GetStartMachine().lock()){
                    ptr_start->RemoveOutgoingTransition(ptr);
                }

                if(auto ptr_finish=ptr->GetFinishMachine().lock()){
                    ptr_finish->RemoveIncomingTransition(ptr);
                }

                environment->Repaint(QRect());
            }
        }
    }

    return true;
}

bool VMTActionDelete::OnMouseReleased([[maybe_unused]] IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,[[maybe_unused]] const QPoint &real){
    std::lock_guard<std::mutex> lock(_mutex);
    if(!environment) return false;


    return false;
}

void VMTActionDelete::Paint(UICanvas& , const QRect &){

}




