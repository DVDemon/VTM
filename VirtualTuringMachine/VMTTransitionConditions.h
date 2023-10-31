#if !defined(EA_8E22BF93_B3F1_4ef0_B3CD_FE57BE2AC4DD__INCLUDED_)
#define EA_8E22BF93_B3F1_4ef0_B3CD_FE57BE2AC4DD__INCLUDED_


class IVMTEnvironment;

#include "interfaces/IVMTAlphabitSource.h"

#include <QPoint>
#include <QString>
#include <QDataStream>
#include <QPixmap>

class IVMTTransition;

class VMTTransitionConditions :  public IVMTAlphabitSource
{
protected:
    std::shared_ptr<VMTAlphabit>  _alphabit;
    QString      _signs;
    IVMTTransition *_transition;
    void Update(IVMTEnvironment *environment);

public:
    VMTTransitionConditions(IVMTTransition *transition,std::shared_ptr<VMTAlphabit> alphabit);
    VMTTransitionConditions(const VMTTransitionConditions& other);
    VMTTransitionConditions& operator=(const VMTTransitionConditions& other);
    void Serialize(QDataStream&);
    void Deserialize(QDataStream&);
    void ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other);
    const QString& GetAsString();
    QPoint  GetDimensions(IVMTEnvironment* environment, int size);
	bool IsAllDisabled();
	bool IsAllEnabled();

    //IVMTAlphabitSource

    std::shared_ptr<VMTAlphabit> GetAlphabit() override;
    bool IsSingleChar() override;
    bool IsEnabled(char sign) override;
    void SetEnable(char sign, bool enable, IVMTEnvironment* environment) override;
    void SetDisableAllExclude(char sign, IVMTEnvironment* environment) override;
    QPixmap* GetImage() override;
    const QString& GetMessage() override;

    virtual ~VMTTransitionConditions();

};
#endif // !defined(EA_8E22BF93_B3F1_4ef0_B3CD_FE57BE2AC4DD__INCLUDED_)
