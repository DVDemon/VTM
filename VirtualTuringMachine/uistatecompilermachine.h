#ifndef UISTATECOMPILERMACHINE_H
#define UISTATECOMPILERMACHINE_H

#include "uistatestub.h"
#include "formcompilermachine.h"
#include "uistatedebugermachine.h"

class UIStateCompilerMachineData;

class UIStateCompilerMachine : public UIStateStub<FormCompilerMachine>
{
protected:
    UIStateCompilerMachine(StateController* parent): UIStateStub<FormCompilerMachine>(parent)
    {
        qDebug() << "UIStateCompilerMachine::ctr";
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

        parent->EnableZoom(true);

        form.ChangeZoom(parent->GetZoom());
        qDebug() << "UIStateCompilerMachine::ctr done";

    }

    void OnLoaded(std::shared_ptr<UIStateData> ) override{
        if(!form.HasErrors())
            parent->ChangeState(std::shared_ptr<UIStateDebugerMachineData>(new UIStateDebugerMachineData()));

    }

    void OnZoomChanged(int zoom) override {
      form.ChangeZoom(zoom);
    }


    void Action(const QString& name) override{
        if(name == QString("button_run"))
            parent->ChangeState(std::shared_ptr<UIStateDebugerMachineData>(new UIStateDebugerMachineData()));


    }

    friend UIStateCompilerMachineData;
};

class UIStateCompilerMachineData : public UIStateData{
public:
    bool IsUndoPlacable() override {
        return false;
    }
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        qDebug() << "UIStateCompilerMachineData::ConstructNewState";
        return std::shared_ptr<UIStateCompilerMachine>(new UIStateCompilerMachine(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateCompilerMachine>(new UIStateCompilerMachine(controller));
    }
    QString GetName() override{
        return QString("Compiler");
    }
};
#endif // UISTATECOMPILERMACHINE_H
