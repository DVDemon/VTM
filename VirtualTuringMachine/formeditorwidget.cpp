#include "formeditorwidget.h"
#include "ui_formeditorwidget.h"
#include <QDateTime>
#include "screentools.h"
#include "vmttheme.h"

FormEditorWidget::FormEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEditorWidget),
    _environment(nullptr),
    _canvas(QBrush(VmtTheme::diagramBackground()),
            QPen(VmtTheme::diagramGrid(), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin),
            QPen(VmtTheme::diagramLine(), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin),
            QSize(32, 32), 16)
{
    ui->setupUi(this);
    VmtTheme::polishWidgetTree(this);
    applyTheme();
    this->setMouseTracking(true);

    ScreenTools st;
    _canvas.SetCellSize(st.GetCellSize());
    _canvas.SetFontSize(st.GetFontSize());

}

void FormEditorWidget::applyTheme()
{
    _canvas.applyThemeColors();
    setStyleSheet(QStringLiteral("FormEditorWidget { background-color: %1; }")
                      .arg(VmtTheme::colorName(VmtTheme::canvasWorkspace())));
    update();
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
        const int margin = std::max(12, static_cast<int>(_canvas.GetStep()));
        screen = screen.marginsAdded(QMargins(margin, margin, margin, margin));
        update(screen);
    }

}

void FormEditorWidget::RepaintThrottled(const QRect &real_rect)
{
    if (!real_rect.isEmpty()) {
        Repaint(real_rect);
        return;
    }

    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - _lastThrottledRepaintMs < 16) {
        return;
    }
    _lastThrottledRepaintMs = now;
    update();
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
