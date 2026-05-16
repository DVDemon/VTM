#include <QDebug>
#include "VMTComplexMachineInner.h"


VMTComplexMachineInner::~VMTComplexMachineInner(){
  //qDebug() << "Complex machine deleted";
}

VMTComplexMachineInner::VMTComplexMachineInner(QString name,  std::shared_ptr<VMTAlphabit>  alphabit) :
    _name(name), _alphabit(alphabit) {
}

VMTComplexMachineInner::VMTComplexMachineInner( std::shared_ptr<VMTAlphabit> alphabit) ://,  IVMTEnvironment* environment):
    _alphabit(alphabit){
}

void VMTComplexMachineInner::ChangeAlphabit(const std::shared_ptr<VMTAlphabit> other){

    if(_alphabit!=other){
      _alphabit = other;
      qDebug() << "VMTComplexMachineInner::ChangeAlphabit";

      for(std::shared_ptr<IVMTMachine> m : _machines ){
          m->ChangeAlphabit(_alphabit);
      }
      for(std::shared_ptr<IVMTTransition> t : _transitions ){
          t->ChangeAlphabit(_alphabit);
      }
    }

}

QRect VMTComplexMachineInner::GetDiagramBounds(){
 return QRect(0,0,0,0);
}

std::shared_ptr<VMTAlphabit> VMTComplexMachineInner::GetAlphabit(){
 return _alphabit;
}

std::vector<std::shared_ptr<IVMTMachine>>& VMTComplexMachineInner::GetMachineCollection(){
 return _machines;
}

std::vector<std::shared_ptr<IVMTTransition>>& VMTComplexMachineInner::GetTransitionCollection(){
 return _transitions;
}

void VMTComplexMachineInner::PaintDiagramm(UICanvas &canvas, QRect rect, [[maybe_unused]] bool all){
  for(std::shared_ptr<IVMTMachine> m : _machines ){
      if (!rect.isEmpty() && !rect.intersects(m->GetBounds())) {
          continue;
      }
      m->Paint(canvas,rect);
  }
  for(std::shared_ptr<IVMTTransition> t : _transitions ){
      const QRect& transitionBounds = t->GetBounds();
      if (!rect.isEmpty() && !transitionBounds.isEmpty() && !rect.intersects(transitionBounds)) {
          continue;
      }
      t->Paint(canvas,rect);
  }
}

void VMTComplexMachineInner::beginRoutingPass()
{
    _routingPass.begin(_transitions);
}

void VMTComplexMachineInner::endRoutingPass()
{
    _routingPass.end();
}

bool VMTComplexMachineInner::isRoutingPassActive() const
{
    return _routingPass.active();
}

std::vector<path_t> VMTComplexMachineInner::blockedPathsFor(const IVMTTransition* exclude) const
{
    if (_routingPass.active()) {
        return _routingPass.blockedPathsFor(exclude);
    }

    std::vector<path_t> blockedPaths;
    blockedPaths.reserve(_transitions.size());
    for (const std::shared_ptr<IVMTTransition>& transition : _transitions) {
        if (!transition || transition.get() == exclude) {
            continue;
        }
        const std::vector<QPoint> polyline = transition->GetRoutingPolyline();
        if (polyline.size() >= 2) {
            blockedPaths.emplace_back(polyline.begin(), polyline.end());
        }
    }
    return blockedPaths;
}

