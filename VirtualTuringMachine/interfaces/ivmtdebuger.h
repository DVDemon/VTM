#ifndef IVMTDEBUGER_H
#define IVMTDEBUGER_H

#include "IVMTCompilerCallback.h"

class IVMTMachine;

class IVMTDebuger : public IVMTCompilerCallback{

protected:
    virtual IVMTMachine* FindNextMachine() = 0;
    virtual void Next() = 0;
    virtual void MainLoop() = 0;
public:
    virtual void DoStep() = 0;
    virtual void DoUndo() = 0;
    virtual void DoStepOver() = 0;
    virtual void DoPause() = 0;
    virtual void DoStop() = 0;

};

#endif // IVMTDEBUGER_H
