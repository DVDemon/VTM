#include "vmtundoelement.h"

VMTUndoElement::VMTUndoElement()
{

}

void VMTUndoElement::SetUIState(std::weak_ptr<UIStateData> ptr){
    _ui_state = ptr;
}

std::weak_ptr<UIStateData>  VMTUndoElement::GetUIState(){
    return _ui_state;
}

void                        VMTUndoElement::SetCurrentMachineName(QString current_machine_name){
  _current_machine_name = current_machine_name;
}

const QString&              VMTUndoElement::GetCurrentMachineName(){
  return _current_machine_name;
}

VMTUndoElement::~VMTUndoElement()
{

}
