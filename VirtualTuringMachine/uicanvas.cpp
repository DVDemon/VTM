#include "uicanvas.h"
#include "vmticons.h"
#include "vmttheme.h"

#include <QTransform>

#include <cmath>

//#define STEP 20
//#define FONT_SIZE 18


UICanvas::UICanvas(QBrush background,QPen net,[[maybe_unused]] QPen foreground,QSize size,size_t font_size):
    _brush_background(background),
    _pen_net(net),
    _pen_foreground(QPen(VmtTheme::diagramLine())),
    _pen_selected(QPen(VmtTheme::primary(), 2)),
    _pen_error(QPen(VmtTheme::error(), 3)),
    _painter(nullptr),
    _fill_normal(QBrush(VmtTheme::machineFill())),
    _fill_selected(VmtTheme::machineFillSelected()),
    _animation(0)
{
    _font_size = font_size;
    _net = size;
    _font = QFont("Courier New", 12, QFont::Bold);


    for(int i=0;i<10;i++){
        QVector<qreal> dashes;
        _animation_pen[i].setWidth(1);
        _animation_pen[i].setDashOffset(i);
        _animation_pen[i].setCapStyle(Qt::RoundCap);

        dashes << 5 << 5;

        _animation_pen[i].setStyle(Qt::CustomDashLine);
        _animation_pen[i].setDashPattern(dashes);

    }

    applyThemeColors();
}

void UICanvas::reloadMachineIcons()
{
    VmtIcons::loadMachineIcons(_images);
}

void UICanvas::applyThemeColors()
{
    _brush_background = QBrush(VmtTheme::diagramBackground());
    _pen_net = QPen(VmtTheme::diagramGrid(), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    _pen_foreground = QPen(VmtTheme::diagramLine());
    _pen_selected = QPen(VmtTheme::primary(), 2);
    _pen_error = QPen(VmtTheme::error(), 3);
    _fill_normal = QBrush(VmtTheme::machineFill());
    _fill_selected = QBrush(VmtTheme::machineFillSelected());
    for (int i = 0; i < 10; ++i) {
        _animation_pen[i].setColor(VmtTheme::diagramLine());
    }
    reloadMachineIcons();
}

void UICanvas::SetFontSize(size_t font_size){
    _font_size = font_size;
}
void UICanvas::SetCellSize(QSize size){
    _net = size;
}
size_t UICanvas::GetStep(){
    return _net.width();
}

QPainter *UICanvas::GetPainter(){
    return _painter;
}

void  UICanvas::SetPainter(QPainter *painter){
    _painter = painter;
}



void   UICanvas::SnapToGris(QPoint &point){
    point.rx() = (point.x()/GetStep())*GetStep()+GetStep()/2;
    point.ry() = (point.y()/GetStep())*GetStep()+GetStep()/2;
}

void   UICanvas::Animate(){
    _animation = (_animation+1)%10;
}



double UICanvas::GetFactor(){
    size_t factor = (_state._zoom*100)/100;
    return (double)factor/100.0;
}

void UICanvas::SetCenter(QPoint point,QSize screen_size){
    double factor = GetFactor();
    _state._center.setX(-point.x()+screen_size.width()/(2*factor));
    _state._center.setY(-point.y()+screen_size.height()/(2*factor));
}

QPoint& UICanvas::GetCenter(){
    return _state._center;
}

QRect  UICanvas::ToScreen(const QRect& rect){
    QRect result;
    double factor = GetFactor();
    result.setLeft((rect.left()+_state._center.x())*factor);
    result.setTop((rect.top()+_state._center.y())*factor);
    result.setWidth(rect.width()*factor);
    result.setHeight(rect.height()*factor);
    return result;
}

QPoint UICanvas::ToScreen(QPoint point){
    double factor = GetFactor();
    return QPoint((point.rx()+_state._center.rx())*factor,
                  (point.ry()+_state._center.ry())*factor);
}
QPoint UICanvas::FromScreen(QPoint &shift){
    double factor = GetFactor();
    return QPoint(shift.rx()/factor-_state._center.rx(),shift.ry()/factor-_state._center.ry());
}

QPoint UICanvas::FromScreen(QPoint &&shift){
    double factor = GetFactor();
    return QPoint(shift.rx()/factor-_state._center.rx(),shift.ry()/factor-_state._center.ry());
}

QRect  UICanvas::FromScreen(const QRect& rect){
    QRect result;
    double factor = GetFactor();
    result.setLeft(rect.left()/factor-_state._center.x());
    result.setTop(rect.top()/factor-_state._center.y());
    result.setWidth(rect.width()/factor);
    result.setHeight(rect.height()/factor);
    return result;
}


void UICanvas::MoveInScreen(QPoint &shift){
    double factor = GetFactor();
    _state._center += QPoint(shift.rx()/factor,shift.ry()/factor);
}
void UICanvas::MoveInScreen(QPoint &&shift){
    double factor = GetFactor();
    _state._center += QPoint(shift.rx()/factor,shift.ry()/factor);
}

void UICanvas::SetInScreen(QPoint &&center){
    double factor = GetFactor();
    _state._center = QPoint(center.rx()/factor,center.ry()/factor);
}

void UICanvas::Move(QPoint &shift){
    _state._center += shift;
}
void UICanvas::Move(QPoint &&shift){
    _state._center += shift;
}


size_t UICanvas::GetZoom() const{
    return _state._zoom;
}

void         UICanvas::SetZoom(size_t zoom){
    if(zoom>0) _state._zoom = zoom;
}

void UICanvas::Paint(QPainter &painter,const QRect &rect,const QRect &bounds){
    std::lock_guard<std::mutex> guard(_mutex);
    _painter = &painter;
    PaintBackground(rect,bounds);
}

void UICanvas::PaintBackground(const QRect &rect,const QRect &bounds){
    if(_painter!=nullptr){
        _painter->fillRect(rect, _brush_background);
        _painter->setPen(_pen_net);

        double factor = GetFactor();
        double step = _net.width()*factor;

        int x_count = (_state._center.x()*factor)/step;
        int y_count = (_state._center.y()*factor)/step;

        QPoint offset(_state._center.x()*factor - x_count*step,_state._center.y()*factor - y_count*step);


        for(double i=0;i<bounds.width();i+=step)
            if(((i+offset.x())>=rect.left())&&((i+offset.x())<=rect.right()))
                _painter->drawLine(QLineF(i+offset.x(),0,i+offset.x(),bounds.height()));

        for(double i=0;i<bounds.height();i+=step)
            if(((i+offset.y())>=rect.top())&&((i+offset.y())<=rect.bottom()))
                _painter->drawLine(QLineF(0,i+offset.y(),bounds.width(),i+offset.y()));
    }

}

void UICanvas::CalculateTextSize(const QString& text,QRect &rect){
    /*
    double factor = GetFactor();
    _font.setPointSizeF(_font_size*factor);
    _painter->setFont(_font);
    QFontMetrics metrics = _painter->fontMetrics();
    QRect rect1 = metrics.boundingRect(text);

    rect.setWidth(rect1.width());
    rect.setHeight(rect1.height());
    */
    //*
    _font.setPointSizeF(_font_size);
    QFontMetrics fm(_font);

    rect.setWidth(fm.horizontalAdvance(text));
    rect.setHeight(fm.height());
    //*/

}

void UICanvas::DrawButton(const QRect &rect){
    std::lock_guard<std::mutex> guard(_mutex);
    const QBrush fill_brush(VmtTheme::machineFill());
    const QPen fill_pen(VmtTheme::diagramNodeBorder());
//    static QPen pen_white(QColor(255,255,255));
//    static QPen pen_blue(QColor(1,84,122));
//    static QBrush fill_blue(QColor(1,84,122));
    double factor = GetFactor();

    QRect to_draw((_state._center.rx()+rect.left())*factor,
                  (_state._center.ry()+rect.top())*factor,
                  rect.width()*factor,rect.height()*factor);
    _painter->setPen(fill_pen);
    _painter->fillRect(to_draw,fill_brush);
    //_painter->setBrush(fill_blue);
    //_painter->drawRect(to_draw);
    //_painter->setPen(pen_blue);
    //_painter->drawEllipse(QRect(to_draw.right()-to_draw.height()/4,to_draw.top()+1,to_draw.height()/2,to_draw.height()-2));
    //_painter->fillRect(QRect(to_draw.left(),to_draw.top(),3,to_draw.height()),fill_blue);

}

void UICanvas::FillRect(const QRect &rect, bool selected){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();
    QRect to_draw((_state._center.rx()+rect.left())*factor,
                  (_state._center.ry()+rect.top())*factor,
                  rect.width()*factor,rect.height()*factor);
    if(!selected)    _painter->fillRect(to_draw,_fill_normal);
    else _painter->fillRect(to_draw,_fill_selected);
}

QPixmap*  UICanvas::GetImage(ImageType image_type){
    return _images[image_type];
}

void UICanvas::DrawImageScreen(ImageType image_type, const QRect &rect){
    std::lock_guard<std::mutex> guard(_mutex);
    _painter->drawPixmap(rect.left(),
                         rect.top(),
                         rect.width(),
                         rect.height(), *_images[image_type]);
}

void UICanvas::DrawImage(ImageType image_type, const QRect &rect){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    _painter->drawPixmap((_state._center.rx()+rect.left())*factor,
                         (_state._center.ry()+rect.top())*factor,
                         rect.width()*factor,
                         rect.height()*factor, *_images[image_type]);
}

void UICanvas::DrawArrow([[maybe_unused]] const QPoint &start,const QPoint &end, [[maybe_unused]] bool selected,bool error){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    if(error) {
        _painter->setPen(_pen_error);
    } else if(selected) {
        _painter->setPen(_pen_selected);
    } else {
        _painter->setPen(_pen_foreground);
    }

    QLine line1((_state._center.x()+end.x()-10)*factor,
               (_state._center.y()+end.y()-10)*factor,
               (_state._center.x()+end.x())*factor,
               (_state._center.y()+end.y())*factor);
    _painter->drawLine(line1);

    QLine line2((_state._center.x()+end.x()-10)*factor,
               (_state._center.y()+end.y()+10)*factor,
               (_state._center.x()+end.x())*factor,
               (_state._center.y()+end.y())*factor);
    _painter->drawLine(line2);
}

void UICanvas::DrawLine(const QPoint &start,const QPoint &end, bool selected,bool error){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    if(error) {
        _painter->setPen(_pen_error);
    } else if(selected) {
        _painter->setPen(_pen_selected);
    } else {
        _painter->setPen(_pen_foreground);
    }

    QLine line((_state._center.x()+start.x())*factor,
               (_state._center.y()+start.y())*factor,
               (_state._center.x()+end.x())*factor,
               (_state._center.y()+end.y())*factor);
    _painter->drawLine(line);
}

void UICanvas::DrawLineAnimation(const QPoint &start,const QPoint &end,[[maybe_unused]]  bool selected){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();
    _painter->setPen(_animation_pen[_animation]);


    QLine line((_state._center.x()+start.x())*factor,
               (_state._center.y()+start.y())*factor,
               (_state._center.x()+end.x())*factor,
               (_state._center.y()+end.y())*factor);
    _painter->drawLine(line);
}

namespace {

QTransform diagramToScreenTransform(const UICanvasState& state, double factor)
{
    QTransform transform;
    transform.scale(factor, factor);
    transform.translate(state._center.x(), state._center.y());
    return transform;
}

} // namespace

void UICanvas::DrawConnectorPath(const QPainterPath& diagramPath, bool selected, bool error, bool animated)
{
    if (diagramPath.isEmpty()) {
        return;
    }

    std::lock_guard<std::mutex> guard(_mutex);
    const double factor = GetFactor();
    const QPainterPath screenPath = diagramToScreenTransform(_state, factor).map(diagramPath);

    _painter->setRenderHint(QPainter::Antialiasing, true);

    if (animated) {
        _painter->setPen(_animation_pen[_animation]);
    } else if (error) {
        _painter->setPen(_pen_error);
    } else if (selected) {
        _painter->setPen(_pen_selected);
    } else {
        _painter->setPen(_pen_foreground);
    }

    _painter->strokePath(screenPath, _painter->pen());
}

void UICanvas::DrawArrowForPath(const QPainterPath& diagramPath, bool selected, bool error)
{
    if (diagramPath.isEmpty()) {
        return;
    }

    std::lock_guard<std::mutex> guard(_mutex);
    const double factor = GetFactor();

    if (error) {
        _painter->setPen(_pen_error);
    } else if (selected) {
        _painter->setPen(_pen_selected);
    } else {
        _painter->setPen(_pen_foreground);
    }

    const qreal approach = 0.94;
    const QPointF tipDiagram = diagramPath.pointAtPercent(1.0);
    const QPointF baseDiagram = diagramPath.pointAtPercent(approach);
    const QPointF delta = tipDiagram - baseDiagram;
    if (delta.manhattanLength() < 0.5) {
        return;
    }

    const qreal angleRad = std::atan2(delta.y(), delta.x());
    const qreal headLength = 10.0 * factor;
    const qreal headWidth = 10.0 * factor;

    const QPointF tip(tipDiagram.x() * factor + _state._center.x() * factor,
                      tipDiagram.y() * factor + _state._center.y() * factor);
    const QPointF dir(std::cos(angleRad), std::sin(angleRad));
    const QPointF normal(-dir.y(), dir.x());
    const QPointF back = tip - dir * headLength;

    _painter->drawLine(QLineF(back + normal * headWidth, tip));
    _painter->drawLine(QLineF(back - normal * headWidth, tip));
}

void UICanvas::DrawRectAnimation(const QRect & rect,bool selected){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();
    _painter->setPen(_animation_pen[_animation]);

    QRect to_draw((_state._center.rx()+rect.left())*factor,
                  (_state._center.ry()+rect.top())*factor,
                  rect.width()*factor,rect.height()*factor);
    if(selected) _painter->fillRect(to_draw,this->_fill_selected);
    _painter->drawRect(to_draw);
}

void UICanvas::DrawRectScreen(const QRect & rect){
    std::lock_guard<std::mutex> guard(_mutex);
    _painter->setPen(_pen_selected);
    _painter->drawRect(rect);
}

void UICanvas::DrawRect(const QRect & rect,bool selected,bool error){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();


    QRect to_draw((_state._center.rx()+rect.left())*factor,
                  (_state._center.ry()+rect.top())*factor,
                  rect.width()*factor,rect.height()*factor);
    if(error) {
        _painter->setPen(_pen_error);
    } else if(selected) {
        _painter->setPen(_pen_selected);
    } else {
        _painter->setPen(_pen_foreground);
    }
    if(selected) {
        _painter->fillRect(to_draw, _fill_selected);
    }
    _painter->drawRect(to_draw);
}

void UICanvas::DrawText(const QString& text,const QPoint center,size_t size,bool selected){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    if(selected) _painter->setPen(_pen_selected);
    else _painter->setPen(_pen_foreground);

    float _heightToFitIn = size*factor;
    float oldFontSize=_font.pointSizeF();
    float newFontSize;
    // Loop
    for (int i=0 ; i<3 ; i++)
    {
        float oldHeight = _painter->fontMetrics().boundingRect('D').height();
        newFontSize = (_heightToFitIn / oldHeight) * oldFontSize;
        _font.setPointSizeF(newFontSize);
        _painter->setFont(_font);
        oldFontSize = newFontSize;
    }

    _font.setPointSizeF(newFontSize);
    _painter->setFont(_font);
    QRect fontRect = _painter->fontMetrics().boundingRect(text);

    QPoint point((_state._center.rx()+center.x())*factor-fontRect.width()/2,
                 (_state._center.ry()+center.y())*factor+fontRect.height()/3);
    _painter->drawText(point,text);
}
/*
void UICanvas::DrawText(const QString& text,const QRect &bounds,bool selected){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    if(selected) _painter->setPen(_pen_selected);
    else _painter->setPen(_pen_foreground);

    float _heightToFitIn = bounds.height()*factor;
    float oldFontSize=_font.pointSizeF();
    float newFontSize;
    // Loop
    for (int i=0 ; i<3 ; i++)
    {
        float oldHeight = _painter->fontMetrics().boundingRect('D').height();
        newFontSize = (_heightToFitIn / oldHeight) * oldFontSize;
        _font.setPointSizeF(newFontSize);
        _painter->setFont(_font);
        oldFontSize = newFontSize;
    }

    _font.setPointSizeF(newFontSize);
    _painter->setFont(_font);
    QRect fontRect = _painter->fontMetrics().boundingRect(text);


    QPoint point((_state._center.rx()+bounds.left()+bounds.width()/2-fontRect.width()/2)*factor,
                 (_state._center.ry()+bounds.bottom()-bounds.height()/2+fontRect.height()/2)*factor);
    _painter->drawText(point,text);
}*/

const UICanvasState& UICanvas::GetState(){
    return _state;
}

void UICanvas::SetState(const UICanvasState& other){
    _state = other;
}

