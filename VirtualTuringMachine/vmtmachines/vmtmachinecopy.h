#ifndef VMTMACHINECOPY_H
#define VMTMACHINECOPY_H

#include "VMTLine.h"
#include "VMTMachineStub.h"
#include "VMTComplexMachine.h"

class VMTMachineCopy : public VMTMachineStub
{
protected:
    std::shared_ptr<VMTComplexMachine> _complex;
public:
    VMTMachineCopy(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;

    std::shared_ptr<IVMTMachine> CreateComplexMachine(IVMTEnvironment* environment) override;

    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineCopy();

};

#endif // VMTMACHINECOPY_H
