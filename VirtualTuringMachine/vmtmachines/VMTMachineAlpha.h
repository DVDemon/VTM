
#if !defined(EA_0735A521_1ACC_4b60_B911_F969C5F9DA47__INCLUDED_)
#define EA_0735A521_1ACC_4b60_B911_F969C5F9DA47__INCLUDED_

#include "VMTLine.h"
#include "VMTMachineStub.h"
#include "interfaces/IVMTAlphabitSource.h"

class VMTMachineAlpha : public VMTMachineStub , public IVMTAlphabitSource
{

public:
    VMTMachineAlpha(char sign,std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    bool IsPowerCalculator() override;
    bool IsAlphabit() override;

    void ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit) override;

    void Paint(UICanvas& canvas, const QRect& rect) override;
    virtual ~VMTMachineAlpha();

    IVMTAlphabitSource *GetAlphabitSource() override;
    void Update(IVMTEnvironment* environment) override;

    //IVMTAlphabitSource
    std::shared_ptr<VMTAlphabit> GetAlphabit() override;
    bool IsSingleChar() override;
    bool IsEnabled(char sign) override;
    void SetEnable(char sign, bool enable,IVMTEnvironment* environment) override;
    QPixmap* GetImage() override;
    QString GetSign();
    const QString& GetMessage() override;
protected:
	char _sign;

};
#endif // !defined(EA_0735A521_1ACC_4b60_B911_F969C5F9DA47__INCLUDED_)
