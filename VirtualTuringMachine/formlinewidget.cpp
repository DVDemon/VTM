#include "formlinewidget.h"

#include <stdexcept>

#include "ui_formlinewidget.h"
#include "vmttheme.h"

FormLineWidget::FormLineWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::FormLineWidget) {
    ui->setupUi(this);
    ui->frame->setLayout(&_layout);

    for (int i = 0; i < _line_edit.size(); i++) {
        _line_edit[i].setHidden(true);
        _line_edit[i].setSizePolicy(QSizePolicy::Minimum,
                                    QSizePolicy::Expanding);
        _line_edit[i].setProperty("index", i);
        QObject::connect(&_line_edit[i], SIGNAL(textChanged(QString)), this,
                         SLOT(onEditChanged(QString)));
        _line_edit[i].installEventFilter(this);
        ui->frame->layout()->addWidget(&_line_edit[i]);
    }
    applyTheme();
}

void FormLineWidget::applyTheme() {
    ui->frame->setStyleSheet(
        QStringLiteral("QFrame { background-color: %1; border: none; }")
            .arg(VmtTheme::colorName(VmtTheme::canvasWorkspace())));
    if (_line) {
        FillLineEdit();
    }
}

void FormLineWidget::SetTestMode() {
    for (int i = 0; i < 64; i++) {
        _line_edit[i].setEnabled(false);
    }
}
void FormLineWidget::SetLine(std::shared_ptr<VMTLine> line) { _line = line; }

void FormLineWidget::Left() {
    isShiftFromType = false;

    if (_line_view.offset == 0 && _line_view.active_position == 0) {
        // TODO(Tias) add custom exception for shifting over left line border
        throw std::logic_error("Trying to shift over left line's border");
    }
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::LeftPage() {
    isShiftFromType = false;

    _position -= _count;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::Right() {
    isShiftFromType = false;

    _position++;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::RightPage() {
    isShiftFromType = false;

    _position += _count;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::Repaint() {
    long right = _position + ((long)_count) + 1;
    if (_line->GetActivePosition() < _position) {
        _position = _line->GetActivePosition();
    } else if (_line->GetActivePosition() >= right) {
        _position = _line->GetActivePosition() - _count;
    }

    FillLineEdit();
}

void FormLineWidget::FillLineEdit() {
    for (long i = 0; i <= (long)_count; i++) {
        _line_edit[i].setText(QString(QChar(_line->GetValueAt(_position + i))));
        _line_edit[i].setStyleSheet(VmtTheme::lineEditTapeStyle(
            _line->IsActivePosition(_position + i)));
    }
}

bool FormLineWidget::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        int index = -1;
        for (int i = 0; i < 64; i++)
            if (&_line_edit[i] == object) index = i;
        if (index > 0) {
            _line->SetActivePosition(_position + index);

            for (long i = 0; i <= (long)_count; i++) {
                _line_edit[i].setStyleSheet(VmtTheme::lineEditTapeStyle(
                    _line->IsActivePosition(_position + i)));
            }
        }
        return false;
    }
    return false;
}

void FormLineWidget::onEditChanged(QString text) {
    QLineEdit* line = dynamic_cast<QLineEdit*>(sender());
    if (line) {
        QVariant var = line->property("index");
        int index = var.toInt();
        // TODO(Tias) improve cell editing logic
    }
}
void FormLineWidget::RightShiftActiveLineEdit(int currentIndex) {
    _line_edit[currentIndex + 1].setFocus();
    _line->ShiftRight();
    Repaint();
}

void FormLineWidget::resizeEvent(QResizeEvent* event) {
    size_t count = event->size().width() / event->size().height();

    if (count != _count) {
        _count = count;
        for (long i = 0; i < 64; i++)
            if (i > (long)_count)
                ui->frame->layout()->itemAt(i)->widget()->setHidden(true);
            else
                ui->frame->layout()->itemAt(i)->widget()->setHidden(false);

        ui->frame->layout()->update();
    }
}

FormLineWidget::~FormLineWidget() { delete ui; }
