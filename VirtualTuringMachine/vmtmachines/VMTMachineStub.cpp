#include "VMTMachineStub.h"
#include "uicanvas.h"
#include "VMTComplexMachine.h"
#include "VMTAlphabit.h"



#include "VMTMachineAlpha.h"
#include "vmtmachinecopy.h"
#include "vmtmachinefinish.h"
#include "vmtmachineleft.h"
#include "vmtmachineleftword.h"
#include "vmtmachineright.h"
#include "vmtmachinerightword.h"
#include "vmtmachinestart.h"
#include <QDebug>



std::shared_ptr<IVMTMachine> VMTMachineStub::CreateMachineByID(MachineType id,
                                                               std::weak_ptr<VMTComplexMachine> parent){

    if(auto parent_ptr = parent.lock()){
        switch(id){
        case MachineType::MT_WRITE:
            return std::shared_ptr<IVMTMachine>(new VMTMachineAlpha(parent_ptr->GetAlphabit()->GetLambda(),parent));
        case MachineType::MT_START:
            return std::shared_ptr<IVMTMachine>(new VMTMachineStart(parent));
        case MachineType::MT_FINISH:
            return std::shared_ptr<IVMTMachine>(new VMTMachineFinish(parent));
        case MachineType::MT_LEFT:
            return std::shared_ptr<IVMTMachine>(new VMTMachineLeft(parent));
        case MachineType::MT_LEFT_WORD:
            return std::shared_ptr<IVMTMachine>(new VMTMachineLeftWord(parent));
        case MachineType::MT_RIGHT:
            return std::shared_ptr<IVMTMachine>(new VMTMachineRight(parent));
        case MachineType::MT_RIGHT_WORD:
            return std::shared_ptr<IVMTMachine>(new VMTMachineRightWord(parent));
        case MachineType::MT_COPY:
            return std::shared_ptr<IVMTMachine>(new VMTMachineCopy(parent));
        default:
            return std::shared_ptr<IVMTMachine>();
        }


    }

    return std::shared_ptr<IVMTMachine>();
}

std::shared_ptr<IVMTMachine> VMTMachineStub::CreateComplexMachine(IVMTEnvironment*){
 return std::shared_ptr<IVMTMachine>();
}

void VMTMachineStub::Serialize(QDataStream& stream){
    stream << (qint32) _bounds.left();
    stream << (qint32) _bounds.top();
    stream << (qint32) _bounds.width();
    stream << (qint32) _bounds.height();
    stream << (qint32) _size.x();
    stream << (qint32) _size.y();
    stream << (qint64) _power;
}

void VMTMachineStub::Deserialize(QDataStream& stream){
    qint32 value;
    qint64 power;
    stream >> value; _bounds.setLeft(value);
    stream >> value; _bounds.setTop(value);
    stream >> value; _bounds.setWidth(value);
    stream >> value; _bounds.setHeight(value);
    stream >> value; _size.setX(value);
    stream >> value; _size.setY(value);
    stream >> power; _power = power;

    _center = _bounds.center();
    Update(nullptr);
}

VMTMachineStub::VMTMachineStub(const QString name, std::weak_ptr<VMTComplexMachine> parent, MachineType id,ImageType it) : IVMTMachine(it){
    this->_error = false;
    this->_selected = false;
    this->_power = 1;
    _name = name;
    _parent = parent;
    _id = id;
}

IVMTAlphabitSource *VMTMachineStub::GetAlphabitSource(){
    return nullptr;
}

void VMTMachineStub::AddIncomingTransition(std::weak_ptr<IVMTTransition> transition){
    _incoming.push_back(transition);
}

void VMTMachineStub::AddOutgoingTransition(std::weak_ptr<IVMTTransition> transition){
    _outgoing.push_back(transition);
}

void VMTMachineStub::RemoveIncomingTransition(std::shared_ptr<IVMTTransition> transition){

    bool found = true;
    while(found){
        found = false;
        for(std::vector<std::weak_ptr<IVMTTransition>>::iterator it= _incoming.begin();(it!=_incoming.end())&&!found;++it){
            if(std::shared_ptr<IVMTTransition> ptr=it->lock()){
                if(ptr == transition) {
                    _incoming.erase(it);
                    qDebug() << "Removed";
                    found = true;
                }
            }
        }
    }
}

void VMTMachineStub::RemoveOutgoingTransition(std::shared_ptr<IVMTTransition> transition){
    bool found = true;
    qDebug() << "Remove outgoing";
    while(found){
        found = false;
        for(std::vector<std::weak_ptr<IVMTTransition>>::iterator it= _outgoing.begin();(it!=_outgoing.end())&&!found;++it){
            if(std::shared_ptr<IVMTTransition> ptr=it->lock()){
                if(ptr == transition) {
                    qDebug() << "Erase";
                    _outgoing.erase(it);
                    qDebug() << "Removed";
                    found = true;
                }
            }
        }
    };
    qDebug() << "Done";
}

void VMTMachineStub::DoAction(std::shared_ptr<VMTLine> line){

}

QPoint& VMTMachineStub::GetSize(){
    return _size;
}

const QRect& VMTMachineStub::GetBounds(){
    return _bounds;
}

const QRect& VMTMachineStub::GetBoundsWithChilds(){
    return this->_bounds_with_childs;
}

const QPoint& VMTMachineStub::GetCenter(){
    _center = QPoint((_bounds.left()+_bounds.right())/2,
                     (_bounds.top()+_bounds.bottom())/2);
    return _center;
}

IVMTMachine::MachineType VMTMachineStub::GetID() {
    return this->_id;
}

std::weak_ptr<VMTComplexMachine> VMTMachineStub::GetParent(){
    return this->_parent;
}

const std::vector<std::weak_ptr<IVMTTransition>>& VMTMachineStub::GetIncomingTransitions(){
    return this->_incoming;
}

const std::vector<std::weak_ptr<IVMTTransition>>& VMTMachineStub::GetOutgoingTransitions(){
    return this->_outgoing;
}

const QPoint& VMTMachineStub::GetInputPoint(){
    return this->_input_point;
}

const QString& VMTMachineStub::GetMachineName(){
    return this->_name;
}

const QString& VMTMachineStub::GetName(){
    return this->_name;
}

const QPoint& VMTMachineStub::GetOutputPoint(){
    return this->_output_point;
}


long VMTMachineStub::GetPower(){
    return this->_power;
}

bool VMTMachineStub::IsAcceptIncoming(){
    return false;
}

bool VMTMachineStub::IsAcceptOutgoing(){
    return true;
}

bool VMTMachineStub::IsAlphabit(){
    return false;
}

bool VMTMachineStub::IsComplex(){
    return false;
}

bool VMTMachineStub::IsError(){
    return this->_error;
}

bool VMTMachineStub::IsInside(const QPoint& point){
    return (this->_bounds.top()<=point.y())&&(this->_bounds.bottom()>=point.y())&&
            (this->_bounds.left()<=point.x())&&(this->_bounds.right()>=point.x());
}

bool VMTMachineStub::IsPowerCalculator(){
    return true;
}

bool VMTMachineStub::IsSelected(){
    return this->_selected;
}

void VMTMachineStub::Move(const QPoint& center,IVMTEnvironment* environment){
    _center = center;
    Update(environment);
}


void VMTMachineStub::Update(IVMTEnvironment* environment){
    _input_point = _center;
    _output_point = _center;
    _internal_bounds.setLeft(_center.rx()-_size.rx()/2);
    _internal_bounds.setTop(_center.ry()-_size.ry()/2);
    _internal_bounds.setWidth(_size.rx());
    _internal_bounds.setHeight(_size.ry());

    _bounds = _internal_bounds;
    _bounds.setX(_internal_bounds.x()-5);
    _bounds.setY(_internal_bounds.y()-5);
    _bounds.setWidth(_internal_bounds.width()+10);
    _bounds.setHeight(_internal_bounds.height()+10);

    if(_power>1){
        _bounds.setWidth(_bounds.width()+_internal_bounds.width());
        _bounds.setTop(_bounds.top()-_internal_bounds.height()/4);
    }

    _input_point.rx() = _bounds.left();
    _input_point.ry() = _center.y();

    _output_point.rx() = _bounds.left()+_bounds.width();
    _output_point.ry() = _center.y();

    for(auto transition:_incoming) if(auto ptr = transition.lock()) ptr->Changed(environment);
    for(auto transition:_outgoing) if(auto ptr = transition.lock()) ptr->Changed(environment);
}

void VMTMachineStub::Move(const QPoint&& center,IVMTEnvironment* environment){
    _center = center;
    Update(environment);
}

void VMTMachineStub::Paint(UICanvas& canvas, const QRect& rect){

    if(this->_selected||this->_error)
    {
        if(_selected)  {
            canvas.DrawRectAnimation(_bounds,_selected);
        }  else  canvas.DrawRect(_bounds,_selected,_error);
    } else{
        //canvas.DrawButton(_bounds);
    }

    if(_power>1){
      QString str = QString::number(_power);
      QRect rect = _internal_bounds;
      rect.setLeft(rect.left()+rect.width());
      rect.setWidth(rect.height()/2);
      rect.setHeight(rect.height()/2);
      //canvas.DrawText(str, rect, false);

      canvas.DrawText(str,rect.center(),rect.height(),false);
    }


}

void VMTMachineStub::RemoveChilds(){
    _incoming.clear();
    _outgoing.clear();
}

//*
void VMTMachineStub::RemoveIncomingTransition(IVMTTransition* transition) {
    for (auto it = _incoming.begin(); it != _incoming.end();) {
        if(auto ptr = it->lock())
        {
            IVMTTransition* current = ptr.get();
            if (current == transition) _incoming.erase(it);
            else it++;
        } else it++;
    };
}


void VMTMachineStub::RemoveOutgoingTransition(IVMTTransition* transition){
     qDebug() << "Remove outgoing??";
    for (auto it = _outgoing.begin(); it != _outgoing.end();) {
        if(auto ptr = it->lock())
        {
            IVMTTransition* current = ptr.get();
            if (current == transition) {
                qDebug() << "Erase:" << ptr.use_count();
                _outgoing.erase(it);
            }
            else it++;
        } else it++;
    };


}
//*/

void VMTMachineStub::SetError(bool error){
    this->_error = error;
}

void VMTMachineStub::SetParent(std::weak_ptr<VMTComplexMachine> parent){
    this->_parent = parent;
}

void VMTMachineStub::SetPower(long power){
    this->_power = power;
}

void VMTMachineStub::SetSelected(bool selected){
    this->_selected =selected;
}

VMTMachineStub::~VMTMachineStub(){

}
