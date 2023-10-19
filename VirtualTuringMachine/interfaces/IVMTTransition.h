
#if !defined(EA_BF914F26_4A17_4121_9768_093186EA560B__INCLUDED_)
#define EA_BF914F26_4A17_4121_9768_093186EA560B__INCLUDED_


#include "VMTTransitionConditions.h"
#include "IVMTAlphabitSource.h"
#include "IVMTMachine.h"


class VMTAlphabit;
class IVMTMachine;
#include "uicanvas.h"

#include <QPoint>
#include <QRect>
#include <QDataStream>

class IVMTEnvironment;

class IVMTTransition
{

public:
    virtual void Serialize(QDataStream&) =0;
    virtual void Deserialize(QDataStream&) =0;
    virtual void BeginDrag(IVMTEnvironment* environment,const QPoint& p_Point) =0;
    virtual void ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit) =0;
    virtual void Changed(IVMTEnvironment* environment) =0;

    virtual void Drag(IVMTEnvironment* environment,const QPoint& p_Point) =0;
    virtual void EndDrag(IVMTEnvironment* environment,const QPoint& p_Point) =0;

    virtual const QRect& GetBounds() =0;
    virtual VMTTransitionConditions& GetConditions() =0;
    virtual void SetConditions(VMTTransitionConditions& other) =0;

    virtual  std::weak_ptr<IVMTMachine>  GetFinish() =0;
    virtual  std::weak_ptr<IVMTMachine>  GetStart() =0;

    virtual bool IsError() =0;
    virtual bool IsInside(IVMTEnvironment* environment,const QPoint& p_Point) =0;

    virtual void Paint(UICanvas& canvas, const QRect& rect) =0;

    virtual void RemoveChilds() =0;

    virtual std::weak_ptr<IVMTMachine> GetStartMachine() = 0;
    virtual std::weak_ptr<IVMTMachine> GetFinishMachine() = 0;

    virtual void SetError(bool bError) =0;
    virtual void SetFinish(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine) =0;
    virtual void SetFinish(IVMTEnvironment* environment,const QPoint& point) =0;
    virtual void SetSelected(bool enable) =0;
    virtual void SetStart(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine) =0;
    virtual void SetStart(IVMTEnvironment* environment,const QPoint& point) =0;
    virtual void SetParent(std::weak_ptr<VMTComplexMachine> parent) =0;

    virtual IVMTAlphabitSource *GetAlphabitSource() = 0;


};
#endif // !defined(EA_BF914F26_4A17_4121_9768_093186EA560B__INCLUDED_)
