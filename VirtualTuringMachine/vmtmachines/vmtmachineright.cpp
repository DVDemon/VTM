#include "vmtmachineright.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineRight::VMTMachineRight(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("right"), parent, IVMTMachine::MachineType::MT_RIGHT,ImageType::MACHINE_RIGHT){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineRight::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineRight::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineRight::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineRight(_parent));

  std::dynamic_pointer_cast<VMTMachineRight>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineRight>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineRight>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineRight>(result)->_power = _power;

  return result;
}

void VMTMachineRight::DoAction(std::shared_ptr<VMTLine> line){
 line->ShiftRight();
}

bool VMTMachineRight::IsPowerCalculator(){
 return true;
}

bool VMTMachineRight::IsAlphabit(){
 return false;
}

void VMTMachineRight::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);

  canvas.DrawImage(ImageType::MACHINE_RIGHT,_internal_bounds);
}

VMTMachineRight::~VMTMachineRight(){

}

