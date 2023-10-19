#include "formeditorwidget.h"
#include "ui_formeditorwidget.h"
#include <QDebug>
#include "screentools.h"

FormEditorWidget::FormEditorWidget(QWidget *parent) :
            QWidget(parent),

    ui(new Ui::FormEditorWidget),
    _canvas(QBrush(QColor(255,255,255)),//QWidget::palette().background(),
            QPen(QColor(200, 200, 200), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin), //2,176,255
            //QPen(QWidget::palette().text().color()),
            QPen(QColor(0, 0, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin),
            QSize(32,32),16),
            _environment(nullptr)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    ScreenTools st;
    _canvas.SetCellSize(st.GetCellSize());
    _canvas.SetFontSize(st.GetFontSize());

}

 void FormEditorWidget::EnableAnimation([[maybe_unused]] bool enable){
 }

void FormEditorWidget::SetEnvironment(IVMTEnvironment *environment){
    _environment = environment;
}

void FormEditorWidget::SetComplexMachine(std::shared_ptr<VMTComplexMachine> m){
    _machine = m;
}

std::shared_ptr<VMTComplexMachine> FormEditorWidget::GetComplexMachine(){
   return _machine;
}

UICanvas& FormEditorWidget::GetCanvas(){
   return _canvas;
}

void FormEditorWidget::SetZoom(size_t zoom){
    _canvas.SetZoom(zoom);
    update();

}

void FormEditorWidget::animate()
{
    _canvas.Animate();
    update();
}

void FormEditorWidget::mouseMoveEvent(QMouseEvent * event){
    if(_action) _action->OnMouseMoved(_environment,event->pos(),_canvas.FromScreen(event->pos()));
}

void FormEditorWidget::mousePressEvent(QMouseEvent * event){
    if(_action) _action->OnMousePressed(_environment,event->pos(),_canvas.FromScreen(event->pos()));
}

void FormEditorWidget::mouseReleaseEvent(QMouseEvent * event){
    if(_action) _action->OnMouseReleased(_environment,event->pos(),_canvas.FromScreen(event->pos()));
}

void FormEditorWidget::mouseDoubleClickEvent(QMouseEvent * ){

}

void FormEditorWidget::Center(){
    QRect rect = _canvas.ToScreen(_machine->GetBoundsWithChilds());

    _canvas.MoveInScreen(QPoint(-rect.topLeft().x(),-rect.topLeft().y()));
    this->repaint();
}

void FormEditorWidget::Paint(UICanvas& canvas,const QRect rect){
    canvas.Paint(*canvas.GetPainter(),rect,rect);
    if(_machine) _machine->PaintDiagram(canvas,rect);
}

void FormEditorWidget::Repaint(const QRect &real_rect){
    if(real_rect.isEmpty()){
            update();
    } else {
        QRect screen = _canvas.ToScreen(real_rect);
        update(screen);
    }

}

void FormEditorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    QRect bounds;
    bounds.setSize(this->size());
    _canvas.Paint(painter,event->rect(),bounds);
    if(_machine) _machine->PaintDiagram(_canvas,event->rect());
    if(_action) _action->Paint(_canvas,event->rect());
    painter.end();
}

std::shared_ptr<IVMTAction> &FormEditorWidget::GetAction(){
    return _action;
}



FormEditorWidget::~FormEditorWidget()
{
    this->setMouseTracking(false);
    this->_action.reset();
    this->_machine.reset();
    delete ui;
}
