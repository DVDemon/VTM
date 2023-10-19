#ifndef VMTMACHINEFINISH_H
#define VMTMACHINEFINISH_H

#include "VMTLine.h"
#include "VMTMachineStub.h"

class VMTMachineFinish : public VMTMachineStub
{
public:
    VMTMachineFinish(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineFinish();

};

#endif // VMTMACHINEFINISH_H
