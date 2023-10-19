#include "vmtmachinerightword.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineRightWord::VMTMachineRightWord(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("right word"), parent, IVMTMachine::MachineType::MT_RIGHT_WORD,ImageType::MACHINE_RIGHT_WORD){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineRightWord::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineRightWord::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineRightWord::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineRightWord(_parent));

  std::dynamic_pointer_cast<VMTMachineRightWord>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineRightWord>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineRightWord>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineRightWord>(result)->_power = _power;

  return result;
}

std::shared_ptr<IVMTMachine> VMTMachineRightWord::CreateComplexMachine(IVMTEnvironment* environment){
 if(_complex) return _complex;

 std::shared_ptr<VMTComplexMachine> result = std::make_shared<VMTComplexMachine>(QString("Right Word"),this->GetParent().lock()->GetAlphabit());

 std::shared_ptr<IVMTMachine> start = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_START,result); result->AddMachine(start);
 std::shared_ptr<IVMTMachine> left = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_RIGHT,result); result->AddMachine(left);
 std::shared_ptr<IVMTMachine> finish = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_FINISH,result); result->AddMachine(finish);

 const int STEP = 200;

 start->Move(QPoint(STEP,STEP),environment);
 left->Move(QPoint(3*STEP,STEP),environment);
 finish->Move(QPoint(5*STEP,STEP),environment);

 result->ConnectMachines(start,left,environment,result);


 std::shared_ptr<IVMTTransition> left_left = result->ConnectMachines(left,left,environment,result);
 left_left->GetConditions().SetEnable(result->GetAlphabit()->GetLambda(),false,environment);

 std::shared_ptr<IVMTTransition> left_finish = result->ConnectMachines(left,finish,environment,result);
 for(int i=0;i<result->GetAlphabit()->GetSignsCount();i++)
       if(result->GetAlphabit()->GetSignAt(i)!=result->GetAlphabit()->GetLambda())
        left_finish->GetConditions().SetEnable(result->GetAlphabit()->GetSignAt(i),false,environment);
 _complex = result;
 return result;
}

void VMTMachineRightWord::DoAction(std::shared_ptr<VMTLine> line){
 line->ShiftRightWord();
}

bool VMTMachineRightWord::IsPowerCalculator(){
 return true;
}

bool VMTMachineRightWord::IsAlphabit(){
 return false;
}

void VMTMachineRightWord::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);
  canvas.DrawImage(ImageType::MACHINE_RIGHT_WORD,_internal_bounds);
}

VMTMachineRightWord::~VMTMachineRightWord(){

}

