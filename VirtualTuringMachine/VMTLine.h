#if !defined(EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_)
#define EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_

#include <memory>
#include <vector>

#include "VMTAlphabit.h"
#include "vmtundoelement.h"

class VMTLine {
   private:
    std::shared_ptr<VMTAlphabit> alphabit_;
    std::vector<QChar> array_;
    size_t cursor_position_;

   public:
    VMTLine(std::shared_ptr<VMTAlphabit> alphabit);

    std::shared_ptr<VMTAlphabit> GetAlphabit();
    long GetMachinePosition();

    void Clear();
    char GetValueAt(long index);
    char GetValueAtMachine();
    bool IsMachinePosition(long index);
    std::shared_ptr<VMTUndoElement> SetMachinePosition(long index);
    std::shared_ptr<VMTUndoElement> SetSignAtMachine(char sign);
    std::shared_ptr<VMTUndoElement> SetValueAt(long index, char sign);
    std::shared_ptr<VMTUndoElement> ShiftLeft();
    std::shared_ptr<VMTUndoElement> ShiftRight();
    QString ToString();

    virtual ~VMTLine();
};
#endif  // !defined(EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_)
