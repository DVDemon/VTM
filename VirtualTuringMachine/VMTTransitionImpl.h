#if !defined(EA_605C0DDC_936B_4cfe_BE03_26F5FE6A1A08__INCLUDED_)
#define EA_605C0DDC_936B_4cfe_BE03_26F5FE6A1A08__INCLUDED_

#include "VMTTransitionConditions.h"
#include "interfaces/IVMTMachine.h"
#include "VMTSerializer.h"
#include "interfaces/IVMTMachine.h"
#include "interfaces/IVMTTransition.h"
#include "vmtmachines/VMTComplexMachine.h"
#include "screentools.h"
#include <QPoint>
#include <QRect>
#include <vector>

#define PRECISION 5

class VMTTransitionImpl : public IVMTTransition
{

public:
    VMTTransitionImpl(std::weak_ptr<VMTComplexMachine> parent);

    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;
    void BeginDrag(IVMTEnvironment* environment,const QPoint& p_Point) override;
    void ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit) override;
    void Changed(IVMTEnvironment* environment) override;

    void Drag(IVMTEnvironment* environment,const QPoint& p_Point) override;
    void EndDrag(IVMTEnvironment* environment,const QPoint& p_Point) override;

    const QRect& GetBounds() override;
    VMTTransitionConditions& GetConditions() override;
    void SetConditions(VMTTransitionConditions& other) override;

    std::weak_ptr<IVMTMachine> GetFinish() override;
    std::weak_ptr<IVMTMachine> GetStart() override;

    bool IsError() override;
    bool IsInside(IVMTEnvironment* environment,const QPoint& p_Point) override;

    void Paint(UICanvas& canvas, const QRect& rect) override;

    void RemoveChilds() override;

    std::weak_ptr<IVMTMachine> GetStartMachine() override;
    std::weak_ptr<IVMTMachine> GetFinishMachine() override;

    void SetError(bool bError) override;
    void SetFinish(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine) override;
    void SetFinish(IVMTEnvironment* environment,const QPoint& point) override;

    void SetSelected(bool enable) override;
    void SetStart(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine) override;
    void SetStart(IVMTEnvironment* environment,const QPoint& point) override;

    void SetParent(std::weak_ptr<VMTComplexMachine> parent) override;
    IVMTAlphabitSource *GetAlphabitSource() override;

    ~VMTTransitionImpl();

protected:
    bool _changed;
    bool _error;
    bool _fixed;
    bool _highlighted;
    double _conditions_segment;
    ScreenTools _st;
    VMTTransitionConditions _conditions;

    QPoint _conditions_point;
    QPoint _drag_point;

    QPoint _start_point;
    QPoint _finish_point;
    QRect  _bounds;

    std::weak_ptr<IVMTMachine> _start_machine;
    std::weak_ptr<IVMTMachine> _finish_machine;

    std::weak_ptr<VMTComplexMachine> _parent;
    std::vector<QPoint> _points;


    int  _conditions_segment_number;
    int  _drag_point_number;
    bool _drag_condition;
    size_t  _condition_width;


    void CalculateConditionsPoint(bool fixed);
    bool CheckLineIntersectMachine(int start_position, int end_position);

    bool IsInsideLine(const QPoint& prev, const QPoint& current, const QPoint& point);

    void Update(IVMTEnvironment* environment);
	void UpdateSegment(int start_position, int end_position);

private:
    QRect GetConditionRectangle(UICanvas& canvas);

};
#endif // !defined(EA_605C0DDC_936B_4cfe_BE03_26F5FE6A1A08__INCLUDED_)
