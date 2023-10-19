#include "VMTLine.h"

VMTLine::VMTLine(std::shared_ptr<VMTAlphabit> alphabit) : _alphabit(alphabit),_machine(0),_zero(0){

}



void VMTLine::CheckPosition(long index){
    long array_index = (index-_zero)/SHIFT_STEP;
    if(index<_zero){
      long new_zero = (index-SHIFT_STEP)/SHIFT_STEP*SHIFT_STEP;
      long add = (_zero-new_zero)/SHIFT_STEP;
      for(int i=0;i<add;i++){
          char *ptr =new char[SHIFT_STEP];
          for(int j=0;j<SHIFT_STEP;j++) ptr[j] = _alphabit->GetLambda();
          _array.insert(_array.begin(),ptr);
      }
      _zero = new_zero;

    } else
    if(array_index>=(long)_array.size()){
        long to_add = array_index-_array.size()+1;
        for(int i=0;i<to_add;i++){
            char *ptr =new char[SHIFT_STEP];
            for(int j=0;j<SHIFT_STEP;j++) ptr[j] = _alphabit->GetLambda();
            _array.push_back(ptr);
        }
    }

}

void VMTLine::ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit){

}

std::shared_ptr<VMTUndoElement>  VMTLine::CopyCurrentWord(){
  return std::shared_ptr<VMTUndoElement> ();
}

std::shared_ptr<VMTAlphabit> VMTLine::GetAlphabit(){
 return _alphabit;
}

long VMTLine::GetMachinePosition(){

 return _machine;
}

long VMTLine::GetRightSignPosition(){
    for(int i=_array.size()-1; i>=0;i--){
        for(int j=SHIFT_STEP-1;j>=0;j--){
            if(_array[i][j]!=_alphabit->GetLambda())
                return i*SHIFT_STEP+j+_zero;
        }
    }
    return -1;
}

long VMTLine::GetLeftSignPosition(){
 for(size_t i=0; i< _array.size();i++){
     for(size_t j=0;j<SHIFT_STEP;j++){
         if(_array[i][j]!=_alphabit->GetLambda())
             return i*SHIFT_STEP+j+_zero;
     }
 }
 return -1;
}

IVMTLineObserver* VMTLine::GetObserver(){
 return nullptr;
}

char VMTLine::GetValueAt(long index){
    CheckPosition(index);
    long array_index = (index-_zero)/SHIFT_STEP;
    long inside_index = (index-_zero)%SHIFT_STEP;

    return _array[array_index][inside_index];
}

char VMTLine::GetValueAtMachine(){
 return this->GetValueAt(_machine);
}

bool VMTLine::IsEqualAlphabit([[maybe_unused]] VMTAlphabit* alphabit){
 return false;
}

bool VMTLine::IsMachinePosition(long index){
 return _machine==index;
}

std::shared_ptr<VMTUndoElement> VMTLine::SetMachinePosition(long index){
 _machine = index;
  return std::shared_ptr<VMTUndoElement> ();
}

void VMTLine::SetObserver([[maybe_unused]] IVMTLineObserver* observer){

}

std::shared_ptr<VMTUndoElement> VMTLine::SetSignAtMachine(char sign){
 return SetValueAt(_machine,sign);
}

std::shared_ptr<VMTUndoElement> VMTLine::SetValueAt(long index, char sign){
    CheckPosition(index);
    long array_index = (index-_zero)/SHIFT_STEP;
    long inside_index = (index-_zero)%SHIFT_STEP;

    _array[array_index][inside_index] = sign;
    return std::shared_ptr<VMTUndoElement> ();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftLeft(){
 _machine--;
 return std::shared_ptr<VMTUndoElement> ();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftLeftWord(){
 do{
        _machine --;
    }while(GetValueAtMachine()!=_alphabit->GetLambda());
    return std::shared_ptr<VMTUndoElement> ();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftRight(){
 _machine ++;
 return std::shared_ptr<VMTUndoElement> ();
}

std::shared_ptr<VMTUndoElement> VMTLine::ShiftRightWord(){
    do{
           _machine ++;
       }while(GetValueAtMachine()!=_alphabit->GetLambda());
    return std::shared_ptr<VMTUndoElement> ();
}

void VMTLine::Clear(){
    for(size_t i=0;i<this->_array.size();i++)
        for(size_t j=0;j<SHIFT_STEP;j++)
          _array[i][j]=_alphabit->GetLambda();
}

QString VMTLine::ToString(){
 QString result;
 for(size_t i=0;i<this->_array.size();i++)
     for(size_t j=0;j<SHIFT_STEP;j++)
       result.append(QChar(_array[i][j]));

 return result;
}


VMTLine::~VMTLine(){

}
