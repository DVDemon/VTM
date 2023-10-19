#include "vmtundoelementmachine.h"
#include <QDebug>
#include "VMTSerializer.h"
#include "vmtproject.h"

VMTUndoElementMachine::VMTUndoElementMachine()
{
  QBuffer buffer(&_array);
  VMTSerializer serializer(VMTProject::GetInstance().GetName());
  serializer.Serialize(&VMTProject::GetInstance(),buffer);
}

void VMTUndoElementMachine::Undo(IVMTEnvironment*){
  QBuffer buffer(&_array);
  VMTSerializer serializer(VMTProject::GetInstance().GetName());
  serializer.Deserialize(&VMTProject::GetInstance(),buffer);
}

VMTUndoElementMachine::~VMTUndoElementMachine(){
}
