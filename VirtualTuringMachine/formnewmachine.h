#ifndef FORMNEWMACHINE_H
#define FORMNEWMACHINE_H
#include "formbase.h"
#include <QWidget>
#include <QStringListModel>

namespace Ui {
class FormNewMachine;
}

class FormNewMachine : public QWidget, public FormBase
{
    Q_OBJECT

public:
    explicit FormNewMachine(QWidget *parent = 0);
    ~FormNewMachine();
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void OnLoaded() override;
private slots:
    void on_button_ok_clicked();

    void on_text_machine_name_textChanged(const QString &arg1);

    void on_list_machines_clicked(const QModelIndex &index);

    void on_button_delete_clicked();

    void on_text_machine_name_returnPressed();

    void on_button_ok_2_clicked();

    void on_button_delete_2_clicked();

private:
    QStringListModel _list_model;
    Ui::FormNewMachine *ui;

    void FillList();
    void Check();
};

#endif // FORMNEWMACHINE_H
