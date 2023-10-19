#include "formnewmachine.h"
#include "ui_formnewmachine.h"
#include "uistateeditormachine.h"
#include "uistatenewmachine.h"
#include "vmtproject.h"
#include <QDebug>
#include "screentools.h"

FormNewMachine::FormNewMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNewMachine)
{
    ui->setupUi(this);

    ScreenTools st;

    st.ResizeButtonBig(ui->button_ok_2);
    st.ResizeButtonBig(ui->button_delete_2);

    FillList();
    Check();

    ui->list_machines->setStyleSheet("background-color:gray;");
    ui->text_machine_name->setStyleSheet("background-color:gray;");
    ui->label->setStyleSheet("color:darkgray;");
    ui->label_2->setStyleSheet("color:darkgray;");
}
void FormNewMachine::FillList(){
    QStringList list;

    for(auto &m: VMTProject::GetInstance().GetMachines()){
        list << m->GetName();
    }
    _list_model.setStringList(list);

    ui->list_machines->setModel(&_list_model);
}

FormNewMachine::~FormNewMachine()
{
    delete ui;
}

void FormNewMachine::BeforeChange(std::shared_ptr<UIStateData> state_data){


    UIStateNewMachineData *data = dynamic_cast<UIStateNewMachineData*>(state_data.get());
    if(data){
//        qDebug() << "Selected machine:" << ui->text_machine_name->text();
        VMTProject::GetInstance().GetCurrentMachineName() = ui->text_machine_name->text();

        if(!VMTProject::GetInstance().GetMachine(VMTProject::GetInstance().GetCurrentMachineName()))
        {
         VMTProject::GetInstance().GetMachines().push_back(
         std::shared_ptr<VMTComplexMachine>(
                        new VMTComplexMachine(VMTProject::GetInstance().GetCurrentMachineName(),
                                              VMTProject::GetInstance().GetAlphabit())));
 //        qDebug() << "Machine added:" << VMTProject::GetInstance().GetCurrentMachineName();
        } //else
 //           qDebug() << "Machine selected:" << VMTProject::GetInstance().GetCurrentMachineName();

    }


}

void FormNewMachine::AfterUndo(std::shared_ptr<UIStateData> state_data) {
    UIStateNewMachineData *data = dynamic_cast<UIStateNewMachineData*>(state_data.get());
    if(data){
      ui->text_machine_name->setText(VMTProject::GetInstance().GetCurrentMachineName());
      Check();
    }
}

void FormNewMachine::OnLoaded(){
    if(VMTProject::GetInstance().GetMachines().size()==0){
        VMTProject::GetInstance().GetCurrentMachineName() ="";
        ui->text_machine_name->setText("Main machine");
    } else
    if(VMTProject::GetInstance().GetCurrentMachineName().length()==0){
        if(VMTProject::GetInstance().GetMachines().size()>0){
           VMTProject::GetInstance().GetCurrentMachineName() = VMTProject::GetInstance().GetMachines()[0]->GetName();
            ui->text_machine_name->setText(VMTProject::GetInstance().GetCurrentMachineName());
        } else {
            VMTProject::GetInstance().GetCurrentMachineName() ="";

        }
    } else {
        ui->text_machine_name->setText(VMTProject::GetInstance().GetCurrentMachineName());
    }
    Check();

}

void FormNewMachine::on_button_ok_clicked()
{
    this->_controller->ChangeState(std::shared_ptr<UIStateEditorMachineData>(new UIStateEditorMachineData()));
}

void FormNewMachine::Check(){
    QString name = ui->text_machine_name->text();
    if(name.length()<1){
        //ui->button_ok->setText(QString("Create or select   "));
        ui->button_delete_2->setVisible(false);
        ui->button_ok_2->setVisible(false);
    } else {
        if(!VMTProject::GetInstance().GetMachine(name)){
             ui->button_ok_2->setVisible(true);
             ui->button_delete_2->setVisible(false);
            // ui->button_ok->setText(QString("Create   "));
        }
        else {
            ui->button_ok_2->setVisible(true);
            ui->button_delete_2->setVisible(true);
           // ui->button_ok->setText(QString("Select   "));
        }
    }
}

void FormNewMachine::on_text_machine_name_textChanged(const QString &)
{
    Check();
}

void FormNewMachine::on_list_machines_clicked(const QModelIndex &index)
{
    ui->text_machine_name->setText(index.data(Qt::DisplayRole ).toString());
    Check();
}

void FormNewMachine::on_button_delete_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "VMT",
                                                                "This operation will delete selected complex machine. Are you sure?",
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {

    } else {
        QString name(ui->text_machine_name->text());
        VMTProject::GetInstance().GetUndoManager()->Clear();
        auto machine= VMTProject::GetInstance().GetMachine(name);

        if(machine){
            qDebug() << "Erace referencr to machine";
            for(auto cm:VMTProject::GetInstance().GetMachines()){
                qDebug() << "Inside:" << cm->GetName();
                cm->RemoveComplexMachine(machine);
            }
            qDebug() << "Erace machine from list";
            for(auto iterator = VMTProject::GetInstance().GetMachines().begin();iterator!=VMTProject::GetInstance().GetMachines().end();iterator++){
                if(machine==*iterator)
                {
                    VMTProject::GetInstance().GetMachines().erase(iterator);
                    ui->text_machine_name->setText(QString(""));
                    machine.reset();
                    FillList();
                    Check();
                    qDebug() << "Erased";
                    return;
                }
            }
        }
    }

}

#include <thread>

void FormNewMachine::on_text_machine_name_returnPressed()
{

}

void FormNewMachine::on_button_ok_2_clicked()
{
 on_button_ok_clicked();
}

void FormNewMachine::on_button_delete_2_clicked()
{
  on_button_delete_clicked();
}
