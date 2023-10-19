#include "vmterrortransition.h"

VMTErrorTransition::VMTErrorTransition(std::shared_ptr<VMTComplexMachine> complex_machine,QString message,std::shared_ptr<IVMTTransition> transition) :
    VMTError(complex_machine,message), _transition(transition)
{

}

std::shared_ptr<IVMTTransition> VMTErrorTransition::GetErrorTransition(){
    return _transition;
}
