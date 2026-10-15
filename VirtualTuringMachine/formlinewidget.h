#ifndef FORMLINEWIDGET_H
#define FORMLINEWIDGET_H

#include <VMTLine.h>

#include <QBrush>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QResizeEvent>
#include <QWidget>
#include <memory>

namespace Ui {
class FormLineWidget;
}

class FormLineWidget : public QWidget {
    Q_OBJECT

   public:
    explicit FormLineWidget(QWidget* parent = 0);
    void SetLine(std::shared_ptr<VMTLine> line);
    void Repaint();
    void Left();
    void LeftPage();
    void Right();
    void RightPage();
    void SetTestMode();
    void applyTheme();
    bool eventFilter(QObject* object, QEvent* event) override;
    ~FormLineWidget();
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

   private:
    Ui::FormLineWidget* ui;
    size_t _count;
    size_t _position;
    void FillLineEdit();
    QLineEdit _line_edit[128];
    QHBoxLayout _layout;
    std::shared_ptr<VMTLine> _line;

    void RightShiftActiveLineEdit(int currentIndex);
    bool isShiftFromType =
        true;  // TODO(Tias) refactor to better shifting logic

   private slots:
    void onEditChanged(QString text);
};

#endif  // FORMLINEWIDGET_H
