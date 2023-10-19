#if !defined(EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_)
#define EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_

#include "interfaces/IVMTLineObserver.h"
#include "VMTAlphabit.h"
#include "vmtundoelement.h"
#include <vector>
#include <memory>


#define SHIFT_STEP 10
class VMTLine
{

private:
    //IVMTLineObserver* _observer;

    std::shared_ptr<VMTAlphabit>     _alphabit;
    std::vector<char*>    _array;
    long _machine;
    long _zero;
    void CheckPosition(long index);

public:
    VMTLine(std::shared_ptr<VMTAlphabit> alphabit);

    void ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit);
    std::shared_ptr<VMTUndoElement> CopyCurrentWord();


    std::shared_ptr<VMTAlphabit>  GetAlphabit();
	long GetMachinePosition();
    long GetLeftSignPosition();
    long GetRightSignPosition();

    IVMTLineObserver* GetObserver();
    void Clear();
	char GetValueAt(long index);
	char GetValueAtMachine();
    bool IsEqualAlphabit(VMTAlphabit* alphabit);
	bool IsMachinePosition(long index);
    std::shared_ptr<VMTUndoElement>  SetMachinePosition(long index);
    void SetObserver(IVMTLineObserver* observer);
    std::shared_ptr<VMTUndoElement>  SetSignAtMachine(char sign);
    std::shared_ptr<VMTUndoElement>  SetValueAt(long index, char sign);
    std::shared_ptr<VMTUndoElement>  ShiftLeft();
    std::shared_ptr<VMTUndoElement>  ShiftLeftWord();
    std::shared_ptr<VMTUndoElement>  ShiftRight();
    std::shared_ptr<VMTUndoElement>  ShiftRightWord();
    QString ToString();

    virtual ~VMTLine();
};
#endif // !defined(EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_)
