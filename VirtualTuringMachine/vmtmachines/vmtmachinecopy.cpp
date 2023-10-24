#include "vmtmachinecopy.h"
#include <QDebug>
#include "uicanvas.h"
#include "VMTComplexMachine.h"

VMTMachineCopy::VMTMachineCopy(std::weak_ptr<VMTComplexMachine> parent) :
    VMTMachineStub(QString("copy"), parent, IVMTMachine::MachineType::MT_COPY,ImageType::MACHINE_COPY){
    _size = QPoint(VMTMachineStub::_st.GetCellSize().width(),VMTMachineStub::_st.GetCellSize().height());

}

void VMTMachineCopy::Serialize(QDataStream& stream){
    VMTMachineStub::Serialize(stream);
}

void VMTMachineCopy::Deserialize(QDataStream& stream){
    VMTMachineStub::Deserialize(stream);
}

std::shared_ptr<IVMTMachine> VMTMachineCopy::Clone(IVMTEnvironment*){
    std::shared_ptr<IVMTMachine> result = std::shared_ptr<IVMTMachine>(new VMTMachineCopy(_parent));

    std::dynamic_pointer_cast<VMTMachineCopy>(result)->_center = _center;
    std::dynamic_pointer_cast<VMTMachineCopy>(result)->_bounds = _bounds;
    std::dynamic_pointer_cast<VMTMachineCopy>(result)->_size = _size;
    std::dynamic_pointer_cast<VMTMachineCopy>(result)->_power = _power;

    return result;
}

std::shared_ptr<IVMTMachine> VMTMachineCopy::CreateComplexMachine(IVMTEnvironment* environment){

    if(_complex) return _complex;

    std::shared_ptr<VMTAlphabit> alphabit = std::shared_ptr<VMTAlphabit> (new VMTAlphabit(*this->GetParent().lock()->GetAlphabit()));
    std::shared_ptr<VMTComplexMachine> result = std::make_shared<VMTComplexMachine>(QString("Copy"),alphabit);

    auto connect_space = [&](std::shared_ptr<IVMTMachine> a,std::shared_ptr<IVMTMachine> b){
        std::shared_ptr<IVMTTransition> line = result->ConnectMachines(a,b,environment,result);
        for(int i=0;i<result->GetAlphabit()->GetSignsCount();i++)
              if(result->GetAlphabit()->GetSignAt(i)!=result->GetAlphabit()->GetLambda())
               line->GetConditions().SetEnable(result->GetAlphabit()->GetSignAt(i),false,environment);
    };

    auto connect_char = [&](std::shared_ptr<IVMTMachine> a,std::shared_ptr<IVMTMachine> b,char c){
        std::shared_ptr<IVMTTransition> line = result->ConnectMachines(a,b,environment,result);
        for(int i=0;i<result->GetAlphabit()->GetSignsCount();i++)
              if(result->GetAlphabit()->GetSignAt(i)!=c)
               line->GetConditions().SetEnable(result->GetAlphabit()->GetSignAt(i),false,environment);
    };

    auto connect_non_space = [&](std::shared_ptr<IVMTMachine> a,std::shared_ptr<IVMTMachine> b){
        std::shared_ptr<IVMTTransition> line = result->ConnectMachines(a,b,environment,result);
        line->GetConditions().SetEnable(result->GetAlphabit()->GetLambda(),false,environment);
    };

    auto connect = [&](std::shared_ptr<IVMTMachine> a,std::shared_ptr<IVMTMachine> b){
        std::shared_ptr<IVMTTransition> line = result->ConnectMachines(a,b,environment,result);
    };



    std::shared_ptr<IVMTMachine> start = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_START,result); result->AddMachine(start);
    std::shared_ptr<IVMTMachine> left = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_LEFT,result); result->AddMachine(left);
    std::shared_ptr<IVMTMachine> right_1 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_RIGHT,result); result->AddMachine(right_1);

    std::shared_ptr<IVMTMachine> right_2 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_RIGHT,result); result->AddMachine(right_2);
    std::shared_ptr<IVMTMachine> finish = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_FINISH,result); result->AddMachine(finish);


    const int STEP = 200;
    start->Move(QPoint(STEP,100),environment);
    left->Move(QPoint(2*STEP,100),environment);
    right_1->Move(QPoint(3*STEP,100),environment);
    right_2->Move(QPoint(4*STEP,0),environment);
    finish->Move(QPoint(5*STEP,0),environment);



    connect(start,left);
    connect_non_space(left,left);
    connect_space(left,right_1);
    connect_space(right_1,right_2);
    connect_non_space(right_2,right_2);
    connect_space(right_2,finish);


    int i = STEP;
    QString astr = alphabit->ToString();
    for(int j=0;j<astr.size();++j){
        QChar c = astr.at(j);
        if(!alphabit->IsLambda(c.toLatin1())){
            std::shared_ptr<IVMTMachine> lambda_2 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_WRITE,result); result->AddMachine(lambda_2);
            std::shared_ptr<IVMTMachine> right_3 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_RIGHT,result); result->AddMachine(right_3);
            std::shared_ptr<IVMTMachine> right_4 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_RIGHT,result); result->AddMachine(right_4);
            std::shared_ptr<IVMTMachine> lambda_3 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_WRITE,result); result->AddMachine(lambda_3);
            std::shared_ptr<IVMTMachine> left_1 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_LEFT,result); result->AddMachine(left_1);
            std::shared_ptr<IVMTMachine> left_2 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_LEFT,result); result->AddMachine(left_2);
            std::shared_ptr<IVMTMachine> lambda_4 = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_WRITE,result); result->AddMachine(lambda_4);

            std::dynamic_pointer_cast<IVMTAlphabitSource>(lambda_3)->SetEnable(c.toLatin1(),true,nullptr);
            std::dynamic_pointer_cast<IVMTAlphabitSource>(lambda_4)->SetEnable(c.toLatin1(),true,nullptr);
            lambda_2->Move(QPoint(4*STEP,i),environment);
            right_3->Move(QPoint(5*STEP,i),environment);
            right_4->Move(QPoint(6*STEP,i),environment);
            lambda_3->Move(QPoint(7*STEP,i),environment);
            left_1->Move(QPoint(8*STEP,i),environment);
            left_2->Move(QPoint(9*STEP,i),environment);
            lambda_4->Move(QPoint(10*STEP,i),environment);

            connect_char(right_1,lambda_2,c.toLatin1());
            connect(lambda_2,right_3);
            connect_non_space(right_3,right_3);
            connect_space(right_3,right_4);
            connect_non_space(right_4,right_4);
            connect_space(right_4,lambda_3);
            connect(lambda_3,left_1);
            connect_non_space(left_1,left_1);
            connect_space(left_1,left_2);
            connect_non_space(left_2,left_2);
            connect_space(left_2,lambda_4);
            connect(lambda_4,right_1);

            i+=STEP;
        }
    }



/*
 result->ConnectMachines(start,left,environment,result);


 std::shared_ptr<IVMTTransition> left_left = result->ConnectMachines(left,left,environment,result);
 left_left->GetConditions().SetEnable(result->GetAlphabit()->GetLambda(),false,environment);

 std::shared_ptr<IVMTTransition> left_finish = result->ConnectMachines(left,finish,environment,result);
 for(int i=0;i<result->GetAlphabit()->GetSignsCount();i++)
       if(result->GetAlphabit()->GetSignAt(i)!=result->GetAlphabit()->GetLambda())
        left_finish->GetConditions().SetEnable(result->GetAlphabit()->GetSignAt(i),false,environment);
*/
    _complex = result;
    return result;
}

void VMTMachineCopy::DoAction(std::shared_ptr<VMTLine> line){
    line->ShiftLeft();
    QString reverse_word;
    if(std::shared_ptr<VMTComplexMachine> ptr = _parent.lock()){
        while(line->GetValueAtMachine()!=ptr->GetAlphabit()->GetLambda()){
            reverse_word.append(QChar(line->GetValueAtMachine()));
            line->ShiftLeft();
        }
    }

    long position = line->GetRightSignPosition()+2;
    for(int i=0;i<reverse_word.length();i++){
        line->SetValueAt(position+i,reverse_word[reverse_word.length()-i-1].toLatin1());
    }
    line->SetMachinePosition(position+reverse_word.length());

}

bool VMTMachineCopy::IsPowerCalculator(){
    return true;
}

bool VMTMachineCopy::IsAlphabit(){
    return false;
}

void VMTMachineCopy::Paint(UICanvas& canvas, const QRect& rect){
    VMTMachineStub::Paint(canvas,rect);
    canvas.DrawImage(ImageType::MACHINE_COPY,_internal_bounds);

}

VMTMachineCopy::~VMTMachineCopy(){

}
