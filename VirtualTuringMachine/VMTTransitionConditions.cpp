#include "VMTTransitionConditions.h"
#include <QDebug>
#include "interfaces/IVMTTransition.h"

VMTTransitionConditions::VMTTransitionConditions(IVMTTransition *transition,std::shared_ptr<VMTAlphabit> alphabit) :
    _transition(transition),_alphabit(alphabit){
 _signs = _alphabit->ToString();
}

VMTTransitionConditions& VMTTransitionConditions::operator=(const VMTTransitionConditions& other){
 _signs = other._signs;
 _transition = other._transition;
 _alphabit = other._alphabit;
 return *this;
}
QPixmap* VMTTransitionConditions::GetImage(){
    static QPixmap image(":/Files/images/toolbars/tools/icon_link.gif");
    return &image;

}

const QString& VMTTransitionConditions::GetMessage(){
    static QString msg("Select sign allowed for selected transition");
    return msg;
}


VMTTransitionConditions::VMTTransitionConditions(const VMTTransitionConditions& other) : _alphabit(other._alphabit){
    this->_signs    = other._signs;
    this->_transition = other._transition;
}


void VMTTransitionConditions::Update(IVMTEnvironment *environment){
  _transition->Changed(environment);
}

void VMTTransitionConditions::Deserialize(QDataStream& stream){
    short int length;
    stream >> length;
    if(length>0)
    {
        char * array = new char[length+1];
        memset(array,0,length+1);
        stream.readRawData(array,length);
        _signs.resize(length);
        for(int i=0;i<length;i++) _signs[i] = QChar(array[i]);
        delete [] array;
    } else _signs="";
}

void VMTTransitionConditions::Serialize(QDataStream& stream){
    stream << (short int)_signs.length();
    const char* array =_signs.toStdString().c_str();
    stream.writeRawData(array,_signs.length());
}

void VMTTransitionConditions::ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other){

     if(IsAllEnabled())
     {
         _alphabit = other;
         _signs = _alphabit->ToString();
     } else
     {
         _alphabit = other;
         QString new_signs;
         for(int i=0;i<_signs.length();i++)
         {
             if(_alphabit->IsSign(_signs[i].toLatin1())) new_signs += _signs[i];
         }
         _signs = new_signs;
         qDebug() << "VMTTransitionConditions::ChangeAlphabit:" << _signs;
     }

}



const QString& VMTTransitionConditions::GetAsString(){
 return this->_signs;
}

QPoint  VMTTransitionConditions::GetDimensions([[maybe_unused]] IVMTEnvironment* environment, [[maybe_unused]] int size){
 return QPoint();
}

bool VMTTransitionConditions::IsAllDisabled(){
    QString alphabit_string = _alphabit->ToString();
    return alphabit_string.size()!=_signs.size();
}

bool VMTTransitionConditions::IsAllEnabled(){
 QString alphabit_string = _alphabit->ToString();
 return alphabit_string.size()==_signs.size();
}


std::shared_ptr<VMTAlphabit> VMTTransitionConditions::GetAlphabit(){
  return _alphabit;
}

bool VMTTransitionConditions::IsSingleChar(){
 return false;
}

bool VMTTransitionConditions::IsEnabled(char sign){
 return _signs.indexOf(QChar(sign))!=-1;
}

void VMTTransitionConditions::SetEnable(char sign, bool enable,IVMTEnvironment* environment){
 if(enable) {
     if(_signs.indexOf(QChar(sign))==-1)
      _signs.append(QChar(sign));
 } else
 {
     int index=_signs.indexOf(QChar(sign));

     if(index!=-1) _signs.remove(index,1);
 }
 Update(environment);
}

VMTTransitionConditions::~VMTTransitionConditions(){

}
