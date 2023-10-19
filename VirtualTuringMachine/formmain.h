#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QWidget>
#include <QListWidget>
#include "formbase.h"

namespace Ui {
class FormMain;
}

class MainWindow;

class FormMain : public QWidget, public FormBase
{
    Q_OBJECT

public:
    explicit FormMain(QWidget *parent = 0);
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    ~FormMain();

private:
    Ui::FormMain *ui;
    void addItem(QListWidget *listWidget, QString iconPath, QString text,int data);
    bool OpenProject(QString &file);
private slots:
    void on_button_new_clicked();
    void on_pushButton_clicked();
    void on_listWidget_itemSelectionChanged();
    void on_pushButton_Recent_clicked();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_clicked(const QModelIndex &index);
    void on_button_open_clicked();
    void on_toolButton_clicked();
    void on_toolButton_2_triggered(QAction *arg1);
    void on_toolButton_2_clicked();
};

#endif // FORMMAIN_H
