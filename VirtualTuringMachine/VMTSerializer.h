#if !defined(EA_FBF9ECAB_E1A9_4b4f_A5F3_7B533457B83F__INCLUDED_)
#define EA_FBF9ECAB_E1A9_4b4f_A5F3_7B533457B83F__INCLUDED_


#include <memory>
#include <QString>
#include <QFile>
#include <QDataStream>

#include <map>
#include <set>
#include <interfaces/IVMTMachine.h>
#include <interfaces/IVMTTransition.h>
#include <vmtmachines/VMTComplexMachine.h>
#include <QDebug>
#include <QBuffer>
#include <QMessageBox>

class VMTProject;

struct machine_compare {
    bool operator() (const std::shared_ptr<VMTComplexMachine> &a, const std::shared_ptr<VMTComplexMachine> &b) const{
        return a->GetName() < b->GetName();
    }
};

class VMTSerializer
{

public:

    class ReadContext{
    public:
        QDataStream _stream;
        QFile       _file;
        bool        _open;


        std::map<long,std::shared_ptr<IVMTMachine>> _machines;
        std::set<std::shared_ptr<VMTComplexMachine>,machine_compare> _complex;
        std::map<long,std::shared_ptr<IVMTTransition>> _transitions;
        std::map<long,std::shared_ptr<VMTComplexMachineInner>> _inner;

        ReadContext(QBuffer &buffer){
          _open = true;
          buffer.open(QIODevice::ReadOnly);
          _stream.setDevice(&buffer);
        }

        ReadContext(QString name){
            _open = false;
            _file.setFileName(name);
            _file.open(QIODevice::ReadOnly);
            if(_file.isOpen()){
                _stream.setDevice(&_file);
                _open = true;
            }else {

                QString msg;
                        msg.append("Error read file: " );
                        msg.append(_file.errorString());
               QMessageBox::question( nullptr,"VMT",msg,QMessageBox::Ok,QMessageBox::Ok);
            }

        }


        virtual ~ReadContext(){
            if(_file.isOpen()) _file.close();
        }
    };

    class WriteContext{
    public:
        long _machine_id;
        long _inner_id;
        long _transition_id;

        QDataStream _stream;
        QFile       _file;
        bool        _open;

        WriteContext(QString name){
            _open = false;
            _file.setFileName(name);
            _file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            if(_file.isOpen()){
                _open = true;
                _stream.setDevice(&_file);
            } else {
                qDebug() << "Error create file::" << _file.errorString();
                QString msg;
                        msg.append("Error create file: " );
                        msg.append(_file.errorString());
               QMessageBox::question( nullptr,"VMT",msg,QMessageBox::Ok,QMessageBox::Ok);
            }
            _machine_id = 1;
            _inner_id = 1;
            _transition_id = 1;
        }

        WriteContext(QBuffer &buffer){
             buffer.open(QIODevice::WriteOnly);
             _stream.setDevice(&buffer);
            _machine_id = 1;
            _inner_id = 1;
            _transition_id = 1;
            _open = true;
        }

        std::map<std::shared_ptr<IVMTMachine>,long> _machines;
        std::map<std::shared_ptr<IVMTTransition>,long> _transitions;
        std::map<std::shared_ptr<VMTComplexMachineInner>,long> _inner;

        virtual ~WriteContext(){
                if(_file.isOpen()) _file.close();
        }
    };




    VMTSerializer(QString & name);

    void Deserialize(VMTProject *project);
    void Deserialize(VMTProject *project,QBuffer &buffer);
    std::shared_ptr<VMTComplexMachine> Deserialize(ReadContext & context,long inner_id,bool binner);

    void Serialize(VMTProject *project);
    void Serialize(VMTProject *project,QBuffer& buffer);
    void Serialize(WriteContext & context,std::shared_ptr<VMTComplexMachine> p_Machine,bool bInner);

	virtual ~VMTSerializer();

    protected:
        QString _name;
        void WriteString(WriteContext & context, const QString &str);
        void ReadString(ReadContext & context,  QString& str);
        void ReplaceInnerObject(bool recursive,std::shared_ptr<VMTComplexMachine> machine,std::map<QString,std::shared_ptr<VMTComplexMachineInner>> inner_map);
        void ClearGarbrageTransitions(std::shared_ptr<VMTComplexMachine> machine);

};
#endif // !defined(EA_FBF9ECAB_E1A9_4b4f_A5F3_7B533457B83F__INCLUDED_)
