#ifndef VMTERRORMACHINE_H
#define VMTERRORMACHINE_H

#include "vmterror.h"
#include "interfaces/IVMTMachine.h"
class VMTErrorMachine : public VMTError
{
protected:
    std::shared_ptr<IVMTMachine> _machine;
public:
    VMTErrorMachine(std::shared_ptr<VMTComplexMachine> _complex_machine,QString _message,std::shared_ptr<IVMTMachine> machine);
    std::shared_ptr<IVMTMachine> GetErrorMachine() override;
};

#endif // VMTERRORMACHINE_H
