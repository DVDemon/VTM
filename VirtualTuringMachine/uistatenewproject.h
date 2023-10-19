#ifndef UISTATENEWPROJECT_H
#define UISTATENEWPROJECT_H
#include "uistatestub.h"
#include "formnewproject.h"
#include "uistateeditormachine.h"
#include "configuration.h"
#include <QMessageBox>
class StateController;
class UIStateNewProjectData;

class UIStateNewProject : public UIStateStub<FormNewProject>
{
protected:


    UIStateNewProject(StateController* parent): UIStateStub<FormNewProject>(parent)
    {
        parent->EnableExercise(false);
        parent->EnableNewMachine(false);
        parent->EnableOpenMachine(false);
        parent->EnableSave(true);
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
      parent->Update();
      if(name == QString("button_save")){
          QString name;
          GetProjectFileName(name);
          VMTSerializer serializer(name);
          serializer.Serialize(&VMTProject::GetInstance());

          Configuration::GetInstance().AddRecentProject(name);
          Configuration::GetInstance().Save();
          QMessageBox box;
          box.setText("Project saved");
          box.exec();

      }

    }

    friend UIStateNewProjectData;

public:
    static void GetProjectFileName(QString &name){
        name = VMTProject::GetInstance().GetLocation();
        name += "/";
        name += VMTProject::GetInstance().GetName();
        if(!name.endsWith(".jdpt")) name +=".jdtp";
    }
};

class UIStateNewProjectData : public UIStateData{

public:
    std::shared_ptr<UIState> ConstructNewState(StateController * controller) override{
        return std::shared_ptr<UIStateNewProject>(new UIStateNewProject(controller));
    }
    std::shared_ptr<UIState> ConstructUndoState(StateController * controller) override{
        return std::shared_ptr<UIStateNewProject>(new UIStateNewProject(controller));
    }

    bool IsNeedWarning() override {
        return true;
    }

    QString GetName() override{
        return name;
    }

    void SetName(QString name) override{
        this->name = name;
    }

    QString name="New machine";
};

#endif // UISTATENEWPROJECT_H
