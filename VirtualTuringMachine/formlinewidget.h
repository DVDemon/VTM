#ifndef FORMLINEWIDGET_H
#define FORMLINEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QDebug>

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>
#include <QLineEdit>
#include <QHBoxLayout>
#include <VMTLine.h>
#include <memory>

namespace Ui {
class FormLineWidget;
}

class FormLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormLineWidget(QWidget *parent = 0);
    void SetLine(std::shared_ptr<VMTLine> line);
    void Repaint();
    void Left();
    void LeftPage();
    void Right();
    void RightPage();
    void SetTestMode();
    bool eventFilter(QObject* object, QEvent* event) override;
    ~FormLineWidget();
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
private:
    Ui::FormLineWidget *ui;
    size_t    _count;
    long      _position;
    void  FillLineEdit();
    QLineEdit _line_edit[128];
    QHBoxLayout _layout;
    std::shared_ptr<VMTLine> _line;

    void RightShiftActiveLineEdit(int currentIndex);
    bool isShiftFromType = true; // Input from keyboard, not from Right, RightPage, etc.

private slots:
    void onEditChanged(QString text);

};

#endif // FORMLINEWIDGET_H
