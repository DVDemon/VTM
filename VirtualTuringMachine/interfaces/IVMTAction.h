
#if !defined(EA_ACB08CBC_DDDD_4b91_9DC8_9C9BCA4CF6F1__INCLUDED_)
#define EA_ACB08CBC_DDDD_4b91_9DC8_9C9BCA4CF6F1__INCLUDED_

#include "IVMTAlphabitSource.h"
#include "uicanvas.h"
#include <QPoint>
#include <QRect>
#include <mutex>

class IVMTAction
{
protected:
    std::mutex _mutex;
public:
	IVMTAction() {

	}

	virtual ~IVMTAction() {

	}


    virtual void Cancel(IVMTEnvironment *environment) =0;
    virtual void Enable(IVMTEnvironment *environment) =0;
    virtual bool OnMouseMoved(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) =0;
    virtual bool OnMousePressed(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) =0;
    virtual bool OnMouseReleased(IVMTEnvironment* environment,const QPoint &screen,const QPoint &real) =0;
    virtual void Paint(UICanvas& canvas, const QRect &rect) =0;

};
#endif // !defined(EA_ACB08CBC_DDDD_4b91_9DC8_9C9BCA4CF6F1__INCLUDED_)
