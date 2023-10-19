#ifndef FORMNEWPROJECT_H
#define FORMNEWPROJECT_H

#include <QWidget>
#include "formbase.h"

namespace Ui {
class FormNewProject;
}

class MainWindow;


class FormNewProject : public QWidget, public FormBase
{
    Q_OBJECT

public:
    explicit FormNewProject(QWidget *parent = 0);
    ~FormNewProject();
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void Save();
private slots:
    void on_button_z2_clicked();

    void on_button_z3_clicked();

    void on_button_z4_clicked();

    void on_button_z10_clicked();

    void on_button_z16_clicked();

    void on_button_z80_clicked();

    void on_button_project_location_clicked();

    void on_text_alphabit_textChanged();

    void on_button_create_clicked();

private:
    Ui::FormNewProject *ui;

    void Check();
};

#endif // FORMNEWPROJECT_H
