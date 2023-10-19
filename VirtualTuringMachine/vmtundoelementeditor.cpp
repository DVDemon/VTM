#include "vmtundoelementeditor.h"
#include <QDebug>
#include "interfaces/IVMTEnvironment.h"

VMTUndoElementEditor::VMTUndoElementEditor(IVMTEnvironment* environment)
{
 _state = environment->GetGraphics().GetState();
}

void VMTUndoElementEditor::Undo(IVMTEnvironment* environment){
 environment->GetGraphics().SetState(_state);
 environment->Repaint(QRect());
}

