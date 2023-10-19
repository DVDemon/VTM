

#if !defined(EA_10E536BA_6054_438c_B839_CD170B2144CB__INCLUDED_)
#define EA_10E536BA_6054_438c_B839_CD170B2144CB__INCLUDED_

#include "VMTComplexMachine.h"
#include "IVMTMachine.h"

class IVMTDebugerCallback
{
public:
    virtual void AddStack(VMTComplexMachine* machine) =0;
	virtual void DebugerFinish() =0;
    virtual void DebugerMachineChanged(VMTComplexMachine* complex, IVMTMachine* current) =0;
    virtual void RemoveStack(VMTComplexMachine* machine) =0;

};
#endif // !defined(EA_10E536BA_6054_438c_B839_CD170B2144CB__INCLUDED_)
