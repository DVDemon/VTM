#include "formlinewidget.h"

#include <qlineedit.h>
#include <qlogging.h>

#include <memory>
#include <stdexcept>

#include "VMTAlphabit.h"
#include "ui_formlinewidget.h"
#include "utils/events.h"
#include "vmttheme.h"

class FormLineWidget::ActivePositionChangeHandler
    : public utils::IEventHandler<size_t> {
    FormLineWidget& widget;

   public:
    ActivePositionChangeHandler(FormLineWidget& root) : widget(root) {};
    void Handle(size_t&& new_index) override {
        while (new_index < widget._line_view.offset) {
            widget.LeftPage();
        }
        while (widget._line_view.offset + widget._line_view.width < new_index) {
            widget.RightPage();
        }

        widget.SyncActivePositionView();
    }
};

FormLineWidget::FormLineWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::FormLineWidget) {
    ui->setupUi(this);
    ui->frame->setLayout(&_layout);

    for (int i = 0; i < _line_edit.size(); i++) {
        QLineEdit& line_edit_cell = _line_edit[i];
        line_edit_cell.setHidden(true);
        line_edit_cell.setSizePolicy(QSizePolicy::Minimum,
                                     QSizePolicy::Expanding);
        QObject::connect(&_line_edit[i], SIGNAL(textChanged(QString)), this,
                         SLOT(onEditChanged(QString)));

        line_edit_cell.installEventFilter(this);
        ui->frame->layout()->addWidget(&line_edit_cell);
    }
    _line_view.active_cell = &_line_edit[0];
    applyTheme();
}

void FormLineWidget::applyTheme() {
    ui->frame->setStyleSheet(
        QStringLiteral("QFrame { background-color: %1; border: none; }")
            .arg(VmtTheme::colorName(VmtTheme::canvasWorkspace())));
    if (_line) {
        SyncLineView();
    }
}

void FormLineWidget::SetTestMode() {
    for (int i = 0; i < 64; i++) {
        _line_edit[i].setEnabled(false);
    }
}
void FormLineWidget::SetLine(std::shared_ptr<VMTLine> line) {
    active_position_change_hanler_ =
        std::make_shared<ActivePositionChangeHandler>(*this);
    _line = line;
    _line->onNewActivePositionEvent += active_position_change_hanler_;
}

void FormLineWidget::Left() {
    if (_line_view.offset > 0) {
        _line_view.offset -= 1;
    } else {
        _line_view.offset = 0;
    }
    SyncLineView();
    SyncActivePositionView();
}

void FormLineWidget::LeftPage() {
    if (_line_view.offset > _line_view.width / 2) {
        _line_view.offset -= _line_view.width / 2;
    } else {
        _line_view.offset = 0;
    }
    SyncLineView();
    SyncActivePositionView();
}

void FormLineWidget::Right() {
    _line_view.offset += 1;
    SyncLineView();
    SyncActivePositionView();
}

void FormLineWidget::RightPage() {
    _line_view.offset += _line_view.width / 2;
    SyncLineView();
    SyncActivePositionView();
}

void FormLineWidget::SyncActivePositionView() {
    if (_line_view.active_cell) {
        _line_view.active_cell->setStyleSheet(
            VmtTheme::lineEditTapeStyle(/*machineHead=*/false));
    }

    if (_line->GetActivePosition() < _line_view.offset ||
        _line->GetActivePosition() - _line_view.offset >= _line_view.width) {
        return;
    }
    size_t active_cell_index = _line->GetActivePosition() - _line_view.offset;
    _line_view.active_cell = &_line_edit.at(active_cell_index);

    _line_view.active_cell->setStyleSheet(
        VmtTheme::lineEditTapeStyle(/*machineHead=*/true));
}

void FormLineWidget::SyncLineView() {
    for (size_t i = 0; i < _line_view.width; ++i) {
        _line_edit[i].setText(
            QString(_line->GetValueAt(_line_view.offset + i)));
    }
    SyncActivePositionView();
}

bool FormLineWidget::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        for (size_t i = 0; i < _line_view.width; ++i) {
            if (&_line_edit[i] == object) {
                _line->SetActivePosition(_line_view.offset + i);
                break;
            }
        }
        SyncActivePositionView();
        return false;
    }
    return false;
}

void FormLineWidget::onEditChanged(QString text) {
    QLineEdit* line_cell = dynamic_cast<QLineEdit*>(sender());
    if (line_cell) {
        int index = line_cell - &_line_edit[0];
        QChar newLetter = _line->GetAlphabit()->GetLambda();
        if (text.size() > 0) {
            newLetter = text.back();
        }
        line_cell->setText(newLetter);
        _line->SetValueAt(_line_view.offset + index, newLetter);
    }
}

void FormLineWidget::resizeEvent(QResizeEvent* event) {
    size_t width = event->size().width() / event->size().height();

    if (width != _line_view.width) {
        _line_view.width = width;
        for (size_t i = 0; i < _line_edit.size(); i++)
            if (i >= _line_view.width)
                ui->frame->layout()->itemAt(i)->widget()->setHidden(true);
            else
                ui->frame->layout()->itemAt(i)->widget()->setHidden(false);

        ui->frame->layout()->update();
    }
}

FormLineWidget::~FormLineWidget() {
    if (_line && active_position_change_hanler_) {
        _line->onNewActivePositionEvent -= active_position_change_hanler_;
    }
    delete ui;
}
