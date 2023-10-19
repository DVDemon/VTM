#ifndef UISTATEEDITORMACHINE_H
#define UISTATEEDITORMACHINE_H
#include "uistatestub.h"
#include "formmain.h"
#include "formeditormachine.h"
#include "uistatecompilermachine.h"
#include "uistateexport4th.h"
#include "vmtproject.h"
#include "interfaces/ivmtundoobserver.h"

class UIStateEditorMachineData;

class UIStateEditorMachine : public UIStateStub<FormEditorMachine> , public IVMTUndoObserver
{
protected:
    UIStateEditorMachine(StateController* parent): UIStateStub<FormEditorMachine>(parent)
    {
        VMTProject::GetInstance().SetTestMode(false);
        if(VMTProject::GetInstance().GetExercise()) parent->EnableExercise(true);
                                                else parent->EnableExercise(false);

        parent->EnableNewMachine(false);
        parent->EnableOpenMachine(false);
        parent->EnableSave(false);
        parent->EnableExport(true);
        parent->EnableRun(true);
        parent->EnableConfigure(false);
        parent->Enable4thExport(true);

        if(VMTProject::GetInstance().GetUndoManager()->IsEnable()) parent->EnableDebugBack(true);
                                                              else parent->EnableDebugBack(false);
        VMTProject::GetInstance().GetUndoManager()->SetObserver(this);
        parent->EnableDebugOver(false);
        parent->EnableDebugPause(false);
        parent->EnableDebugRun(false);
        parent->EnableDebugStep(false);
        parent->EnableDebugStop(false);

        parent->EnableOk(false);
        parent->EnableCancel(false);

        parent->EnableZoom(true);
        form.ChangeZoom(parent->GetZoom());
    }

    void OnKeyPressed(QKeyEvent* event) override{
      form.OnKeyPressed(event);
    }

    void OnKeyReleased(QKeyEvent* event) override{
       form.OnKeyReleased(event);
    }
    void EnableUndoButton(bool enable) override{
        parent->EnableDebugBack(enable);
    }

    void OnZoomChanged(int zoom) override {
      form.ChangeZoom(zoom);
    }

    void Action(const QString& name) override{
          if(name == QString("button_run"))
          {
              form.Run();
          } else
          if(name == QString("button_exercise"))
              {
                  form.Exercise();
              } else
          if(name == QString("button_export"))
          {
              form.ExportMachine();
          } else
          if(name == QString("button_back"))
          {
              form.Undo();
          }else
          if(name == QString("button_4th"))
              {
              this->parent->ChangeState(
                          std::shared_ptr<UIStateExport4thData>(new UIStateExport4thData()));
              }
    }

    friend UIStateEditorMachineData;
};

#include "vmtmachines/VMTComplexMachine.h"
#include <memory>

class UIStateEditorMachineData : public UIStateData{
public:
    //std::shared_ptr<VMTComplexMachine> _machine;

    bool IsNeedWarning() override {
        return false;
    }

    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateEditorMachine>(new UIStateEditorMachine(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateEditorMachine>(new UIStateEditorMachine(controller));
    }
    QString GetName() override{
        return VMTProject::GetInstance().GetCurrentMachineName();
    }
};
#endif // UISTATEEDITORMACHINE_H
