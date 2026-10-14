#include "VMTLine.h"

#include <cstddef>

VMTLine::VMTLine(std::shared_ptr<VMTAlphabit> alphabit)
    : alphabit_(alphabit), cursor_position_(0) {}

std::shared_ptr<VMTAlphabit> VMTLine::GetAlphabit() { return alphabit_; }

char VMTLine::GetValueAt(long index) {
    // TODO
}

bool VMTLine::IsMachinePosition(long index) {
    return cursor_position_ == index;
}

std::shared_ptr<VMTUndoElement> VMTLine::SetMachinePosition(long index) {
    cursor_position_ = index;
    return std::shared_ptr<VMTUndoElement>();
}

std::shared_ptr<VMTUndoElement> VMTLine::SetSignAtMachine(char sign) {
    return SetValueAt(cursor_position_, sign);
}

std::shared_ptr<VMTUndoElement> VMTLine::SetValueAt(long index, char sign) {
    // TODO
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftLeft() {
    // TODO
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftRight() {
    // TODO
}

void VMTLine::Clear() {
    // TODO
}

QString VMTLine::ToString() {
    // TODO
}

VMTLine::~VMTLine() {}
