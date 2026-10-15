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
    size_t active_position_;

   public:
    VMTLine(std::shared_ptr<VMTAlphabit> alphabit);

    std::shared_ptr<const VMTAlphabit> GetAlphabit() const;
    size_t GetActivePosition() const;
    QChar GetValueAt(size_t index) const;
    QChar GetValueAtActivePosition() const;
    bool IsActivePosition(size_t index) const;
    QString ToString() const;

    std::shared_ptr<VMTUndoElement> SetActivePosition(size_t index);
    std::shared_ptr<VMTUndoElement> SetSignAtActivePosition(QChar sign);
    std::shared_ptr<VMTUndoElement> SetValueAt(size_t index, QChar sign);
    std::shared_ptr<VMTUndoElement> ShiftLeft();
    std::shared_ptr<VMTUndoElement> ShiftRight();
    void Clear();

    virtual ~VMTLine();
};
#endif  // !defined(EA_0A644956_9080_4fc2_B4F8_CA6F597178CE__INCLUDED_)
