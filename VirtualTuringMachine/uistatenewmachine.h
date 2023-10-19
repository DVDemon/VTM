#ifndef UISTATENEWMACHINE_H
#define UISTATENEWMACHINE_H

#include "uistatestub.h"
#include "formnewmachine.h"

class StateController;
class UIStateNewMachineData;

class UIStateNewMachine : public UIStateStub<FormNewMachine>
{
protected:


    UIStateNewMachine(StateController* parent): UIStateStub<FormNewMachine>(parent)
    {
        parent->EnableExercise(false);
        parent->EnableNewMachine(false);
        parent->EnableOpenMachine(false);
        parent->EnableSave(false);
        parent->EnableExport(false);
        parent->EnableRun(false);
        parent->EnableConfigure(false);
        parent->Enable4thExport(false);

        parent->EnableDebugBack(false);
        parent->EnableDebugOver(false);
        parent->EnableDebugPause(false);
        parent->EnableDebugRun(false);
        parent->EnableDebugStep(false);
        parent->EnableDebugStop(false);

        parent->EnableOk(false);
        parent->EnableCancel(false);

        parent->EnableZoom(false);
    }

    void Action(const QString& name) override{
      if(name == QString("button_ok")){

      }

    }

    friend UIStateNewMachineData;
};

class UIStateNewMachineData : public UIStateData{

public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateNewMachine>(new UIStateNewMachine(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateNewMachine>(new UIStateNewMachine(controller));
    }
    QString GetName() override{
        return "Machines";
    }

};

#endif // UISTATENEWMACHINE_H
