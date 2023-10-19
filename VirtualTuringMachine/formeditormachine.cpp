#include "formeditormachine.h"
#include "ui_formeditormachine.h"
#include "uistateeditormachine.h"
#include "vmtproject.h"

#include "vmtactions/vmtactiontranslate.h"
#include "vmtactions/vmtactiondelete.h"
#include "vmtactions/vmtactionpointer.h"
#include "vmtactions/vmtactionmachine.h"
#include "vmtactions/vmtactionlink.h"
#include "vmtactions/vmtactioncreateandconnect.h"
#include "vmtmachines/VMTMachineStub.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "formactionhint.h"
#include "formalphabit.h"
#include <QFileDialog>
#include <QImage>
#include <QImageWriter>
#include <QLabel>
#include <chrono>
#include "screentools.h"
#include <QScreen>

FormEditorMachine::FormEditorMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEditorMachine),
    _form_alphabit(nullptr),
    _form_calculator(nullptr),
    _form_complex_machines(nullptr),
    _form_action_hint(nullptr)
{
    qDebug() << "FormEditorMachine::Constructor";
    ui->setupUi(this);


    QString machine_name = VMTProject::GetInstance().GetCurrentMachineName();

    ui->widget_editor->SetEnvironment(this);
   //*




    ui->widget_editor->SetComplexMachine(VMTProject::GetInstance().GetMachine(machine_name));


    /*/
    std::shared_ptr<IVMTMachine> left_word = VMTMachineStub::CreateMachineByID(IVMTMachine::MachineType::MT_COPY,VMTProject::GetInstance().GetMachine(machine_name));
    std::shared_ptr<IVMTMachine> complex_stub = left_word->CreateComplexMachine(this);
    std::shared_ptr<VMTComplexMachine> complex_machine= VMTProject::GetInstance().GetMachine(machine_name);
    complex_machine->SetInnerObject(std::dynamic_pointer_cast<VMTComplexMachine>(complex_stub)->GetInnerObject(),complex_machine);
    ui->widget_editor->SetComplexMachine(complex_machine);

    //*/
    ui->widget_editor->GetComplexMachine()->DeselectAll();

    _tools_group = std::shared_ptr<QButtonGroup>(new QButtonGroup(ui->tool_frame));
    _tools_group->addButton(ui->tool_link);
    _tools_group->addButton(ui->tool_machine_copy);
    _tools_group->addButton(ui->tool_machine_finish);
    _tools_group->addButton(ui->tool_machine_lambda);
    _tools_group->addButton(ui->tool_machine_left);
    _tools_group->addButton(ui->tool_machine_left_word);
    _tools_group->addButton(ui->tool_machine_right);
    _tools_group->addButton(ui->tool_machine_right_word);
    _tools_group->addButton(ui->tool_machine_start);
    _tools_group->addButton(ui->tool_pointer);
    _tools_group->addButton(ui->tool_translate);
    _tools_group->addButton(ui->tool_trash);
    _tools_group->addButton(ui->tool_complex);
    _tools_group->setExclusive(true);

    QString style = "QToolButton:checked{ background-color: rgb(80, 80, 80); border: none; }";
    ui->tool_link->setStyleSheet(style);
    ui->tool_machine_copy->setStyleSheet(style);
    ui->tool_machine_finish->setStyleSheet(style);
    ui->tool_machine_lambda->setStyleSheet(style);
    ui->tool_machine_left->setStyleSheet(style);
    ui->tool_machine_left_word->setStyleSheet(style);
    ui->tool_machine_right->setStyleSheet(style);
    ui->tool_machine_right_word->setStyleSheet(style);
    ui->tool_machine_start->setStyleSheet(style);
    ui->tool_pointer->setStyleSheet(style);
    ui->tool_translate->setStyleSheet(style);
    ui->tool_trash->setStyleSheet(style);
    ui->tool_complex->setStyleSheet(style);


    ui->_navi->setCheckable(true);
    ui->_navi->setChecked(false);

    ScreenTools st;

    st.ResizeButton(ui->tool_link);
    st.ResizeButton(ui->tool_machine_copy);
    st.ResizeButton(ui->tool_machine_finish);
    st.ResizeButton(ui->tool_machine_lambda);
    st.ResizeButton(ui->tool_machine_left);
    st.ResizeButton(ui->tool_machine_left_word);
    st.ResizeButton(ui->tool_machine_right);
    st.ResizeButton(ui->tool_machine_right_word);
    st.ResizeButton(ui->tool_machine_start);
    st.ResizeButton(ui->tool_pointer);
    st.ResizeButton(ui->tool_translate);
    st.ResizeButton(ui->tool_trash);
    st.ResizeButton(ui->tool_complex);
    st.ResizeButton(ui->_navi);

    ui->properties_frame->setHidden(true);
    st.ResizePropertyPanel(ui->properties_frame);


    //ui->properies_splitter->setStretchFactor(0,3);
    //ui->properies_splitter->setStretchFactor(1,1);

}


void FormEditorMachine::Run(){
    VMTProject::GetInstance().SetTestMode(false);
    _controller->ChangeState(
                std::shared_ptr<UIStateCompilerMachineData>(new UIStateCompilerMachineData()));
}
void FormEditorMachine::Exercise(){
    VMTProject::GetInstance().SetTestMode(true);
    _controller->ChangeState(
                std::shared_ptr<UIStateCompilerMachineData>(new UIStateCompilerMachineData()));
}

void FormEditorMachine::Undo(){
    if(auto ptr = GetMachine().lock()){
        if(VMTProject::GetInstance().GetUndoManager()->Recall(this))
        {
            QString machine_name = VMTProject::GetInstance().GetCurrentMachineName();
            ui->widget_editor->SetComplexMachine(VMTProject::GetInstance().GetMachine(machine_name));
            if(ui->widget_editor->GetAction()){
                ui->widget_editor->GetAction()->Cancel(this);
                ui->widget_editor->GetAction().reset();
                if( _tools_group->checkedButton()){
                    _tools_group->setExclusive(false);
                    _tools_group->checkedButton()->setChecked(false);
                    _tools_group->setExclusive(true);
                }
            }
            this->Repaint(QRect());
        }
    }

}

void FormEditorMachine::OnKeyPressed(QKeyEvent* event){
 if(event->key() == Qt::Key_Escape){
   this->SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(this,ui->_navi->isChecked())));
 } else if(event->key() == Qt::Key_Space){
     if(!_action_space){
      _action_space = ui->widget_editor->GetAction();
      SetAction(std::shared_ptr<IVMTAction>(new VMTActionTranslate(this)));
     }
 }
}

void FormEditorMachine::OnKeyReleased(QKeyEvent* event){
    if(event->key() == Qt::Key_Space){
         if(_action_space){
          SetAction(std::shared_ptr<IVMTAction>(_action_space));
          _action_space = nullptr;
         }
     }
}

void FormEditorMachine::ExportMachine(){
    QFileDialog dlg;
    dlg.setWindowTitle("Export Turing Machine Image");
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    //dlg.setLabelText(QFileDialog::LookIn,"OPEN");
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setViewMode(QFileDialog::List);
    QStringList filters;
    filters <<"PNG (*.png)";
    dlg.setOption(QFileDialog::HideNameFilterDetails,false);
    dlg.setNameFilters(filters);
    dlg.resize(QApplication::primaryScreen()->availableSize());
    if(dlg.exec()==QDialog::Accepted)
    {
        //QString file = QFileDialog::getSaveFileName(this,"Export to PNG",VMTProject::GetInstance().GetLocation(),QString("png"));
        QStringList fileName=dlg.selectedFiles();
        if(fileName.length()>0)
        {
            QString file = fileName[0];
            if(file.length()>0){
                if(!ui->widget_editor->GetComplexMachine()) qDebug() << "WTF!";
                QRect rect = ui->widget_editor->GetComplexMachine()->GetBoundsWithChilds();


                rect.setLeft(rect.left()-20);
                rect.setTop(rect.top()-20);
                rect.setWidth(rect.width()+40);
                rect.setHeight(rect.height()+40);

                QImage image(QSize(rect.width(),rect.height()), QImage::Format_ARGB32);
                QPainter painter;
                painter.begin(&image); // Here is the error. See below
                   ScreenTools st;
                UICanvas canvas(QBrush(QColor(255,255,255)),//QWidget::palette().background(),
                                QPen(QColor(192, 192, 192), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin),
                                QPen(QColor(0, 0, 0)),st.GetCellSize(),st.GetFontSize());

                canvas.SetPainter(&painter);

                qDebug() << rect;
                QPoint p = rect.topLeft();
                p.setX(-p.x());
                p.setY(-p.y());
                canvas.MoveInScreen(p);


                //QRect area =   canvas.ToScreen(ui->widget_editor->GetComplexMachine()->GetBoundsWithChilds());
                //rect.setWidth(rect.width()-rect.left());
                //rect.setHeight(rect.height()-rect.top());

                rect.moveLeft(0);
                rect.moveTop(0);
                qDebug() << rect;
                ui->widget_editor->Paint(canvas,rect);
                painter.end();
                if(file.indexOf(".png")!=file.length()-4) file.append(".png");
                QImageWriter imageWriter(file,"png");


                if(!imageWriter.write( image ))
                {
                    QString msg;
                    msg.append("Error create file: " );
                    msg.append(imageWriter.errorString());
                    QMessageBox::question( nullptr,"VMT",msg,QMessageBox::Ok,QMessageBox::Ok);
                }

            }
        }
    }
}

void FormEditorMachine::BeforeChange(std::shared_ptr<UIStateData> state_data){
    UIStateEditorMachineData *data = dynamic_cast<UIStateEditorMachineData*>(state_data.get());
    if(data){
        //data->_machine = ui->widget_editor->GetComplexMachine();
    }
}

void FormEditorMachine::AfterUndo(std::shared_ptr<UIStateData> state_data) {
    UIStateEditorMachineData *data = dynamic_cast<UIStateEditorMachineData*>(state_data.get());
    if(data){
        //ui->widget_editor->GetComplexMachine() = data->_machine;
    }
}

FormEditorMachine::~FormEditorMachine()
{
    delete ui;
}

void FormEditorMachine::ChangeZoom(size_t zoom){
    ui->widget_editor->SetZoom(zoom);
}

void FormEditorMachine::keyPressEvent (QKeyEvent* event) {

if (event->key () == Qt::Key_Escape) {
         SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(this,ui->_navi->isChecked())));
         ui->tool_pointer->setChecked(true);
        }


}

// IVMTEnvironment

void FormEditorMachine::DisableAlphabit(){
    if(_form_alphabit)
    {
        ui->properties->layout()->removeWidget(_form_alphabit);
        delete _form_alphabit;
        _form_alphabit = nullptr;
    }
    ui->properties_frame->setHidden(true);
}

void FormEditorMachine::DisableCalculator(){
    if(_form_calculator)
    {
        ui->properties->layout()->removeWidget(_form_calculator);
        delete _form_calculator;
        _form_calculator = nullptr;
    }
    ui->properties_frame->setHidden(true);
}


void FormEditorMachine::EnableAlphabit(IVMTAlphabitSource* alphabit,const QString& msg,QPixmap &img){
    ScreenTools st;
    if(_form_alphabit) DisableAlphabit();
    if(!ui->properties->layout()) ui->properties->setLayout(new QHBoxLayout());
    ui->properties->layout()->setContentsMargins(0,0,0,0);
    ui->properties->layout()->addWidget(_form_alphabit = new FormAlphabit(alphabit,this,ui->properties));
    ui->label_title->setText(msg);
    ui->label_image->setPixmap(img.scaled(st.GetImageSize(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_image->setText("");
    ui->properties_frame->setHidden(false);
}

void FormEditorMachine::EnableCalculator(std::shared_ptr<IVMTMachine> machine){
    ScreenTools st;
    if(_form_calculator) DisableCalculator();
    if(!ui->properties->layout()) ui->properties->setLayout(new QHBoxLayout());
    ui->properties->layout()->setContentsMargins(0,0,0,0);
    ui->properties->layout()->addWidget(_form_calculator = new FormCalculator(machine,this,ui->properties));
    //ui->label_image->setPixmap(*GetGraphics().GetImage(machine->GetImageType()));
    ui->label_image->setPixmap(GetGraphics().GetImage(machine->GetImageType())->scaled(st.GetImageSize(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->label_title->setText("Select machine power");
    ui->properties_frame->setHidden(false);
}



UICanvas& FormEditorMachine::GetGraphics(){
    return  ui->widget_editor->GetCanvas();
}



std::weak_ptr<VMTComplexMachine> FormEditorMachine::GetMachine(){
    return std::weak_ptr<VMTComplexMachine>(ui->widget_editor->GetComplexMachine());
}

void FormEditorMachine::SetMachine(std::shared_ptr<VMTComplexMachine> machine){
    ui->widget_editor->SetComplexMachine(machine);
}

void FormEditorMachine::Repaint(const QRect& rect){
    ui->widget_editor->Repaint(rect);
}


void FormEditorMachine::MoveInScreen(QPoint &&shift){
    ui->widget_editor->GetCanvas().MoveInScreen(shift);
}

void FormEditorMachine::Move(QPoint &&shift){
    ui->widget_editor->GetCanvas().Move(shift);
}


void FormEditorMachine::SetAction(std::shared_ptr<IVMTAction> action){
    DisableFormComplexMachines();
    if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Cancel(this);
    ui->widget_editor->GetAction() = action;

    if(action){
        if(dynamic_cast<VMTActionLink*>(action.get())){
            ui->tool_link->setChecked(true);
        } else
            if(dynamic_cast<VMTActionPointer*>(action.get())){
                ui->tool_pointer->setChecked(true);
            } else
                if(dynamic_cast<VMTActionCreateAndConnect*>(action.get())){
                    ui->tool_pointer->setChecked(true);
                }


    }

    if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Enable(this);
}

void FormEditorMachine::on_tool_pointer_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(this,ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_link_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionLink(this)));
}


void FormEditorMachine::on_tool_translate_clicked()
{

    static std::chrono::high_resolution_clock::time_point  last = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point ct = std::chrono::high_resolution_clock::now();

    if(std::chrono::duration_cast<std::chrono::milliseconds>(ct-last).count()>0)
        if(std::chrono::duration_cast<std::chrono::milliseconds>(ct-last).count()<1000)
        {
            ui->widget_editor->Center();
        }

    last = ct;

    SetAction(std::shared_ptr<IVMTAction>(new VMTActionTranslate(this)));
}

void FormEditorMachine::on_tool_trash_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionDelete(this)));
}

void FormEditorMachine::on_tool_machine_start_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_START,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_lambda_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_WRITE,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_finish_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_FINISH,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_left_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_LEFT,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_right_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_RIGHT,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_left_word_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_LEFT_WORD,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_right_word_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_RIGHT_WORD,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::on_tool_machine_copy_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionMachine(this,
                                                               VMTMachineStub::CreateMachineByID(
                                                                   IVMTMachine::MachineType::MT_COPY,
                                                                   this->GetMachine()),ui->_navi->isChecked())));
}

void FormEditorMachine::EnableActionHint(const QString &hint,QPixmap& img){
    ScreenTools st;
    ui->label_title->setText(hint);
    ui->label_image->setPixmap(img.scaled(st.GetImageSize(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_image->setText("");
    ui->properties_frame->setHidden(false);
}

void FormEditorMachine::DisableActionHint(){
    ui->properties_frame->setHidden(true);
}

void FormEditorMachine::EnableFormComplexMachines(){
    ScreenTools st;
    static QPixmap img(":/Files/images/machine_complex.gif");
    if(_form_complex_machines) DisableFormComplexMachines();
    if(!ui->properties->layout()) ui->properties->setLayout(new QHBoxLayout());
    ui->properties->layout()->addWidget(_form_complex_machines = new FormComplexMachines(this,ui->properties,ui->_navi->isChecked()));

    ui->label_title->setText("Please select sub-machine name and click to diagram");
    ui->label_image->setPixmap(img.scaled(st.GetImageSize(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_image->setText("");

    ui->properties_frame->setHidden(false);
}

void FormEditorMachine::DisableFormComplexMachines(){
    if(_form_complex_machines)
    {
        ui->properties->layout()->removeWidget(_form_complex_machines);
        delete _form_complex_machines;
        _form_complex_machines = nullptr;
    }
    ui->properties_frame->setHidden(true);
}

void FormEditorMachine::EnableAnimation(bool enable){
    ui->widget_editor->EnableAnimation(enable);
}

void FormEditorMachine::on_tool_complex_clicked()
{
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionTranslate(this)));
    EnableFormComplexMachines();
}

void FormEditorMachine::on__navi_clicked()
{
  SetAction(std::shared_ptr<IVMTAction>(new VMTActionPointer(this,ui->_navi->isChecked())));
}
