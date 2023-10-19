#ifndef VMTPROJECT_H
#define VMTPROJECT_H

#include "VMTAlphabit.h"
#include "vmtproject.h"
#include "exercise.h"
#include <memory>
#include <vector>
#include <QString>
#include <vmtmachines/VMTComplexMachine.h>
#include <VMTSerializer.h>
class UIStateData;

class VMTProject
{
protected:
    static VMTProject _instance;


    QString                    _name;
    QString                    _location;
    QString                    _current_machine_name;
    std::weak_ptr<UIStateData> _ui_state;
    bool                       _test_mode;

    std::shared_ptr<VMTAlphabit>                    _alphabit;
    std::shared_ptr<VMTUndoManager>                 _undo;
    std::shared_ptr<Exercise>                       _exercise;
    std::vector<std::shared_ptr<VMTComplexMachine>> _machines;

    VMTProject();
public:
    static VMTProject  & GetInstance();


    bool          IsTestMode();
    QString     & GetName();
    QString     & GetLocation();
    QString     & GetCurrentMachineName();

    void SetTestMode(bool);
    void SetAlphabit(std::shared_ptr<VMTAlphabit> alphabit);
    void SetExercise(std::shared_ptr<Exercise>);
    void SetUIState(std::shared_ptr<UIStateData> state);
    std::shared_ptr<VMTAlphabit>                      GetAlphabit();
    std::shared_ptr<Exercise>                         GetExercise();
    std::shared_ptr<VMTUndoManager>                   GetUndoManager();
    std::shared_ptr<VMTComplexMachine>                GetMachine(QString& name);
    std::shared_ptr<VMTComplexMachine>                GetCurrentMachine();
    std::weak_ptr<UIStateData>                        GetUIState();
    std::vector<std::shared_ptr<VMTComplexMachine>> & GetMachines();
};

#endif // VMTPROJECT_H
