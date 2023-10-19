#if !defined(EA_B62B4E89_40FD_4575_97A8_8AF48A7036B8__INCLUDED_)
#define EA_B62B4E89_40FD_4575_97A8_8AF48A7036B8__INCLUDED_



#include <memory>
#include <vector>
#include <QPoint>
#include <QRect>
#include <QString>
#include <vector>

//#include "IVMTMachine.h"
//#include "IVMTTransition.h"
//#include "IVMTEnvironment.h"
//#include "VMTAlphabit.h"
#include "VMTMachineStub.h"
#include "VMTComplexMachineInner.h"
class VMTUndoElementMachine;

class VMTComplexMachine : public VMTMachineStub
{
protected:
    std::shared_ptr<VMTComplexMachineInner> _inner;

    static QPoint calculateDimension(const QString name, IVMTEnvironment* environment);
    void CheckBounds(const QPoint &p,bool first);
    QString _nick_name;
public:
    VMTComplexMachine(const QString& name,  std::shared_ptr<VMTAlphabit> alphabit);
    VMTComplexMachine(const VMTComplexMachine &other);


    void DeselectAll();
    void Update(IVMTEnvironment* environment) override;
    void UpdateChilds(IVMTEnvironment* environment);
    std::shared_ptr<IVMTMachine> Clone(IVMTEnvironment* environment) override;
    std::shared_ptr<VMTComplexMachine> CreatePlainCopy(IVMTEnvironment* environment);
    void AddMachine(std::shared_ptr<IVMTMachine> machine);
    void AddTransition(std::shared_ptr<IVMTTransition> transition);


    static std::shared_ptr<IVMTTransition> ConnectMachines(std::shared_ptr<IVMTMachine> first,std::shared_ptr<IVMTMachine> second,IVMTEnvironment *environment,std::shared_ptr<VMTComplexMachine> parent);

    void ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit) override;

    std::weak_ptr<IVMTMachine>    FindMachine(IVMTEnvironment* environment,const QPoint& point);
    std::weak_ptr<IVMTTransition> FindTransition(IVMTEnvironment* environment,const QPoint& p_Point);
    std::shared_ptr<VMTAlphabit>  GetAlphabit();

    void SetInnerObject(std::shared_ptr<VMTComplexMachineInner> other,std::shared_ptr<VMTComplexMachine> current);
    std::shared_ptr<VMTComplexMachineInner> GetInnerObject();
    const QRect& GetBoundsWithChilds() override;
    std::vector<IVMTMachine*> GetMachines();
    std::vector<std::shared_ptr<IVMTMachine>>& GetMachineCollection();
    std::vector<std::shared_ptr<IVMTTransition>>& GetTransitionCollection();

    bool IsComplex() override;

    void Paint(UICanvas& canvas, const QRect& rect) override;
    void PaintDiagram(UICanvas& canvas, const QRect& rect);

    void RemoveComplexMachine(std::shared_ptr<VMTComplexMachine> machine);
    void RemoveMachine(std::shared_ptr<IVMTMachine> machine);
    void RemoveTransition(std::shared_ptr<IVMTTransition> transition);
    void ReplaceMachine(VMTComplexMachine* machine);
    void SetName(const QString& name);
    const QString GetNickName();



    virtual ~VMTComplexMachine();

};
#endif // !defined(EA_B62B4E89_40FD_4575_97A8_8AF48A7036B8__INCLUDED_)
