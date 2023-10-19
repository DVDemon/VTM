#if !defined(EA_88586C2F_1B51_464b_A272_C70BB4980D1F__INCLUDED_)
#define EA_88586C2F_1B51_464b_A272_C70BB4980D1F__INCLUDED_

#include <QPoint>
#include <QRect>
#include "IVMTAction.h"
#include <memory>

class IVMTActionController
{

public:
    virtual void SetAction(std::shared_ptr<IVMTAction> action) =0;

};
#endif // !defined(EA_88586C2F_1B51_464b_A272_C70BB4980D1F__INCLUDED_)
