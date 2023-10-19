#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include "statecontroller.h"
#include "formwarning.h"
#include <QCloseEvent>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public StateController
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget* GetContainer() override;
    void OnChanged(std::shared_ptr<UIState>     state,std::shared_ptr<UIStateData> new_state_data) override;
    void ShowWarning(std::shared_ptr<UIStateData>) override;
    void HideWarning(bool undo) override;
    void EnableExercise(bool enable) override;
    void EnableNewMachine(bool enable) override;
    void EnableOpenMachine(bool enable) override;
    void EnableSave(bool enable) override;
    void EnableExport(bool enable) override;
    void EnableRun(bool enable) override;
    void EnableConfigure(bool enable) override;
    void EnableDebugBack(bool enable) override;
    void EnableDebugOver(bool enable) override;
    void EnableDebugPause(bool enable) override;
    void EnableDebugRun(bool enable) override;
    void EnableDebugStep(bool enable) override;
    void EnableDebugStop(bool enable) override;
    void EnableOk(bool enable) override;
    void EnableCancel(bool enable) override;
    void EnableZoom(bool enable) override;
    void Enable4thExport(bool enable) override;
    void closeEvent (QCloseEvent *event) override;
    int  GetZoom() override;
protected:
    void keyPressEvent (QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *) override;
private:
    Ui::MainWindow *ui;
    FormWarning *_form_warning;
    QWidget     *_form_warning_state;

private slots:
    void onBackClicked();
    void onRefClicked();
    void on_button_new_machine_clicked();
    void on_button_open_machine_clicked();
    void on_button_save_clicked();
    void on_button_export_clicked();
    void on_button_run_clicked();
    void on_button_configure_clicked();
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void on_slider_zoom_sliderMoved(int position);
    void on_button_debug_back_clicked();
    void on_button_debug_step_clicked();
    void on_slider_zoom_sliderPressed();
    void on_button_4th_clicked();
    void on_button_debug_run_clicked();
    void on_button_debug_pause_clicked();
    void on_button_debug_stop_clicked();
    void on_button_exercise_clicked();
};

#endif // MAINWINDOW_H
