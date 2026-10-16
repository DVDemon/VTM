#include "VMTLine.h"

#include <format>
#include <memory>
#include <stdexcept>

#include "VMTAlphabit.h"

VMTLine::VMTLine(std::shared_ptr<VMTAlphabit> alphabit)
    : alphabit_(alphabit),
      active_position_(0),
      onNewActivePositionEvent(onNewActivePositionEvent_) {}

std::shared_ptr<const VMTAlphabit> VMTLine::GetAlphabit() const {
    return alphabit_;
}

QChar VMTLine::GetValueAt(size_t index) const {
    if (index >= array_.size()) {
        return alphabit_->GetLambda();
    }
    return array_.at(index);
}

size_t VMTLine::GetActivePosition() const { return active_position_; }
QChar VMTLine::GetValueAtActivePosition() const {
    return GetValueAt(GetActivePosition());
}

bool VMTLine::IsActivePosition(size_t index) const {
    return active_position_ == index;
}

std::shared_ptr<VMTUndoElement> VMTLine::SetActivePosition(size_t index) {
    active_position_ = index;
    onNewActivePositionEvent_(std::forward<size_t>(index));
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::SetSignAtActivePosition(QChar sign) {
    return SetValueAt(active_position_, sign);
}

std::shared_ptr<VMTUndoElement> VMTLine::SetValueAt(size_t index, QChar sign) {
    if (!alphabit_->Contains(sign)) {
        // TODO(Tias) add custom exception for sign not in alphabit
        throw std::logic_error("Trying set sign not from alphabit with code: " +
                               std::to_string(sign.unicode()));
    }
    constexpr size_t max_meaningfull_line_length = 10000;
    if (index >= max_meaningfull_line_length) {
        throw std::logic_error(std::format(
            "Trying to set sign at position: {}, which is bigger than maxinum "
            "meaningful line length: {}. There might be overflow in integer",
            index, max_meaningfull_line_length));
    }
    while (index >= array_.size()) {
        array_.push_back(GetAlphabit()->GetLambda());
    }
    array_.at(index) = sign;
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftLeft() {
    if (active_position_ == 0) {
        throw std::out_of_range("Trying to go out of line's left border");
    }

    return SetActivePosition(active_position_ - 1);
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftRight() {
    while (active_position_ + 1 >= array_.size()) {
        array_.push_back(alphabit_->GetLambda());
    }
    return SetActivePosition(active_position_ + 1);
}

void VMTLine::Clear() {
    array_.clear();
    active_position_ = 0;
}

QString VMTLine::ToString() const { return QString{array_.data()}; }

VMTLine::~VMTLine() {}
