#ifndef ANDROID
#define ANDROID
#endif


#ifndef FORMEDITORWIDGET_H
#define FORMEDITORWIDGET_H



#include <QWidget>
#include <QPaintEvent>

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <uicanvas.h>
#include "vmtmachines/VMTComplexMachine.h"
#include "interfaces/IVMTAction.h"
class IVMTEnvironment;

namespace Ui {
class FormEditorWidget;
}

class FormEditorWidget :
                public QWidget

{
    Q_OBJECT

public:
    explicit FormEditorWidget(QWidget *parent = 0);
    void SetZoom(size_t zoom);
    void SetEnvironment(IVMTEnvironment *environment);
    UICanvas& GetCanvas();
    void SetComplexMachine(std::shared_ptr<VMTComplexMachine> m);
    std::shared_ptr<VMTComplexMachine> GetComplexMachine();
    std::shared_ptr<IVMTAction> &GetAction();
    void Paint(UICanvas& canvas,const QRect rect);
    void Repaint(const QRect &real_rect);
    void Center();
    void EnableAnimation(bool enable);

    ~FormEditorWidget();

public slots:
    void animate();

protected:
    QPoint& GetPaintAreaOffset();
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::FormEditorWidget *ui;
    IVMTEnvironment *_environment;
    UICanvas _canvas;
    std::shared_ptr<IVMTAction>        _action;
    std::shared_ptr<VMTComplexMachine> _machine;
};

#endif // FORMEDITORWIDGET_H
