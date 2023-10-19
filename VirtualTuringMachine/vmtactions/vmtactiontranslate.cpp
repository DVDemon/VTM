#include "vmtactiontranslate.h"

VMTActionTranslate::VMTActionTranslate(IVMTActionController *controller) :
_controller(controller), _position(0,0),_is_shift(false),_mouse(0,0),_shift(0,0)
{

}
void VMTActionTranslate::Cancel(IVMTEnvironment *environment){
  environment->DisableActionHint();
}

void VMTActionTranslate::Enable(IVMTEnvironment *environment){
    static QPixmap img(":/Files/images/toolbars/tools/icon_hand_black.png");
    environment->EnableActionHint("Drag editor field in any direction. Double click on the Hand icon - center the editor field.",img);
}

bool VMTActionTranslate::OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real){
 static      QRect empty_rect;
 _position = real;

 if(_is_shift){
     environment->MoveInScreen(QPoint(-_shift));
     _shift = screen-_mouse;
     environment->MoveInScreen(QPoint(_shift));
     environment->Repaint(empty_rect);
 }

 return true;
}

#include "vmtundoelementeditor.h"
#include "vmtproject.h"

bool VMTActionTranslate::OnMousePressed([[maybe_unused]] IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,[[maybe_unused]] const QPoint &real){
 _is_shift = true;
 _mouse=screen;
 _shift = QPoint(0,0);
 VMTProject::GetInstance().GetUndoManager()->Remember(
             std::shared_ptr<VMTUndoElementEditor>(new VMTUndoElementEditor(environment)));
 return true;
}

bool VMTActionTranslate::OnMouseReleased([[maybe_unused]] IVMTEnvironment* environment,[[maybe_unused]] const QPoint &screen,[[maybe_unused]]const QPoint &real){
 _is_shift = false;
 return true;
}

void VMTActionTranslate::Paint(UICanvas& , const QRect &){

    //QRect action_rect(_position.rx(),_position.ry(),10,10);
    //canvas.DrawRect(action_rect);
}
