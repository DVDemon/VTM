#include "formdebugermachine.h"
#include "ui_formdebugermachine.h"
#include <set>
#include "vmtproject.h"
#include "vmtactions/vmtactiontranslate.h"
#include "screentools.h"
#include "configuration.h"


FormDebugerMachine::FormDebugerMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDebugerMachine),
    _debugger(VMTProject::GetInstance().GetAlphabit(),VMTProject::GetInstance().GetCurrentMachine()),
    _current_test(0),_model(nullptr)
{
    ui->setupUi(this);
    ui->widget_line->SetLine(_debugger.GetLine());
    ui->widget_editor->SetEnvironment(this);
    ui->widget_editor->SetComplexMachine( _debugger.GetComplexMachine());
    ui->widget_editor->GetComplexMachine()->DeselectAll();

    _stop = false;

    if(VMTProject::GetInstance().IsTestMode()){
        ui->widget_line->SetTestMode();
        _model = createModel();
        ui->_test_list->setModel(_model);
    }else {
        ui->_tests_frame->setVisible(false);
    }

    ScreenTools st;

    st.ResizeButton(ui->button_left);
    st.ResizeButton(ui->button_left_page);
    st.ResizeButton(ui->button_right);
    st.ResizeButton(ui->button_right_page);

}

QStandardItemModel *FormDebugerMachine::createModel()
{
    QStandardItemModel *model = new QStandardItemModel();
    QList<QStandardItem *> listItem;

    if(VMTProject::GetInstance().GetExercise()){
        int i=1;
        for(auto &test : VMTProject::GetInstance().GetExercise()->GetTests()){
            QStandardItem *item = new QStandardItem();
            QString text("Test ");
            text+= QString::number(i++);
            item->setText(text);
            listItem << item;
        }
        model->appendColumn(listItem);
    }
    return model;
}

void FormDebugerMachine::BeforeChange(std::shared_ptr<UIStateData>){

}

void FormDebugerMachine::AfterUndo(std::shared_ptr<UIStateData>) {

}

void FormDebugerMachine::ChangeZoom(int zoom){
    ui->widget_editor->SetZoom(zoom);
}

void FormDebugerMachine::EnableActionHint([[maybe_unused]] const QString &hint,[[maybe_unused]] QPixmap& img) {};
void FormDebugerMachine::DisableActionHint(){};
void FormDebugerMachine::DisableAlphabit(){}
void FormDebugerMachine::DisableCalculator() {}
void FormDebugerMachine::EnableAlphabit([[maybe_unused]] IVMTAlphabitSource* ,[[maybe_unused]] const QString& ,QPixmap &){}
void FormDebugerMachine::EnableCalculator([[maybe_unused]] std::shared_ptr<IVMTMachine> ){}
void FormDebugerMachine::EnableAnimation([[maybe_unused]] bool enable){};

UICanvas& FormDebugerMachine::GetGraphics(){
    return  ui->widget_editor->GetCanvas();
}

std::weak_ptr<VMTComplexMachine> FormDebugerMachine::GetMachine(){
    return std::weak_ptr<VMTComplexMachine>(ui->widget_editor->GetComplexMachine());
}

void FormDebugerMachine::Repaint([[maybe_unused]] const QRect& rect){
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

void FormDebugerMachine::StartExercise(){
    _debugger.ToStart(this);
    _stop = false;
    _current_test = 0;
    PrepareTest();

}

void FormDebugerMachine::PrepareTest(){
    if(VMTProject::GetInstance().GetExercise()){
        std::shared_ptr<Exercise> ex = VMTProject::GetInstance().GetExercise();
        if(_current_test<static_cast<int>(ex->GetTests().size())){
            std::shared_ptr<Exercise> ex = VMTProject::GetInstance().GetExercise();
            auto test = ex->GetTests().at(_current_test);

            for(QChar &ch: test.first){
                _debugger.GetLine()->SetSignAtMachine(ch.toLatin1());
                _debugger.GetLine()->ShiftRight();
            }
            RunTimer();
        } else {
            ex->SetCompleted(true);
            Configuration::GetInstance().Save();
            QMessageBox box;
            box.setText("All tests passed");
            box.exec();
        }
    }
}

void FormDebugerMachine::CheckTest(){
    if(VMTProject::GetInstance().GetExercise()){
        std::shared_ptr<Exercise> ex = VMTProject::GetInstance().GetExercise();
        if(_current_test<static_cast<int>(ex->GetTests().size())){
            std::shared_ptr<Exercise> ex = VMTProject::GetInstance().GetExercise();
            auto test = ex->GetTests().at(_current_test);

            int length = test.second.length();
            for(int i=0;i<length;i++) _debugger.GetLine()->ShiftLeft();

            bool ok=true;
            for(QChar &ch: test.second){
                if(_debugger.GetLine()->GetValueAtMachine()!=ch.toLatin1()) ok=false;
                _debugger.GetLine()->ShiftRight();
            }
            if(ok) {
                _model->item(_current_test)->setIcon(QIcon(QPixmap(":/Files/images/ok_black.png")));
                _debugger.GetLine()->Clear();
                _current_test++;
                _debugger.ToStart(this);
                PrepareTest();
            } else  {
                _model->item(_current_test)->setIcon(QIcon(QPixmap(":/Files/images/cancel.png")));

                QMessageBox box;
                box.setText("Test failed");
                box.exec();
            }
        }
    }
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
        //_debugger.ToStart(this);

        if(VMTProject::GetInstance().IsTestMode()){
            CheckTest();
        }
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
