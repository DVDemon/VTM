#ifndef UISTATEEXERCISES_H
#define UISTATEEXERCISES_H

#include "uistatestub.h"
#include "formexercises.h"
#include "uistateeditormachine.h"
#include "configuration.h"
#include <memory>
#include <QMessageBox>
class StateController;
class UIStateExercisesData;

class UIStateExercises : public UIStateStub<FormExercises>
{
public:


    UIStateExercises(StateController* parent): UIStateStub<FormExercises>(parent)
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

    void Action([[maybe_unused]] const QString& name) override{

    }

    friend UIStateExercisesData;
};

class UIStateExercisesData : public UIStateData{

public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::make_shared<UIStateExercises>(controller);
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::make_shared<UIStateExercises>(controller);
    }

    bool IsNeedWarning() override {
        return false;
    }

    QString GetName() override{
        return name;
    }

    void SetName(QString name) override{
        this->name = name;
    }

    QString name="Exercises";
};
#endif // UISTATEEXERCISES_H
