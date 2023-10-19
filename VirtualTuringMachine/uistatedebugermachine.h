#ifndef UISTATEDEBUGERMACHINE_H
#define UISTATEDEBUGERMACHINE_H

#include "uistatestub.h"
#include "formdebugermachine.h"
class UIStateDebugerMachineData;

class UIStateDebugerMachine : public UIStateStub<FormDebugerMachine>
{
protected:
    UIStateDebugerMachine(StateController* parent): UIStateStub<FormDebugerMachine>(parent)
    {

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
        if(VMTProject::GetInstance().IsTestMode()){
            parent->EnableExercise(true);
            parent->EnableDebugRun(false);
            parent->EnableDebugStep(false);
        } else {
            parent->EnableExercise(false);
            parent->EnableDebugRun(true);
            parent->EnableDebugStep(true);
        }
        parent->EnableDebugStop(false);

        parent->EnableOk(false);
        parent->EnableCancel(false);

        parent->EnableZoom(true);
        form.ChangeZoom(parent->GetZoom());
    }

    void OnZoomChanged(int zoom) override {
      form.ChangeZoom(zoom);
    }


    void Action(const QString& name) override{
        if(name == QString("button_debug_step"))
            form.Step();
        if(name == QString("button_debug_run"))
            form.Run();
        if(name == QString("button_debug_stop"))
            form.Stop();
        if(name == QString("button_exercise"))
            form.StartExercise();
    }

    friend UIStateDebugerMachineData;
};

class UIStateDebugerMachineData : public UIStateData{
public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateDebugerMachine>(new UIStateDebugerMachine(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateDebugerMachine>(new UIStateDebugerMachine(controller));
    }
    QString GetName() override{
        return QString("Debuger");
    }
};
#endif // UISTATEDEBUGERMACHINE_H
