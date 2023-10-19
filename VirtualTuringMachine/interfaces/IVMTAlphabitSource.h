#if !defined(EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_)
#define EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_

#include <memory>
#include "VMTAlphabit.h"
#include <QString>
class QPixmap;

class IVMTEnvironment;
class IVMTAlphabitSource
{

public:
    virtual std::shared_ptr<VMTAlphabit> GetAlphabit() =0;
    virtual bool IsSingleChar() = 0;
	virtual bool IsEnabled(char sign) =0;
    virtual QPixmap* GetImage() = 0;
    virtual const QString& GetMessage() = 0;
    virtual void SetEnable(char sign, bool enable,IVMTEnvironment* environment) =0;

};
#endif // !defined(EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_)
