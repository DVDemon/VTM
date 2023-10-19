#include "formcompilermachine.h"
#include "ui_formcompilermachine.h"
#include "vmtproject.h"
#include "vmtactions/vmtactiontranslate.h"

FormCompilerMachine::FormCompilerMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCompilerMachine)
{
    ui->setupUi(this);
    ui->widget_editor->SetEnvironment(this);
}

void FormCompilerMachine::BeforeChange(std::shared_ptr<UIStateData>){
    qDebug() << "Clear errors";
    ClearErrors();
}

void FormCompilerMachine::ClearErrors(){
    for(auto cm: VMTProject::GetInstance().GetMachines()){
        for(std::shared_ptr<IVMTMachine> m: cm->GetMachineCollection()){
            m->SetError(false);
        }
        for(const std::weak_ptr<IVMTTransition> &t: cm->GetOutgoingTransitions()){
            if(auto t_ptr = t.lock()){
                t_ptr->SetError(false);
            }
        }
    }
}

void FormCompilerMachine::AfterUndo(std::shared_ptr<UIStateData>) {

}

void FormCompilerMachine::ChangeZoom(int zoom){
    ui->widget_editor->SetZoom(zoom);
}

void FormCompilerMachine::EnableActionHint([[maybe_unused]] const QString &hint,[[maybe_unused]] QPixmap& img) {};
void FormCompilerMachine::DisableActionHint(){};
void FormCompilerMachine::DisableAlphabit(){}
void FormCompilerMachine::DisableCalculator() {}
void FormCompilerMachine::EnableAlphabit([[maybe_unused]] IVMTAlphabitSource*,const QString& ,[[maybe_unused]] QPixmap & ){}
void FormCompilerMachine::EnableCalculator([[maybe_unused]] std::shared_ptr<IVMTMachine> ){}
void FormCompilerMachine::EnableAnimation([[maybe_unused]] bool enable){};

UICanvas& FormCompilerMachine::GetGraphics(){
    return  ui->widget_editor->GetCanvas();
}

std::weak_ptr<VMTComplexMachine> FormCompilerMachine::GetMachine(){
    return std::weak_ptr<VMTComplexMachine>(ui->widget_editor->GetComplexMachine());
}

void FormCompilerMachine::Repaint([[maybe_unused]] const QRect& rect){
    ui->widget_editor->update();
}

void FormCompilerMachine::MoveInScreen(QPoint &&shift){
    ui->widget_editor->GetCanvas().MoveInScreen(shift);
}

void FormCompilerMachine::Move(QPoint &&shift){
    ui->widget_editor->GetCanvas().Move(shift);
}

void FormCompilerMachine::SetMachine(std::shared_ptr<VMTComplexMachine> machine){
    ui->widget_editor->SetComplexMachine(machine);
}
void FormCompilerMachine::OnLoaded(){
    Check();
    SetAction(std::shared_ptr<IVMTAction>(new VMTActionTranslate(this)));
}

void FormCompilerMachine::SetAction(std::shared_ptr<IVMTAction> action){
 if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Cancel(this);
 ui->widget_editor->GetAction() = action;
 if(ui->widget_editor->GetAction()) ui->widget_editor->GetAction()->Enable(this);
}

bool FormCompilerMachine::HasErrors(){
    return !_errors.empty();
}

void FormCompilerMachine::Check(){
    std::vector<std::shared_ptr<VMTComplexMachine>>   to_go;

    for(auto m: VMTProject::GetInstance().GetMachines())
        to_go.push_back(m);

    while(to_go.size()>0){
        std::shared_ptr<VMTComplexMachine> complex_machine = to_go.at(0);

        CheckStart(complex_machine);
        CheckFinish(complex_machine);
        CheckConnection(complex_machine);
        CheckAllCases(complex_machine);
        CheckDublicate(complex_machine);
        to_go.erase(to_go.begin());
    }

    for(VMTError* err: _errors){
        ui->list_errors->addItem(err->GetMessage());
    }

    if(_errors.size()>0) {
        ui->list_errors->item(0)->setSelected(true);
        on_list_errors_currentRowChanged(0);
    }
}


void FormCompilerMachine::CheckStart(std::shared_ptr<VMTComplexMachine> complex_machine){
    int count=0;

    for(std::shared_ptr<IVMTMachine> m: complex_machine->GetMachineCollection()){
        if(m->GetID()==IVMTMachine::MachineType::MT_START){
            count ++;
        }
    }

    if(count==0) _errors.push_back(new VMTError(complex_machine,QString("No start machine found in %1").arg(complex_machine->GetName())));
    if(count>1) _errors.push_back(new VMTError(complex_machine,QString("Oney one start machine should be in %1, now=%2").arg(complex_machine->GetName(),QString::number(count))));
}

void FormCompilerMachine::CheckFinish(std::shared_ptr<VMTComplexMachine> complex_machine){
    int count=0;

    for(std::shared_ptr<IVMTMachine> m: complex_machine->GetMachineCollection()){
        if(m->GetID()==IVMTMachine::MachineType::MT_FINISH){
            count ++;
        }
    }

    if(count==0) this->_errors.push_back(new VMTError(complex_machine,QString("No finish machine found in %1").arg(complex_machine->GetName())));
}

void FormCompilerMachine::CheckConnection(std::shared_ptr<VMTComplexMachine> complex_machine){
    for(std::shared_ptr<IVMTMachine> m: complex_machine->GetMachineCollection()){
        if(m->GetID()==IVMTMachine::MachineType::MT_START){
            if(m->GetIncomingTransitions().size()>0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("No incoming transition allowed for start machine in %1").arg(complex_machine->GetName()),m));
        } else {
            if(m->GetIncomingTransitions().size()==0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("No incoming transition found for %1 machine in %2").arg(m->GetName(),complex_machine->GetName()),m));

        }

        if(m->GetID()==IVMTMachine::MachineType::MT_FINISH){
            if(m->GetOutgoingTransitions().size()>0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("No outgoing transition allowed for finish machine in %1").arg(complex_machine->GetName()),m));
        } else {
            if(m->GetOutgoingTransitions().size()==0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("No outgoing transition found for %1 machine in %2").arg(m->GetName(),complex_machine->GetName()),m));

        }


    }
}

void FormCompilerMachine::CheckAllCases(std::shared_ptr<VMTComplexMachine> complex_machine){
    for(std::shared_ptr<IVMTMachine> m: complex_machine->GetMachineCollection()){
        if(m->GetID()!=IVMTMachine::MachineType::MT_FINISH){
            QString all_str=complex_machine->GetAlphabit()->ToString();

            for(std::weak_ptr<IVMTTransition> t: m->GetOutgoingTransitions()){
                if(auto t_ptr = t.lock()){
                    QString tr_str = t_ptr->GetConditions().GetAsString();
                    for(QChar c:tr_str){
                        for(int i=0;i<all_str.length();){
                            if(all_str[i]==c) all_str.remove(i,1);
                            else i++;
                        }
                    }
                }
            }

            if(all_str.length()>0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("No outgoing transition from %1 machine in %2 for signs '%3'").arg(m->GetName(),complex_machine->GetName(),all_str),m));
        }
    }


}

void FormCompilerMachine::CheckDublicate(std::shared_ptr<VMTComplexMachine> complex_machine){
    for(std::shared_ptr<IVMTMachine> m: complex_machine->GetMachineCollection()){

        if(m->GetID()!=IVMTMachine::MachineType::MT_FINISH){
            QString all_str;
            QString dublicate;
            for(std::weak_ptr<IVMTTransition> t: m->GetOutgoingTransitions()){
                if(auto t_ptr = t.lock()){
                    QString tr_str = t_ptr->GetConditions().GetAsString();
                    for(QChar c:tr_str){
                        if(all_str.indexOf(c)>=0){
                            dublicate.append(c);
                        } else all_str.append(c);
                    }
                }
            }

            if(dublicate.length()>0)
                _errors.push_back(new VMTErrorMachine(complex_machine,QString("More then one outgoing transition from %1 machine in %2 for signs '%3'").arg(m->GetName(),complex_machine->GetName(),dublicate),m));

        }
    }
}

FormCompilerMachine::~FormCompilerMachine()
{
    //ClearErrors();
    delete ui;
}



void FormCompilerMachine::on_list_errors_currentRowChanged(int currentRow)
{
    qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged";
    ClearErrors();

    if(currentRow>=0){
        qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged::Select";

        VMTError* err= _errors[currentRow];
        ui->widget_editor->SetComplexMachine(err->GetMachine());
        qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged::Deselect";

        if(! ui->widget_editor->GetComplexMachine()) qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged::No complex machine";
        ui->widget_editor->GetComplexMachine()->DeselectAll();

        qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged::Highlight";

        if(err->GetErrorMachine()){
            err->GetErrorMachine()->SetError(true);
            QPoint point = err->GetErrorMachine()->GetCenter();

            ui->widget_editor->GetCanvas().SetCenter(point,ui->widget_editor->size());
        } else
        if(err->GetErrorTransition()){
            err->GetErrorTransition()->SetError(true);
            QPoint point = err->GetErrorTransition()->GetBounds().center();

            ui->widget_editor->GetCanvas().SetCenter(point,ui->widget_editor->size());
        }
        qDebug() << "FormCompilerMachine::on_list_errors_currentRowChanged::done";
        ui->widget_editor->update();
    }
}
