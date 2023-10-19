#include "vmterror.h"

VMTError::VMTError(std::shared_ptr<VMTComplexMachine> complex_machine,QString message) :
   _complex_machine(complex_machine),_message(message)
{

}

const QString& VMTError::GetMessage(){
    return _message;
}

std::shared_ptr<VMTComplexMachine> VMTError::GetMachine(){
    return _complex_machine;

}

std::shared_ptr<IVMTMachine> VMTError::GetErrorMachine(){
    return std::shared_ptr<IVMTMachine>();
}

std::shared_ptr<IVMTTransition> VMTError::GetErrorTransition(){
    return std::shared_ptr<IVMTTransition>();
}
