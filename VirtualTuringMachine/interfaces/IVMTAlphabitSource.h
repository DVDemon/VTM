#if !defined(EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_)
#define EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_

#include <QString>
#include <memory>

#include "VMTAlphabit.h"
class QPixmap;

class IVMTEnvironment;
class IVMTAlphabitSource {
   public:
    virtual std::shared_ptr<VMTAlphabit> GetAlphabit() = 0;
    virtual bool IsSingleChar() = 0;
    virtual bool IsEnabled(QChar sign) = 0;
    virtual QPixmap* GetImage() = 0;
    virtual const QString& GetMessage() = 0;
    virtual void SetEnable(QChar sign, bool enable,
                           IVMTEnvironment* environment) = 0;
    virtual void SetDisableAllExclude(
        [[maybe_unused]] QChar sign,
        [[maybe_unused]] IVMTEnvironment* environment) {};

    virtual ~IVMTAlphabitSource() = default;
};
#endif  // !defined(EA_E676FEFF_500A_4f44_9F83_0DB87EFB6E44__INCLUDED_)
