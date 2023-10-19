#include "vmtexport4th.h"
#include "vmtmachines/VMTComplexMachine.h"

#include <QDebug>
#include <functional>
#include <string>
#include <QFile>
#include <QTextStream>

#include <sstream>

namespace std {
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

}

#include "vmtmachines/VMTMachineAlpha.h"

VMTExport4th::VMTExport4th(std::shared_ptr<VMTComplexMachine> machine,
                           QString delimiter, QString left, QString right, QString stop, QString quotes) :
    _machine(machine),
    _progress(0),
    _error(false),
    _machine_count(0),
    _delimiter(delimiter),
    _left(left),
    _right(right),
    _stop(stop),
    _quotes(quotes),
    _callback(nullptr){
}

bool            VMTExport4th::IsError(){
    std::lock_guard<std::mutex> lock(_mtx);
    return _error;
}

const QString  VMTExport4th::GetErrorMessage(){
    std::lock_guard<std::mutex> lock(_mtx);
    return _error_message;
}

const QString   VMTExport4th::GetInformation(){
    std::lock_guard<std::mutex> lock(_mtx);
    return _information;
}

size_t          VMTExport4th::GetMachineCount(){
    std::lock_guard<std::mutex> lock(_mtx);
    return _machine_count;
}

void VMTExport4th::Export(TextCallback *callback,IVMTEnvironment *environment){
    if(IsRuning()) return;
    _callback       = callback;
    _can_run        = true;
    _progress       = 0;
    _error          = false;
    _error_message  = "";
    _information    = "";
    _machine_count  = 0;

    _thread = std::shared_ptr<std::thread>(new std::thread(std::ref(*this),environment));

}

size_t  VMTExport4th::GetProgress(){
    std::lock_guard<std::mutex> lock(_mtx);
    return _progress;
}

bool    VMTExport4th::IsRuning(){
    std::lock_guard<std::mutex> lock(_mtx);
    if(_thread) return true;
    return false;
}

void    VMTExport4th::Stop(){
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _can_run = false;
    }

    while(IsRuning()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

void   VMTExport4th::AddInformation(QString && str){
    std::lock_guard<std::mutex> lock(_mtx);
    _information += str;
    if(_callback)    _callback->Information(_information);
}

template <class FUNCTION_COMPLEX, class FUNCTION_SIMPLE>
void MachineFunction(std::shared_ptr<VMTComplexMachine> complex, FUNCTION_COMPLEX fc,FUNCTION_SIMPLE fs){
    for(auto m : complex->GetMachineCollection()){
        if(m->GetID() == IVMTMachine::MachineType::MT_COMPLEX) {
            if(!fc(complex,std::dynamic_pointer_cast<VMTComplexMachine>(m))) return;
            MachineFunction<FUNCTION_COMPLEX,FUNCTION_SIMPLE>(std::dynamic_pointer_cast<VMTComplexMachine>(m),fc,fs);
        } else if(!fs(complex,m)) return;
    }
}



QString    VMTExport4th::FormatString(size_t state,QChar c,Command cmd,size_t state2){
    QString cmd_sz;

    switch(cmd){
    case left: cmd_sz = _left;break;
    case right: cmd_sz= _right;break;
    case stop: cmd_sz = _stop;break;
    }

    return QString("%1%2%3%4%5%6%7%8%9").arg(
                QString::number(state),
                _delimiter,
                _quotes,
                c,
                _quotes,
                _delimiter,
                cmd_sz,
                _delimiter,
                QString::number(state2));
}

QString    VMTExport4th::FormatString(size_t state,QChar c,QString write,size_t state2){
    return QString("%1%2%3%4%5%6%7").arg(
                QString::number(state),
                _delimiter,
                _quotes+c+_quotes,
                _delimiter,
                _quotes+write+_quotes,
                _delimiter,
                QString::number(state2));
}



void VMTExport4th::operator ()(IVMTEnvironment *environment){
    if(!_callback) return;

    {
        AddInformation("Checking for recursive calls ... \r\n");
        std::set<QString> stack;
        stack.insert(_machine->GetName());
        MachineFunction(_machine,
        [&](std::shared_ptr<VMTComplexMachine> complex,std::shared_ptr<VMTComplexMachine> m)->bool{
            if(stack.count(m->GetName())) {
                _error          = true;
                _error_message  = QString("Recursive machine call to %1 detected in %2").arg(m->GetName(),complex->GetName());
                return false;
            } else return true;
        },
        [&](std::shared_ptr<VMTComplexMachine> complex,std::shared_ptr<IVMTMachine> m) ->bool{ return true;});
    }

    if(!_error) {
        std::map<std::string,size_t>                  state_index;
        std::deque<std::string>                       call_stack;
        size_t index=0;
        std::string prefix;
        MapState(index,prefix,call_stack,_machine,state_index,environment);
        ProcessComplex(0,prefix,call_stack,_machine,state_index,environment);




        AddInformation("Export complete \r\n");
        _callback->Done();
    } else{
        AddInformation("Export doesn't support recursive calls \r\n");
    }

    {
        std::lock_guard<std::mutex> lock(_mtx);
        _thread->detach();
        _thread.reset();
        _can_run = false;
    }
}

std::string VMTExport4th::GetFullName(std::string prefix,std::shared_ptr<IVMTMachine> machine){
    std::string result = prefix;
    result +=":";
    result += std::to_string((unsigned long long)machine.get());
    return result;
}
/*
std::string VMTExport4th::GetFullName(std::deque<std::string> &call_stack,std::shared_ptr<IVMTMachine> machine,size_t power){
    std::string result ="";

    for(std::string s:call_stack)
        result+=s+":";
    result += std::to_string((unsigned long long)machine.get());
    result += "-";
    result += std::to_string(power);

    //qDebug() << result.c_str();
    return result;
}*/

void VMTExport4th::MapState(size_t& index,std::string prefix,std::deque<std::string> &call_stack,std::shared_ptr<VMTComplexMachine> complex,std::map<std::string,size_t>& state_index,IVMTEnvironment *environment){
    call_stack.push_back(std::to_string((unsigned long long)complex.get()));
    std::string new_prefix = GetFullName(prefix,complex);
    for(auto m: complex->GetMachineCollection()){
            std::string full_name = GetFullName(new_prefix,m);
            state_index[full_name]=index++;
            qDebug() << (index-1) << " = " << full_name.c_str();
            if(auto current_complex=std::dynamic_pointer_cast<VMTComplexMachine>(m)){
                MapState(index,new_prefix,call_stack,current_complex,state_index,environment);
            } else {
                switch(m->GetID()){
                case IVMTMachine::MachineType::MT_RIGHT_WORD:
                case IVMTMachine::MachineType::MT_LEFT_WORD:
                case IVMTMachine::MachineType::MT_COPY:{
                    std::shared_ptr<VMTComplexMachine> current_complex =
                            std::dynamic_pointer_cast<VMTComplexMachine>(m->CreateComplexMachine(environment));
                    MapState(index,new_prefix,call_stack,current_complex,state_index,environment);
                    break;
                }
                }
            }


    }
    call_stack.pop_back();
}

void       VMTExport4th::ProcessComplex(size_t parent_index,
                                        std::string prefix,
                                        std::deque<std::string> &call_stack,
                                        std::shared_ptr<VMTComplexMachine> complex,
                                        std::map<std::string,size_t>& state_index,
                                        IVMTEnvironment *environment){
    std::set<std::shared_ptr<IVMTMachine>>  machine_set;
    std::shared_ptr<IVMTMachine> start_machine = nullptr;
    std::string full_name = GetFullName(prefix,complex);

    for(auto m: complex->GetMachineCollection())
        if(m->GetID()==IVMTMachine::MachineType::MT_START) start_machine = m;


    if(start_machine){
        call_stack.push_back(std::to_string((unsigned long long)complex.get()));
        qDebug() << "Begin complex machine:" << complex->GetMachineName()+" ,"+QString(std::to_string((unsigned long long)complex.get()).c_str());
        Process(parent_index,full_name,call_stack,start_machine,state_index,machine_set,environment);

        if(call_stack.size()==1){
            std::shared_ptr<IVMTMachine> end_machine = nullptr;
            for(auto m: complex->GetMachineCollection())
                if(m->GetID()==IVMTMachine::MachineType::MT_FINISH)
                    {
                        end_machine = m;
                        QString str = complex->GetAlphabit()->ToString();
                        size_t index_start = state_index[GetFullName(full_name,end_machine)];//GetFullName(call_stack,end_machine)];

                        for(QChar c:str)
                            _callback->AppendText(FormatString(index_start,c,c,index_start));
                    }
        }
        call_stack.pop_back();
        qDebug() << "End   complex machine:" << complex->GetMachineName();
    }
}

void       VMTExport4th::Process(size_t parent_index,
                                 std::string prefix,
                                 std::deque<std::string> &call_stack,
                                 std::shared_ptr<IVMTMachine> current,
                                 std::map<std::string,size_t>& state_index,
                                 std::set<std::shared_ptr<IVMTMachine>>  &machine_set,
                                 IVMTEnvironment *environment){

    machine_set.insert(current);
    std::string full_name = GetFullName(prefix,current);
    size_t index = state_index[full_name];
    qDebug() << "Process: Index: "<< index <<" , full_name:"<< full_name.c_str();

    for(auto l : current->GetIncomingTransitions())
        if(std::shared_ptr<IVMTTransition> lptr = l.lock()){
            auto mptr = lptr->GetStartMachine().lock();
            std::string incoming_name = GetFullName(prefix,mptr); // whate power???
            size_t index_start = state_index[incoming_name];

            switch(mptr->GetID()){
            case IVMTMachine::MachineType::MT_START:
                if(parent_index){
                    qDebug() << "Replace index: " << index_start << " to parent:" << parent_index;
                    index_start = parent_index;
                }
                break;
                // if left machine is complex - then change index to finish machine of complex
            case IVMTMachine::MachineType::MT_COMPLEX:
            case IVMTMachine::MachineType::MT_LEFT_WORD:
            case IVMTMachine::MachineType::MT_RIGHT_WORD:
            case IVMTMachine::MachineType::MT_COPY:
                std::shared_ptr<VMTComplexMachine> left_complex = std::dynamic_pointer_cast<VMTComplexMachine>(mptr);
                if(!left_complex) left_complex = std::dynamic_pointer_cast<VMTComplexMachine>(mptr->CreateComplexMachine(environment));
                if(left_complex){
                    std::shared_ptr<IVMTMachine> finish_machine = nullptr;
                    for(auto m: left_complex->GetMachineCollection())
                        if(m->GetID()==IVMTMachine::MachineType::MT_FINISH) finish_machine = m;

                    call_stack.push_back(std::to_string((unsigned long long)left_complex.get()));
                    std::string complex_name = GetFullName(prefix,left_complex);
                    size_t finish_index = state_index[GetFullName(complex_name,finish_machine)]; // add complex machine!!!!
                    call_stack.pop_back();
                    qDebug() << "Replace index: " << index_start << " to parent:" << finish_index;
                    index_start = finish_index;
                }
                break;
            }


            QString condition = lptr->GetConditions().GetAsString();
            qDebug() << "start machine:" << index_start << " end machine:" << index << " [" <<condition << "] " << (lptr.get());

            switch(current->GetID()){
            case IVMTMachine::MachineType::MT_LEFT:
                for(auto c: condition)
                    _callback->AppendText(FormatString(index_start,c,Command::left,index));
                break;
            case IVMTMachine::MachineType::MT_RIGHT:
                for(auto c: condition)
                    _callback->AppendText(FormatString(index_start,c,Command::right,index));
                break;
            case IVMTMachine::MachineType::MT_LEFT_WORD:
            case IVMTMachine::MachineType::MT_RIGHT_WORD:
            case IVMTMachine::MachineType::MT_COPY:
            case IVMTMachine::MachineType::MT_COMPLEX:
            {
                for(auto c: condition)
                   _callback->AppendText(FormatString(index_start,c,c,index));
                break;
            }
            case IVMTMachine::MachineType::MT_WRITE:{
                std::shared_ptr<VMTMachineAlpha> ma = std::dynamic_pointer_cast<VMTMachineAlpha>(current);

                if(!ma) {
                    _error = true;
                    _error_message = "Error: Pointer cast error";
                    AddInformation(QString("Error: Pointer cast error\r\n"));
                }


                for(auto c: condition)
                    _callback->AppendText(FormatString(index_start,c,ma->GetSign(),index));

                break;
            }
            case IVMTMachine::MachineType::MT_START:{
                QString str = current->GetParent().lock()->GetAlphabit()->ToString();
                for(QChar c:str)
                    _callback->AppendText(FormatString(index_start,c,c,index));
                break;
            }
            case IVMTMachine::MachineType::MT_FINISH:
                if(parent_index){
                    for(auto c: condition)
                        _callback->AppendText(FormatString(index_start,c,c,index));
                } else
                    for(auto c: condition)
                        _callback->AppendText(FormatString(index_start,c,Command::stop,index));
                break;
            }
        }
        // check for complex
        //*
        std::shared_ptr<VMTComplexMachine> current_complex = std::dynamic_pointer_cast<VMTComplexMachine>(current);
        if(!current_complex) current_complex = std::dynamic_pointer_cast<VMTComplexMachine>(current->CreateComplexMachine(environment));
        if(current_complex){
                ProcessComplex(index,prefix,call_stack,current_complex,state_index,environment);
        }
        //*/

        // check for outgoing connection
        for(std::weak_ptr<IVMTTransition> link:current->GetOutgoingTransitions()){
            std::shared_ptr<IVMTTransition> link_ptr = link.lock();
            std::shared_ptr<IVMTMachine> m = link_ptr->GetFinishMachine().lock();
            if(!machine_set.count(m))
                    Process(parent_index,prefix,call_stack,link_ptr->GetFinishMachine().lock(),state_index,machine_set,environment);
        }



}


VMTExport4th::~VMTExport4th(){
    Stop();
}


