#ifndef VMTERRORTRANSITION_H
#define VMTERRORTRANSITION_H

#include "vmterror.h"
#include "interfaces/IVMTTransition.h"

class VMTErrorTransition : public VMTError
{
protected:
    std::shared_ptr<IVMTTransition> _transition;
public:
    VMTErrorTransition(std::shared_ptr<VMTComplexMachine> complex_machine,QString message,std::shared_ptr<IVMTTransition> transition);
    std::shared_ptr<IVMTTransition> GetErrorTransition() override;
};

#endif // VMTERRORTRANSITION_H
