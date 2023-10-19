#include "uicanvas.h"
#include <cmath>

//#define STEP 20
//#define FONT_SIZE 18


UICanvas::UICanvas(QBrush background,QPen net,QPen foreground,QSize size,size_t font_size):
    _brush_background(background),
    _fill_normal(background),
    _fill_selected(QColor(232,143,12)),//#012C40
    _pen_net(net),
    _pen_selected(QColor(255,0,0)),//#01547a
    _pen_foreground(QColor(0,0,0)),
    _pen_error(QColor(178,97,0),3),
    _painter(nullptr),
    _animation(0)
{
    _font_size = font_size;
    _net = size;
    _font = QFont("Courier New", 12, QFont::Bold);


    for(int i=0;i<10;i++){
        QVector<qreal> dashes;
        _animation_pen[i].setColor(Qt::black);
        _animation_pen[i].setWidth(1);
        _animation_pen[i].setDashOffset(i);
        _animation_pen[i].setCapStyle(Qt::RoundCap);

        dashes << 5 << 5;

        _animation_pen[i].setStyle(Qt::CustomDashLine);
        _animation_pen[i].setDashPattern(dashes);

    }

    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_lambda_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_start_black.png"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_finish_black.png"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_l_small_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_r_small_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_l_big_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_r_big_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_k_black.gif"));
    _images.push_back(new QPixmap(":/Files/images/cancel.png"));
    _images.push_back(new QPixmap(":/Files/images/machine_complex.gif"));
    _images.push_back(new QPixmap(":/Files/images/toolbars/tools/icon_link.gif"));
    _images.push_back(new QPixmap(":/Files/images/circle.png"));
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
    static QBrush fill_brush(QColor(255,255,255));//0,149,218));
    static QPen fill_pen(QColor(132,132,132));
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

    if(error) _painter->setPen(_pen_error);
    else _painter->setPen(_pen_foreground);

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

void UICanvas::DrawLine(const QPoint &start,const QPoint &end, [[maybe_unused]] bool selected,bool error){
    std::lock_guard<std::mutex> guard(_mutex);
    double factor = GetFactor();

    if(error) _painter->setPen(_pen_error);
    else _painter->setPen(_pen_foreground);

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
    if(error) _painter->setPen(_pen_error);
    else _painter->setPen(_pen_foreground);
    if(selected) _painter->fillRect(to_draw,this->_fill_selected);
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

