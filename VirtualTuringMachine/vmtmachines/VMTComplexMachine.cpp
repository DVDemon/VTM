#include "VMTComplexMachine.h"
#include <QDebug>
#include "vmtundoelementmachine.h"
#include "uicanvas.h"
#include "VMTTransitionImpl.h"
#include <map>

VMTComplexMachine::VMTComplexMachine(const QString& name, std::shared_ptr<VMTAlphabit> alphabit) :
  VMTMachineStub(name,std::weak_ptr<VMTComplexMachine>(),IVMTMachine::MachineType::MT_COMPLEX, ImageType::MACHINE_COMPLEX){
    _inner = std::shared_ptr<VMTComplexMachineInner>(new VMTComplexMachineInner(alphabit));

    SetName(name);
}

void VMTComplexMachine::DeselectAll(){
    for(std::shared_ptr<IVMTMachine> m: this->GetMachineCollection())
        m->SetSelected(false);
}

const QString VMTComplexMachine::GetNickName(){
  return _nick_name;
}

void VMTComplexMachine::Update(IVMTEnvironment* environment){
    if(environment){
    QRect bounds;
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width()*GetNickName().size(),VMTMachineStub::_st.GetCellSize().height());
   // environment->GetGraphics().CalculateTextSize(GetNickName(),bounds);
    //_size.rx() = bounds.width();
    //_size.ry() = bounds.height();
    _internal_bounds.setWidth(bounds.width());
    _internal_bounds.setHeight(bounds.height());
    }

   VMTMachineStub::Update(environment);

   //if(environment)
   //  environment->Repaint(QRect());
}


std::shared_ptr<VMTComplexMachine> VMTComplexMachine::CreatePlainCopy(IVMTEnvironment* environment){

    std::map<std::shared_ptr<IVMTMachine>,std::shared_ptr<IVMTMachine>> mapping_first;
    std::map<std::shared_ptr<IVMTMachine>,std::shared_ptr<IVMTMachine>> mapping_last;

    std::shared_ptr<VMTComplexMachine> result =
            std::shared_ptr<VMTComplexMachine>(new VMTComplexMachine(this->GetName(),
                                                                     this->GetAlphabit()));
    for(std::shared_ptr<IVMTMachine> m: this->GetMachineCollection()){
        std::shared_ptr<IVMTMachine> m_copy = m->Clone(environment);

        result->AddMachine(m_copy);
        m_copy->Update(environment);
        mapping_first[m] = m_copy;
        mapping_last[m] = m_copy;

        std::shared_ptr<IVMTMachine> prev = m_copy;
        if(m_copy->GetPower()>1){
            size_t power = m_copy->GetPower();
            m_copy->SetPower(1);
            QPoint center = m_copy->GetCenter();
            for(int i=1;i<power;i++){
                std::shared_ptr<IVMTMachine> m_copy_k = m_copy->Clone(environment);
                center.setY(center.y()+100);
                m_copy_k->Move(center,environment);
                result->AddMachine(m_copy_k);
                m_copy_k->Update(environment);

                std::shared_ptr<VMTTransitionImpl> transition = std::make_shared<VMTTransitionImpl>(result);

                transition->SetStart(environment,prev);
                transition->SetFinish(environment,m_copy_k);

                prev->AddOutgoingTransition(transition);
                m_copy_k->AddIncomingTransition(transition);

                result->AddTransition(transition);

                prev = m_copy_k;
                mapping_last[m] = m_copy_k;
            }
        }
    }

    for(std::shared_ptr<IVMTTransition> t: this->GetTransitionCollection()){
        std::shared_ptr<IVMTTransition> t_new = std::make_shared<VMTTransitionImpl>(result);

        std::shared_ptr<IVMTMachine> new_start;
        std::shared_ptr<IVMTMachine> new_finish;
        if(t->GetStartMachine().lock()){
            std::shared_ptr<IVMTMachine> m_start = t->GetStartMachine().lock();
            new_start = mapping_last[m_start];
        }

        if(t->GetFinishMachine().lock()){
            std::shared_ptr<IVMTMachine> m_finish = t->GetFinishMachine().lock();
            new_finish = mapping_first[m_finish];
        }

        t_new->SetStart(environment,new_start);
        t_new->SetFinish(environment,new_finish);

        new_start->AddOutgoingTransition(t_new);
        new_finish->AddIncomingTransition(t_new);

        t_new->SetConditions(t->GetConditions());

        result->AddTransition(t_new);
    }

    return result;

}
std::shared_ptr<IVMTMachine> VMTComplexMachine::Clone(IVMTEnvironment* environment){
    qDebug() << "VMTComplexMachine::Clone() Begin";
  std::shared_ptr<VMTComplexMachine> result =
          std::shared_ptr<VMTComplexMachine>(new VMTComplexMachine(this->GetName(),
                                                                   this->GetAlphabit()));
  result->ReplaceMachine(this);
  std::dynamic_pointer_cast<VMTComplexMachine>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTComplexMachine>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTComplexMachine>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTComplexMachine>(result)->_power = _power;

  return result;
}
VMTComplexMachine::VMTComplexMachine(const VMTComplexMachine &other) :
VMTMachineStub(other._name,other._parent,IVMTMachine::MachineType::MT_COMPLEX,ImageType::MACHINE_COMPLEX){

}

void VMTComplexMachine::CheckBounds(const QPoint &p,bool first){
        if(first){
            _bounds.moveLeft(p.x());
            _bounds.moveTop(p.y());
        } else {
         if((_bounds.left()>p.x())) {_bounds.setLeft(p.x());}
         if((_bounds.top()>p.y())) {_bounds.setTop(p.y());}
        }

        if((_bounds.left()+_bounds.width())<p.x()) {_bounds.setWidth(p.x()-_bounds.left());}
        if((_bounds.top()+_bounds.height())<p.y()) {_bounds.setHeight(p.y()-_bounds.top());}

}

const QRect& VMTComplexMachine::GetBoundsWithChilds(){

    bool first = true;
  for(std::shared_ptr<IVMTMachine> m: _inner->GetMachineCollection()){
      CheckBounds(m->GetBounds().topLeft(),first); first=false;
      CheckBounds(m->GetBounds().bottomRight(),first);
  }

  for(std::shared_ptr<IVMTTransition> t: _inner->GetTransitionCollection()){
      CheckBounds(t->GetBounds().topLeft(),first);
      CheckBounds(t->GetBounds().bottomRight(),first);
  }

  return _bounds;
}

std::vector<std::shared_ptr<IVMTMachine>>& VMTComplexMachine::GetMachineCollection(){
    return _inner->GetMachineCollection();
}

std::vector<std::shared_ptr<IVMTTransition>>& VMTComplexMachine::GetTransitionCollection(){
   return _inner->GetTransitionCollection();
}

void VMTComplexMachine::AddMachine(std::shared_ptr<IVMTMachine> machine){
    if(_inner)
        _inner->GetMachineCollection().push_back(machine);
}

void VMTComplexMachine::AddTransition(std::shared_ptr<IVMTTransition> transition){
    if(_inner)
        _inner->GetTransitionCollection().push_back(transition);

}

#include "VMTTransitionImpl.h"

std::shared_ptr<IVMTTransition> VMTComplexMachine::ConnectMachines(std::shared_ptr<IVMTMachine> first,std::shared_ptr<IVMTMachine> second,IVMTEnvironment *environment,std::shared_ptr<VMTComplexMachine> parent){

    std::shared_ptr<IVMTTransition> tr = std::shared_ptr<IVMTTransition>(new VMTTransitionImpl(parent));
    first->AddOutgoingTransition(tr);
    second->AddIncomingTransition(tr);
    tr->SetStart(environment,first);
    tr->SetFinish(environment,second);
    parent->AddTransition(tr);
    return tr;
}

void VMTComplexMachine::ChangeAlphabit(std::shared_ptr<VMTAlphabit> other){
 if(_inner)
     _inner->ChangeAlphabit(other);
}

std::weak_ptr<IVMTMachine> VMTComplexMachine::FindMachine(IVMTEnvironment* environment,const QPoint& point){

    for(std::shared_ptr<IVMTMachine> m : _inner->GetMachineCollection())
    {
       if(m->IsInside(point)) return std::weak_ptr<IVMTMachine>(m);
    }

 return std::weak_ptr<IVMTMachine>();
}

std::weak_ptr<IVMTTransition> VMTComplexMachine::FindTransition(IVMTEnvironment* environment,const QPoint& point){
    for(std::shared_ptr<IVMTTransition> m : _inner->GetTransitionCollection())
    {
       if(m->IsInside(environment,point)) return std::weak_ptr<IVMTTransition>(m);
    }

 return std::weak_ptr<IVMTTransition>();
}

std::shared_ptr<VMTAlphabit> VMTComplexMachine::GetAlphabit(){
 return _inner->GetAlphabit();
}

void VMTComplexMachine::SetInnerObject(std::shared_ptr<VMTComplexMachineInner> other,std::shared_ptr<VMTComplexMachine> current){
  _inner = other;

  for(auto m: GetMachineCollection())  m->SetParent(current);
  for(auto t: GetTransitionCollection()) t->SetParent(current);
}

std::shared_ptr<VMTComplexMachineInner> VMTComplexMachine::GetInnerObject(){

 return _inner;
}

/*
std::vector<VMTComplexMachine*> VMTComplexMachine::GetComplexMachines(){
  return std::vector<VMTComplexMachine*>();
}

std::vector<IVMTMachine*> VMTComplexMachine::GetMachines(){
 return std::vector<IVMTMachine*>();
}

std::vector<IVMTTransition*> VMTComplexMachine::GetTransitions(){
 return std::vector<IVMTTransition*>();
}
*/
bool VMTComplexMachine::IsComplex(){
    return true;
}

void VMTComplexMachine::UpdateChilds(IVMTEnvironment* environment){

    for(std::shared_ptr<IVMTMachine> m: _inner->GetMachineCollection()){
        m->Update(environment);
    }
}

void VMTComplexMachine::Paint(UICanvas& canvas, const QRect& rect){
    VMTMachineStub::Paint(canvas,rect);
    canvas.DrawText(GetNickName(),_internal_bounds.center(),_internal_bounds.height(),false);
    //canvas.DrawText(GetNickName(),_internal_bounds,this->IsSelected());
}

void VMTComplexMachine::PaintDiagram(UICanvas& canvas, const QRect& rect){
    if(_inner){
        _inner->PaintDiagramm(canvas,rect,true);
    }
}

void VMTComplexMachine::RemoveComplexMachine(std::shared_ptr<VMTComplexMachine> machine){
    bool found = false;
    do{
        found = false;
        for(auto m: _inner->GetMachineCollection()){
            std::shared_ptr<VMTComplexMachine> cm = std::dynamic_pointer_cast<VMTComplexMachine>(m);
            if(cm){
                if(cm->_inner==machine->_inner){
                    qDebug() << "VMTComplexMachine::RemoveComplexMachine";
                    RemoveMachine(m);
                    found = true;
                    break;
                }
            }
        }
    }while(found);
}

#include <algorithm>
void VMTComplexMachine::RemoveMachine(std::shared_ptr<IVMTMachine> machine){





    std::vector<std::shared_ptr<IVMTTransition>> trans_to_del;

    qDebug() << "Found transition to delete";
    for(std::shared_ptr<IVMTTransition> t:_inner->GetTransitionCollection())
    {
        bool added=false;
        if(auto ptr = t->GetStart().lock())
        {
            if(ptr==machine) {
                trans_to_del.push_back(t);
                added = true;
            }
        }

        if(!added)
        if(auto ptr = t->GetFinish().lock())
        {
            if(ptr==machine) trans_to_del.push_back(t);
        }
    }

    for(std::shared_ptr<IVMTTransition> t:trans_to_del){
        _inner->GetTransitionCollection().erase(
        std::remove(_inner->GetTransitionCollection().begin(),_inner->GetTransitionCollection().end(),t),
                    _inner->GetTransitionCollection().end());
        if(auto ptr_start=t->GetStartMachine().lock()){
            ptr_start->RemoveOutgoingTransition(t);
        }
        if(auto ptr_finish=t->GetFinishMachine().lock()){
            ptr_finish->RemoveIncomingTransition(t);
        }

    }

    _inner->GetMachineCollection().erase(
    std::remove(_inner->GetMachineCollection().begin(),_inner->GetMachineCollection().end(),machine),
                _inner->GetMachineCollection().end());

}

void VMTComplexMachine::RemoveTransition(std::shared_ptr<IVMTTransition> transition){
    _inner->GetTransitionCollection().erase(
    std::remove(_inner->GetTransitionCollection().begin(),_inner->GetTransitionCollection().end(),transition),
                _inner->GetTransitionCollection().end());
}


void VMTComplexMachine::ReplaceMachine(VMTComplexMachine* machine){
 if(machine){
     this->_inner = machine->_inner;
 }
}

//void VMTComplexMachine::SetInner(VMTComplexMachine* src, VMTComplexMachine* parent){
//}

//void VMTComplexMachine::SetInnerObject(VMTComplexMachineInner* inner){
//}

void VMTComplexMachine::SetName(const QString& name){
 this->_name = name;
 QStringList list = name.split(" ");
 this->_nick_name = "";
 for(QString s : list) {
     this->_nick_name.append(s[0].toUpper());
 }
 this->_nick_name += QString::number(_name.size());

}



VMTComplexMachine::~VMTComplexMachine(){
 //qDebug() << "Delete complex machine";
}
