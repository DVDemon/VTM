#include "pathfinder.h"

#include <QDebug>

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <vector>

namespace {

constexpr int kMargin = 100;

int manhattanCells(std::pair<size_t, size_t> a, std::pair<size_t, size_t> b)
{
    return static_cast<int>(std::abs(static_cast<long>(a.first) - static_cast<long>(b.first)))
           + static_cast<int>(std::abs(static_cast<long>(a.second) - static_cast<long>(b.second)));
}

bool pointNearSegment(const QPoint& point, const QPoint& a, const QPoint& b, int tolerance)
{
    if (a.x() == b.x()) {
        if (std::abs(point.x() - a.x()) > tolerance) {
            return false;
        }
        const int minY = std::min(a.y(), b.y());
        const int maxY = std::max(a.y(), b.y());
        return point.y() >= minY - tolerance && point.y() <= maxY + tolerance;
    }

    if (a.y() == b.y()) {
        if (std::abs(point.y() - a.y()) > tolerance) {
            return false;
        }
        const int minX = std::min(a.x(), b.x());
        const int maxX = std::max(a.x(), b.x());
        return point.x() >= minX - tolerance && point.x() <= maxX + tolerance;
    }

    return false;
}

bool pointNearPolyline(const QPoint& point, const path_t& polyline, int tolerance)
{
    if (polyline.size() < 2) {
        return false;
    }

    for (size_t i = 1; i < polyline.size(); ++i) {
        if (pointNearSegment(point, polyline[i - 1], polyline[i], tolerance)) {
            return true;
        }
    }

    return false;
}

} // namespace

Matrix::Matrix(const QPoint& start, const QPoint& finish, const QRect& bounds, size_t grid_size)
    : start(start), finish(finish), grid_size(grid_size)
{
    this->bounds = QRect(bounds.x() - kMargin,
                         bounds.y() - kMargin,
                         bounds.width() + kMargin * 2,
                         bounds.height() + kMargin * 2);
    this->bounds = this->bounds.united(QRect(start, finish).normalized());

    this->bounds.setBottom((this->bounds.bottom() / static_cast<int>(grid_size)) * static_cast<int>(grid_size)
                           + static_cast<int>(grid_size) / 2);
    this->bounds.setTop((this->bounds.top() / static_cast<int>(grid_size)) * static_cast<int>(grid_size)
                        + static_cast<int>(grid_size) / 2);

    this->bounds.setWidth((this->bounds.width() / static_cast<int>(grid_size)) * static_cast<int>(grid_size));
    this->bounds.setHeight((this->bounds.height() / static_cast<int>(grid_size)) * static_cast<int>(grid_size));

    if (this->bounds.width() < this->bounds.height()) {
        this->bounds.setWidth(this->bounds.height());
    } else {
        this->bounds.setHeight(this->bounds.width());
    }

    cells_count = static_cast<size_t>(this->bounds.width() / static_cast<int>(grid_size));
    if (cells_count < 2) {
        cells_count = 2;
    }

    cells.assign(cells_count * cells_count, Cell {});
    start_cell = point_to_cell(start);
    finish_cell = point_to_cell(finish);

    at(start_cell).wall = false;
    at(finish_cell).wall = false;
}

size_t Matrix::index(std::pair<size_t, size_t> cell) const
{
    return cell.first + cell.second * cells_count;
}

Cell& Matrix::at(std::pair<size_t, size_t> cell)
{
    return cells[index(cell)];
}

const Cell& Matrix::at(std::pair<size_t, size_t> cell) const
{
    return cells[index(cell)];
}

QPoint Matrix::cell_to_point(std::pair<size_t, size_t> cell) const
{
    QPoint result(bounds.left(), bounds.top());
    result.rx() += static_cast<int>(cell.first * bounds.width() / static_cast<int>(cells_count));
    result.ry() += static_cast<int>(cell.second * bounds.height() / static_cast<int>(cells_count));
    return result;
}

std::pair<size_t, size_t> Matrix::point_to_cell(const QPoint& point) const
{
    if (cells_count == 0) {
        return {0, 0};
    }

    const int cellWidth = std::max(1, bounds.width() / static_cast<int>(cells_count));
    const int cellHeight = std::max(1, bounds.height() / static_cast<int>(cells_count));

    size_t bestI = 0;
    size_t bestJ = 0;
    int bestDistance = std::numeric_limits<int>::max();

    const size_t guessI =
        static_cast<size_t>(std::clamp((point.x() - bounds.left()) / cellWidth, 0, static_cast<int>(cells_count) - 1));
    const size_t guessJ =
        static_cast<size_t>(std::clamp((point.y() - bounds.top()) / cellHeight, 0, static_cast<int>(cells_count) - 1));

    for (size_t di = 0; di <= 1; ++di) {
        for (size_t dj = 0; dj <= 1; ++dj) {
            const long i = static_cast<long>(guessI) + static_cast<long>(di) - 1;
            const long j = static_cast<long>(guessJ) + static_cast<long>(dj) - 1;
            if (i < 0 || j < 0 || i >= static_cast<long>(cells_count) || j >= static_cast<long>(cells_count)) {
                continue;
            }

            const std::pair<size_t, size_t> cell {static_cast<size_t>(i), static_cast<size_t>(j)};
            const QPoint center = cell_to_point(cell);
            const int distance = std::abs(center.x() - point.x()) + std::abs(center.y() - point.y());
            if (distance < bestDistance) {
                bestDistance = distance;
                bestI = cell.first;
                bestJ = cell.second;
            }
        }
    }

    return {bestI, bestJ};
}

void Matrix::FillWalls(wall_checker_t checker)
{
    for (size_t i = 0; i < cells_count; ++i) {
        for (size_t j = 0; j < cells_count; ++j) {
            const std::pair<size_t, size_t> cell {i, j};
            if (cell == start_cell || cell == finish_cell) {
                at(cell).wall = false;
                continue;
            }
            at(cell).wall = checker(cell_to_point(cell));
        }
    }
}

void Matrix::markBlockedPaths(const std::vector<path_t>& blockedPaths, int pathCorridor)
{
    for (const path_t& blockedPath : blockedPaths) {
        if (blockedPath.size() < 2) {
            continue;
        }

        for (size_t segmentIndex = 1; segmentIndex < blockedPath.size(); ++segmentIndex) {
            const QPoint& from = blockedPath[segmentIndex - 1];
            const QPoint& to = blockedPath[segmentIndex];
            const int steps = std::max(std::abs(to.x() - from.x()), std::abs(to.y() - from.y()));
            const int stride = std::max(1, steps / static_cast<int>(cells_count) + 1);

            for (int step = 0; step <= steps; step += stride) {
                const double t = steps == 0 ? 0.0 : static_cast<double>(step) / steps;
                const QPoint sample(static_cast<int>(from.x() + (to.x() - from.x()) * t),
                                    static_cast<int>(from.y() + (to.y() - from.y()) * t));

                for (int dx = -pathCorridor; dx <= pathCorridor; dx += pathCorridor / 2 + 1) {
                    for (int dy = -pathCorridor; dy <= pathCorridor; dy += pathCorridor / 2 + 1) {
                        const QPoint probe(sample.x() + dx, sample.y() + dy);
                        const std::pair<size_t, size_t> cell = point_to_cell(probe);
                        if (cell == start_cell || cell == finish_cell) {
                            continue;
                        }
                        at(cell).wall = true;
                    }
                }
            }
        }
    }
}

bool Matrix::segment_is_clear(std::pair<size_t, size_t> from, std::pair<size_t, size_t> to) const
{
    const int x0 = static_cast<int>(from.first);
    const int y0 = static_cast<int>(from.second);
    const int x1 = static_cast<int>(to.first);
    const int y1 = static_cast<int>(to.second);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (true) {
        if (x < 0 || y < 0 || x >= static_cast<int>(cells_count) || y >= static_cast<int>(cells_count)) {
            return false;
        }

        const std::pair<size_t, size_t> cell {static_cast<size_t>(x), static_cast<size_t>(y)};
        if (cell != from && cell != to && at(cell).wall) {
            return false;
        }

        if (x == x1 && y == y1) {
            break;
        }

        const int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (err2 < dx) {
            err += dx;
            y += sy;
        }
    }

    return true;
}

bool Pathfinder::pointBlocksPath(const QPoint& point,
                                 const wall_checker_t& checker,
                                 const std::vector<path_t>& blockedPaths,
                                 int pathCorridor) const
{
    if (checker && checker(point)) {
        return true;
    }

    for (const path_t& blockedPath : blockedPaths) {
        if (pointNearPolyline(point, blockedPath, pathCorridor)) {
            return true;
        }
    }

    return false;
}

path_t Pathfinder::GetPath(const QPoint& start,
                           const QPoint& finish,
                           const QRect& bounds,
                           size_t grid_size,
                           wall_checker_t checker,
                           const std::vector<path_t>& blockedPaths,
                           int pathCorridor)
{
    matrix = Matrix(start, finish, bounds, grid_size);

    const wall_checker_t combinedChecker = [this, checker, blockedPaths, pathCorridor](const QPoint& point) {
        return pointBlocksPath(point, checker, blockedPaths, pathCorridor);
    };

    matrix.FillWalls(combinedChecker);
    matrix.markBlockedPaths(blockedPaths, pathCorridor);

    path_t gridPath;
    if (!searchPath(gridPath)) {
        return {};
    }

    return enforceTerminalDirections(simplifyPath(gridPath));
}

path_t Pathfinder::enforceTerminalDirections(path_t path) const
{
    if (path.empty()) {
        return path;
    }

    const int stub = std::max(1, static_cast<int>(matrix.grid_size / 2));
    path_t result;

    auto appendUnique = [&result](const QPoint& point) {
        if (result.empty() || result.back() != point) {
            result.push_back(point);
        }
    };

    appendUnique(path.front());

    // Исходящий участок: первый шаг только вправо от точки старта.
    if (path.size() == 1 || path[1].x() <= path.front().x()) {
        appendUnique(QPoint(path.front().x() + stub, path.front().y()));
    }

    for (size_t i = 1; i < path.size(); ++i) {
        appendUnique(path[i]);
    }

    // Входящий участок: последний шаг только слева к точке финиша.
    if (result.size() >= 2) {
        const QPoint& prev = result[result.size() - 2];
        QPoint& end = result.back();
        if (end.x() >= prev.x()) {
            result.insert(result.end() - 1, QPoint(end.x() - stub, end.y()));
        }
    }

    return result;
}

bool Pathfinder::searchPath(path_t& path)
{
    const size_t cellCount = matrix.cells_count * matrix.cells_count;
    std::vector<int> gScore(cellCount, std::numeric_limits<int>::max());
    std::vector<int> parent(cellCount, -1);

    const size_t startIndex = matrix.index(matrix.start_cell);
    const size_t finishIndex = matrix.index(matrix.finish_cell);

    struct Node {
        int fScore;
        size_t index;
    };

    auto cmp = [](const Node& lhs, const Node& rhs) { return lhs.fScore > rhs.fScore; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> open(cmp);

    gScore[startIndex] = 0;
    parent[startIndex] = -1;
    open.push({manhattanCells(matrix.start_cell, matrix.finish_cell), startIndex});

    static const std::pair<int, int> kDirections[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::vector<bool> closed(cellCount, false);

    while (!open.empty()) {
        const size_t currentIndex = open.top().index;
        open.pop();

        if (closed[currentIndex]) {
            continue;
        }
        closed[currentIndex] = true;

        if (currentIndex == finishIndex) {
            path.clear();
            for (int cursor = static_cast<int>(finishIndex); cursor >= 0; cursor = parent[static_cast<size_t>(cursor)]) {
                const size_t i = static_cast<size_t>(cursor) % matrix.cells_count;
                const size_t j = static_cast<size_t>(cursor) / matrix.cells_count;
                path.push_back(matrix.cell_to_point({i, j}));
                if (static_cast<size_t>(cursor) == startIndex) {
                    break;
                }
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        const size_t currentI = currentIndex % matrix.cells_count;
        const size_t currentJ = currentIndex / matrix.cells_count;
        const std::pair<size_t, size_t> current {currentI, currentJ};

        for (const auto& direction : kDirections) {
            const long nextI = static_cast<long>(currentI) + direction.first;
            const long nextJ = static_cast<long>(currentJ) + direction.second;
            if (nextI < 0 || nextJ < 0 || nextI >= static_cast<long>(matrix.cells_count)
                || nextJ >= static_cast<long>(matrix.cells_count)) {
                continue;
            }

            const std::pair<size_t, size_t> next {static_cast<size_t>(nextI), static_cast<size_t>(nextJ)};
            if (matrix.at(next).wall) {
                continue;
            }

            const size_t nextIndex = matrix.index(next);
            const int tentative = gScore[currentIndex] + 1;
            if (tentative >= gScore[nextIndex]) {
                continue;
            }

            parent[nextIndex] = static_cast<int>(currentIndex);
            gScore[nextIndex] = tentative;
            const int fScore = tentative + manhattanCells(next, matrix.finish_cell);
            open.push({fScore, nextIndex});
        }
    }

    path.clear();
    return false;
}

path_t Pathfinder::simplifyPath(const path_t& gridPath) const
{
    if (gridPath.size() <= 2) {
        return gridPath;
    }

    std::vector<std::pair<size_t, size_t>> cells;
    cells.reserve(gridPath.size());
    for (const QPoint& point : gridPath) {
        cells.push_back(matrix.point_to_cell(point));
    }

    path_t simplified;
    simplified.push_back(gridPath.front());

    size_t anchor = 0;
    for (size_t probe = anchor + 1; probe < cells.size(); ++probe) {
        if (!matrix.segment_is_clear(cells[anchor], cells[probe])) {
            simplified.push_back(gridPath[probe - 1]);
            anchor = probe - 1;
        }
    }

    if (simplified.back() != gridPath.back()) {
        simplified.push_back(gridPath.back());
    }

    path_t orthogonal;
    orthogonal.push_back(simplified.front());
    for (size_t i = 1; i < simplified.size(); ++i) {
        const QPoint& from = orthogonal.back();
        const QPoint& to = simplified[i];

        if (from.x() != to.x() && from.y() != to.y()) {
            orthogonal.emplace_back(to.x(), from.y());
        }

        if (orthogonal.back() != to) {
            orthogonal.push_back(to);
        }
    }

    return orthogonal;
}
