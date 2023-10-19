#ifndef UISTATE_H
#define UISTATE_H

#include <QString>
#include <QKeyEvent>
#include <memory>
class StateController;
class UIStateData;

class UIState
{
protected:
    StateController *parent;
    UIState(StateController* p) : parent(p) {
    }

public:

    virtual void BeforeChange(std::shared_ptr<UIStateData> state_data) = 0;
    virtual void AfterUndo(std::shared_ptr<UIStateData> state_data) = 0;
    virtual void OnLoaded(std::shared_ptr<UIStateData>) {};
    virtual void Action(const QString& name) = 0;
    virtual void OnZoomChanged(int) {}
    virtual void OnKeyPressed(QKeyEvent* event) {};
    virtual void OnKeyReleased(QKeyEvent* event) {};
    virtual void RemoveState() = 0;
    virtual ~UIState(){

    }
};

class UIStateData{
public:
    virtual bool IsUndoPlacable() { return true;}
    virtual bool IsNeedWarning() { return false;}
    virtual std::shared_ptr<UIState> ConstructNewState(StateController * controller) = 0;
    virtual std::shared_ptr<UIState> ConstructUndoState(StateController * controller) = 0;
    virtual QString GetName() = 0;
    virtual void SetName(QString) {};
};

#endif // UISTATE_H
