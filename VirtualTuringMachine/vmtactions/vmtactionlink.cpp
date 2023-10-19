#include "vmtactionlink.h"
#include "interfaces/IVMTMachine.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "VMTTransitionImpl.h"
#include "vmtundomanager.h"
#include "vmtundoelementmachine.h"
#include "vmtproject.h"
#include <QDebug>

VMTActionLink::VMTActionLink(IVMTActionController *controller) :
    _controller(controller)
{
 _current_machine = std::weak_ptr<IVMTMachine>();

}

VMTActionLink::VMTActionLink(IVMTActionController *controller,std::shared_ptr<IVMTMachine> machine) : _controller(controller)
{
 _current_machine = std::weak_ptr<IVMTMachine>();
 _machine_parameter = machine;

}
void VMTActionLink::RecreateTransition(IVMTEnvironment *environment){
    _transition = std::shared_ptr<IVMTTransition>(new VMTTransitionImpl(environment->GetMachine()));
    _machine_start = nullptr;
    _machine_end   = nullptr;
    static QPixmap img(":/Files/images/toolbars/tools/icon_link_black.png");
    environment->EnableAlphabit(_transition->GetAlphabitSource(),QString("Select first machine in transition."),img);
}

void VMTActionLink::Cancel(IVMTEnvironment *environment){
    std::lock_guard<std::mutex> lock(_mutex);

    environment->EnableAnimation(false);
    if(_transition) {
        _transition->RemoveChilds();
    }

    environment->DisableAlphabit();

    if(auto ptr = _current_machine.lock()){
        ptr->SetSelected(false);
    }

    if(_machine_start) _machine_start->SetSelected(false);
    if(_machine_end) _machine_end->SetSelected(false);

    _transition    = nullptr;
    _machine_start = nullptr;
    _machine_end   = nullptr;
    _current_machine =   std::weak_ptr<IVMTMachine>();
    QRect _action_rect;
    environment->Repaint(_action_rect);

}

void VMTActionLink::Enable(IVMTEnvironment *environment){
 if(environment){
  RecreateTransition(environment);
  if(_machine_parameter)
  {
      if(_machine_parameter->IsAcceptOutgoing()){
      _machine_start = _machine_parameter;
      _transition->SetStart(environment,_machine_start);
      _transition->SetFinish(environment,_machine_start->GetCenter());
      _transition->SetSelected(true);
      _machine_start->SetSelected(true);
      static QPixmap img(":/Files/images/toolbars/tools/icon_link_black.png");
      environment->EnableAlphabit(_transition->GetAlphabitSource(),
                                  QString("Select last machine in transition."),img);
      }
      _machine_parameter = nullptr;
  }
  environment->EnableAnimation(true);
 }
}

bool VMTActionLink::OnMouseMoved(IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,const QPoint &real){
    std::lock_guard<std::mutex> lock(_mutex);
    if(!environment) return false;

    if(auto ptr = _current_machine.lock()){
        if(ptr!=_machine_start) ptr->SetSelected(false);
        QRect _bounds = ptr->GetBounds();
        _bounds = _bounds.united(_transition->GetBounds());
        _bounds.setLeft(_bounds.left()-5);
        _bounds.setTop(_bounds.top()-5);
        _bounds.setWidth(_bounds.width()+10);
        _bounds.setHeight(_bounds.height()+10);
        environment->Repaint(_bounds);
    }

    if(auto complex_machine = environment->GetMachine().lock()){
        _current_machine =complex_machine->FindMachine(environment,real);
        if(auto ptr = _current_machine.lock()){
            ptr->SetSelected(true);
            QRect _bounds = ptr->GetBounds();
            _bounds = _bounds.united(_transition->GetBounds());
            _bounds.setLeft(_bounds.left()-5);
            _bounds.setTop(_bounds.top()-5);
            _bounds.setWidth(_bounds.width()+10);
            _bounds.setHeight(_bounds.height()+10);
            environment->Repaint(_bounds);
        }
    }

    if(_machine_start==nullptr){

    }else{
        if(_transition){
            QRect _bounds = _transition->GetBounds();
            _transition->SetFinish(environment,real);
            _bounds = _bounds.united(_transition->GetBounds());

        _bounds.setLeft(_bounds.left()-5);
        _bounds.setTop(_bounds.top()-5);
        _bounds.setWidth(_bounds.width()+10);
        _bounds.setHeight(_bounds.height()+10);
        environment->Repaint(_bounds);
        }
    }

    return true;
}

bool VMTActionLink::OnMousePressed(IVMTEnvironment* environment,[[maybe_unused]]const QPoint &screen,const QPoint &real){
    std::lock_guard<std::mutex> lock(_mutex);
    if(!environment) return false;
    QRect action_rect;
    if(auto complex_machine = environment->GetMachine().lock()){
        if(auto ptr = complex_machine->FindMachine(environment,real).lock()){
             if(_machine_start==nullptr){
                if(ptr->IsAcceptOutgoing()){
                  _machine_start = ptr;
                  _transition->SetStart(environment,ptr);
                  _transition->SetFinish(environment,real);
                  _transition->SetSelected(true);
                  _machine_start->SetSelected(true);
                  static QPixmap img(":/Files/images/toolbars/tools/icon_link_black.png");
                  environment->EnableAlphabit(_transition->GetAlphabitSource(),QString("Select last machine in transition."),img);
                }
             } else {
                 if(_machine_end==nullptr){
                     _machine_end = ptr;
                     _transition->SetFinish(environment,ptr);
                     _machine_start->AddOutgoingTransition(std::weak_ptr<IVMTTransition>(_transition));
                     _machine_end->AddIncomingTransition(std::weak_ptr<IVMTTransition>(_transition));
                     VMTProject::GetInstance().GetUndoManager()->Remember(
                                 std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));
                     complex_machine->AddTransition(_transition);
                     _transition->SetSelected(false);
                     if(_machine_start) _machine_start->SetSelected(false);
                     if(_machine_end) _machine_end->SetSelected(false);

                     RecreateTransition(environment);
                 }

             }

            environment->Repaint(action_rect);
        }
    }
    return true;
}

bool VMTActionLink::OnMouseReleased(IVMTEnvironment* environment,[[maybe_unused]]const QPoint &screen,[[maybe_unused]]const QPoint &real){
    if(!environment) return false;
    return false;
}

void VMTActionLink::Paint(UICanvas& canvas, const QRect &rect){
    std::lock_guard<std::mutex> lock(_mutex);
    if(_machine_start!=nullptr){
        if(_transition)
            _transition->Paint(canvas,rect);

        canvas.DrawRectAnimation(_machine_start->GetBounds());
    }
}

