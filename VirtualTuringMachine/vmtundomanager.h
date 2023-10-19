#ifndef VMTUNDOMANAGER_H
#define VMTUNDOMANAGER_H

#include <stack>
class VMTComplexMachine;
class VMTLine;
class IVMTEnvironment;

#include "vmtundoelement.h"
#include "interfaces/ivmtundoobserver.h"

class VMTUndoManager
{
protected:
    std::stack<std::shared_ptr<VMTUndoElement>> _stack;
    IVMTUndoObserver *_observer;
public:
    VMTUndoManager();
    void SetObserver(IVMTUndoObserver * observer);
    void Remember(std::shared_ptr<VMTUndoElement> element);
    bool Recall(IVMTEnvironment *environment);
    void Clear();
    bool IsEnable();
};

#endif // VMTUNDOMANAGER_H
