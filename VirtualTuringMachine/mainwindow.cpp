#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyleFactory>
#include "uistatemain.h"
#include "formwarning.h"
#include "clickablelabel.h"
#include "screentools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _form_warning(nullptr)

{
    ui->setupUi(this);


   // QPalette pal(palette());

    // set black background
    //pal.setColor(QPalette::Background, QColor("#012C40"));
    //pal.setColor(QPalette::ButtonText,Qt::white);
    //ui->frame_header->setAutoFillBackground(true);
    //ui->frame_header->setPalette(pal);
    //ui->widgetLabel->setPalette(pal);

    //ui->frame_header->setStyleSheet("background-color:#01547a;");

    QObject::connect(ui->button_back, SIGNAL(clicked()),
                     this, SLOT(onBackClicked()));

    QHBoxLayout *layout = new QHBoxLayout;
    ui->widget_container->setLayout(layout);
    ui->widget_container->setStyleSheet("color:#ffffff;background-color:#012C40;");

    ui->button_back->setHidden(true);

    ScreenTools st;

    st.ResizeButton(ui->button_back);
    st.ResizeButton(ui->button_cancel);
    st.ResizeButton(ui->button_configure);
    st.ResizeButton(ui->button_debug_back);
    st.ResizeButton(ui->button_debug_over);
    st.ResizeButton(ui->button_debug_pause);
    st.ResizeButton(ui->button_debug_run);
    st.ResizeButton(ui->button_debug_step);
    st.ResizeButton(ui->button_debug_stop);
    st.ResizeButton(ui->button_export);
    st.ResizeButton(ui->button_4th);
    st.ResizeButton(ui->button_new_machine);
    st.ResizeButton(ui->button_ok);
    st.ResizeButton(ui->button_open_machine);
    st.ResizeButton(ui->button_exercise);
    st.ResizeButton(ui->button_run);
    st.ResizeButton(ui->button_save);

   // st.ResizeSlider(ui->slider_zoom);
   // st.ResizeFrame(ui->frame_zoom);

   //st.ResizeLabel(ui->label_2);

    this->ChangeState(std::shared_ptr<UIStateMainData>(new UIStateMainData()));
}

void MainWindow::EnableExercise(bool enable) {
    ui->button_exercise->setHidden(!enable);
}

void MainWindow::EnableNewMachine(bool enable) {
    ui->button_new_machine->setHidden(!enable);
}

void MainWindow::EnableOpenMachine(bool enable) {
    ui->button_open_machine->setHidden(!enable);
}

void MainWindow::EnableSave(bool enable) {
    ui->button_save->setHidden(!enable);
}

void MainWindow::EnableExport(bool enable) {
    ui->button_export->setHidden(!enable);
}

void MainWindow::EnableRun(bool enable) {
    ui->button_run->setHidden(!enable);
}

void MainWindow::EnableConfigure(bool enable) {
    ui->button_configure->setHidden(!enable);
}

void MainWindow::EnableDebugBack(bool enable){
    ui->button_debug_back->setHidden(!enable);
}

void MainWindow::EnableDebugOver(bool enable){
    ui->button_debug_over->setHidden(!enable);
}

void MainWindow::EnableDebugPause(bool enable){
    ui->button_debug_pause->setHidden(!enable);
}

void MainWindow::EnableDebugRun(bool enable){
    ui->button_debug_run->setHidden(!enable);
}

void MainWindow::EnableDebugStep(bool enable){
    ui->button_debug_step->setHidden(!enable);
}

void MainWindow::EnableDebugStop(bool enable){
    ui->button_debug_stop->setHidden(!enable);
}

void MainWindow::EnableOk(bool enable){
    ui->button_ok->setHidden(!enable);
}

void MainWindow::EnableCancel(bool enable){
    ui->button_cancel->setHidden(!enable);
}

void MainWindow::EnableZoom(bool enable){
    ui->slider_zoom->setHidden(!enable);
    ui->frame_zoom->setHidden(!enable);
}

void MainWindow::Enable4thExport(bool enable){
    ui->button_4th->setHidden(!enable);
}

#include <QMessageBox>
void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "VMT",
                                                                "This will delete any unsaved project data. Are you sure?",
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::ShowWarning(std::shared_ptr<UIStateData>){

    if(!_form_warning){
        if(GetContainer()->layout()->count()>0)
        {
            //_form_warning_state = dynamic_cast<QWidget*>(GetContainer()->layout()->children().at(0));
            //_form_warning_state->setHidden(true);
            _form_warning = new FormWarning(this,GetContainer());
            GetContainer()->layout()->addWidget(_form_warning);
            GetContainer()->layout()->setContentsMargins(0,0,0,0);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(_state){
        _state->OnKeyReleased(event);
    }
}

void MainWindow::keyPressEvent (QKeyEvent* event) {
    if (event->key () == Qt::Key_Back) {
        onBackClicked();
        event->accept ();
    } else
    if(_state){
        _state->OnKeyPressed(event);
    }
}

void MainWindow::HideWarning(bool undo){
    if(_form_warning){
        GetContainer()->layout()->removeWidget(_form_warning);
        delete _form_warning;
        _form_warning = nullptr;
        //_form_warning_state->setHidden(false);
        if(undo) UndoForce();
    }
}

void MainWindow::onBackClicked(){
    this->Undo();
}

void MainWindow::onRefClicked(){
    ClickableLabel *button = dynamic_cast<ClickableLabel*> (sender());
    if(button)
    {
        QVariant var = button->property("state");
        unsigned int number = var.toUInt();
        bool need_continue = true;
        while(need_continue&(this->undo.size()>number)) need_continue=this->Undo();
    }
}

void MainWindow::OnChanged(std::shared_ptr<UIState>     state,std::shared_ptr<UIStateData> new_state_data){

    qDebug() << "MainWindow::OnChanged";

    bool find;
    do{
        find = false;
        for(auto i=ui->widgetLabel->children().begin();
            (i!=ui->widgetLabel->children().end())&&(!find);i++){
            QObject *obj = *i;

            if(dynamic_cast<QPushButton*>(obj)) {
                find = true;
                ui->widgetLabel->layout()->removeWidget(dynamic_cast<QPushButton*>(obj));
                delete obj;
                obj = nullptr;
            }
            if(dynamic_cast<QLabel*>(obj)) {
                find = true;
                ui->widgetLabel->layout()->removeWidget(dynamic_cast<QLabel*>(obj));
                delete obj;
            }
        }
    }while(find);

    unsigned int count = 0;
    for(auto i: undo){
        QString name = i->GetName();
        name+="\\";
        ClickableLabel *label = new ClickableLabel(name,ui->widgetLabel);


        QFont font = label->font();
        font.setUnderline(true);
        //font.setBold(true);
        label->setProperty("state",count++);
        label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        label->setStyleSheet("color:#ffffff;background-color:#012C40;");
        label->connect(label, SIGNAL(clicked()),
                       this, SLOT(onRefClicked()));
        label->setFont(font);
        ui->widgetLabel->layout()->addWidget(label);
    }

    QLabel *label = new QLabel(new_state_data->GetName(),ui->widgetLabel);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    label->setStyleSheet("color:#ffffff;background-color:#012C40;");
    ui->widgetLabel->layout()->addWidget(label);
    ui->button_back->setHidden(undo.empty());
    state->OnLoaded(new_state_data);
    qDebug() << "MainWindow::OnLoaded done";

}

QWidget* MainWindow::GetContainer(){
    return ui->widget_container;
}

MainWindow::~MainWindow()
{
    _state.reset();
    delete ui;
}

void MainWindow::on_button_new_machine_clicked()
{
    QVariant var = ui->button_new_machine->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_open_machine_clicked()
{
    QVariant var = ui->button_open_machine->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_save_clicked()
{
    QVariant var = ui->button_save->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_export_clicked()
{
    QVariant var = ui->button_export->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_run_clicked()
{
    QVariant var = ui->button_run->property("name");
    QString  action  = var.toString();
    _state->Action(action);

}

void MainWindow::on_button_configure_clicked()
{
    QVariant var = ui->button_configure->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_ok_clicked()
{
    QVariant var = ui->button_ok->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_cancel_clicked()
{
    QVariant var = ui->button_cancel->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_slider_zoom_sliderMoved(int position)
{
    _state->OnZoomChanged(position);
}

int  MainWindow::GetZoom(){
    return ui->slider_zoom->sliderPosition();
}


void MainWindow::on_button_debug_back_clicked()
{
    QVariant var = ui->button_debug_back->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_debug_step_clicked()
{
    QVariant var = ui->button_debug_step->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_slider_zoom_sliderPressed()
{

    _state->OnZoomChanged(ui->slider_zoom->sliderPosition());
}

void MainWindow::on_button_4th_clicked()
{
    QVariant var = ui->button_4th->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_debug_run_clicked()
{
    QVariant var = ui->button_debug_run->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_debug_pause_clicked()
{
    QVariant var = ui->button_debug_pause->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_debug_stop_clicked()
{
    QVariant var = ui->button_debug_stop->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}

void MainWindow::on_button_exercise_clicked()
{
    QVariant var = ui->button_exercise->property("name");
    QString  action  = var.toString();
    _state->Action(action);
}
