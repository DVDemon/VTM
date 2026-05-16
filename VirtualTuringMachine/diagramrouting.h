#ifndef DIAGRAMROUTING_H
#define DIAGRAMROUTING_H

#include "interfaces/IVMTMachine.h"
#include "pathfinder.h"

#include <QPoint>
#include <QRect>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

class IVMTTransition;

class MachineSpatialIndex
{
public:
    explicit MachineSpatialIndex(int cellSize = 64);

    void rebuild(const std::vector<std::shared_ptr<IVMTMachine>>& machines, int clearanceMargin = 0);
    bool contains(const QPoint& point) const;

private:
    int _cellSize = 64;
    std::unordered_map<std::int64_t, std::vector<QRect>> _cells;

    static std::int64_t cellKey(int x, int y);
    void addRect(const QRect& rect);
};

class DiagramRoutingPass
{
public:
    void begin(const std::vector<std::shared_ptr<IVMTTransition>>& transitions);
    void end();
    bool active() const { return _active; }

    std::vector<path_t> blockedPathsFor(const IVMTTransition* exclude) const;

private:
    struct SnapshotEntry {
        const IVMTTransition* transition = nullptr;
        path_t polyline;
    };

    bool _active = false;
    std::vector<SnapshotEntry> _snapshot;
};

#endif // DIAGRAMROUTING_H
