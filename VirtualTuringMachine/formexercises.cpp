#include "formexercises.h"
#include "ui_formexercises.h"
#include "uistateexercises.h"
#include <QStandardItem>
#include "screentools.h"
#include "configuration.h"
#include "uistatenewproject.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QScreen>

FormExercises::FormExercises(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormExercises)
{
    ui->setupUi(this);
    ScreenTools st;

    st.ResizeButtonBig(ui->_ok);
    st.ResizeButtonBig(ui->_open);

    ui->_list->setModel(createModel());
    ui->_list->setIconSize(st.GetBigButtonSize());
    ui->_text->setStyleSheet("background-color: rgb(232,143,12)");
    ui->_text->setHtml("<H3>Please select exercise</H3>");
    ui->_text_ru->setHtml("<H3>Пожалуйста выберете упражнение</H3>");
    ui->_text_ru->setStyleSheet("background-color: rgb(232,143,12)");
    ui->_ok->setVisible(false);
    ui->_open->setVisible(false);

    ui->_list->setSelectionRectVisible(false);

    connect(ui->_list->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(OnSelectionChanged(QItemSelection)));
}

void FormExercises::OnSelectionChanged(const QItemSelection& selection){
   if(selection.indexes().isEmpty()) {
       ui->_text->setHtml("<H3>Please select exercise</H3>");
       ui->_text_ru->setHtml("<H3>Пожалуйста выберете упражнение</H3>");
       ui->_ok->setVisible(false);
       ui->_open->setVisible(false);
   } else {
       int index = selection.indexes().first().row();
       int i=0;


       for(std::shared_ptr<Exercise> ex:Configuration::GetInstance().GetExercises()){
           if(i==index){
               ui->_text->setHtml("<H2>Exercise "+QString::number(ex->GetIndex())+"</H2><p>"+ex->GetTextEn() + "</p>");
               ui->_text_ru->setHtml("<H2>Упражнение "+QString::number(ex->GetIndex())+"</H2><p>"+ex->GetTextRu() + "</p>");
               ui->_ok->setVisible(true);
               ui->_open->setVisible(true);
               _exercise = ex;

           }
           i++;
       }



   }
}


void FormExercises::BeforeChange(std::shared_ptr<UIStateData> state_data){


    UIStateExercisesData *data = dynamic_cast<UIStateExercisesData*>(state_data.get());
    if(data){
//        state_data->SetName(ui->text_project_name->text());

//        Save();
    }


}

QAbstractItemModel *FormExercises::createModel()
{
    QStandardItemModel *model = new QStandardItemModel();
    QList<QStandardItem *> listItem;


    for(std::shared_ptr<Exercise> ex : Configuration::GetInstance().GetExercises()){
        QStandardItem *item = new QStandardItem();
        QString name;
        if(ex->GetIndex()<10) name = ":/Files/images/exercises/ex_0";
                        else  name = ":/Files/images/exercises/ex_";

        name += QString::number(ex->GetIndex());
        qDebug() << name;
        if(ex->GetCompleted()) name +="_done";
        name += ".png";
        item->setIcon(QIcon(QPixmap(name)));
        QString text = "Exercise "+QString::number(ex->GetIndex());
        item->setText(text);
        item->setEditable(false);
        item->setDragEnabled(false);
        item->setDropEnabled(false);
        item->setSelectable(true);
        item->setCheckable(false);
        item->setRowCount(0);
        listItem << item;
    }
    model->appendColumn(listItem);

return model;

}

void FormExercises::AfterUndo(std::shared_ptr<UIStateData> state_data) {
    UIStateExercisesData *data = dynamic_cast<UIStateExercisesData*>(state_data.get());
    if(data){
 //       ui->text_project_name->setText(VMTProject::GetInstance().GetName());//data->name);
 //       ui->text_project_location->setText(VMTProject::GetInstance().GetLocation());//data->location);
 //       ui->text_alphabit->setText(VMTProject::GetInstance().GetAlphabit()->ToString());//data->alphabit);
 //       Check();
    }
}

FormExercises::~FormExercises()
{
    delete ui;
}

void FormExercises::on__ok_clicked()
{
    if( VMTProject::GetInstance().GetUndoManager())
        VMTProject::GetInstance().GetUndoManager()->Clear();
    VMTProject::GetInstance().GetName() = "Exercise "+QString::number(_exercise->GetIndex());;
    VMTProject::GetInstance().GetLocation() = Configuration::GetInstance().GetDefaultLocation();
    VMTProject::GetInstance().GetAlphabit()->ReplaceString(_exercise->GetAlphabit());
    VMTProject::GetInstance().SetExercise(_exercise);
    _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
}

bool FormExercises::OpenProject(QString &file){
    //qDebug() << "Open project:" << file;
    VMTSerializer serializer(file);
    serializer.Deserialize(&VMTProject::GetInstance());
    VMTProject::GetInstance().GetUndoManager()->Clear();

    QFileInfo file_info(file);
    VMTProject::GetInstance().GetLocation() = file_info.absoluteDir().absolutePath();
    VMTProject::GetInstance().GetName() = file_info.baseName();

    return true;
}


void FormExercises::on__open_clicked()
{
    QFileDialog dlg;
    dlg.setWindowTitle("Open Virtual Machine Project");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setViewMode(QFileDialog::List);
    QStringList filters;
    filters <<"Any files (*)"
            <<"Turing machine files (*.jdtp)";
    dlg.setOption(QFileDialog::HideNameFilterDetails,false);
    dlg.setNameFilters(filters);
    dlg.resize(QApplication::primaryScreen()->availableSize());
    if(dlg.exec()==QDialog::Accepted)
    {
        QStringList fileName=dlg.selectedFiles();

        if(fileName.length()>0){
            QString file = fileName[0];
            if(file.length()>0){
                if(OpenProject(file)){
                    if( VMTProject::GetInstance().GetUndoManager())
                        VMTProject::GetInstance().GetUndoManager()->Clear();
                    VMTProject::GetInstance().GetAlphabit()->ReplaceString(_exercise->GetAlphabit());
                    VMTProject::GetInstance().SetExercise(_exercise);
                    Configuration::GetInstance().AddRecentProject(file);
                    Configuration::GetInstance().Save();
                    _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
                }
            }
        }
    }
}
