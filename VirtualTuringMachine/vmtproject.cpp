#include "vmtproject.h"

VMTProject VMTProject::_instance;

VMTProject  & VMTProject::GetInstance(){
    return _instance;
}

VMTProject::VMTProject() : _exercise(nullptr),_test_mode(false)
{
    _alphabit = std::shared_ptr<VMTAlphabit>(new VMTAlphabit());
    _undo =  std::shared_ptr<VMTUndoManager>(new VMTUndoManager());
}

bool  VMTProject::IsTestMode(){
    return _test_mode;
}
void VMTProject::SetTestMode(bool test){
   _test_mode = test;
}
std::shared_ptr<Exercise> VMTProject::GetExercise(){
    return _exercise;
}

void VMTProject::SetExercise(std::shared_ptr<Exercise> ex){
    _exercise = ex;
}

void VMTProject::SetUIState(std::shared_ptr<UIStateData> state){
    _ui_state = state;
}

std::weak_ptr<UIStateData>                        VMTProject::GetUIState(){
    return _ui_state;
}

void VMTProject::SetAlphabit(std::shared_ptr<VMTAlphabit> alphabit){
    _alphabit = alphabit;
}

QString     & VMTProject::GetCurrentMachineName(){
    return _current_machine_name;
}

std::shared_ptr<VMTComplexMachine> VMTProject::GetCurrentMachine(){
    for(auto machine: _machines){
        if(_current_machine_name == machine->GetName()) return machine;
    }

    return std::shared_ptr<VMTComplexMachine>();
}

std::shared_ptr<VMTComplexMachine> VMTProject::GetMachine(QString& name){

    for(auto machine: _machines){
        if(name == machine->GetName()) return machine;
    }

    return std::shared_ptr<VMTComplexMachine>();
}

std::vector<std::shared_ptr<VMTComplexMachine>> & VMTProject::GetMachines(){
    return _machines;
}

std::shared_ptr<VMTUndoManager>  VMTProject::GetUndoManager(){
    return _undo;
}

std::shared_ptr<VMTAlphabit> VMTProject::GetAlphabit(){
    return _alphabit;
}

QString     & VMTProject::GetName(){
    return _name;
}

QString     & VMTProject::GetLocation(){
    return _location;
}
