#ifndef VMTDEBUGER_H
#define VMTDEBUGER_H

#include "VMTLine.h"
#include "VMTAlphabit.h"
#include "vmtmachines/VMTComplexMachine.h"

#include <memory>
#include <stack>

class VMTDebuger
{
protected:
    std::shared_ptr<VMTLine>  _line;
    std::shared_ptr<VMTComplexMachine> _complex_machine;

    class State{
       public:
        std::shared_ptr<VMTComplexMachine> complex_machine;
        std::shared_ptr<IVMTMachine> machine;
    };

    std::stack<State> _stack;
    State             _state;
    VMTDebuger::State FindNextMachine();


public:
    VMTDebuger(std::shared_ptr<VMTAlphabit> alphabit,std::shared_ptr<VMTComplexMachine> complex_machine);

    std::shared_ptr<VMTLine>           GetLine();
    std::shared_ptr<VMTComplexMachine> GetComplexMachine();

    void ToStart(IVMTEnvironment *environment);
    bool IsFinish();
    void Step(IVMTEnvironment *environment);
};

#endif // VMTDEBUGER_H
