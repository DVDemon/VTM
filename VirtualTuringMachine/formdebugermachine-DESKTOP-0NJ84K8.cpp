#include "formdebugermachine.h"
#include "ui_formdebugermachine.h"
#include <set>
#include "vmtproject.h"
#include "vmtmachines/VMTComplexMachineInner.h"
#include "vmtactions/vmtactiontranslate.h"
#include "screentools.h"

FormDebugerMachine::FormDebugerMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDebugerMachine),
    _debugger(VMTProject::GetInstance().GetAlphabit(),VMTProject::GetInstance().GetCurrentMachine())
{
    ui->setupUi(this);
    ui->widget_line->SetLine(_debugger.GetLine());
    ui->widget_editor->SetEnvironment(this);
    ui->widget_editor->SetComplexMachine( _debugger.GetComplexMachine());
    ui->widget_editor->GetComplexMachine()->DeselectAll();
    ui->toolBox->setHidden(true);
    _stop = false;

    ScreenTools st;

    st.ResizeButton(ui->button_left);
    st.ResizeButton(ui->button_left_page);
    st.ResizeButton(ui->button_right);
    st.ResizeButton(ui->button_right_page);

}

void FormDebugerMachine::BeforeChange(std::shared_ptr<UIStateData>){

}

void FormDebugerMachine::AfterUndo(std::shared_ptr<UIStateData>) {

}

void FormDebugerMachine::ChangeZoom(int zoom){
  ui->widget_editor->SetZoom(zoom);
}

void FormDebugerMachine::EnableActionHint(const QString &hint,QPixmap& img) {};
void FormDebugerMachine::DisableActionHint(){};
void FormDebugerMachine::DisableAlphabit(){}
void FormDebugerMachine::DisableCalculator() {}
void FormDebugerMachine::EnableAlphabit(IVMTAlphabitSource* ,const QString& ,QPixmap &){}
void FormDebugerMachine::EnableCalculator(std::shared_ptr<IVMTMachine> ){}
void FormDebugerMachine::EnableAnimation(bool enable){};

UICanvas& FormDebugerMachine::GetGraphics(){
    return  ui->widget_editor->GetCanvas();
}

std::weak_ptr<VMTComplexMachine> FormDebugerMachine::GetMachine(){
    return std::weak_ptr<VMTComplexMachine>(ui->widget_editor->GetComplexMachine());
}

void FormDebugerMachine::Repaint(const QRect& rect){
    ui->widget_editor->update();
}

void FormDebugerMachine::MoveInScreen(QPoint &&shift){
    ui->widget_editor->GetCanvas().MoveInScreen(shift);
}

void FormDebugerMachine::Move(QPoint &&shift){
    ui->widget_editor->GetCanvas().Move(shift);
}

void FormDebugerMachine::SetMachine(std::shared_ptr<VMTComplexMachine> machine){
    ui->widget_editor->SetComplexMachine(machine);
    ui->widget_editor->update();
}

void FormDebugerMachine::OnLoaded(){
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionTranslate(this)));
}

void FormDebugerMachine::SetAction(std::shared_ptr<IVMTAction> action){
 if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Cancel(this);
 ui->widget_editor->GetAction() = action;
 if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Enable(this);
}

FormDebugerMachine::~FormDebugerMachine()
{
    delete ui;
}

void FormDebugerMachine::Run(){
    _controller->EnableDebugStep(false);
    _controller->EnableDebugRun(false);
    _controller->EnableDebugStop(true);
    _debugger.ToStart(this);
    _stop = false;
    RunTimer();
}

void FormDebugerMachine::RunTimer(){
    if((!_debugger.IsFinish())&&(!_stop)){
        Step();
        _timer.singleShot(50,this,SLOT(RunTimer()));
    } else{
        _stop=false;
        _controller->EnableDebugStep(true);
        _controller->EnableDebugRun(true);
        _controller->EnableDebugStop(false);
        _debugger.ToStart(this);
    }
}
void FormDebugerMachine::Stop(){
    _stop = true;
}

void FormDebugerMachine::Step(){
    _debugger.Step(this);
    ui->widget_line->Repaint();
}

void FormDebugerMachine::on_button_left_page_clicked()
{
  ui->widget_line->LeftPage();
}

void FormDebugerMachine::on_button_left_clicked()
{
  ui->widget_line->Left();
}

void FormDebugerMachine::on_button_right_clicked()
{
  ui->widget_line->Right();
}

void FormDebugerMachine::on_button_right_page_clicked()
{
  ui->widget_line->RightPage();
}
