#include "VMTSerializer.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "VMTAlphabit.h"
#include "vmtproject.h"
#include "VMTTransitionImpl.h"
#include "QDebug"
#include <cstring>

#define TYPE_MACHINE     1
#define TYPE_TRANSITION  2

VMTSerializer::VMTSerializer(QString & name) : _name(name){
}

void VMTSerializer::Deserialize(VMTProject *project){
    project->GetCurrentMachineName() = "";
    project->GetMachines().clear();

    ReadContext context(_name);

    qDebug() << _name;

    if(context._open){
        qDebug() << "Start deserialize";
        QString signs;
        ReadString(context,signs);
        project->GetAlphabit()->ReplaceString(signs);

        qDebug() << "Alphabit:" << project->GetAlphabit()->ToString();
        qint64 count;
        context._stream >> count;

        qDebug() << "Machine count:" << count;

        for(int i=0;i<count;i++){
            std::shared_ptr<VMTComplexMachine> machine = Deserialize(context,1,true);
        }

        std::map<QString,std::shared_ptr<VMTComplexMachineInner>> inner_map;

        qDebug() << "Total machines " << context._machines.size();
        for(auto machine: context._complex){
            if(machine){
                qDebug() << "add " << machine->GetName();
                project->GetMachines().push_back(machine);
                inner_map[machine->GetName()] = machine->GetInnerObject();
            }
        }

        qDebug() << "Replace inner machines";
        for(auto &machine: project->GetMachines()){
            ReplaceInnerObject(true,machine,inner_map);
        }

        qDebug() << "Clear garbrage";
        for(auto &machine: project->GetMachines())
                    ClearGarbrageTransitions(machine);

    } else qDebug() << "Opps not open!";
    qDebug() << "Finish deserialize";
}

void VMTSerializer::Deserialize(VMTProject *project,QBuffer &buffer){
    //QString current_name= project->GetCurrentMachineName() = "";
    project->GetMachines().clear();

    ReadContext context(buffer);

    qDebug() << _name;

    if(context._open){
        qDebug() << "Start deserialize";
        QString signs;
        ReadString(context,signs);
        project->GetAlphabit()->ReplaceString(signs);

        qDebug() << "Alphabit:" << project->GetAlphabit()->ToString();
        qint64 count;
        context._stream >> count;

        qDebug() << "Machine count:" << count;

        for(int i=0;i<count;i++){
            std::shared_ptr<VMTComplexMachine> machine = Deserialize(context,1,true);
        }

        std::map<QString,std::shared_ptr<VMTComplexMachineInner>> inner_map;

        qDebug() << "Total machines " << context._machines.size();
        for(auto machine: context._complex){
            if(machine){
                qDebug() << "add " << machine->GetName();
                project->GetMachines().push_back(machine);
                inner_map[machine->GetName()] = machine->GetInnerObject();
            }
        }

        for(auto &machine: project->GetMachines()){
            ReplaceInnerObject(true,machine,inner_map);
        }

        for(auto &machine: project->GetMachines())
                    ClearGarbrageTransitions(machine);

    } else qDebug() << "Opps not open!";
}

void VMTSerializer::ReplaceInnerObject(bool recursive,std::shared_ptr<VMTComplexMachine> machine, std::map<QString,std::shared_ptr<VMTComplexMachineInner>> inner_map){
    auto ptr = inner_map.find(machine->GetName());
    if(ptr!=inner_map.end()){
        if(machine->GetInnerObject()!=ptr->second){
            qDebug() << "Replacer replace:" << machine->GetName();
            machine->SetInnerObject(ptr->second,machine);

        } else {
            qDebug() << "Replacer skip:" << machine->GetName();
        }

        if(recursive)
            for(auto &child: machine->GetMachineCollection()){
                std::shared_ptr<VMTComplexMachine> ptr_child = std::dynamic_pointer_cast<VMTComplexMachine>(child);
                if(ptr_child)
                {
                    qDebug() << "Machine:" << ptr_child->GetName() << " has " << ptr_child->GetMachineCollection().size() << " childs";
                    ReplaceInnerObject(false,ptr_child,inner_map);
                }
            }

    } else qDebug() << "Replacer not found map for " << machine->GetName();
}

std::shared_ptr<VMTComplexMachine> VMTSerializer::Deserialize(ReadContext & context, long inner_id,bool binner){
    std::shared_ptr<VMTComplexMachine> machine;
    if(context._open)
    {
        QString value;
        machine = std::shared_ptr<VMTComplexMachine>(new VMTComplexMachine(value,std::shared_ptr<VMTAlphabit>(new VMTAlphabit())));
        machine->Deserialize(context._stream);


        ReadString(context,value); machine->SetName(value);
        qDebug() << "Complex machine name:" << machine->GetName();
        ReadString(context,value); machine->GetAlphabit()->ReplaceString(value);
        qDebug() << "Complex machine alphabit:" << machine->GetAlphabit()->ToString();

        if(!binner){
            qDebug() << "Finding inner machine";
            auto ptr = context._inner.find(inner_id);
            if(ptr!=context._inner.end()){
                machine->SetInnerObject(ptr->second,machine);
                qDebug() << "Found";

            } else qDebug() << "ERROR!!! ID:" << inner_id;
        }

        if(binner)
        {
            qDebug() << "Deserialize inner machine";
            context._inner[inner_id]=machine->GetInnerObject();

            bool found= false;
            for(auto &complex_machine : context._complex)
                if(complex_machine->GetName() == machine->GetName()) found = true;
            if(!found) {
                context._complex.insert(machine);
            }

            qint64 total_components;
            context._stream >> total_components;
            qDebug() << "Total components:" << total_components;

            while(total_components>0){
                qint32 type_id;
                context._stream >> type_id;
                qDebug() << "MACHINE_TYPE:" << type_id;
                total_components--;

                switch(type_id){
                case TYPE_MACHINE:
                {
                    qint64 id; context._stream >> id;
                    qint32 machine_type; context._stream >> machine_type;
                    qDebug() << "\n---------------Machine id:" << id << " Type:"<< machine_type;

                    if(machine_type==IVMTMachine::MachineType::MT_COMPLEX){
                         qDebug() << "\n---------------Machine   : complex";
                        bool inner_local; context._stream >> inner_local;
                        qint64 inner_id_local; context._stream >> inner_id_local;
                        std::shared_ptr<VMTComplexMachine> stub =Deserialize(context,inner_id_local,inner_local);
                        stub->SetParent(machine);
                        auto ptr = context._machines.find(id);
                        if(ptr!=context._machines.end()){
                            qDebug() << "Insert  complex old id=" << id;
                            machine->AddMachine(ptr->second);
                        } else {
                            qDebug() << "Insert complex new id=" << id;
                            context._machines[id]=stub;
                            machine->AddMachine(stub);
                        }

                    } else {
                        qDebug() << "Deserialize machine id=" << id;
                        if(machine_type==0) exit(-1);
                        std::shared_ptr<IVMTMachine> stub = VMTMachineStub::CreateMachineByID((IVMTMachine::MachineType)machine_type,machine);
                        if(stub)
                        {
                            qDebug() << "start deserialize";
                            stub->Deserialize(context._stream);
                            qDebug() << "end deserialize";
                            auto ptr = context._machines.find(id);
                            if(ptr!=context._machines.end()){
                                qDebug() << "Add machine old id=" << id;
                                machine->AddMachine(ptr->second);
                            } else {
                                qDebug() << "Add machine new id=" << id;
                                context._machines[id]=stub;
                                machine->AddMachine(stub);
                            }
                        } else exit(-1);
                    }
                    qDebug() << "\n--------------- End deserialize machine" ;

                    break;
                }
                case TYPE_TRANSITION:
                {
                    qint64 id; context._stream >> id;
                    qDebug() << "Deserialize tranision id=" << id;
                    std::shared_ptr<IVMTTransition> stub =  std::shared_ptr<IVMTTransition>(new VMTTransitionImpl(machine));
                    qDebug() << "Insert transition new id=" << id;
                    context._transitions[id]=stub;
                    machine->AddTransition(stub);


                    context._stream >> id;
                    qDebug() << "Deserialize tranision start machine id=" << id;
                    if(id!=0){
                        auto ptr = context._machines.find(id);
                        if(ptr!=context._machines.end()){
                            qDebug() << "Found id=" << id;
                            stub->SetStart(nullptr,ptr->second);
                            ptr->second->AddOutgoingTransition(stub);
                        } else qDebug() << "Transition read fail: start";
                    }
                    context._stream >> id;
                    qDebug() << "Deserialize tranision finish machine id=" << id;
                    if(id!=0){
                        auto ptr = context._machines.find(id);
                        if(ptr!=context._machines.end()){
                            qDebug() << "Found id=" << id;
                            stub->SetFinish(nullptr,ptr->second);
                            ptr->second->AddIncomingTransition(stub);
                        } else qDebug() << "Transition read fail: finish";
                    }
                    qDebug() << "Start desirialize transition" << id;
                    stub->Deserialize(context._stream);
                    qDebug() << "Finish desirialize transition" << id;
                    break;
                }
                default:
                    exit(-1);
                }
            }
        }
    }

    return machine;
}

void VMTSerializer::Serialize(VMTProject *project){
    WriteContext context(_name);

    qDebug() << "context created";
    if(context._open){
         qDebug() << "context opened";
        WriteString(context,project->GetAlphabit()->ToString());
        context._stream << (qint64) project->GetMachines().size();

        for(auto machine : project->GetMachines()){
             qDebug() << "serialize mschine:" << machine->GetName();
            context._inner[machine->GetInnerObject()] = context._inner_id++;
            Serialize(context,machine,true);
        }
    }
}

void VMTSerializer::Serialize(VMTProject *project,QBuffer& buffer){
    WriteContext context(buffer);

    if(context._open){
        WriteString(context,project->GetAlphabit()->ToString());
        context._stream << (qint64) project->GetMachines().size();

        for(auto machine : project->GetMachines()){
            context._inner[machine->GetInnerObject()] = context._inner_id++;
            Serialize(context,machine,true);
        }
    }
}

void VMTSerializer::ReadString(ReadContext & context, QString& str){
    if(context._open)
    {
        short int length;
        context._stream >> length;

        if(length>0)
        {
            char * array = new char[length+1];
            std::memset(array,0,length+1);
            context._stream.readRawData(array,length);

            str = QString::fromUtf8((const char*)array);
            delete[] array;
        }
    }
}

void VMTSerializer::WriteString(WriteContext & context, const QString& str){
    if(context._open)
    {
        //context._stream << (short int)str.length();
        QByteArray array = str.toUtf8();
        context._stream << (short int)array.length();
        const char* carray =  array.constData();
        for (int i = 0; i < array.length(); i++) {
            context._stream << (qint8) (carray[i]);
            // context._stream << (qint8)array[i];//(str[i]);

        }

    }
}

void VMTSerializer::Serialize(WriteContext & context,std::shared_ptr<VMTComplexMachine> machine,bool bInner)
{
    if(context._open)
    {
        machine->Serialize(context._stream);

        WriteString(context,machine->GetName());
        WriteString(context,machine->GetAlphabit()->ToString());



        if(!bInner) return;

        qDebug() << "Serialize machine structure:" << machine->GetName();
        // fill machine hash
        qint64 total_components = 0;
        for(auto &stub : machine->GetMachineCollection())
        {
            auto find = context._machines.find(stub);
            if(find==context._machines.end())
                context._machines[stub]=context._machine_id++;

            total_components	++;
        }
        // fill transition hash

        for(auto &stub : machine->GetTransitionCollection())
        {
            auto find = context._transitions.find(stub);
            if(find==context._transitions.end())
                context._transitions[stub]=context._transition_id++;
            total_components ++;
        }
        context._stream << (qint64)total_components;


        for(auto stub : machine->GetMachineCollection())
        {
            long nID =context._machines[stub];
            context._stream << (qint32) TYPE_MACHINE;
            context._stream << (qint64) nID;
            context._stream << (qint32) stub->GetID();

            if (stub->GetID()==IVMTMachine::MachineType::MT_COMPLEX)
            {
                qDebug() << "----- Serialize complex machine:" << stub->GetName();
                std::shared_ptr<VMTComplexMachine> ptr = std::dynamic_pointer_cast<VMTComplexMachine>(stub);
                if(ptr)
                {
                    if(context._inner.find(ptr->GetInnerObject())!=context._inner.end())
                    {
                        context._stream << false;
                        context._stream << (qint64) context._inner[ptr->GetInnerObject()];
                        Serialize(context,ptr,false);
                    } else
                    {
                        context._stream << true;
                        context._stream << (qint64)context._inner_id;
                        qDebug() << "Add inner:" << context._inner_id << " for:" << ptr->GetName();
                        context._inner[ptr->GetInnerObject()] = context._inner_id++;
                        Serialize(context,ptr,true);
                    }
                    qDebug() << "----- Done serialize complex machine:" << stub->GetName();
                }
            } else
            {
                qDebug() << "----- Serialize simple machine:" << stub->GetName();
                stub->Serialize(context._stream);
                qDebug() << "----- Done serialize simple machine:" << stub->GetName();
            }
        }

        // save transitions
        for(auto stub : machine->GetTransitionCollection())
        {
            long nID = context._transitions[stub];

            // save transition type
            context._stream << (qint32) TYPE_TRANSITION;
            context._stream << (qint64) nID;

            if(auto ptr=stub->GetStartMachine().lock())
            {
                context._stream << (qint64)context._machines[ptr];
                qDebug() << "Ptr" << (qint64)context._machines[ptr];
            } else context._stream << (qint64)0;


            if(auto ptr=stub->GetFinishMachine().lock())
            {
                context._stream << (qint64)context._machines[ptr];
                qDebug() << "Ptr" << (qint64)context._machines[ptr];
            } else context._stream << (qint64)0;

            // save transition internal data
            stub->Serialize(context._stream);
        }
    }
}

void VMTSerializer::ClearGarbrageTransitions(std::shared_ptr<VMTComplexMachine> machine){
    for(std::shared_ptr<IVMTMachine> m: machine->GetMachineCollection()){

        bool removed  = true;

        while(removed){
            removed = false;
            for(const std::weak_ptr<IVMTTransition>& t: m->GetIncomingTransitions()){
                bool found = false;

                if(auto tptr = t.lock()){
                    for(std::shared_ptr<IVMTTransition> tc: machine->GetTransitionCollection())  if(tc==tptr) found = true;
                    if(!found)
                    {
                        m->RemoveIncomingTransition(tptr);
                        qDebug() << "!!!!!!!!!!! Removed garbrage";
                        removed = true;
                        break;
                    }
                }
            }
        }

        removed  = true;

        while(removed){
            removed = false;
            for(const std::weak_ptr<IVMTTransition> &t: m->GetOutgoingTransitions()){
                bool found = false;

                if(auto tptr = t.lock()){
                    for(std::shared_ptr<IVMTTransition> &tc: machine->GetTransitionCollection())  if(tc==tptr) found = true;
                    if(!found)
                    {
                        m->RemoveOutgoingTransition(tptr);
                        qDebug() << "!!!!!!!!!!! Removed garbrage";
                        removed = true;
                        break;
                    }
                }
            }
        }
    }
}

VMTSerializer::~VMTSerializer(){

}
