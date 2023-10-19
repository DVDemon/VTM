#include "vmtactionpointer.h"
#include "interfaces/IVMTMachine.h"
#include "vmtmachines/VMTComplexMachine.h"
#include <QDebug>
#include "vmtproject.h"
#include "vmtundoelementmachine.h"
#include "vmtactioncreateandconnect.h"
#include "vmtactionlink.h"
#include "screentools.h"

VMTActionPointer::VMTActionPointer(IVMTActionController *controller, bool navi) :
    _controller(controller), _action_rect(0,0,0,0),_is_shift(false),_mouse(0,0),_shift(0,0),_navi(navi)
{
    _machine = std::weak_ptr<IVMTMachine>();
    ScreenTools st;
    _control_size = st.GetBigButtonSize();
}

VMTActionPointer::VMTActionPointer(IVMTActionController *controller,std::shared_ptr<IVMTMachine> select,bool navi):
    _controller(controller), _action_rect(0,0,0,0),_is_shift(false),_mouse(0,0),_shift(0,0),_navi(navi)
{
    _machine = select;
    ScreenTools st;
    _control_size = st.GetBigButtonSize();
}

void VMTActionPointer::Cancel(IVMTEnvironment *environment){

    environment->EnableAnimation(false);

    if(auto ptr = _machine.lock()){
        ptr->SetSelected(false);
        environment->Repaint(_action_rect);
    }

    if(auto ptr = _transition.lock()){
        ptr->SetSelected(false);
        environment->Repaint(_action_rect);
    }
    _is_shift = false;
    _machine = std::weak_ptr<IVMTMachine>();
    _transition = std::weak_ptr<IVMTTransition>();

    environment->DisableAlphabit();
    environment->DisableCalculator();

}

void VMTActionPointer::Enable(IVMTEnvironment *environment){
    if(environment) environment->EnableAnimation(true);
    if(auto ptr=_machine.lock())
    {
        Select(_machine,environment);
        environment->Repaint(QRect());
    }
}

void VMTActionPointer::GetMachineRect(std::shared_ptr<IVMTMachine> machine,QRect &rect){
    rect = machine->GetBounds();
    for(const std::weak_ptr<IVMTTransition> &t: machine->GetIncomingTransitions())
        if( auto tt = t.lock())
            rect =rect.united(tt->GetBounds());

    for(const std::weak_ptr<IVMTTransition> &t: machine->GetOutgoingTransitions())
        if( auto tt = t.lock())
            rect =rect.united(tt->GetBounds());
}

bool VMTActionPointer::OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){
    if(!environment) return false;

    _screen_mouse = screen;
    bool repaint = false;
    if(_is_shift)
    {
        if(auto ptr = _machine.lock()){
            _shift = real-_mouse;
            QRect old_rect;
            GetActionRect(ptr,old_rect,environment);
            ptr->Move(_shift,environment);
            GetActionRect(ptr,_action_rect,environment);
            _action_rect = _action_rect.united(old_rect);

            FillControls(environment);


            repaint = true;
        } else
            if(auto ptr = _transition.lock()){
                _shift = real-_mouse;
                ptr->Drag(environment,real);
                _action_rect = ptr->GetBounds();
                repaint = true;

            }
    }

    if(repaint){
        environment->Repaint(_action_rect);
    }

    return true;
}


bool VMTActionPointer::CheckControlClick(IVMTEnvironment *environment,const QPoint& real){
    for(VMTActionPointerControl ctrl : _controls){
        if((ctrl._bounds.left()<=real.x())&&
                (ctrl._bounds.right()>=real.x())&&
                (ctrl._bounds.top()<=real.y())&&
                (ctrl._bounds.bottom()>=real.y()))
        {

            if(auto ptr = _machine.lock()){
                if(auto complex_machine = environment->GetMachine().lock()){
                    if(ctrl._machine_type!=IVMTMachine::MachineType::NONE)
                        _controller->SetAction(std::shared_ptr<IVMTAction>(new VMTActionCreateAndConnect(_controller,ptr,
                                                                                                         VMTMachineStub::CreateMachineByID(
                                                                                                             ctrl._machine_type,
                                                                                                             complex_machine))));
                    else _controller->SetAction(std::shared_ptr<IVMTAction>(new VMTActionLink(_controller,ptr)));
                }
            }

            return true;
        }
    }

    return false;
}

void VMTActionPointer::Select(std::weak_ptr<IVMTMachine> machine,IVMTEnvironment *environment){
    if(auto ptr = _machine.lock()){
        VMTProject::GetInstance().GetUndoManager()->Remember(
                    std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));

        ptr->SetSelected(true);
        if(ptr->IsAlphabit())
            if(ptr->GetAlphabitSource())
                environment->EnableAlphabit(ptr->GetAlphabitSource(),"Select sign for \"Write\" machine",*environment->GetGraphics().GetImage(ptr->GetImageType()));

        if(ptr->IsPowerCalculator())
            environment->EnableCalculator(ptr);

        FillControls(environment);

    }

}

void VMTActionPointer::GetActionRect(std::shared_ptr<IVMTMachine> machine,QRect& rect,IVMTEnvironment *environment){

  GetMachineRect(machine,rect);
  if(_navi){
      QRect rect_navi;
      QPoint screen_center = environment->GetGraphics().ToScreen(machine->GetCenter());
      rect_navi.setLeft(screen_center.x()-_control_size.width()*2);
      rect_navi.setTop(screen_center.y()-_control_size.height()*2);
      rect_navi.setWidth(_control_size.width()*4);
      rect_navi.setHeight(_control_size.height()*4);
      rect_navi = environment->GetGraphics().FromScreen(rect_navi);
      rect = rect_navi.united(rect);
  } else {
      rect.setLeft(rect.left()-_control_size.width());
      rect.setTop(rect.top()-_control_size.height());
      rect.setWidth(rect.width()+2*_control_size.width());
      rect.setHeight(rect.height()+2*_control_size.height());
  }
}

bool VMTActionPointer::OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){
    if(!environment) return false;
    bool repaint = false;

    _screen_mouse = screen;
    _action_rect.setWidth(0);
    _action_rect.setHeight(0);


    if(CheckControlClick(environment,screen)) return true;

    if(auto ptr = _machine.lock()){
        ptr->SetSelected(false);
        environment->DisableAlphabit();
        environment->DisableCalculator();
        GetActionRect(ptr,_action_rect,environment);
        _machine = std::weak_ptr<IVMTMachine>();
        repaint = true;

    }

    if(auto ptr_trans = _transition.lock()){
        ptr_trans->SetSelected(false);
        environment->DisableAlphabit();
        environment->DisableCalculator();
        _transition = std::weak_ptr<IVMTTransition>();
        repaint = true;
    }

    if(auto complex_machine = environment->GetMachine().lock()){



        _machine = complex_machine->FindMachine(environment,real);
        if(auto ptr=_machine.lock()){
            Select(_machine,environment);
            _is_shift = true;
            _mouse=real-ptr->GetCenter();
            _shift = QPoint(0,0);
            QRect old_rect=_action_rect;
            GetActionRect(ptr,_action_rect,environment);

            if(!old_rect.isEmpty()) _action_rect = _action_rect.united(old_rect);
            repaint = true;

        }


        if(_machine.use_count()==0){
            _controls.clear();
            _transition = complex_machine->FindTransition(environment,real);
            if(auto ptr_trans = _transition.lock()){
                VMTProject::GetInstance().GetUndoManager()->Remember(
                            std::shared_ptr<VMTUndoElementMachine>(new VMTUndoElementMachine()));

                ptr_trans->SetSelected(true);

                if(ptr_trans->GetAlphabitSource())
                    environment->EnableAlphabit(ptr_trans->GetAlphabitSource(),ptr_trans->GetAlphabitSource()->GetMessage(),*ptr_trans->GetAlphabitSource()->GetImage());
                _action_rect = ptr_trans->GetBounds();

                _is_shift = true;
                _shift = QPoint(0,0);
                ptr_trans->BeginDrag(environment,real);

                repaint = true;
            }
        }
    }



    if(repaint){
        environment->Repaint(_action_rect);
    }
    return true;
}

bool VMTActionPointer::OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){
    if(!environment) return false;

    bool repaint = false;
    if(_is_shift)
        if(auto ptr = _machine.lock()){
            QRect old_rect;
            GetMachineRect(ptr,old_rect);

            QPoint center = ptr->GetCenter();
            environment->GetGraphics().SnapToGris(center);
            ptr->Move(center,environment);

            GetActionRect(ptr,_action_rect,environment);
            _action_rect = _action_rect.united(old_rect);
            FillControls(environment);
            repaint = true;
        }

    if(auto ptr = _transition.lock()){
        _shift = real-_mouse;
        ptr->EndDrag(environment,real);

        _action_rect = ptr->GetBounds();
        repaint = true;
    }
    _is_shift = false;

    if(repaint){
    environment->Repaint(_action_rect);
    }
    return false;
}

void VMTActionPointer::FillControls(IVMTEnvironment *environment){
    if(_navi)
    if(environment)
    if(auto ptr = _machine.lock()){
        if(ptr->GetID() == IVMTMachine::MachineType::MT_FINISH) return;
        _controls.clear();

        QRect m_bounds = environment->GetGraphics().ToScreen(ptr->GetBounds());
        QRect bounds,rect;
        rect.setLeft(m_bounds.center().x()-_control_size.width()/2);
        rect.setTop(m_bounds.center().y()-_control_size.width()/2);
        rect.setWidth(_control_size.width());
        rect.setHeight(_control_size.height());

        bounds = rect;



        bounds.moveLeft(rect.right());
        bounds.moveTop(rect.top()-bounds.height());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::LINK,IVMTMachine::MachineType::NONE));

        bounds.moveTop(rect.top());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_RIGHT,IVMTMachine::MachineType::MT_RIGHT));

        bounds.moveTop(rect.top()+bounds.height());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_RIGHT_WORD,IVMTMachine::MachineType::MT_RIGHT_WORD));

        bounds.moveLeft(rect.right()-bounds.width());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_FINISH,IVMTMachine::MachineType::MT_FINISH));

        bounds.moveLeft(rect.right()-2*bounds.width());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_LEFT_WORD,IVMTMachine::MachineType::MT_LEFT_WORD));

        bounds.moveTop(rect.top());
         _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_LEFT,IVMTMachine::MachineType::MT_LEFT));

        bounds.moveTop(rect.top()-bounds.height());
        _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_COPY,IVMTMachine::MachineType::MT_COPY));

        bounds.moveLeft(rect.left());
         _controls.push_back(VMTActionPointerControl(bounds,ImageType::MACHINE_LAMBDA,IVMTMachine::MachineType::MT_WRITE));
    }
}

void VMTActionPointer::Paint(UICanvas& canvas, const QRect &rect){
    std::lock_guard<std::mutex> lock(_mutex);



    if(auto ptr = _machine.lock()){
       if(!_controls.empty()){
           QRect rect;
           QRect m_bounds = canvas.ToScreen(ptr->GetBounds());
           rect.setLeft(m_bounds.center().x()-_control_size.width()*3/2);
           rect.setTop(m_bounds.center().y()-_control_size.height()*3/2);
           rect.setWidth(_control_size.width()*3);
           rect.setHeight(_control_size.height()*3);
           canvas.DrawImageScreen(ImageType::NAVIGATION_CIRCLE,rect);
       } else canvas.DrawRectAnimation(ptr->GetBounds());


    }
}




