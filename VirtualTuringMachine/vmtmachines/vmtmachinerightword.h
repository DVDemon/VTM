#ifndef VMTMACHINERIGHTWORD_H
#define VMTMACHINERIGHTWORD_H


#include "VMTLine.h"
#include "VMTMachineStub.h"
#include "VMTComplexMachine.h"

class VMTMachineRightWord : public VMTMachineStub
{
protected:
    std::shared_ptr<VMTComplexMachine> _complex;
public:
    VMTMachineRightWord(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;

    std::shared_ptr<IVMTMachine> CreateComplexMachine(IVMTEnvironment* environment) override;

    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;


    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineRightWord();

};

#endif // VMTMACHINERIGHTWORD_H
