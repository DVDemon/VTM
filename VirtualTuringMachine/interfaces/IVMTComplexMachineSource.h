#if !defined(EA_88018E24_42D5_4480_B5B6_7A9DEF785464__INCLUDED_)
#define EA_88018E24_42D5_4480_B5B6_7A9DEF785464__INCLUDED_

#include "VMTComplexMachine.h"
#include "uicanvas.h"
#include "IVMTEnvironment.h"

class IVMTComplexMachineSource
{

public:
    virtual VMTComplexMachine* GetComplexMachine() =0;
    virtual UICanvas* GetDrawContext() =0;
    virtual IVMTEnvironment* GetEnvironment() =0;

};
#endif // !defined(EA_88018E24_42D5_4480_B5B6_7A9DEF785464__INCLUDED_)
