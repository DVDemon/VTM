#include "exercise.h"


Exercise::Exercise() : _index(0),_completed(false),_alphabit("")
{

}

Exercise::Exercise(const Exercise& other){
   _alphabit = other._alphabit;
   _completed = other._completed;
   _index = other._index;
   _tests = other._tests;
   _text_en = other._text_en;
   _text_ru = other._text_ru;
}

void Exercise::Clear(){
    _tests.clear();
    _text_ru = "";
    _text_en = "";
    _index   = 0;
}

const QString & Exercise::GetAlphabit(){
    return _alphabit;
}

void Exercise::SetAlphabit(const QString &alphabit){
    _alphabit = alphabit;
}

bool Exercise::GetCompleted(){
    return _completed;
}

void Exercise::SetCompleted(bool completed){
    _completed = completed;
}

int Exercise::GetIndex(){
    return _index;
}

const QString & Exercise::GetTextRu(){
    return _text_ru;
}
const QString & Exercise::GetTextEn(){
    return _text_en;
}
const std::vector<std::pair<QString,QString>>& Exercise::GetTests(){
    return _tests;
}

void Exercise::SetIndex(int index){
    _index = index;
}

void Exercise::SetTextRu(const QString& text){
    _text_ru = text;
}

void Exercise::SetTextEn(const QString& text){
    _text_en = text;
}

void Exercise::AddTest(const QString& start,const QString& finish){
    _tests.push_back(std::pair<QString,QString>(start,finish));
}

QDebug operator<<(QDebug os, Exercise& ex){
    os << "Exercise:" << ex.GetIndex() << " ," << ex.GetTextEn() << " ," << ex.GetTextRu() ;
    for(const auto &p : ex.GetTests()){
        os << "Test: " << p.first <<" ," << p.second;
    }

    return os;
}
