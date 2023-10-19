#include "vmtdebuger.h"
#include <QDebug>

VMTDebuger::VMTDebuger(std::shared_ptr<VMTAlphabit> alphabit,std::shared_ptr<VMTComplexMachine> complex_machine) :
    _complex_machine(complex_machine),_line(std::shared_ptr<VMTLine>(new VMTLine(alphabit)))

{
    _state.complex_machine = _complex_machine;

    for(std::shared_ptr<IVMTMachine> ptr: _complex_machine->GetMachineCollection())
        if(ptr->GetID() == IVMTMachine::MachineType::MT_START){
            _state.machine = ptr;
            break;
        }
    _state.machine->SetSelected(true);
}

std::shared_ptr<VMTLine>           VMTDebuger::GetLine(){
    return _line;
}

std::shared_ptr<VMTComplexMachine> VMTDebuger::GetComplexMachine(){
    return _complex_machine;
}

void VMTDebuger::ToStart(IVMTEnvironment *environment){
    if(_state.machine)
        if(_state.complex_machine){
            _state.machine->SetSelected(false);
        }


    _state.complex_machine = _complex_machine;

    for(std::shared_ptr<IVMTMachine> ptr: _complex_machine->GetMachineCollection())
        if(ptr->GetID() == IVMTMachine::MachineType::MT_START){
            _state.machine = ptr;
            break;
        }
    _state.machine->SetSelected(true);
    environment->SetMachine(_state.complex_machine);

}
bool VMTDebuger::IsFinish(){
    if(_stack.empty())
        if(_state.machine)
            if(_state.complex_machine)
                if(_state.machine->GetID()==IVMTMachine::MachineType::MT_FINISH){
                    return true;
                }

    return false;
}

VMTDebuger::State VMTDebuger::FindNextMachine(){
    char sign = _line->GetValueAtMachine();
    VMTDebuger::State result;
    result.complex_machine = _state.complex_machine;
    result.machine = _state.machine;
    for(const std::weak_ptr<IVMTTransition> &ptr: _state.machine->GetOutgoingTransitions()){
        if(auto t = ptr.lock())
            if(t->GetConditions().IsEnabled(sign)){
                result.machine = t->GetFinishMachine().lock();
                break;
            }
    }

    return result;
}

void VMTDebuger::Step(IVMTEnvironment *environment){
    if(_state.machine)
        if(_state.complex_machine){
            _state.machine->SetSelected(false);

            switch(_state.machine->GetID()){
            case IVMTMachine::MachineType::MT_COMPLEX:{
                long power= _state.machine->GetPower();
                _stack.push(_state);
                _state.complex_machine = std::dynamic_pointer_cast<VMTComplexMachine>(_state.machine);
                environment->SetMachine(_state.complex_machine);
                for(std::shared_ptr<IVMTMachine> ptr: _state.complex_machine->GetMachineCollection())
                    if(ptr->GetID() == IVMTMachine::MachineType::MT_START){
                        _state.machine = ptr;
                        break;
                    }
                for(int i=0;i<power-1;i++)
                        _stack.push(_state);
                break;
            }
            case IVMTMachine::MachineType::MT_RIGHT_WORD:
            case IVMTMachine::MachineType::MT_LEFT_WORD:
            case IVMTMachine::MachineType::MT_COPY:{
                long power= _state.machine->GetPower();
                auto old_machine = _state.machine;
                _stack.push(_state);
                _state.complex_machine = std::dynamic_pointer_cast<VMTComplexMachine>(_state.machine->CreateComplexMachine(environment));//std::dynamic_pointer_cast<VMTComplexMachine>(_state.machine);
                environment->SetMachine(_state.complex_machine);
                for(std::shared_ptr<IVMTMachine> ptr: _state.complex_machine->GetMachineCollection())
                    if(ptr->GetID() == IVMTMachine::MachineType::MT_START){
                        _state.machine = ptr;
                        break;
                    }

                for(int i=0;i<power-1;i++)
                        _stack.push(_state);
                break;
            }

            case IVMTMachine::MachineType::MT_START:
                _state = FindNextMachine();
                break;
            case IVMTMachine::MachineType::MT_FINISH:
                if(_stack.empty()){
                    _state.complex_machine = _complex_machine;
                    for(std::shared_ptr<IVMTMachine> ptr: _complex_machine->GetMachineCollection())
                        if(ptr->GetID() == IVMTMachine::MachineType::MT_START){
                            _state.machine = ptr;
                            break;
                        }
                } else {
                    _state = _stack.top();
                    _stack.pop();
                    _state = FindNextMachine();
                    environment->SetMachine(_state.complex_machine);

                }
                break;
            default:
                for(int i=0;i<_state.machine->GetPower();i++)
                    _state.machine->DoAction(_line);
                _state = FindNextMachine();
                break;
            }
            _state.machine->SetSelected(true);
            environment->Repaint(QRect());
        }
}
