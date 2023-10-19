#if !defined(EA_A202567F_BD4B_438c_AEB5_364C3A649984__INCLUDED_)
#define EA_A202567F_BD4B_438c_AEB5_364C3A649984__INCLUDED_

class IVMTTransition;
class VMTComplexMachine;
class UICanvas;

#include <memory>
#include <QRect>
#include <QPoint>
#include <vector>


#include "vmtundomanager.h"
#include "IVMTAlphabitSource.h"
#include "IVMTEnvironment.h"
#include "VMTLine.h"
#include <QDataStream>
#include "uicanvas.h"

class IVMTMachine
{
protected:
    ImageType _image_type;
public:
    IVMTMachine() = delete;
    IVMTMachine(ImageType it) : _image_type(it) {};
    virtual std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment*) = 0;
	virtual ~IVMTMachine() {}

    enum MachineType {
        MT_LEFT=1,
        MT_LEFT_WORD=2,
        MT_RIGHT=3,
        MT_RIGHT_WORD=4,
        MT_WRITE=5,
        MT_COPY=6,
        MT_START=7,
        MT_FINISH=8,
        MT_COMPLEX=9,
        NONE

    };

    virtual std::shared_ptr<IVMTMachine> CreateComplexMachine(IVMTEnvironment*) = 0;
    virtual void Serialize(QDataStream&) = 0;
    virtual void Deserialize(QDataStream&) = 0;

    virtual void AddIncomingTransition(std::weak_ptr<IVMTTransition> transition) =0;
    virtual void AddOutgoingTransition(std::weak_ptr<IVMTTransition> transition) =0;

    virtual void RemoveIncomingTransition(std::shared_ptr<IVMTTransition> transition) = 0;
    virtual void RemoveOutgoingTransition(std::shared_ptr<IVMTTransition> transition) = 0;

    virtual void DoAction(std::shared_ptr<VMTLine> line) =0;
    virtual const QRect& GetBounds() =0;
    virtual QPoint& GetSize() = 0;
    virtual const QRect& GetBoundsWithChilds() =0;
    virtual const QPoint& GetCenter() =0;
    virtual MachineType GetID() =0;
    virtual std::weak_ptr<VMTComplexMachine> GetParent() =0;
    virtual ImageType GetImageType() {return _image_type;}

    virtual const std::vector<std::weak_ptr<IVMTTransition>>& GetIncomingTransitions() =0;
    virtual const std::vector<std::weak_ptr<IVMTTransition>>& GetOutgoingTransitions() =0;

    virtual const QPoint& GetInputPoint() =0;
    virtual const QString& GetMachineName() =0;
    virtual const QString& GetName() =0;
    virtual const QPoint& GetOutputPoint() =0;

    virtual void ChangeAlphabit(std::shared_ptr<VMTAlphabit>) {}
    virtual IVMTAlphabitSource *GetAlphabitSource() = 0;

	virtual long GetPower() =0;
	virtual bool IsAcceptIncoming() =0;
	virtual bool IsAcceptOutgoing() =0;
	virtual bool IsAlphabit() =0;
	virtual bool IsComplex() =0;
	virtual bool IsError() =0;
    virtual bool IsInside(const QPoint& point) =0;
	virtual bool IsPowerCalculator() =0;
	virtual bool IsSelected() =0;

    virtual void Move(const QPoint& center,IVMTEnvironment* environment) =0;
    virtual void Move(const QPoint&& center,IVMTEnvironment* environment) =0;
    virtual void Paint(UICanvas& canvas, const QRect& rect) =0;
	virtual void RemoveChilds() =0;
    virtual void RemoveIncomingTransition(IVMTTransition* transition) =0;
    virtual void RemoveOutgoingTransition(IVMTTransition* transition) =0;
    virtual void Update(IVMTEnvironment* environment) = 0;

	virtual void SetError(bool error) =0;
    virtual void SetParent(std::weak_ptr<VMTComplexMachine> parent) =0;
	virtual void SetPower(long power) =0;
	virtual void SetSelected(bool selected) =0;

};
#endif // !defined(EA_A202567F_BD4B_438c_AEB5_364C3A649984__INCLUDED_)
