#if !defined(EA_6C84CC22_6831_40e5_83A8_7296CDE08AAE__INCLUDED_)
#define EA_6C84CC22_6831_40e5_83A8_7296CDE08AAE__INCLUDED_


class IVMTAlphabitSource;
class IVMTMachine;
class IVMTCalculatorCallback;
class IVMTAlphabitSource;
class IVMTDebuger;
class IVMTComplexMachineSource;
class VMTComplexMachine;
class VMTLine;
#include <memory>
#include "uicanvas.h"

class IVMTEnvironment
{

public:
    virtual void EnableActionHint(const QString &hint,QPixmap& img) = 0;
    virtual void DisableActionHint() = 0;
    virtual void DisableAlphabit() =0;
    virtual void DisableCalculator() =0;
    virtual void EnableAlphabit(IVMTAlphabitSource* p_Alphabit,const QString& msg,QPixmap &img) =0;
    virtual void EnableCalculator(std::shared_ptr<IVMTMachine> machine) =0;
    virtual UICanvas& GetGraphics() =0;
    virtual std::weak_ptr<VMTComplexMachine> GetMachine() =0;
    virtual void SetMachine(std::shared_ptr<VMTComplexMachine> machine) =0;
    virtual void Repaint(const QRect& rect) =0;
    virtual void MoveInScreen(QPoint &&shift) =0;
    virtual void Move(QPoint &&shift) =0;
    virtual void EnableAnimation(bool enable) = 0;

};
#endif // !defined(EA_6C84CC22_6831_40e5_83A8_7296CDE08AAE__INCLUDED_)
