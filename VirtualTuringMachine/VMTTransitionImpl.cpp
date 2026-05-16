#include "VMTTransitionImpl.h"
#include "vmtmachines/VMTComplexMachine.h"
#include <algorithm>
#include <cmath>
#define PRECISION 5

namespace {

void updateTransitionBoundsFromPoints(const std::vector<QPoint>& points, QRect& bounds)
{
    if (points.empty()) {
        return;
    }

    bounds.setLeft(points[0].x());
    bounds.setTop(points[0].y());
    bounds.setWidth(0);
    bounds.setHeight(0);

    for (const QPoint& p : points) {
        if (bounds.left() > p.x()) {
            bounds.setLeft(p.x());
        }
        if (bounds.top() > p.y()) {
            bounds.setTop(p.y());
        }
        if ((bounds.left() + bounds.width()) < p.x()) {
            bounds.setWidth(p.x() - bounds.left());
        }
        if ((bounds.top() + bounds.height()) < p.y()) {
            bounds.setHeight(p.y() - bounds.top());
        }
    }
}

int routingStubLength(int gridStep)
{
    return std::max(gridStep * 2, 20);
}

void buildRubberBandRoute(const QPoint& startOutput,
                          const QPoint& finishInput,
                          int stub,
                          std::vector<QPoint>& points)
{
    points.clear();
    points.push_back(startOutput);

    QPoint current = points.back();
    points.push_back(QPoint(current.x() + stub, current.y()));
    points.push_back(finishInput);

    current = points.back();
    points.insert(points.begin() + static_cast<std::ptrdiff_t>(points.size() - 1),
                  QPoint(current.x() - stub, current.y()));
}

} // namespace

VMTTransitionImpl::VMTTransitionImpl(std::weak_ptr<VMTComplexMachine> parent) :
    _conditions(this, parent.lock()->GetAlphabit()), _parent(parent){
    _fixed = false;
    _changed = true;
    _highlighted = false;
    _error = false;
    _conditions_segment_number = 2;
    _conditions_segment = 0.5;
    _drag_point_number = -1;
    _drag_condition    = false;

    ScreenTools st;
    _condition_width = st.GetFontSize();
}

void VMTTransitionImpl::SetParent(std::weak_ptr<VMTComplexMachine> parent){
    _parent = parent;
}
std::weak_ptr<IVMTMachine> VMTTransitionImpl::GetStartMachine(){
    return _start_machine;
}

std::weak_ptr<IVMTMachine> VMTTransitionImpl::GetFinishMachine(){
    return _finish_machine;
}

void VMTTransitionImpl::SetConditions(VMTTransitionConditions& other){
    this->_conditions = other;
}

void VMTTransitionImpl::Serialize(QDataStream& stream){
     stream << true;
     stream << (double) _start_point.x();
     stream << (double) _start_point.y();

     stream << true;
     stream << (double) _finish_point.x();
     stream << (double) _finish_point.y();

     stream << (double) _conditions_point.x();
     stream << (double) _conditions_point.y();

     stream << (qint32) _points.size();

    for(auto point: _points){
        stream << (double) point.x();
        stream << (double) point.y();
    }

    stream << (qint32) _conditions_segment_number;
    stream << (double) _conditions_segment;
     stream << _fixed;
    _conditions.Serialize(stream);

}

void VMTTransitionImpl::Deserialize(QDataStream& stream){
    bool flag;
    double value;

    stream >> flag;
    qDebug() << "flag:" << flag;
    if(flag){
    stream >> value; _start_point.setX(value);
    qDebug() << "x:" << value;
    stream >> value; _start_point.setY(value);
    qDebug() << "y:" << value;
    }
    stream >> flag;
    qDebug() << "flag:" << flag;
    if(flag)
    {

    stream >> value; _finish_point.setX(value);
    qDebug() << "x:" << value;
    stream >> value; _finish_point.setY(value);
    qDebug() << "y:" << value;
    }

    stream >> value; _conditions_point.setX(value);
    qDebug() << "x:" << value;
    stream >> value; _conditions_point.setY(value);
    qDebug() << "y:" << value;

    qint32 size;

    stream >> size;
    qDebug() << "size:" << size;
   _points.clear();
   for(int i=0;i<size;i++){
       QPoint point;
       stream >> value; point.setX(value);
       qDebug() << "x:" << value;
       stream >> value; point.setY(value);
       qDebug() << "y:" << value;
       _points.push_back(point);
   }

   qint32 value_int;
   stream >> value_int;_conditions_segment_number = value_int;
   stream >> value;_conditions_segment = value;

   stream >> _fixed;
   _conditions.Deserialize(stream);
   _changed = false;
   _committedPoints = _points;
   updateTransitionBoundsFromPoints(_points, _bounds);

}

IVMTAlphabitSource *VMTTransitionImpl::GetAlphabitSource(){
    return &_conditions;
}

std::vector<QPoint> VMTTransitionImpl::GetRoutingPolyline() const{
    if (!_committedPoints.empty()) {
        return _committedPoints;
    }
    return _points;
}

void VMTTransitionImpl::ChangeAlphabit(std::shared_ptr<VMTAlphabit> alphabit){
    _conditions.ChangeAlphabit(alphabit);
}

void VMTTransitionImpl::BeginDrag(IVMTEnvironment* environment,const QPoint& point){

    _drag_point_number = -1;
    _drag_condition    = false;

    for(size_t i=2;(_drag_point_number==-1)&&(i<_points.size()-2);i++)
    {
        QPoint current = _points[i];

        if ((current.x()-PRECISION<=point.x())&&
                (current.x()+PRECISION>=point.x())&&
                (current.y()-PRECISION<=point.y())&&
                (current.y()+PRECISION>=point.y()))
        {
            _drag_point.rx() =  current.x()-point.x();
            _drag_point.ry() = current.y()-point.y();
            _drag_point_number = i;
        }
    }

    if (_drag_point_number==-1)
    {
        QRect rect = GetConditionRectangle(environment->GetGraphics());

        if (rect.contains(point))
        {
            _drag_point.rx() = _conditions_point.x()-point.x();
            _drag_point.ry() = _conditions_point.y()-point.y();
            _drag_condition = true;
        }
    }
}

void VMTTransitionImpl::Drag([[maybe_unused]] IVMTEnvironment* environment,const QPoint& point){

    if (_drag_point_number==-1)
    {
        if(_drag_condition)
        {
            bool done = false;
            int iterations = 0;
            do
            {
                iterations ++;
                QPoint prev = _points[_conditions_segment_number];
                QPoint current = _points[_conditions_segment_number+1];


                double a= current.y() - prev.y();
                double b= current.x() - prev.x();
                double dK = (a*(point.y()-prev.y())+b*(point.x()-prev.x()))/(a*a+b*b);

                if ((iterations<2)&&(dK<0))
                {
                    if (_conditions_segment_number>0) {

                            _conditions_segment_number--;
                    }
                    else
                    {
                        dK=0;
                        done = true;
                    }
                } else
                    if ((iterations<2)&&(dK>1))
                    {
                    if (_conditions_segment_number<static_cast<int>(_points.size())-2) _conditions_segment_number++;
                        else
                        {
                            dK=1;
                            done = true;
                        }
                    } else done = true;

                if(done)
                {
                    if(dK<0) dK=0;
                    if(dK>1) dK = 1;
                    _conditions_segment = dK;
                    done = true;
                }

            }while(!done);
        }
    } else
    {
        if((_drag_point_number>=0)&&(_drag_point_number<static_cast<int>(_points.size())))
                _points[_drag_point_number] = QPoint(point.x()+_drag_point.x(),point.y()+_drag_point.y());
    }

    CalculateConditionsPoint(_fixed);

    _bounds.setLeft(_points[0].x());
    _bounds.setTop(_points[0].y());
    _bounds.setWidth(0);
    _bounds.setHeight(0);

    for(QPoint p: _points){
        if(_bounds.left()>p.x()) {_bounds.setLeft(p.x());}
        if(_bounds.top()>p.y()) {_bounds.setTop(p.y());}
        if((_bounds.left()+_bounds.width())<p.x()) {_bounds.setWidth(p.x()-_bounds.left());}
        if((_bounds.top()+_bounds.height())<p.y()) {_bounds.setHeight(p.y()-_bounds.top());}
    }

}
void VMTTransitionImpl::EndDrag([[maybe_unused]] IVMTEnvironment* environment,[[maybe_unused]] const QPoint& p_Point){
 _drag_condition = false;
 _drag_point_number = false;
}

void VMTTransitionImpl::Changed(IVMTEnvironment* environment){
    _changed = true;
    Update(environment);
}

void VMTTransitionImpl::UpdatePreview(IVMTEnvironment* environment)
{
    if (!environment) {
        return;
    }

    const int stub = routingStubLength(static_cast<int>(environment->GetGraphics().GetStep()));

    QPoint startOutput = _start_point;
    QPoint finishInput = _finish_point;
    if (auto ptr = _start_machine.lock()) {
        startOutput = ptr->GetOutputPoint();
    }
    if (auto ptr = _finish_machine.lock()) {
        finishInput = ptr->GetInputPoint();
    }

    buildRubberBandRoute(startOutput, finishInput, stub, _points);
    CalculateConditionsPoint(_fixed);
    updateTransitionBoundsFromPoints(_points, _bounds);
}

const QRect& VMTTransitionImpl::GetBounds(){
 return _bounds;
}
VMTTransitionConditions& VMTTransitionImpl::GetConditions(){
    return _conditions;
}
std::weak_ptr<IVMTMachine> VMTTransitionImpl::GetFinish(){
    return _finish_machine;
}

std::weak_ptr<IVMTMachine> VMTTransitionImpl::GetStart(){
    return _start_machine;
}

bool VMTTransitionImpl::IsError(){
    return this->_error;
}

struct Event {
    int x, y1, y2;
    bool isStart;

    Event(int x, int y1, int y2, bool isStart) : x(x), y1(y1), y2(y2), isStart(isStart) {}

    bool operator<(const Event& e) const {
        if (x != e.x) return x < e.x;
        return isStart < e.isStart;
    }
};

bool VMTTransitionImpl::IsIntersectLine([[maybe_unused]] IVMTEnvironment* environment,const QPoint& segmentStart,const QPoint& segmentEnd){


    std::vector<Event> events;
    for (size_t i = 0; i + 1 < _points.size(); ++i) {
        int x1 = std::min(_points[i].x(), _points[i + 1].x());
        int x2 = std::max(_points[i].x(), _points[i + 1].x());
        int y1 = std::min(_points[i].y(), _points[i + 1].y());
        int y2 = std::max(_points[i].y(), _points[i + 1].y());
        events.push_back(Event(x1, y1, y2, true));
        events.push_back(Event(x2, y1, y2, false));
    }

    int x1 = std::min(segmentStart.x(), segmentEnd.x());
    int x2 = std::max(segmentStart.x(), segmentEnd.x());
    int y1 = std::min(segmentStart.y(), segmentEnd.y());
    int y2 = std::max(segmentStart.y(), segmentEnd.y());
    events.push_back(Event(x1, y1, y2, true));
    events.push_back(Event(x2, y1, y2, false));

    std::sort(events.begin(), events.end());

    std::multiset<int> yCoords;
    for (const Event& event : events) {
        if (event.isStart) {
                if (yCoords.lower_bound(event.y1) != yCoords.upper_bound(event.y2)) {
                return true;
                }
                yCoords.insert(event.y1);
        } else {
                yCoords.erase(yCoords.find(event.y1));
        }
    }

    return false;
}

bool VMTTransitionImpl::IsInside(IVMTEnvironment* environment,const QPoint& point){
    //Update(environment);

    for(size_t i=1;i<_points.size();i++)
        if(IsInsideLine(_points[i-1],_points[i],point)) return true;



    if (GetConditionRectangle(environment->GetGraphics()).contains(point))
        return true;

    return false;
}

bool VMTTransitionImpl::IsInsideLine(const QPoint& prev, const QPoint& current, const QPoint& point){
    int nBorderWidth = PRECISION;
    bool  bIntersept = false;

    QPoint p_Tmp1;
    QPoint p_Tmp2;

    p_Tmp1.rx() = current.x()<prev.x()?current.x():prev.x();
    p_Tmp2.rx() = current.x()>prev.x()?current.x():prev.x();
    p_Tmp1.ry() = current.y()<prev.y()?current.y():prev.y();
    p_Tmp2.ry() = current.y()>prev.y()?current.y():prev.y();


    if((p_Tmp1.x()-nBorderWidth<=point.x())&&
            (p_Tmp1.y()-nBorderWidth<=point.y())&&
            (p_Tmp2.x()+nBorderWidth>=point.x())&&
            (p_Tmp2.y()+nBorderWidth>=point.y())) bIntersept = true;

    if(bIntersept)
    {

        if (current.x()!=prev.x())
        {
            double dTg = ((double)(current.y()-prev.y()))/((double)(current.x()-prev.x()));
            double Fi = std::atan(dTg);
            double dSin = std::sin(Fi);
            double dCos = std::cos(Fi);

            double delta = (((double)point.y())-((double)(point.x()-prev.x()))*dTg -(double)prev.y())/
                    (dTg*dSin+dCos);

            if(std::abs(delta)<=nBorderWidth)
            {
                double tMax = ((double)(current.x()-prev.x()))/dCos;

                double t = (point.x()+dSin*delta-prev.x())/dCos;

                if((t<=tMax)&&(t>=0)) return true;
                if((t>=tMax)&&(t<=0)) return true;

            }
        } else return true;

        long dLineLength = ((long)nBorderWidth)*((long)nBorderWidth);
        long dLength = ((long)(point.x()-prev.x()))*((long)(point.x()-prev.x()))
                +((long)(point.y()-prev.y()))*((long)(point.y()-prev.y()));
        if(dLength<=dLineLength)
            return true;

        dLength = ((long)(point.x()-current.x()))*((long)(point.x()-current.x()))
                +((long)(point.y()-current.y()))*((long)(point.y()-current.y()));
        if(dLength<=dLineLength)
            return true;

    }

    return false;

}

void VMTTransitionImpl::Paint(UICanvas& canvas, const QRect& rect){
    if (!rect.isEmpty() && !_bounds.isEmpty() && !rect.intersects(_bounds)) {
        return;
    }

    bool first = true;
    QPoint last;
    for(QPoint current: _points)
    {
        if(!first){
            if(this->_highlighted)
                canvas.DrawLineAnimation(last,current,_highlighted);
            else canvas.DrawLine(last,current,_highlighted,_error);
        } else first = false;
        last = current;
    }

    if(_points.size()>1){
        canvas.DrawArrow(_points[_points.size()-2],_points[_points.size()-1],_highlighted);
    }

    if(!_conditions.IsAllEnabled()){
        if(_conditions.GetAsString().length()>0){
            QRect rect= GetConditionRectangle(canvas);
            QString text = _conditions.GetAsString();
            canvas.FillRect(rect,this->_highlighted);
            //canvas.DrawRectAnimation(_bounds,true);
            canvas.DrawText(text,_conditions_point,_condition_width/2,this->_highlighted);

           // canvas.DrawText(text,rect);
        } else {
             QRect rect= GetConditionRectangle(canvas);
             canvas.DrawImage(ImageType::CANCEL,rect);
        }

    }
}
void VMTTransitionImpl::RemoveChilds(){
    if(auto ptr=_start_machine.lock())
        ptr->RemoveOutgoingTransition(this);

    if(auto ptr=_finish_machine.lock())
        ptr->RemoveIncomingTransition(this);

}
void VMTTransitionImpl::SetError(bool error){
    this->_error = error;
}

void VMTTransitionImpl::SetFinish(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine){
    if(auto ptr= _finish_machine.lock()){
        ptr->RemoveIncomingTransition(this);
    }

    _finish_machine = std::weak_ptr<IVMTMachine>(machine);
    _finish_point = QPoint();
    Changed(environment);
}


void VMTTransitionImpl::SetFinish(IVMTEnvironment* environment,const QPoint& point){
    if(auto ptr= _finish_machine.lock()){
        ptr->RemoveIncomingTransition(this);
    }

    _finish_machine = std::weak_ptr<IVMTMachine>();
    _finish_point = point;
    Changed(environment);
}

void VMTTransitionImpl::SetSelected(bool enable){
    _highlighted = enable;
}

void VMTTransitionImpl::SetStart(IVMTEnvironment* environment,std::shared_ptr<IVMTMachine> machine){
    if(auto ptr= _start_machine.lock()){
        ptr->RemoveOutgoingTransition(this);
    }

    _start_machine = std::weak_ptr<IVMTMachine>(machine);
    _start_point = QPoint();
    Changed(environment);
}

void VMTTransitionImpl::AttachStartMachine(const std::shared_ptr<IVMTMachine>& machine)
{
    _start_machine = machine;
    _start_point = QPoint();
}

void VMTTransitionImpl::AttachFinishMachine(const std::shared_ptr<IVMTMachine>& machine)
{
    _finish_machine = machine;
    _finish_point = QPoint();
}

void VMTTransitionImpl::SetStart(IVMTEnvironment* environment,const QPoint& point){
    if(auto ptr= _start_machine.lock()){
        ptr->RemoveIncomingTransition(this);
    }

    _start_machine = std::weak_ptr<IVMTMachine>();
    _start_point = point;
    Changed(environment);
}



VMTTransitionImpl::~VMTTransitionImpl(){

}

void VMTTransitionImpl::CalculateConditionsPoint([[maybe_unused]] bool fixed){
    if(static_cast<int>(_points.size())>_conditions_segment_number+1)
    {
        QPoint& one = _points[_conditions_segment_number];
        QPoint& two = _points[_conditions_segment_number+1];

        _conditions_point.rx() = one.x()+(two.x()-one.x())*_conditions_segment;
        _conditions_point.ry() = one.y()+(two.y()-one.y())*_conditions_segment;
    }

}

#include "pathfinder.h"
#include "diagramrouting.h"

void VMTTransitionImpl::Update([[maybe_unused]] IVMTEnvironment* environment){
    if(_changed)
    {
        if(!environment) {
            return;
        }
        const int gridStep = static_cast<int>(environment->GetGraphics().GetStep());
        const int routeStub = routingStubLength(gridStep);
        const int clearanceMargin = gridStep * 2;

        if(auto ptr = _start_machine.lock()){
             _start_point = ptr->GetOutputPoint();
             _start_point.rx() += routeStub;
        }

        if(auto ptr = _finish_machine.lock()){
             _finish_point = ptr->GetInputPoint();
             _finish_point.rx() -= routeStub;
        }

        Pathfinder p;
        constexpr int kRouteMargin = 200;
        QRect total_bound = QRect(_start_point, _finish_point).normalized();
        if (auto ptr = _start_machine.lock()) {
            total_bound = total_bound.united(ptr->GetBounds());
        }
        if (auto ptr = _finish_machine.lock()) {
            total_bound = total_bound.united(ptr->GetBounds());
        }
        total_bound = total_bound.marginsAdded(QMargins(kRouteMargin, kRouteMargin, kRouteMargin, kRouteMargin));

        std::vector<path_t> blockedPaths;
        MachineSpatialIndex spatialIndex(std::max(gridStep * 2, 8));
        if (auto parent_ptr = _parent.lock()) {
            blockedPaths = parent_ptr->blockedPathsFor(this);
            spatialIndex.rebuild(parent_ptr->GetMachineCollection(), clearanceMargin);
        }
        path_t path = p.GetPath(_start_point,
                                _finish_point,
                                total_bound,
                                gridStep * 2,
                                [&](const QPoint& point) { return spatialIndex.contains(point); },
                                blockedPaths,
                                static_cast<int>(gridStep));
        if(!path.empty()){
             _points.clear();
             if(auto ptr = _start_machine.lock()){
                _points.push_back(ptr->GetOutputPoint());
             } else _points.push_back(_start_point);
             for(const auto &p: path) _points.push_back(p);
             if(auto ptr = _finish_machine.lock()){
                _points.push_back(ptr->GetInputPoint());
             } else _points.push_back(_finish_point);
        } else
        {
            if(!_fixed)
            {
                QPoint startOutput = _start_point;
                QPoint finishInput = _finish_point;
                if (auto ptr = _start_machine.lock()) {
                    startOutput = ptr->GetOutputPoint();
                }
                if (auto ptr = _finish_machine.lock()) {
                    finishInput = ptr->GetInputPoint();
                }
                buildRubberBandRoute(startOutput, finishInput, routeStub, _points);
                UpdateSegment(1, 2);
            } else
            {
                if(auto ptr = _start_machine.lock()){
                    _points[0]=ptr->GetOutputPoint();
                } else _points[0]=_start_point;

                QPoint current = _points[0];
                _points[1]=QPoint(current.rx()+routeStub,current.ry());

                if(auto ptr = _finish_machine.lock()){
                    _points[_points.size()-1]=ptr->GetInputPoint();
                } else _points[_points.size()-1]=_finish_point;

                current = _points[_points.size()-1];
                _points[_points.size()-2]=QPoint(current.rx()-routeStub,current.ry());
            }
        }

        CalculateConditionsPoint(_fixed);
        _changed = false;
        _committedPoints = _points;

        updateTransitionBoundsFromPoints(_points, _bounds);

    }
}
void VMTTransitionImpl::UpdateSegment(int start_position, int end_position){
    QPoint start  = _points[start_position];
    QPoint end    = _points[end_position];

    if (start.rx()<end.rx())
    {
        QPoint one = QPoint((start.rx()+end.rx())/2,start.ry());
        QPoint two = QPoint((start.rx()+end.rx())/2,end.ry());
        _points.insert(_points.begin()+end_position,one);
        _points.insert(_points.begin()+end_position+1,two);

    } else
    {
        QPoint one = QPoint(start.rx(),(start.ry()+end.ry())/2);
        QPoint two = QPoint(end.rx(),(start.ry()+end.ry())/2);
        _points.insert(_points.begin()+end_position,one);
        _points.insert(_points.begin()+end_position+1,two);
    }



    for(size_t i=1;i<_points.size()-2;i++)
    {
        int iteration = 1;
        while (CheckLineIntersectMachine(i, i + 1) && iteration < 12) {
            ++iteration;
        }
    }

}

bool VMTTransitionImpl::CheckLineIntersectMachine(int start_position, int end_position){
    bool intersect = false;
    QPoint start  = _points[start_position];
    QPoint end    = _points[end_position];

    if(auto parent_ptr=_parent.lock())
    {
        for(auto machine: parent_ptr->GetMachineCollection())
        {
            QRect rect = machine->GetBounds();
            if(start.ry()==end.ry())
            {
                if ((rect.top()<=start.ry())&&(rect.top()+rect.height()>=start.ry()))
                {
                    if (end.rx()>start.rx())
                    {
                        if ((start.rx()<=rect.left())&&(end.rx()>=rect.left()+rect.width()))
                        {
                            intersect = true;
                        }
                    } else
                    {
                        if ((end.rx()<=rect.left())&&(start.rx()>=rect.left()+rect.width()))
                        {
                            intersect  = true;
                        }
                    }
                }
                if (intersect)
                {
                    start.setY(start.y()-PRECISION);
                    end.setY(end.y()-PRECISION);
                    _points[start_position] = start;
                    _points[end_position] = end;
                }
            } else
                if(start.rx()==end.rx())
                {
                    if ((rect.left()<=start.rx())&&(rect.left()+rect.width()>=start.rx()))
                    {
                        if (end.ry()>start.ry())
                        {
                            if ((start.ry()<=rect.top()+rect.height())&&(end.ry()>=rect.top()))
                            {
                                intersect = true;
                            }
                        } else
                        {
                            if ((end.ry()<=rect.top()+rect.height())&&(start.ry()>=rect.top()))
                            {
                                intersect  = true;
                            }
                        }
                    }
                    if (intersect)
                    {
                        start.setX(start.x()-PRECISION);
                        end.setX(end.x()-PRECISION);
                        _points[start_position] = start;
                        _points[end_position] = end;
                    }
                }
        }
    }


    return intersect;
}

QRect VMTTransitionImpl::GetConditionRectangle([[maybe_unused]] UICanvas& canvas){
    CalculateConditionsPoint(false);
    QRect rect(_conditions_point.x(),_conditions_point.y(),_condition_width,_condition_width);
    QString text = _conditions.GetAsString();
    if(text.length()>0){
        rect.setWidth(_st.GetCellSize().width()*text.length());
        rect.setHeight(_st.GetCellSize().height());
//      canvas.CalculateTextSize(text,rect);
    }

    rect.moveLeft(rect.left()-rect.width()/2);
    rect.moveTop(rect.top()-rect.height()/2);
/*
    rect.setTop(rect.top()-rect.height()/2);
    rect.setHeight(rect.height());

    rect.setLeft(rect.left()-rect.width()/2);
    rect.setWidth(rect.width());*/

    return rect;
}
