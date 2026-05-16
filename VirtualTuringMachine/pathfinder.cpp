#include "pathfinder.h"
#include <QDebug>
#include <cmath>

static const int MARGIN=100;

Matrix::Matrix(const QPoint &start,
               const QPoint &finish,
               const QRect  &bounds,
               size_t grid_size){
    Matrix::grid_size   = grid_size;
    Matrix::start    = start;
    Matrix::finish   = finish;
    Matrix::bounds   = {bounds.x()-MARGIN,
                       bounds.y() - MARGIN,
                       bounds.width()+MARGIN*2,
                       bounds.height()+MARGIN*2};
    QRect r1(start,finish);
    Matrix::bounds = Matrix::bounds.united(r1.normalized());

    Matrix::bounds.setBottom((Matrix::bounds.bottom()/grid_size)*grid_size+grid_size/2);
    Matrix::bounds.setTop((Matrix::bounds.top()/grid_size)*grid_size+grid_size/2);

    Matrix::bounds.setWidth((Matrix::bounds.width()/grid_size)*grid_size);
    Matrix::bounds.setHeight((Matrix::bounds.height()/grid_size)*grid_size);

    if(Matrix::bounds.width()<Matrix::bounds.height()) Matrix::bounds.setWidth(Matrix::bounds.height());
                                                  else Matrix::bounds.setHeight(Matrix::bounds.width());


    Matrix::cells_count = Matrix::bounds.width()/grid_size;
    cells.resize(cells_count*cells_count);

    for(size_t i=0;i<cells_count;++i)
        for(size_t j=0;j<cells_count;++j){
            QPoint current = cell_to_point({i,j});
            QPoint current_start = cell_to_point(start_cell);
            QPoint current_finish = cell_to_point(finish_cell);

            if(abs(current.x()-start.x())+abs(current.y()-start.y())<
                abs(current_start.x()-start.x())+abs(current_start.y()-start.y())){
                start_cell.first = i;
                start_cell.second = j;
            }

            if(abs(current.x()-finish.x())+abs(current.y()-finish.y())<
                abs(current_finish.x()-finish.x())+abs(current_finish.y()-finish.y())){
                finish_cell.first = i;
                finish_cell.second = j;
            }
        }


    qDebug() << "";
    qDebug() << "bounds ["<<bounds.left()<<","<<bounds.top()<<","<<bounds.width()<<","<<bounds.height()<<"]";
    qDebug() << "start ["<<start.x() <<"," << start.y() <<"] -> ["<< start_cell.first <<"," << start_cell.second <<"]";
    qDebug() << "finish ["<<finish.x() <<"," << finish.y() <<"] -> ["<< finish_cell.first <<"," << finish_cell.second <<"]";

}

void  Matrix::clear_visited(){
    for(auto &c : cells) c.visited = false;
}

std::pair<size_t,size_t> Matrix::get_direction_to_finish([[maybe_unused]] std::pair<size_t,size_t>& current){
    std::pair<size_t,size_t> result;
    return result;
}

Cell& Matrix::at(std::pair<size_t,size_t> i){
    return cells[i.first+i.second*cells_count];
}

QPoint Matrix::cell_to_point(std::pair<size_t,size_t> i){
    QPoint result{bounds.left(),bounds.top()};
    result.rx() += i.first * bounds.width() / cells_count;
    result.ry() += i.second * bounds.height() / cells_count;
    return result;
}


void Matrix::FillWalls(wall_checker_t checker){

    for(size_t i=0;i<cells_count;++i)
        for(size_t j=0;j<cells_count;++j){
            at({i,j}).wall = checker(cell_to_point({i,j}));
        }
}

void Matrix::PaintWalls(){

    for(size_t j=0;j<cells_count;++j)
    {
        QString str;
        for(size_t i=0;i<cells_count;++i){
            if(start_cell == std::pair<size_t,size_t>{i,j}) str+="[S]";
            else if(finish_cell == std::pair<size_t,size_t>{i,j}) str+="[F]";
            else if(at({i,j}).wall) str+="[X]";
            else if(at({i,j}).visited) str+="[o]";
            else str+="[ ]";
            //painter(cell_to_point({i,j}));
        }
        qDebug() << str;
    }
}

path_t Pathfinder::GetPath(const QPoint &start,
                           const QPoint &finish,
                           const QRect  &bounds,
                           size_t grid_size,
                           wall_checker_t checker){
    qDebug() << "Pathfinder: create matrix";
    matrix = Matrix(start,finish,bounds,grid_size);
    qDebug() << "Pathfinder: fill wals";
    matrix.FillWalls(checker);


    path_t result;

    if(!CreatePath(matrix,result)) {
        qDebug() << "Pathfinder: there are no path";
    } //else matrix.PaintWalls();

    return result;
}


#include <stack>
bool Pathfinder::CreatePath(Matrix& matrix,path_t& path){
    qDebug() << "Pathfinder: create path";

    std::pair<size_t,size_t> current;
    std::stack<std::pair<size_t,size_t>> stack;

    stack.push(matrix.start_cell);

    do{
        current = stack.top();
        stack.pop();


        if(current!=matrix.finish_cell)
        {
            path.push_back(matrix.cell_to_point(current));
            matrix.at(current).visited = true;
            std::vector<std::pair<size_t,size_t>> to_add
                {
                    {current.first-1,current.second},
                    {current.first+1,current.second},
                    {current.first,current.second-1},
                    {current.first,current.second+1}
            };

            std::sort(std::begin(to_add),std::end(to_add),
                      [&](const std::pair<size_t,size_t>&lhv,std::pair<size_t,size_t>&rhv)->bool{
                            auto len1 = ((int)lhv.first-(int)matrix.finish_cell.first)*((int)lhv.first-(int)matrix.finish_cell.first)+
                                        ((int)lhv.second-(int)matrix.finish_cell.second)*((int)lhv.second-(int)matrix.finish_cell.second);
                            auto len2 = ((int)rhv.first-(int)matrix.finish_cell.first)*((int)rhv.first-(int)matrix.finish_cell.first)+
                                        ((int)rhv.second-(int)matrix.finish_cell.second)*((int)rhv.second-(int)matrix.finish_cell.second);
                            return len1>len2;
                        });

            for(auto& p: to_add)
                if(p.first<matrix.cells_count)
                if(p.second<matrix.cells_count)
                if(!matrix.at(p).wall)
                if(!matrix.at(p).visited)
                if(p!=matrix.start_cell)
                    stack.push(p);
         }

    }while((current!=matrix.finish_cell)&&(!stack.empty()));


    if(current!=matrix.finish_cell) {
         path.clear();
         return false;
    }

    qDebug() << "Before compact";
    for(const auto &p:path) qDebug() << "[" << p.x() << "," << p.y() << "]";

//    // try to cut edges
    std::vector<bool> path_optimizer;
    path_optimizer.resize(path.size(),false);

    for(int i=2;i<log2(path.size());i++){
         int step = path.size()/pow(2,i);
         qDebug() << "step:" << step;

         if(step>2)
            for(int j=step;j<path.size();j+=step){
                auto &p1 = path[j-step+1];
                auto &p2 = path[j-1];

                double dx = double(p2.x()) - double(p1.x());
                double dy = double(p2.y()) - double(p1.y());

                double length = std::max(abs(dx),abs(dy));
                double dx_inc = dx/length;
                double dy_inc = dy/length;

                bool can_be_removed = true;

                qDebug() << "[" << p1.x() <<"," <<p1.y() << "] -> [" << p2.x() <<"," <<p2.y() << "]";

                for(double d =0 ;can_be_removed&&(d<length);d+=1){
                    qDebug() << "[" << p1.x()+dx_inc*d <<"," <<p1.y()+dy_inc*d << "] -> " << matrix.at({p1.x()+dx_inc*d, p1.y()+dy_inc*d}).wall;
                    can_be_removed = !matrix.at({p1.x()+dx_inc*d,
                                                 p1.y()+dy_inc*d}).wall;
                }

                if(can_be_removed)
                    for(size_t jj=j-step+1;jj<j-1;++jj)
                        path_optimizer[jj] = true;

            }
    }

    qDebug() << "Can be removed";
    for(const auto &p:path_optimizer) qDebug() << "[" << p << "]";

    path_t op_path;
    for(size_t i=0;i<path.size();++i)
         if(!path_optimizer[i]) op_path.push_back(path[i]);

    // creating straght lines from point path
    bool compacting_x = false;
    bool compacting_y = false;
    QPoint prev{-1000,-1000};
    path_t new_path;
    for(const auto &p:op_path) {

         if(p.x() == prev.x()){
            if(compacting_y) {
                compacting_y = false;
                new_path.push_back(prev);
            }
            if(compacting_x){

            } else compacting_x = true;

         } else
            if(p.y() == prev.y()){

            if(compacting_x) {
                compacting_x = false;
                new_path.push_back(prev);
            }
            if(compacting_y){

            } else compacting_y = true;

         } else
         {
            if(compacting_x||compacting_y) new_path.push_back(prev);
            compacting_x = false;
            compacting_y = false;
            new_path.push_back(p);
         }

         prev = p;
    }

    if(compacting_x||compacting_y) new_path.push_back(path.back());

    qDebug() << "After compact";
    path = new_path;
    for(const auto &p:new_path) qDebug() << "[" << p.x() << "," << p.y() << "]";

    return true;
}
