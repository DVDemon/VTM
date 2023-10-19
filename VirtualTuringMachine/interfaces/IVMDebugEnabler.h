#if !defined(EA_B29202CE_6D51_4b60_8631_FEBF36244D66__INCLUDED_)
#define EA_B29202CE_6D51_4b60_8631_FEBF36244D66__INCLUDED_

#include "ivmtdebuger.h"

class IVMDebugEnabler
{

public:
	IVMDebugEnabler() {

	}

	virtual ~IVMDebugEnabler() {

	}

	virtual void DisableDebug() =0;
    virtual void EnableDebug(IVMTDebuger* debuger) =0;

};
#endif // !defined(EA_B29202CE_6D51_4b60_8631_FEBF36244D66__INCLUDED_)
