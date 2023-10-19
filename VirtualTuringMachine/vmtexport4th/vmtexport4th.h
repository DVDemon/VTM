#ifndef VMTEXPORT4TH_H
#define VMTEXPORT4TH_H
#include <mutex>
#include <memory>
#include <thread>
#include <set>
#include <deque>
#include <map>
#include <stack>
#include <iostream>
#include <tuple>
#include <QString>
#include <QTextStream>
#include "interfaces/IVMTEnvironment.h"

class VMTComplexMachine;
class IVMTMachine;

class TextCallback{
public:
    virtual void Done() = 0;
    virtual void Clear() = 0;
    virtual void AppendText(const QString &text) = 0;
    virtual void Information(const QString &text) = 0;
};

class VMTExport4th
{
protected:
    enum Command{
        left,right,stop
    };

    std::mutex _mtx;
    size_t     _progress;
    bool       _can_run;
    std::shared_ptr<std::thread>       _thread;
    std::shared_ptr<VMTComplexMachine> _machine;

    QString _delimiter;
    QString _left;
    QString _right;
    QString _stop;
    QString _quotes;

    bool       _error;
    QString    _error_message;
    QString    _information;
    QString    _output;
    size_t     _machine_count;

    TextCallback *_callback;

    void       AddInformation(QString && str);
    QString    FormatString(size_t state,QChar c,Command cmd,size_t state2);
    QString    FormatString(size_t state,QChar c,QString write,size_t state2);

    void        MapState(size_t& index,
                         std::string prefix,
                         std::deque<std::string> &call_stack,
                         std::shared_ptr<VMTComplexMachine> complex,
                         std::map<std::string,size_t>& state_index,
                         IVMTEnvironment *environment);

   // std::string GetFullName(std::deque<std::string> &call_stack,std::shared_ptr<IVMTMachine> machine,size_t power);
    std::string GetFullName(std::string prefix,std::shared_ptr<IVMTMachine> machine);
    void       Process(size_t parent_index,
                       std::string prefix,
                       std::deque<std::string> &call_stack,
                       std::shared_ptr<IVMTMachine> current,
                       std::map<std::string,size_t>& state_index,
                       std::set<std::shared_ptr<IVMTMachine>>  &machine_set,
                       IVMTEnvironment *environment);

    void       ProcessComplex(size_t parent_index,
                              std::string prefix,
                              std::deque<std::string> &call_stack,
                              std::shared_ptr<VMTComplexMachine> complex,
                              std::map<std::string,size_t>& state_index,
                              IVMTEnvironment *environment);


public:
    VMTExport4th(std::shared_ptr<VMTComplexMachine> machine,
                 QString delimiter, QString left, QString right, QString stop, QString quotes);
    void    Export(TextCallback *_callback,IVMTEnvironment *environment);
    size_t  GetProgress();
    bool    IsRuning();
    void    Stop();


    bool            IsError();
    const QString   GetErrorMessage();
    const QString   GetInformation();
    size_t          GetMachineCount();

    void    operator()(IVMTEnvironment *environment);
    virtual ~VMTExport4th();
};

#endif // VMTEXPORT4TH_H
