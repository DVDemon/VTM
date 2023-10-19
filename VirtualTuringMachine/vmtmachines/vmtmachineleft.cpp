#include "vmtmachineleft.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineLeft::VMTMachineLeft(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("left"), parent, IVMTMachine::MachineType::MT_LEFT,ImageType::MACHINE_LEFT){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineLeft::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineLeft::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineLeft::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineLeft(_parent));

  std::dynamic_pointer_cast<VMTMachineLeft>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineLeft>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineLeft>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineLeft>(result)->_power = _power;

  return result;
}

void VMTMachineLeft::DoAction(std::shared_ptr<VMTLine> line){
 line->ShiftLeft();
}

bool VMTMachineLeft::IsPowerCalculator(){
 return true;
}

bool VMTMachineLeft::IsAlphabit(){
 return false;
}

void VMTMachineLeft::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);

  canvas.DrawImage(ImageType::MACHINE_LEFT,_internal_bounds);
}

VMTMachineLeft::~VMTMachineLeft(){

}

