#include "screentools.h"
#include <QApplication>
#include <QScreen>
#include <QPicture>

#define MM2INCH 0.0393700787401575
#define INCH2MM 25.4

ScreenTools::ScreenTools()
{
    mDpi = QApplication::primaryScreen()->physicalDotsPerInch();
    int pxls =cvtMm2Px(6);
    size.setWidth(pxls);
    size.setHeight(pxls);

    pxls =cvtMm2Px(10);
    size2.setWidth(pxls);
    size2.setHeight(pxls);

    _font_size = cvtMm2Px(3);
    pxls =cvtMm2Px(4);
    net.setWidth(pxls);
    net.setHeight(pxls);
}

size_t       ScreenTools::GetFontSize(){
    return _font_size;
}
const QSize& ScreenTools::GetCellSize(){
    return net;
}

const QSize& ScreenTools::GetBigButtonSize(){
    return size2;
}
int ScreenTools::cvtMm2Px( qreal mm )
{
    qreal inch = mm*MM2INCH;

    return cvtInch2Px( inch );
}

int ScreenTools::cvtInch2Px( qreal in )
{
    qreal px = in * mDpi;

    return (int)(px+0.5);
}

void ScreenTools::ResizeButtonBig(QToolButton *btn){
    btn->setIconSize(size2);
    btn->setMaximumHeight(size2.height());
}
/*
void ScreenTools::ResizeLabel(QLabel *label){

 //   label->setMaximumWidth(size.width()/4);
 //   label->setMaximumHeight(size.height()/4);
}
*/
void ScreenTools::ResizeButton(QToolButton *btn){
    btn->setIconSize(size);
    btn->setMaximumHeight(size.height());
}
void ScreenTools::ResizeButton(QPushButton *btn){
    btn->setIconSize(size);
    btn->setMaximumHeight(size.height());
}
/*
void ScreenTools::ResizeSlider(QSlider *sld){
  sld->setMinimumSize(size);
}

void ScreenTools::ResizeFrame(QFrame *frame){
 frame->setMinimumSize(size);
}
*/
void ScreenTools::ResizePropertyPanel(QFrame *frame){
 int pxls =cvtMm2Px(6);
 frame->setMinimumSize(QSize(pxls,pxls));
}

const QSize& ScreenTools::GetImageSize(){

    return size;
}
