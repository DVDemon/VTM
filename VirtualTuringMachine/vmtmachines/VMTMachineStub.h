#if !defined(EA_978A931C_4910_4dd8_9125_0E050BC6179C__INCLUDED_)
#define EA_978A931C_4910_4dd8_9125_0E050BC6179C__INCLUDED_


#include "VMTLine.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTMachine.h"
#include "interfaces/IVMTTransition.h"

#include "../screentools.h"

class VMTComplexMachine;

class VMTMachineStub : public IVMTMachine
{
protected:
    QRect _internal_bounds;
    QRect _bounds;
    QRect _bounds_with_childs;
    QPoint _center;
    QPoint _size;

    QPoint _input_point;
    QPoint _output_point;

    bool _error;
    bool _selected;
    long _power;

    IVMTMachine::MachineType _id;
    QString _name;
    std::weak_ptr<VMTComplexMachine> _parent;


    std::vector<std::weak_ptr<IVMTTransition>> _incoming;
    std::vector<std::weak_ptr<IVMTTransition>> _outgoing;

    ScreenTools _st;
public:

    static std::shared_ptr<IVMTMachine> CreateMachineByID(MachineType id, std::weak_ptr<VMTComplexMachine> parent);

    VMTMachineStub(const QString name,std::weak_ptr<VMTComplexMachine> parent, IVMTMachine::MachineType id,ImageType it);

    std::shared_ptr<IVMTMachine> CreateComplexMachine(IVMTEnvironment*) override;
    void Serialize(QDataStream&) override;
    void Deserialize(QDataStream&) override;

     void Update(IVMTEnvironment* environment) override;


    void AddIncomingTransition(std::weak_ptr<IVMTTransition> transition) override;
    void AddOutgoingTransition(std::weak_ptr<IVMTTransition> transition) override;

    void RemoveIncomingTransition(std::shared_ptr<IVMTTransition> transition) override;
    void RemoveOutgoingTransition(std::shared_ptr<IVMTTransition> transition) override;


    void DoAction(std::shared_ptr<VMTLine> line) override;
    QPoint& GetSize() override;
    const QRect& GetBounds() override;
    const QRect& GetBoundsWithChilds() override;
    const QPoint& GetCenter() override;
    MachineType GetID() override;
    std::weak_ptr<VMTComplexMachine> GetParent() override;

    const std::vector<std::weak_ptr<IVMTTransition>>& GetIncomingTransitions() override;
    const std::vector<std::weak_ptr<IVMTTransition>>& GetOutgoingTransitions() override;

    const QPoint& GetInputPoint() override;
    const QString& GetMachineName() override;
    const QString& GetName() override;
    const QPoint& GetOutputPoint() override;

    IVMTAlphabitSource *GetAlphabitSource() override;

    long GetPower() override;
    bool IsAcceptIncoming() override;
    bool IsAcceptOutgoing() override;
    bool IsAlphabit() override;
    bool IsComplex() override;
    bool IsError() override;
    bool IsInside(const QPoint& point) override;
    bool IsPowerCalculator() override;
    bool IsSelected() override;

    void Move(const QPoint& center,IVMTEnvironment* environment) override;
    void Move(const QPoint&& center,IVMTEnvironment* environment) override;
    void Paint(UICanvas& canvas, const QRect& rect) override;
    void RemoveChilds() override;
    void RemoveIncomingTransition(IVMTTransition* transition) override;
    void RemoveOutgoingTransition(IVMTTransition* transition) override;

    void SetError(bool error) override;
    void SetParent(std::weak_ptr<VMTComplexMachine> parent) override;
    void SetPower(long power) override;
    void SetSelected(bool selected) override;

    virtual ~VMTMachineStub();



};
#endif // !defined(EA_978A931C_4910_4dd8_9125_0E050BC6179C__INCLUDED_)
