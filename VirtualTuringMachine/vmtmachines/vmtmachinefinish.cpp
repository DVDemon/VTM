#include "vmtmachinefinish.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineFinish::VMTMachineFinish(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("finish"), parent, IVMTMachine::MachineType::MT_FINISH,ImageType::MACHINE_FINISH){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineFinish::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineFinish::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineFinish::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineFinish(_parent));

  std::dynamic_pointer_cast<VMTMachineFinish>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineFinish>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineFinish>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineFinish>(result)->_power = _power;

  return result;
}

void VMTMachineFinish::DoAction(std::shared_ptr<VMTLine> ){

}

bool VMTMachineFinish::IsPowerCalculator(){
 return false;
}

bool VMTMachineFinish::IsAlphabit(){
 return false;
}

void VMTMachineFinish::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);

  canvas.DrawImage(ImageType::MACHINE_FINISH,_internal_bounds);
}

VMTMachineFinish::~VMTMachineFinish(){

}
