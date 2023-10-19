#ifndef FORMEDITORMACHINE_H
#define FORMEDITORMACHINE_H

#include <QWidget>
#include "formbase.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTActionController.h"
#include <QButtonGroup>
#include "formalphabit.h"
#include "formcalculator.h"
#include "formcomplexmachines.h"
#include "formactionhint.h"

namespace Ui {
class FormEditorMachine;
}

class FormEditorMachine : public QWidget, public FormBase , public IVMTEnvironment , public IVMTActionController
{
    Q_OBJECT

public:
    explicit FormEditorMachine(QWidget *parent = 0);
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void ChangeZoom(size_t zoom);
    void ExportMachine();
    void Undo();
    void Exercise();
    void Run();


    void EnableFormComplexMachines();
    void DisableFormComplexMachines();

    void OnKeyPressed(QKeyEvent* event);
    void OnKeyReleased(QKeyEvent* event);

    ~FormEditorMachine();

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

protected:
    void keyPressEvent (QKeyEvent* event) override;

private slots:
    void on_tool_pointer_clicked();

    void on_tool_link_clicked();

    void on_tool_translate_clicked();

    void on_tool_trash_clicked();

    void on_tool_machine_start_clicked();

    void on_tool_machine_lambda_clicked();

    void on_tool_machine_finish_clicked();

    void on_tool_machine_left_clicked();

    void on_tool_machine_right_clicked();

    void on_tool_machine_left_word_clicked();

    void on_tool_machine_right_word_clicked();

    void on_tool_machine_copy_clicked();



    void on_tool_complex_clicked();

    void on__navi_clicked();

private:
    Ui::FormEditorMachine *ui;
    std::shared_ptr<QButtonGroup> _tools_group;
    FormAlphabit* _form_alphabit;
    FormCalculator *_form_calculator;
    FormComplexMachines *_form_complex_machines;
    FormActionHint *_form_action_hint;
    std::shared_ptr<IVMTAction> _action_space;

};

#endif // FORMEDITORMACHINE_H
