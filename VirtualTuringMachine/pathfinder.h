#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QPoint>
#include <QRect>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

using wall_checker_t = std::function<bool(const QPoint&)>;
using wall_painter_t = std::function<void(const QPoint&)>;
using path_t         = std::vector<QPoint>;

struct Cell {
    bool wall = false;
};

struct Matrix {
    size_t cells_count_x = 0;
    size_t cells_count_y = 0;
    QRect bounds;
    QPoint start;
    QPoint finish;
    size_t grid_size = 0;

    std::pair<size_t, size_t> start_cell {0, 0};
    std::pair<size_t, size_t> finish_cell {0, 0};
    std::vector<Cell> cells;

    Matrix() = default;
    Matrix(const QPoint& start, const QPoint& finish, const QRect& bounds, size_t grid_size);

    Cell& at(std::pair<size_t, size_t> cell);
    const Cell& at(std::pair<size_t, size_t> cell) const;
    size_t index(std::pair<size_t, size_t> cell) const;
    QPoint cell_to_point(std::pair<size_t, size_t> cell) const;
    std::pair<size_t, size_t> point_to_cell(const QPoint& point) const;

    void FillWalls(wall_checker_t checker);
    void markBlockedPaths(const std::vector<path_t>& blockedPaths, int pathCorridor);
    bool segment_is_clear(std::pair<size_t, size_t> from, std::pair<size_t, size_t> to) const;
};

class Pathfinder
{
public:
    Pathfinder() = default;

    path_t GetPath(const QPoint& start,
                   const QPoint& finish,
                   const QRect& bounds,
                   size_t grid_size,
                   wall_checker_t checker,
                   const std::vector<path_t>& blockedPaths = {},
                   int pathCorridor = 6);

private:
    Matrix matrix;
    bool searchPath(path_t& path);
    path_t simplifyPath(const path_t& gridPath) const;
    path_t enforceTerminalDirections(path_t path) const;
    bool pointBlocksPath(const QPoint& point,
                         const wall_checker_t& checker,
                         const std::vector<path_t>& blockedPaths,
                         int pathCorridor) const;
};

#endif // PATHFINDER_H
