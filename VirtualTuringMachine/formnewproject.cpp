#include "formnewproject.h"
#include "ui_formnewproject.h"
#include <QDebug>
#include "uistatenewproject.h"
#include "uistatenewmachine.h"
#include <set>
#include <QFileDialog>
#include "vmtproject.h"
#include "screentools.h"
#include "vmttheme.h"
#include "VMTJsonSerializer.h"
#include <QPushButton>
#include <QScreen>
#include <QApplication>
#include <QVBoxLayout>

FormNewProject::FormNewProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNewProject)
{
    qDebug() << "Form New project construcor";
    ui->setupUi(this);
    VmtTheme::applyThemedWidgets(this);
    VmtTheme::applyIconToolBar(ui->frame);
    //ui->combo_project_type->addItem("Turing machine project");
    //ui->combo_project_type->setCurrentIndex(0);


    ui->text_project_location->setText(VMTProject::GetInstance().GetLocation());
    ui->text_project_name->setText(VMTProject::GetInstance().GetName());
    ui->text_alphabit->setText(VMTProject::GetInstance().GetAlphabit()->ToString());//data->alphabit);

    if(VMTProject::GetInstance().GetExercise()){
        ui->text_alphabit->setEnabled(false);
        ui->text_project_name->setEnabled(false);
        ui->button_z2->setEnabled(false);
        ui->button_z3->setEnabled(false);
        ui->button_z4->setEnabled(false);
        ui->button_z10->setEnabled(false);
        ui->button_z16->setEnabled(false);
        ui->button_z80->setEnabled(false);
    }

    ScreenTools st;

    st.ResizeButtonBig(ui->button_z2);
    st.ResizeButtonBig(ui->button_z3);
    st.ResizeButtonBig(ui->button_z4);
    st.ResizeButtonBig(ui->button_z10);
    st.ResizeButtonBig(ui->button_z16);
    st.ResizeButtonBig(ui->button_z80);

    st.ResizeButtonBig(ui->button_create);

    auto* extras = new QVBoxLayout();
    auto* saveJson = new QPushButton(tr("Save as JSON…"), this);
    saveJson->setStyleSheet(VmtTheme::primaryButtonStyle());
    connect(saveJson, &QPushButton::clicked, this, &FormNewProject::on_save_json_clicked);
    extras->addWidget(saveJson);

    ui->verticalLayout_2->addLayout(extras);

    Check();
}

void FormNewProject::on_save_json_clicked()
{
    Save();
    QString path = VMTProject::GetInstance().GetLocation();
    path += QLatin1Char('/');
    path += VMTProject::GetInstance().GetName();
    if (!path.endsWith(QLatin1String(".json"), Qt::CaseInsensitive)) {
        path += QLatin1String(".vmt.json");
    }
    path = QFileDialog::getSaveFileName(
        this,
        tr("Save JSON project"),
        path,
        tr("JSON projects (*.vmt.json *.json)"));
    if (path.isEmpty()) return;
    VMTJsonSerializer json(path);
    if (json.serialize(&VMTProject::GetInstance())) {
        Configuration::GetInstance().AddRecentProject(path);
        Configuration::GetInstance().Save();
    }
}

void FormNewProject::Save(){
    VMTProject::GetInstance().GetName() = ui->text_project_name->text();
    VMTProject::GetInstance().GetLocation() = ui->text_project_location->text();
    VMTProject::GetInstance().GetAlphabit()->ReplaceString(ui->text_alphabit->toPlainText());
    for(auto m: VMTProject::GetInstance().GetMachines()) m->ChangeAlphabit(VMTProject::GetInstance().GetAlphabit());



}

void FormNewProject::BeforeChange(std::shared_ptr<UIStateData> state_data){


    UIStateNewProjectData *data = dynamic_cast<UIStateNewProjectData*>(state_data.get());
    if(data){
        state_data->SetName(ui->text_project_name->text());

        Save();
    }


}

void FormNewProject::AfterUndo(std::shared_ptr<UIStateData> state_data) {
    UIStateNewProjectData *data = dynamic_cast<UIStateNewProjectData*>(state_data.get());
    if(data){
        ui->text_project_name->setText(VMTProject::GetInstance().GetName());//data->name);
        ui->text_project_location->setText(VMTProject::GetInstance().GetLocation());//data->location);
        ui->text_alphabit->setText(VMTProject::GetInstance().GetAlphabit()->ToString());//data->alphabit);
        Check();
    }
}

FormNewProject::~FormNewProject(){
    delete ui;
}



void FormNewProject::on_button_z2_clicked()
{
    ui->text_alphabit->setText(" 01");
}

void FormNewProject::on_button_z3_clicked()
{
    ui->text_alphabit->setText(" 012");
}

void FormNewProject::on_button_z4_clicked()
{
    ui->text_alphabit->setText(" 01234567");
}

void FormNewProject::on_button_z10_clicked()
{
    ui->text_alphabit->setText(" 0123456789");
}

void FormNewProject::on_button_z16_clicked()
{
    ui->text_alphabit->setText(" 0123456789ABCDEF");
}

void FormNewProject::on_button_z80_clicked()
{
    ui->text_alphabit->setText(" 0123456789-+=_~abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()/\\");
}

void FormNewProject::on_button_project_location_clicked()
{
    QFileDialog dlg;
    dlg.setWindowTitle("Project place");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    //dlg.setLabelText(QFileDialog::LookIn,"OPEN");
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setViewMode(QFileDialog::List);
    dlg.resize(QApplication::primaryScreen()->availableSize());
    if(dlg.exec()==QDialog::Accepted)
    {
        //QString file = QFileDialog::getSaveFileName(this,"Export to PNG",VMTProject::GetInstance().GetLocation(),QString("png"));
        QStringList fileName=dlg.selectedFiles();

        //QString file =  QFileDialog::getExistingDirectory();
        if(fileName.length()>0){
            QString file = fileName[0];
            if(file.size()>0)
                ui->text_project_location->setText(file);
        }
    }

}



void FormNewProject::on_text_alphabit_textChanged()
{
    QString alphabit = ui->text_alphabit->toPlainText();
    std::set<QChar> alphabit_set;
    bool changed = false;
    for(QChar a: alphabit) alphabit_set.insert(a);
    for(int i = 0; i < static_cast<int>(alphabit.size());){
        if(alphabit_set.find(alphabit[i])!=alphabit_set.end()){
            alphabit_set.erase(alphabit[i]);
            i++;
        } else {
            alphabit.remove(i,1);
            changed = true;
        }
    }

    if(changed) ui->text_alphabit->setText(alphabit);
    Check();

}

void FormNewProject::Check(){
    QString alphabit = ui->text_alphabit->toPlainText();
    if(alphabit.length()<2){
        ui->button_create->setVisible(false);//setDisabled(true);
    } else ui->button_create->setVisible(true);
}

void FormNewProject::on_button_create_clicked()
{
    this->_controller->ChangeState(std::shared_ptr<UIStateNewMachineData>(new UIStateNewMachineData()));
}
