#ifndef UISTATEEXPORT4TH_H
#define UISTATEEXPORT4TH_H

#include "uistatestub.h"
#include "formexport4th.h"

class StateController;
class UIStateExport4thData;

class UIStateExport4th : public UIStateStub<FormExport4th>
{
protected:


    UIStateExport4th(StateController* parent): UIStateStub<FormExport4th>(parent)
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

    friend UIStateExport4thData;
};

class UIStateExport4thData : public UIStateData{

public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateExport4th>(new UIStateExport4th(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateExport4th>(new UIStateExport4th(controller));
    }
    QString GetName() override{
        return "Export 4th";
    }

};
#endif // UISTATEEXPORT4TH_H
