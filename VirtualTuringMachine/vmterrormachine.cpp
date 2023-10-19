#include "vmterrormachine.h"

VMTErrorMachine::VMTErrorMachine(std::shared_ptr<VMTComplexMachine> complex_machine,
                                 QString message,std::shared_ptr<IVMTMachine> machine) :
    VMTError(complex_machine,message), _machine(machine)
{

}

std::shared_ptr<IVMTMachine> VMTErrorMachine::GetErrorMachine(){
    return _machine;
}

