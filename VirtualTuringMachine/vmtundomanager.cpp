#include "vmtundomanager.h"
#include "interfaces/IVMTEnvironment.h"
#include "vmtproject.h"

VMTUndoManager::VMTUndoManager() : _observer(nullptr)
{

}

void VMTUndoManager::SetObserver(IVMTUndoObserver * observer){
   _observer = observer;
}

bool VMTUndoManager::IsEnable(){
     return !_stack.empty();
}

void VMTUndoManager::Remember(std::shared_ptr<VMTUndoElement> element){
    if(element)  {
        element->SetCurrentMachineName(VMTProject::GetInstance().GetCurrentMachineName());
        element->SetUIState(VMTProject::GetInstance().GetUIState());
        _stack.push(element);
        qDebug() << "Remember";
        if(_observer) _observer->EnableUndoButton(IsEnable());
    }
}

bool VMTUndoManager::Recall(IVMTEnvironment *environment){

    if(!_stack.empty()){
        std::shared_ptr<VMTUndoElement> element = _stack.top();
        _stack.pop();
        element->Undo(environment);
        VMTProject::GetInstance().GetCurrentMachineName() = element->GetCurrentMachineName();
        if(_observer) _observer->EnableUndoButton(IsEnable());
        return true;
    } else return false;

}

void VMTUndoManager::Clear(){
    while(!_stack.empty()) _stack.pop();

    qDebug() << "Observer!!!";
    if(_observer) _observer->EnableUndoButton(IsEnable());
}
