#ifndef VMTMACHINESTART_H
#define VMTMACHINESTART_H


#include "VMTLine.h"
#include "VMTMachineStub.h"

class VMTMachineStart : public VMTMachineStub
{
public:
    VMTMachineStart(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineStart();

};

#endif // VMTMACHINESTART_H
