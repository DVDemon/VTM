#ifndef VMTCOMPLEXMACHINEINNER_H
#define VMTCOMPLEXMACHINEINNER_H

#include <QPoint>
#include <QRect>
#include <QString>
#include <vector>

#include "VMTAlphabit.h"
#include "diagramrouting.h"
#include "interfaces/IVMTEnvironment.h"
#include "interfaces/IVMTTransition.h"

class VMTComplexMachineInner {
   public:
    VMTComplexMachineInner();
    virtual ~VMTComplexMachineInner();

    VMTComplexMachineInner(QString name, std::shared_ptr<VMTAlphabit> alphabit);
    VMTComplexMachineInner(std::shared_ptr<VMTAlphabit> alphabit);

    void ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other);

    QRect GetDiagramBounds();

    std::shared_ptr<VMTAlphabit> GetAlphabit();
    std::vector<std::shared_ptr<IVMTMachine>>& GetMachineCollection();
    std::vector<std::shared_ptr<IVMTTransition>>& GetTransitionCollection();
    void PaintDiagramm(UICanvas& canvas, QRect rect, bool all);

    void beginRoutingPass();
    void endRoutingPass();
    bool isRoutingPassActive() const;
    std::vector<path_t> blockedPathsFor(const IVMTTransition* exclude) const;

   private:
    QString _name;
    std::shared_ptr<VMTAlphabit> _alphabit;
    std::vector<std::shared_ptr<IVMTMachine>> _machines;
    std::vector<std::shared_ptr<IVMTTransition>> _transitions;
    DiagramRoutingPass _routingPass;
};
#endif  // !defined(EA_701AC423_8239_46bb_B6A5_2B97167167A9__INCLUDED_)
