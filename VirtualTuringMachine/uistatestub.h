#ifndef UISTATESTUB_H
#define UISTATESTUB_H

#include "uistate.h"
#include "mainwindow.h"
#include "statecontroller.h"
#include <QLayout>
#include <QDebug>

template <class T> class UIStateStub : public UIState{
protected:
    T       form;
public:
    UIStateStub(StateController* parent) : UIState(parent),form(parent->GetContainer()){
        qDebug() << "UIStateStub::ctr";
        parent->GetContainer()->layout()->addWidget(&form);
        parent->GetContainer()->layout()->setContentsMargins(0,0,0,0);
        qDebug() << "UIStateStub::SetController";
        form.SetController(parent);
        qDebug() << "UIStateStub::OnLoaded";
        form.OnLoaded();
        qDebug() << "UIStateStub::ctr done";
    }

    void BeforeChange(std::shared_ptr<UIStateData> state_data) override{
      form.BeforeChange(state_data);
    }

    void AfterUndo(std::shared_ptr<UIStateData> state_data) override {
      form.AfterUndo(state_data);
    }

    void RemoveState() override {
        qDebug() << "UIStateStub::Remove widget";
        parent->GetContainer()->layout()->removeWidget(&form);
        form.setVisible(false);
        qDebug() << "UIStateStub::Remove widget done";
    }

    ~UIStateStub(){

    }
};

#endif // UISTATESTUB_H
