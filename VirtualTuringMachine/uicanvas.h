#ifndef UICANVAS_H
#define UICANVAS_H

#include <QPainter>
#include <QPoint>
#include <QBrush>
#include <QPen>
#include <QPainter>

#include <mutex>
#include <vector>

enum ImageType{
    MACHINE_LAMBDA = 0,
    MACHINE_START = 1,
    MACHINE_FINISH = 2,
    MACHINE_LEFT = 3,
    MACHINE_RIGHT = 4,
    MACHINE_LEFT_WORD = 5,
    MACHINE_RIGHT_WORD = 6,
    MACHINE_COPY = 7,
    CANCEL=8,
    MACHINE_COMPLEX=9,
    LINK=10,
    NAVIGATION_CIRCLE=11
};

class UICanvasState{
  public:
    QPoint _center;
    size_t _zoom;

    UICanvasState() {
       _center = QPoint(0,0);
       _zoom = 100;
    }

    UICanvasState &operator= (const UICanvasState &other){
        _center = other._center;
        _zoom = other._zoom;
        return *this;
    }
};

class UICanvas
{
protected:
    QBrush _brush_background;
    QPen   _pen_net;
    QPen   _pen_foreground;
    QPen   _pen_selected;
    QPen   _pen_error;
    UICanvasState _state;

    QPainter *_painter;
    QFont  _font;

    QBrush _fill_normal;
    QBrush _fill_selected;


    double GetFactor();
    void PaintBackground(const QRect &rect,const QRect &bounds);

    QPen _animation_pen[10];
    int _animation;
    std::mutex _mutex;

    std::vector<QPixmap*> _images;
    QSize _net;
    size_t _font_size;

public:
    size_t GetStep();
    void SetFontSize(size_t font_size);
    void SetCellSize(QSize size);

    UICanvas(QBrush background,QPen net,QPen foreground,QSize size,size_t font_size);

    const UICanvasState& GetState();
    void SetState(const UICanvasState& other);
    QPainter *GetPainter();
    void   Animate();
    QPoint &GetCenter();
    void   SnapToGris(QPoint &point);
    void   SetPainter(QPainter *painter);
    QPixmap* GetImage(ImageType type);

    size_t       GetZoom() const;
    void         SetZoom(size_t zoom);
    QPoint FromScreen(QPoint &shift);
    QPoint FromScreen(QPoint &&shift);
    QRect  FromScreen(const QRect& rect);
    QPoint ToScreen(QPoint point);
    QRect  ToScreen(const QRect& rect);
    void SetInScreen(QPoint &&);
    void MoveInScreen(QPoint &shift);
    void MoveInScreen(QPoint &&shift);
    void Move(QPoint &shift);
    void Move(QPoint &&shift);
    void SetCenter(QPoint point,QSize screen_size);
    void Paint(QPainter &painter, const QRect &rect,const QRect &bounds);

    void CalculateTextSize(const QString& text,QRect &rect);
    void FillRect(const QRect &rect, bool selected=false);
    void DrawButton(const QRect &rect);
    void DrawImageScreen(ImageType image_type, const QRect &rect);
    void DrawImage(ImageType image_type, const QRect &rect);

    void DrawLine(const QPoint &start,const QPoint &end, bool selected=false,bool error=false);
    void DrawArrow(const QPoint &start,const QPoint &end, bool selected=false,bool error=false);
    void DrawLineAnimation(const QPoint &start,const QPoint &end, bool selected=false);
    void DrawRect(const QRect & rect,bool selected=false,bool error=false);
    void DrawRectScreen(const QRect & rect);
    void DrawRectAnimation(const QRect & rect,bool selected=false);
    //void DrawText(const QString& text,const QRect &bounds,bool selected=false);
    void DrawText(const QString& text,const QPoint center,size_t size,bool selected = false);
};

#endif // UICANVAS_H
