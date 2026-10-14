#include "VMTLine.h"

#include <memory>
#include <stdexcept>

#include "VMTAlphabit.h"

VMTLine::VMTLine(std::shared_ptr<VMTAlphabit> alphabit)
    : alphabit_(alphabit), active_position_(0) {}

std::shared_ptr<const VMTAlphabit> VMTLine::GetAlphabit() const {
    return alphabit_;
}

QChar VMTLine::GetValueAt(long index) const {
    if (index > array_.size()) {
        return alphabit_->GetLambda();
    }
    return array_.at(index);
}

QChar VMTLine::GetValueAtActivePosition() const {
    return GetValueAt(GetActivePosition());
}

bool VMTLine::IsActivePosition(long index) const {
    return active_position_ == index;
}

std::shared_ptr<VMTUndoElement> VMTLine::SetActivePosition(long index) {
    active_position_ = index;
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::SetSignAtActivePosition(QChar sign) {
    return SetValueAt(active_position_, sign);
}

std::shared_ptr<VMTUndoElement> VMTLine::SetValueAt(long index, QChar sign) {
    if (!alphabit_->Contains(sign)) {
        // TODO(Tias) add custom exception for sign not in alphabit
        throw std::logic_error("Trying set sign not from alphabit with code: " +
                               std::to_string(sign.unicode()));
    }
    array_.at(index) = sign;
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftLeft() {
    if (active_position_ == 0) {
        throw std::out_of_range("Trying to go out of line's left border");
    }
    --active_position_;
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftRight() {
    active_position_++;
    while (active_position_ >= array_.size()) {
        array_.push_back(alphabit_->GetLambda());
    }
    return std::shared_ptr<VMTUndoElement>();
}

void VMTLine::Clear() {
    array_.clear();
    active_position_ = 0;
}

QString VMTLine::ToString() const { return QString{array_.data()}; }

VMTLine::~VMTLine() {}
