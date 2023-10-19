#ifndef VMTERROR_H
#define VMTERROR_H

#include <QString>
#include <vmtmachines/VMTComplexMachine.h>
#include <memory>

class VMTError
{
protected:
    std::shared_ptr<VMTComplexMachine> _complex_machine;
    QString _message;
public:
    VMTError(std::shared_ptr<VMTComplexMachine> complex_machine,QString message);
    const QString& GetMessage();
    std::shared_ptr<VMTComplexMachine> GetMachine();
    virtual std::shared_ptr<IVMTMachine> GetErrorMachine();
    virtual std::shared_ptr<IVMTTransition> GetErrorTransition();
};

#endif // VMTERROR_H
