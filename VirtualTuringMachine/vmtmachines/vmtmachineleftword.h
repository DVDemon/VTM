#ifndef VMTMACHINELEFTWORD_H
#define VMTMACHINELEFTWORD_H


#include "VMTLine.h"
#include "VMTMachineStub.h"
#include "VMTComplexMachine.h"

class VMTMachineLeftWord : public VMTMachineStub
{
protected:
    std::shared_ptr<VMTComplexMachine> _complex;
public:
    VMTMachineLeftWord(std::weak_ptr<VMTComplexMachine> parent);

    std::shared_ptr<IVMTMachine> CreateComplexMachine(IVMTEnvironment*) override;

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineLeftWord();

};


#endif // VMTMACHINELEFTWORD_H
