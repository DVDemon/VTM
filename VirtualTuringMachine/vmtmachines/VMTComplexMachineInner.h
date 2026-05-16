#ifndef VMTCOMPLEXMACHINEINNER_H
#define VMTCOMPLEXMACHINEINNER_H


#include <QString>
#include <QRect>
#include <QPoint>
#include <vector>
#include <map>

#include "VMTAlphabit.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTTransition.h"
#include "diagramrouting.h"

class VMTComplexMachineInner
{

public:
    VMTComplexMachineInner();
    virtual ~VMTComplexMachineInner();

    VMTComplexMachineInner(QString name,  std::shared_ptr<VMTAlphabit>  alphabit);//,  IVMTEnvironment* environment);
    VMTComplexMachineInner( std::shared_ptr<VMTAlphabit> alphabit);//,  IVMTEnvironment* environment);
    //void SetEnvironment(IVMTEnvironment* environment);
    void ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other);

    //std::vector<VMTComplexMachine> FindComplexMachineByName(QString name);
    //std::vector<VMTComplexMachine> GetComplexMachines();
    QRect GetDiagramBounds();

    std::shared_ptr<VMTAlphabit> GetAlphabit();
    //IVMTEnvironment* GetEnvironment();
    std::vector<std::shared_ptr<IVMTMachine>>& GetMachineCollection();
    std::vector<std::shared_ptr<IVMTTransition>>& GetTransitionCollection();
    void PaintDiagramm(UICanvas& canvas, QRect rect, bool all);

    void beginRoutingPass();
    void endRoutingPass();
    bool isRoutingPassActive() const;
    std::vector<path_t> blockedPathsFor(const IVMTTransition* exclude) const;

private:
    QString _name;
    std::shared_ptr<VMTAlphabit>     _alphabit;
    //IVMTEnvironment *_environment;
    std::vector<std::shared_ptr<IVMTMachine>> _machines;
    std::vector<std::shared_ptr<IVMTTransition>> _transitions;
    DiagramRoutingPass _routingPass;

};
#endif // !defined(EA_701AC423_8239_46bb_B6A5_2B97167167A9__INCLUDED_)
