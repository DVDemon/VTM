#ifndef FORMCOMPILERMACHINE_H
#define FORMCOMPILERMACHINE_H

#include <QWidget>
#include "formbase.h"

#include <vector>
#include "vmterrormachine.h"
#include "vmterrortransition.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"

namespace Ui {
class FormCompilerMachine;
}

class FormCompilerMachine : public QWidget , public FormBase, public IVMTEnvironment, public IVMTActionController
{
    Q_OBJECT

public:
    explicit FormCompilerMachine(QWidget *parent = 0);
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void OnLoaded() override;
    void ChangeZoom(int zoom);

    bool HasErrors();

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
    //IVMTActionController
    void SetAction(std::shared_ptr<IVMTAction> action) override;
    void EnableAnimation(bool enable) override;

    ~FormCompilerMachine();

private slots:
    void on_list_errors_currentRowChanged(int currentRow);

private:
    Ui::FormCompilerMachine *ui;
    std::vector<VMTError*> _errors;

    void ClearErrors();
    void Check();
    void CheckStart(std::shared_ptr<VMTComplexMachine> p_Machine);
    void CheckFinish(std::shared_ptr<VMTComplexMachine> p_Machine);
    void CheckConnection(std::shared_ptr<VMTComplexMachine> p_Machine);
    void CheckAllCases(std::shared_ptr<VMTComplexMachine> p_Machine);
    void CheckDublicate(std::shared_ptr<VMTComplexMachine> p_Machine);


};

#endif // FORMCOMPILERMACHINE_H
