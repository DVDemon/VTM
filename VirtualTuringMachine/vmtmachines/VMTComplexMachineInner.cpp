#include <QDebug>
#include "VMTComplexMachineInner.h"


VMTComplexMachineInner::~VMTComplexMachineInner(){
  //qDebug() << "Complex machine deleted";
}

VMTComplexMachineInner::VMTComplexMachineInner(QString name,  std::shared_ptr<VMTAlphabit>  alphabit) ://,  IVMTEnvironment* environment):
_alphabit(alphabit), _name(name) {
}

VMTComplexMachineInner::VMTComplexMachineInner( std::shared_ptr<VMTAlphabit> alphabit) ://,  IVMTEnvironment* environment):
    _alphabit(alphabit){
}

void VMTComplexMachineInner::ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other){

    if(_alphabit!=other){
      _alphabit = other;
      qDebug() << "VMTComplexMachineInner::ChangeAlphabit";

      for(std::shared_ptr<IVMTMachine> m : _machines ){
          m->ChangeAlphabit(_alphabit);
      }
      for(std::shared_ptr<IVMTTransition> t : _transitions ){
          t->ChangeAlphabit(_alphabit);
      }
    }

}

QRect VMTComplexMachineInner::GetDiagramBounds(){
 return QRect(0,0,0,0);
}

std::shared_ptr<VMTAlphabit> VMTComplexMachineInner::GetAlphabit(){
 return _alphabit;
}

std::vector<std::shared_ptr<IVMTMachine>>& VMTComplexMachineInner::GetMachineCollection(){
 return _machines;
}

std::vector<std::shared_ptr<IVMTTransition>>& VMTComplexMachineInner::GetTransitionCollection(){
 return _transitions;
}

void VMTComplexMachineInner::PaintDiagramm(UICanvas &canvas, QRect rect, bool all){
  for(std::shared_ptr<IVMTMachine> m : _machines ){
      m->Paint(canvas,rect);
  }
  for(std::shared_ptr<IVMTTransition> t : _transitions ){
      t->Paint(canvas,rect);
  }
}

