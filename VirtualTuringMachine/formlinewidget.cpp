#include "formlinewidget.h"
#include "ui_formlinewidget.h"
#include "vmtproject.h"


FormLineWidget::FormLineWidget(QWidget *parent) :
    _count(0),_position(0),
    QWidget(parent),
    ui(new Ui::FormLineWidget)
{
    ui->setupUi(this);
    ui->frame->setLayout(&_layout);

    for(int i=0;i<64;i++){
        _line_edit[i].setHidden(true);
        _line_edit[i].setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
        _line_edit[i].setProperty("index",i);
        QObject::connect(&_line_edit[i], SIGNAL(textChanged(QString)),
                          this, SLOT(onEditChanged(QString)));
        _line_edit[i].installEventFilter(this);
        ui->frame->layout()->addWidget(&_line_edit[i]);
    }
}

void FormLineWidget::SetTestMode(){
    for(int i=0;i<64;i++){
        _line_edit[i].setEnabled(false);
    }
}
void FormLineWidget::SetLine(std::shared_ptr<VMTLine> line){
    _line = line;
}

void FormLineWidget::Left(){
    isShiftFromType = false;

    _position--;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::LeftPage(){
    isShiftFromType = false;

    _position-=_count;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::Right(){
    isShiftFromType = false;

    _position++;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::RightPage(){
    isShiftFromType = false;

    _position+=_count;
    FillLineEdit();

    isShiftFromType = true;
}

void FormLineWidget::Repaint(){
    long right = _position+((long)_count)+1;
    if(_line->GetMachinePosition()<_position){
        _position = _line->GetMachinePosition();
    } else if(_line->GetMachinePosition()>=right)
    {
      _position = _line->GetMachinePosition()-_count;
    }

    FillLineEdit();
}

void  FormLineWidget::FillLineEdit(){
 for(long i=0;i<=(long)_count;i++){
     _line_edit[i].setText(QString(QChar(_line->GetValueAt(_position+i))));
     if(_line->IsMachinePosition(_position+i)) _line_edit[i].setStyleSheet("QLineEdit { background: #01547a; selection-background-color:#01547a; color:#ffffff }");
                                    else _line_edit[i].setStyleSheet("");
 }
}

bool FormLineWidget::eventFilter(QObject* object, QEvent* event){
    if(event->type() == QEvent::MouseButtonRelease) {
        int index = -1;
        for(int i=0;i<64;i++)
            if(&_line_edit[i]==object) index=i;
        if(index>0){
            _line->SetMachinePosition(_position+index);
            //VMTProject::GetInstance().GetUndoManager()->Remember(_line->SetMachinePosition(_position+index));

            for(long i=0;i<=(long)_count;i++){
                if(_line->IsMachinePosition(_position+i)) _line_edit[i].setStyleSheet("QLineEdit { background: #01547a; selection-background-color:#01547a; color:#ffffff }");
                                               else _line_edit[i].setStyleSheet("");
            }
        }
            return false; // lets the event continue to the edit
        }
    return false;
}

void FormLineWidget::onEditChanged(QString text){
    QLineEdit *line = dynamic_cast<QLineEdit*> (sender());
    if(line){
        QVariant var = line->property("index");
        int  index = var.toInt();

        if(text.length()>1){
            QString old(QChar(_line->GetValueAt(index+_position)));
            if(text.indexOf(old)==0){

              if(_line->GetAlphabit()->IsSign(text[1].toLatin1()))
                    _line_edit[index].setText(QString(text[1]));
               else _line_edit[index].setText(QString(QChar(_line->GetValueAt(_position+index)))); // else set old i think

            } else {
                if(_line->GetAlphabit()->IsSign(text[0].toLatin1()))
                      _line_edit[index].setText(QString(text[0]));
                 else _line_edit[index].setText(QString(QChar(_line->GetValueAt(_position+index)))); // else set old i think
            }
        } else
        if(text.length()==0)
        {
            if(_line->GetAlphabit())
               _line_edit[index].setText(QString(QChar(_line->GetAlphabit()->GetLambda())));
        } else{
          if(_line->GetAlphabit()->IsSign(text[0].toLatin1()))
              _line->SetValueAt(_position+index,text[0].toLatin1());
              //VMTProject::GetInstance().GetUndoManager()->Remember(_line->SetValueAt(_position+index,text[0].toLatin1()));
           else _line_edit[index].setText(QString(QChar(_line->GetValueAt(_position+index))));

           std::shared_ptr<VMTAlphabit> alphabit = _line->GetAlphabit();
           if(
               alphabit->IsSign(text[0].toLatin1()) and
               not(alphabit->IsLambda(text[0].toLatin1())) and
               isShiftFromType and
               _line_edit[index].hasFocus()
            ) {
               qDebug() << "shifting right, text = " << text;
               RightShiftActiveLineEdit(index);
           }
        }
    }

}
void FormLineWidget::RightShiftActiveLineEdit(int currentIndex) {
    _line_edit[currentIndex+1].setFocus();
    _line->ShiftRight();
    Repaint();
}

void FormLineWidget::resizeEvent(QResizeEvent * event){
    size_t count = event->size().width()/event->size().height();

    if(count!=_count){
        _count = count;
        for(long i=0;i<64;i++)
        if(i>(long)_count) ui->frame->layout()->itemAt(i)->widget()->setHidden(true);
                else ui->frame->layout()->itemAt(i)->widget()->setHidden(false);

        ui->frame->layout()->update();
    }
}

FormLineWidget::~FormLineWidget()
{
    delete ui;
}
