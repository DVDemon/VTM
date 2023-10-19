#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include <uistate.h>
#include <vector>
#include <QDebug>
#include <QMessageBox>
#include "vmtproject.h"
#include <QApplication>
#include <deque>

class StateController{
protected:
    std::vector<std::shared_ptr<UIStateData>> undo;
    std::shared_ptr<UIState>     _state;
    std::shared_ptr<UIStateData> _state_data;

    std::deque<std::shared_ptr<UIState>> _old_states;
public:
    virtual QWidget* GetContainer() = 0;

    void Update()
    {
       if(_state)  _state->BeforeChange(_state_data);
    }

    void ChangeState(std::shared_ptr<UIStateData> new_state_data){
        qDebug() << "Clear old states";
        ClearOldStates();
        qDebug() << "Clear observer";
        VMTProject::GetInstance().GetUndoManager()->SetObserver(nullptr);
        qDebug() << "StateController::Before change";
        if(_state)  _state->BeforeChange(_state_data);
        if(_state_data)
        {
            qDebug() << "StateController::Place undo";
            if(_state_data->IsUndoPlacable()) undo.push_back(_state_data);
        }

        _state_data = new_state_data;
        VMTProject::GetInstance().SetUIState(_state_data);
        qDebug() << "StateController::Construct state";
        if(_state_data){
         if(_state) {
             _state->RemoveState();
             _old_states.push_back(_state);
         }
         _state = _state_data->ConstructNewState(this);
         qDebug() << "StateController::Call On Change";
         OnChanged(_state,_state_data);
        }
        qDebug() << "StateController::ChangeState Done";
    }

    virtual void ClearOldStates(){
        _old_states.clear();
    }

    virtual void ShowWarning(std::shared_ptr<UIStateData>) =0;
    virtual void HideWarning(bool undo) =0;
    virtual void OnChanged( std::shared_ptr<UIState> , std::shared_ptr<UIStateData> ){

    }

    bool Undo(){
        bool warning = false;
        if(_state_data){
            qDebug() << "Clear observer";
            VMTProject::GetInstance().GetUndoManager()->SetObserver(nullptr);
            if(_state_data->IsNeedWarning()){
                warning = true;

                QMessageBox::StandardButton resBtn = QMessageBox::question( nullptr,
                                                                            "VMT",
                                                                            "This will delete any unsaved project data. Are you sure?",
                                                                            QMessageBox::No | QMessageBox::Yes,
                                                                            QMessageBox::No);
                if (resBtn != QMessageBox::Yes) {
                    return false;
                    //event->ignore();
                } else {
                   warning = false;
                    //event->accept();
                }

                //ShowWarning(state_data);

            }
        }

        if(!warning)  UndoForce();
        return true;
    }

    void UndoForce(){
        if(!undo.empty()){
            _state_data = undo.back();
            undo.pop_back();
            if(_state)  _state->BeforeChange(_state_data);
            if(_state_data){
            _state = _state_data->ConstructUndoState(this);
            if(_state)
            _state->AfterUndo(_state_data);
            OnChanged(_state,_state_data);
            }
        } else QApplication::quit();
    }

    virtual void EnableExercise(bool enable) = 0;
    virtual void EnableNewMachine(bool enable) = 0;
    virtual void EnableOpenMachine(bool enable) = 0;
    virtual void EnableSave(bool enable) = 0;
    virtual void EnableExport(bool enable) = 0;
    virtual void EnableRun(bool enable) = 0;
    virtual void EnableConfigure(bool enable) = 0;
    virtual void Enable4thExport(bool enable) = 0;

    virtual void EnableDebugBack(bool enable) = 0;
    virtual void EnableDebugOver(bool enable) = 0;
    virtual void EnableDebugPause(bool enable) = 0;
    virtual void EnableDebugRun(bool enable) = 0;
    virtual void EnableDebugStep(bool enable) = 0;
    virtual void EnableDebugStop(bool enable) = 0;

    virtual void EnableOk(bool enable) = 0;
    virtual void EnableCancel(bool enable) = 0;

    virtual void EnableZoom(bool enable) = 0;
    virtual int  GetZoom() = 0;

};

#endif // STATECONTROLLER_H
