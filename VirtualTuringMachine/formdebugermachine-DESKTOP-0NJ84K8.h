#ifndef FORMDEBUGERMACHINE_H
#define FORMDEBUGERMACHINE_H

#include <QWidget>
#include <QTimer>
#include "formbase.h"
#include "vmtdebuger.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"

namespace Ui {
class FormDebugerMachine;
}

class FormDebugerMachine : public QWidget , public FormBase, public IVMTEnvironment, public IVMTActionController
{
    Q_OBJECT

public:
    explicit FormDebugerMachine(QWidget *parent = 0);
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void OnLoaded() override;
    void ChangeZoom(int zoom);
    void Step();
    void Run();
    void Stop();

    // IVMTEnvironment
    void EnableActionHint(const QString &hint,QPixmap& img) override;
    void DisableActionHint() override;
    void DisableAlphabit() override;
    void DisableCalculator() override;
    void EnableAlphabit(IVMTAlphabitSource* p_Alphabit,const QString& msg,QPixmap &img) override;
    void EnableCalculator(std::shared_ptr<IVMTMachine> machine) override;
    UICanvas& GetGraphics() override;
    std::weak_ptr<VMTComplexMachine> GetMachine() override;
    void Repaint(const QRect& rect) override;
    void MoveInScreen(QPoint &&shift) override;
    void Move(QPoint &&shift) override;
    void SetMachine(std::shared_ptr<VMTComplexMachine> machine) override;
    void EnableAnimation(bool enable) override;

    //IVMTActionController
    void SetAction(std::shared_ptr<IVMTAction> action) override;

    ~FormDebugerMachine();

public slots:
        void RunTimer();

private slots:
    void on_button_left_page_clicked();

    void on_button_left_clicked();

    void on_button_right_clicked();

    void on_button_right_page_clicked();



private:
    Ui::FormDebugerMachine *ui;
    bool _stop;
    QTimer _timer;


    VMTDebuger _debugger;



};

#endif // FORMDEBUGERMACHINE_H
