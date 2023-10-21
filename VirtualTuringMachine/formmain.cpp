#include "formmain.h"
#include "ui_formmain.h"
#include <QDebug>
#include <QFileDialog>
#include "configuration.h"
#include "vmtproject.h"
#include <QApplication>
#include <QScreen>
#include "screentools.h"
#include "uistateexercises.h"
#include <QDesktopServices>
#include <QUrl>

FormMain::FormMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMain)
{
    ui->setupUi(this);



    for(const QString &name: Configuration::GetInstance().GetRecentProjects())
        addItem(ui->listWidget, ":/Files/images/toolbars/main/icon_open_black.png",name,1);


    VMTProject::GetInstance().GetMachines().clear();

    ScreenTools st;

    st.ResizeButtonBig(ui->button_new);
    st.ResizeButtonBig(ui->button_open);
    st.ResizeButtonBig(ui->toolButton);

    ui->listWidget->setStyleSheet("background-color:gray;");

//    QObject::connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)),
//                     this, SLOT(onItemClicked(QListWidgetItem *)));
}

void FormMain::BeforeChange(std::shared_ptr<UIStateData>){

}

void FormMain::AfterUndo(std::shared_ptr<UIStateData>) {

}

void FormMain::addItem(QListWidget *listWidget, QString iconPath, QString text,int data)
{
    QIcon icon(iconPath);
    QListWidgetItem *item = new QListWidgetItem(icon, text);
    QVariant variant(data);
    item->setData(Qt::UserRole, variant);
    listWidget->addItem(item);
}


#include "uistatenewproject.h"

FormMain::~FormMain()
{
    qDebug() << "Delete FormMain";
    delete ui;
}

void FormMain::on_button_new_clicked()
{
    qDebug() << "Clear undo manager";
    if( VMTProject::GetInstance().GetUndoManager())
        VMTProject::GetInstance().GetUndoManager()->Clear();
    VMTProject::GetInstance().GetName() = "MyProject";
    VMTProject::GetInstance().GetLocation() = Configuration::GetInstance().GetDefaultLocation();
    QString alphabit("");
    VMTProject::GetInstance().GetAlphabit()->ReplaceString(alphabit);
    qDebug() << "Change state";
    _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
}

void FormMain::on_pushButton_clicked()
{

}

void FormMain::on_listWidget_itemSelectionChanged()
{

}

void FormMain::on_pushButton_Recent_clicked()
{
}

bool FormMain::OpenProject(QString &file){
    //qDebug() << "Open project:" << file;
    VMTSerializer serializer(file);
    serializer.Deserialize(&VMTProject::GetInstance());
    VMTProject::GetInstance().GetUndoManager()->Clear();

    QFileInfo file_info(file);
    VMTProject::GetInstance().GetLocation() = file_info.absoluteDir().absolutePath();
    VMTProject::GetInstance().GetName() = file_info.baseName();

    return true;
}

void FormMain::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QListWidgetItem *item=ui->listWidget->item(row);
    if(item){
        QString file = item->text();
        if(OpenProject(file))
            _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
    }
}

void FormMain::on_listWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    QListWidgetItem *item=ui->listWidget->item(row);
    if(item){
        QString file = item->text();
        if(OpenProject(file))
            _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
    }
}

void FormMain::on_button_open_clicked()
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
                    Configuration::GetInstance().AddRecentProject(file);
                    Configuration::GetInstance().Save();
                    _controller->ChangeState(std::shared_ptr<UIStateNewProjectData>(new UIStateNewProjectData()));
                }
            }
        }
    }
}

void FormMain::on_toolButton_clicked()
{
    if( VMTProject::GetInstance().GetUndoManager())
        VMTProject::GetInstance().GetUndoManager()->Clear();
    VMTProject::GetInstance().GetName() = "MyExercise";
    VMTProject::GetInstance().GetLocation() = Configuration::GetInstance().GetDefaultLocation();
    QString alphabit("");
    VMTProject::GetInstance().GetAlphabit()->ReplaceString(alphabit);
    _controller->ChangeState(std::make_shared<UIStateExercisesData>());

}

void FormMain::on_toolButton_2_triggered([[maybe_unused]] QAction *arg1)
{

}

void FormMain::on_toolButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl("https://mai.ru/"));
}
