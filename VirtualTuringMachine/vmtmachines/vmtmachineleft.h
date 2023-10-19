#ifndef VMTMACHINELEFT_H
#define VMTMACHINELEFT_H


#include "VMTLine.h"
#include "VMTMachineStub.h"

class VMTMachineLeft : public VMTMachineStub
{
public:
    VMTMachineLeft(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineLeft();

};

#endif // VMTMACHINELEFT_H
