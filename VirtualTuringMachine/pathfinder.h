#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QPoint>
#include <QRect>
#include <functional>
#include <vector>


using wall_checker_t = std::function<bool(const QPoint&)>;
using wall_painter_t = std::function<void(const QPoint&)>;
using path_t         = std::vector<QPoint>;

struct Cell {
    QPoint point{0,0};
    bool   wall{};
    bool   visited{};
};

struct Matrix{
    Matrix() {};
    size_t cells_count {0};
    QRect  bounds;
    QPoint start {0,0};
    QPoint finish {0,0};
    size_t grid_size;

    std::pair<size_t,size_t> start_cell {0,0};
    std::pair<size_t,size_t> finish_cell {0,0};
    std::vector<Cell> cells;

    Matrix(const QPoint &start,
           const QPoint &finish,
           const QRect  &bounds,
           size_t grid_size);
    void  clear_visited();
    std::pair<size_t,size_t> get_direction_to_finish(std::pair<size_t,size_t>& current);
    Cell& at(std::pair<size_t,size_t> i);
    QPoint cell_to_point(std::pair<size_t,size_t> i);
    void FillWalls(wall_checker_t checker);
    void PaintWalls();
};

class Pathfinder
{

private:
    Matrix matrix;
    bool CreatePath(Matrix& matrix,path_t& path);

public:
    Pathfinder() {};

    path_t GetPath(const QPoint &start,
                   const QPoint &finish,
                   const QRect  &bounds,
                   size_t grid_size,
                   wall_checker_t checker);

};

#endif // PATHFINDER_H
