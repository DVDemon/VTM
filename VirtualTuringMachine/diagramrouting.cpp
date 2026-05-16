#include "diagramrouting.h"

#include "interfaces/IVMTTransition.h"

#include <algorithm>

std::int64_t MachineSpatialIndex::cellKey(int x, int y)
{
    return (static_cast<std::int64_t>(x) << 32) ^ static_cast<std::uint32_t>(y);
}

MachineSpatialIndex::MachineSpatialIndex(int cellSize)
    : _cellSize(std::max(8, cellSize))
{
}

void MachineSpatialIndex::addRect(const QRect& rect)
{
    if (rect.isEmpty()) {
        return;
    }

    const int minX = rect.left() / _cellSize;
    const int maxX = rect.right() / _cellSize;
    const int minY = rect.top() / _cellSize;
    const int maxY = rect.bottom() / _cellSize;

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            _cells[cellKey(x, y)].push_back(rect);
        }
    }
}

void MachineSpatialIndex::rebuild(const std::vector<std::shared_ptr<IVMTMachine>>& machines,
                                int clearanceMargin)
{
    _cells.clear();
    for (const std::shared_ptr<IVMTMachine>& machine : machines) {
        if (!machine) {
            continue;
        }

        QRect obstacle = machine->GetBounds();
        if (clearanceMargin > 0) {
            obstacle = obstacle.marginsAdded(
                QMargins(clearanceMargin, clearanceMargin, clearanceMargin, clearanceMargin));
        }
        addRect(obstacle);
    }
}

bool MachineSpatialIndex::contains(const QPoint& point) const
{
    const std::int64_t key = cellKey(point.x() / _cellSize, point.y() / _cellSize);
    const auto it = _cells.find(key);
    if (it == _cells.end()) {
        return false;
    }

    for (const QRect& rect : it->second) {
        if (rect.contains(point)) {
            return true;
        }
    }

    return false;
}

void DiagramRoutingPass::begin(const std::vector<std::shared_ptr<IVMTTransition>>& transitions)
{
    _snapshot.clear();
    _snapshot.reserve(transitions.size());

    for (const std::shared_ptr<IVMTTransition>& transition : transitions) {
        if (!transition) {
            continue;
        }
        const std::vector<QPoint> polyline = transition->GetCommittedRoutingPolyline();
        if (polyline.size() >= 2) {
            _snapshot.push_back({transition.get(), {polyline.begin(), polyline.end()}});
        }
    }

    _active = true;
}

void DiagramRoutingPass::end()
{
    _active = false;
    _snapshot.clear();
}

std::vector<path_t> DiagramRoutingPass::blockedPathsFor(const IVMTTransition* exclude) const
{
    std::vector<path_t> blockedPaths;
    if (!_active) {
        return blockedPaths;
    }

    blockedPaths.reserve(_snapshot.size());
    for (const SnapshotEntry& entry : _snapshot) {
        if (entry.transition == exclude) {
            continue;
        }
        blockedPaths.push_back(entry.polyline);
    }

    return blockedPaths;
}
