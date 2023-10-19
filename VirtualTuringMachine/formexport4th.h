#ifndef FORMEXPORT4TH_H
#define FORMEXPORT4TH_H

#include "formbase.h"
#include <memory>
#include <QWidget>
#include <QTimer>
#include "vmtexport4th/vmtexport4th.h"

namespace Ui {
class FormExport4th;
}

class FormExport4th : public QWidget , public FormBase, public TextCallback
{
    Q_OBJECT

public:
    explicit FormExport4th(QWidget *parent = 0);
    ~FormExport4th();
    void BeforeChange(std::shared_ptr<UIStateData> state_data) override;
    void AfterUndo(std::shared_ptr<UIStateData> state_data) override;
    void OnLoaded() override;

    void Done() override;
    void Clear() override;
    void AppendText(const QString &text) override;
    void Information(const QString &text) override;
public slots:

private slots:


    void on__export_clicked();

    void on__copy_clicked();
    void OnTimer();

private:
    Ui::FormExport4th *ui;
    VMTExport4th*  _export;
    QString _output;
    QString _information;
    bool    _done;

    std::shared_ptr<VMTComplexMachine> CreatePlainMachine();
};

#endif // FORMEXPORT4TH_H
