#include "clickablelabel.h"
#include "QDebug"

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
   // connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
}

ClickableLabel::~ClickableLabel()
{
}


void ClickableLabel::mousePressEvent([[maybe_unused]] QMouseEvent* event)
{
    emit clicked();
}
