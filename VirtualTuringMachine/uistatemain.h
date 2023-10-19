#ifndef UISTATEMAIN_H
#define UISTATEMAIN_H
#include "uistatestub.h"
#include "formmain.h"

class UIStateMainData;
class UIStateMain : public UIStateStub<FormMain>
{
protected:
    UIStateMain(StateController* parent): UIStateStub<FormMain>(parent)
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

    void Action(const QString& ) override{

    }

friend UIStateMainData;
};

class UIStateMainData : public UIStateData{
public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateMain>(new UIStateMain(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateMain>(new UIStateMain(controller));
    }
    QString GetName() override{
        return QString("VTM");
    }
};
#endif // UISTATEMAIN_H
