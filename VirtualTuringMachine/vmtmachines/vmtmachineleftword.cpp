#include "vmtmachineleftword.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"
#include "VMTTransitionImpl.h"

VMTMachineLeftWord::VMTMachineLeftWord(std::weak_ptr<VMTComplexMachine> parent) :
 VMTMachineStub(QString("left word"), parent, IVMTMachine::MachineType::MT_LEFT_WORD,ImageType::MACHINE_LEFT_WORD){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());
}

void VMTMachineLeftWord::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineLeftWord::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineLeftWord::Clone(IVMTEnvironment*){
  std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineLeftWord(_parent));

  std::dynamic_pointer_cast<VMTMachineLeftWord>(result)->_center = _center;
  std::dynamic_pointer_cast<VMTMachineLeftWord>(result)->_bounds = _bounds;
  std::dynamic_pointer_cast<VMTMachineLeftWord>(result)->_size = _size;
  std::dynamic_pointer_cast<VMTMachineLeftWord>(result)->_power = _power;

  return result;
}

std::shared_ptr<IVMTMachine> VMTMachineLeftWord::CreateComplexMachine(IVMTEnvironment* environment){

 if(_complex) return _complex;
 std::shared_ptr<VMTComplexMachine> result = std::make_shared<VMTComplexMachine>(QString("Left Word"),this->GetParent().lock()->GetAlphabit());

 std::shared_ptr<IVMTMachine> start = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_START,result); result->AddMachine(start);
 std::shared_ptr<IVMTMachine> left = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_LEFT,result); result->AddMachine(left);
 std::shared_ptr<IVMTMachine> finish = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_FINISH,result); result->AddMachine(finish);

 const int STEP=200;
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

 _complex= result;
 return result;
}

void VMTMachineLeftWord::DoAction(std::shared_ptr<VMTLine> line){
 line->ShiftLeftWord();
}

bool VMTMachineLeftWord::IsPowerCalculator(){
 return true;
}

bool VMTMachineLeftWord::IsAlphabit(){
 return false;
}

void VMTMachineLeftWord::Paint(UICanvas& canvas, const QRect& rect){
  VMTMachineStub::Paint(canvas,rect);

  canvas.DrawImage(ImageType::MACHINE_LEFT_WORD,_internal_bounds);
}

VMTMachineLeftWord::~VMTMachineLeftWord(){

}

