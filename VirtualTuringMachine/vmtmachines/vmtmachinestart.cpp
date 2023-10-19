#include "vmtmachinestart.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineStart::VMTMachineStart(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("start"), parent, IVMTMachine::MachineType::MT_START,ImageType::MACHINE_START){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineStart::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineStart::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineStart::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineStart(_parent));

  std::dynamic_pointer_cast<VMTMachineStart>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineStart>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineStart>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineStart>(result)->_power = _power;

  return result;
}

void VMTMachineStart::DoAction(std::shared_ptr<VMTLine> ){

}

bool VMTMachineStart::IsPowerCalculator(){
 return false;
}

bool VMTMachineStart::IsAlphabit(){
 return false;
}

void VMTMachineStart::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);
  canvas.DrawImage(ImageType::MACHINE_START,_internal_bounds);
}

VMTMachineStart::~VMTMachineStart(){

}
