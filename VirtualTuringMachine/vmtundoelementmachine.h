#ifndef VMTUNDOELEMENTMACHINE_H
#define VMTUNDOELEMENTMACHINE_H

#include "vmtundoelement.h"
#include <QBuffer>
#include <QByteArray>

class VMTUndoElementMachine : public VMTUndoElement
{
protected:
    QByteArray _array;
public:
    VMTUndoElementMachine();
    void Undo(IVMTEnvironment*) override;
    virtual ~VMTUndoElementMachine();
};

#endif // VMTUNDOELEMENTMACHINE_H
