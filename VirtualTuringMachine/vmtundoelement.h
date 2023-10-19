#ifndef VMTUNDOELEMENT_H
#define VMTUNDOELEMENT_H

#include <memory>
#include <QString>

class VMTLine;
class VMTComplexMachine;
class UIStateData;
class IVMTEnvironment;

class VMTUndoElement
{
protected:
    std::weak_ptr<UIStateData> _ui_state;
    QString                    _current_machine_name;
public:
    VMTUndoElement();
    void                        SetUIState(std::weak_ptr<UIStateData> ptr);
    std::weak_ptr<UIStateData>  GetUIState();
    void                        SetCurrentMachineName(QString current_machine_name);
    const QString&              GetCurrentMachineName();
    virtual void Undo(IVMTEnvironment*) = 0;
    virtual ~VMTUndoElement();
};

#endif // VMTUNDOELEMENT_H
