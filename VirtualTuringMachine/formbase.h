#ifndef FORMBASE_H
#define FORMBASE_H

#include "statecontroller.h"
#include "uistate.h"

class FormBase{
protected:
    StateController *_controller = nullptr;
public:
    void SetController(StateController *c){
        _controller = c;
    }

    virtual void BeforeChange(std::shared_ptr<UIStateData> state_data) = 0;
    virtual void AfterUndo(std::shared_ptr<UIStateData> state_data) = 0;
    virtual void OnLoaded() {};

};

#endif // FORMBASE_H
