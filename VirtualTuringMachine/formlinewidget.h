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
#include <array>
#include <memory>

namespace Ui {
class FormLineWidget;
}

class FormLineWidget : public QWidget {
    Q_OBJECT

   public:
    explicit FormLineWidget(QWidget* parent = 0);
    void SetLine(std::shared_ptr<VMTLine> line);
    // void Repaint();
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
    void SyncLineView();
    void SyncActivePositionView();

   private:
    class ActivePositionChangeHandler;
    struct LineView {
        size_t offset = 0;
        size_t width = 0;
        QLineEdit* active_cell = nullptr;
    };

    LineView _line_view;
    std::shared_ptr<ActivePositionChangeHandler> active_position_change_hanler_;

    Ui::FormLineWidget* ui;
    std::array<QLineEdit, 64> _line_edit;
    QHBoxLayout _layout;
    std::shared_ptr<VMTLine> _line;
   private slots:
    void onEditChanged(QString text);
};

#endif  // FORMLINEWIDGET_H
