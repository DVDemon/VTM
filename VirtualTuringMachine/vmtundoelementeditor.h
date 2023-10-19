#ifndef VMTUNDOELEMENTEDITOR_H
#define VMTUNDOELEMENTEDITOR_H

#include "vmtundoelement.h"
#include "uicanvas.h"

class VMTUndoElementEditor : public VMTUndoElement
{
protected:
    UICanvasState _state;
public:
    VMTUndoElementEditor(IVMTEnvironment*);
    void Undo(IVMTEnvironment*) override;
};

#endif // VMTUNDOELEMENTEDITOR_H
