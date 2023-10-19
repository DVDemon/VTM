

#include "VMTAlphabit.h"
#include <QDebug>

VMTAlphabit::~VMTAlphabit(){
 qDebug() << "VMTAlphabit::~VMTAlphabit";
}

VMTAlphabit::VMTAlphabit(){

}

bool VMTAlphabit::operator !=(const VMTAlphabit &other){
 if(_signs.length()!=other._signs.length()) return false;
 else
 {
     for(int i=0;i<_signs.length();i++){
         bool found = false;
         for(int j=0;(j<other._signs.length())&&(!found);j++){
             if(_signs[i]==other._signs[j]) found=true;
         }
         if(!found) return false;
     }

     return true;
 }
}

VMTAlphabit::VMTAlphabit(const VMTAlphabit& other){
 _signs = other._signs;
}

void VMTAlphabit::ReplaceString(const QString& other){
 _signs = other;
}

void VMTAlphabit::AddSign(char sign){
 _signs.append(QChar(sign));
}


char VMTAlphabit::GetLambda(){
    if(_signs.size()>0) return _signs[0].toLatin1();
	return 0;
}


char VMTAlphabit::GetSignAt(int index){
    if(index>=0){
        if(index<_signs.size())
            return _signs[index].toLatin1();
    }
	return 0;
}


int VMTAlphabit::GetSignsCount(){

    return _signs.size();
}

#include <set>
bool VMTAlphabit::IsEqual(const VMTAlphabit &other){

    if(GetSignsCount()!=other._signs.size()) return false;
    std::set<QChar> other_set;
    for(int i=0;i<other._signs.size();i++) other_set.insert(other._signs[i]);

    for(int i=0;i<_signs.size();i++)
        if(other_set.find(_signs[i])==other_set.end()) return false;

    return true;
}


bool VMTAlphabit::IsLambda(char sign){

    return (sign==GetLambda());
}


bool VMTAlphabit::IsSign(char sign){
    for(int i=0;i<_signs.size();i++){
        if(_signs[i].toLatin1()==sign) return true;
    }
	return false;
}



const QString& VMTAlphabit::ToString(){
    return  _signs;
}
