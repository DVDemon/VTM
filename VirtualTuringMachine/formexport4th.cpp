#include "formexport4th.h"
#include "ui_formexport4th.h"
#include "vmtproject.h"
#include <qfiledialog.h>
#include <qscreen.h>
#include <QClipboard>
#include "screentools.h"

FormExport4th::FormExport4th(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormExport4th),
    _export(nullptr)
{
    ui->setupUi(this);
    ui->_copy->setVisible(false);

    ScreenTools st;
    st.ResizeButtonBig(ui->_copy);
    st.ResizeButtonBig(ui->_export);
    _done = false;
    _output = "";
    _information = "";
}

void FormExport4th::BeforeChange([[maybe_unused]] std::shared_ptr<UIStateData> state_data){
}

void FormExport4th::AfterUndo([[maybe_unused]] std::shared_ptr<UIStateData> state_data) {
}

void FormExport4th::OnLoaded(){
    //ui->edit_location->setText(VMTProject::GetInstance().GetLocation());
    qDebug() << VMTProject::GetInstance().GetCurrentMachineName();

}

FormExport4th::~FormExport4th()
{
    delete _export;
    delete ui;
}

std::shared_ptr<VMTComplexMachine> FormExport4th::CreatePlainMachine(){
    std::vector<std::shared_ptr<VMTComplexMachine>> machines;

    for(auto m: VMTProject::GetInstance().GetMachines())
        machines.push_back(m->CreatePlainCopy(nullptr));

    for(auto &m: machines){
        for(auto &im: m->GetMachineCollection()){
          std::shared_ptr<VMTComplexMachine> mc = std::dynamic_pointer_cast<VMTComplexMachine>(im);
          if(mc){
              for(auto &mr:machines){
                  if(mr->GetMachineName()==mc->GetMachineName()){
                      mc->ReplaceMachine(mr.get());
                  }
              }
          }
        }
    }

    std::shared_ptr<VMTComplexMachine> result;
    for(auto &m: machines){
        if(m->GetMachineName()==VMTProject::GetInstance().GetCurrentMachine()->GetMachineName())
            result = m;
    }
    return result;
}

void FormExport4th::on__export_clicked()
{
    if(_export) return;
    ui->_export->setVisible(false);


    Clear();

    _done = false;
    _export = new VMTExport4th(CreatePlainMachine(),
                               ui->_delimiter->text(),
                               ui->_left->text(),
                               ui->_right->text(),
                               ui->_stop->text(),
                               ui->_quotes->text());
    _export->Export(this,nullptr);
    QTimer::singleShot(200, this, SLOT(OnTimer()));

}

void FormExport4th::OnTimer(){
    qDebug() << "OnTimer";
    if(!_done){
        QTimer::singleShot(200, this, SLOT(OnTimer()));
    } else {
        ui->_text->setText(_output);
        ui->information->setText(_information);
        ui->_copy->setVisible(true);
    }
}

void FormExport4th::Done(){
    _done = true;
}

void FormExport4th::Clear(){
    ui->_text->clear();
    _output = "";
    _information = "";
}
void FormExport4th::AppendText(const QString &text){
    //ui->_text->append(text);
    _output.append(text);
    _output.append("\r\n");

}

void FormExport4th::Information(const QString &text){
    //ui->information->setText(text);
    _information.append(text);
}

void FormExport4th::on__copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString text = ui->_text->toPlainText();
    clipboard->setText(text);
    QMessageBox box;
    box.setText("Text copied into clipboard");
    box.exec();
}
