#include "diagrambezier.h"

#include <QPainterPathStroker>

#include <algorithm>
#include <cmath>

namespace DiagramBezier {

namespace {

QPoint extrapolateStart(const std::vector<QPoint>& points)
{
    return points.front() + (points.front() - points[1]);
}

QPoint extrapolateEnd(const std::vector<QPoint>& points)
{
    return points.back() + (points.back() - points[points.size() - 2]);
}

QPoint pointAt(const std::vector<QPoint>& points, int index)
{
    if (index < 0) {
        return extrapolateStart(points);
    }
    if (index >= static_cast<int>(points.size())) {
        return extrapolateEnd(points);
    }
    return points[static_cast<size_t>(index)];
}

} // namespace

void snapPortHeights(std::vector<QPoint>& points)
{
    if (points.size() < 2) {
        return;
    }

    const int startY = points.front().y();
    const int endY = points.back().y();

    points.front().setY(startY);
    points.back().setY(endY);

    if (points.size() >= 2) {
        points[1].setY(startY);
    }
    if (points.size() >= 3) {
        points[points.size() - 2].setY(endY);
    }
}

QPainterPath buildConnectorPath(const std::vector<QPoint>& points)
{
    QPainterPath path;
    if (points.empty()) {
        return path;
    }

    if (points.size() == 1) {
        path.moveTo(points.front());
        return path;
    }

    if (points.size() == 2) {
        path.moveTo(points.front());
        path.lineTo(points.back());
        return path;
    }

    path.moveTo(points.front());
    for (size_t i = 0; i + 1 < points.size(); ++i) {
        const QPoint p0 = pointAt(points, static_cast<int>(i) - 1);
        const QPoint p1 = points[i];
        const QPoint p2 = points[i + 1];
        const QPoint p3 = pointAt(points, static_cast<int>(i) + 2);

        QPoint c1 = p1 + (p2 - p0) / 6;
        QPoint c2 = p2 - (p3 - p1) / 6;

        if (i == 0) {
            c1.setY(p1.y());
        }
        if (i + 1 == points.size() - 1) {
            c2.setY(p2.y());
        }

        path.cubicTo(c1, c2, p2);
    }

    return path;
}

std::vector<QPoint> flattenPath(const QPainterPath& path, qreal maxSegmentLength)
{
    std::vector<QPoint> samples;
    if (path.isEmpty()) {
        return samples;
    }

    const qreal length = path.length();
    if (length <= 0.0) {
        samples.push_back(path.currentPosition().toPoint());
        return samples;
    }

    const qreal step = std::max(maxSegmentLength, 1.0);
    const int steps = std::max(2, static_cast<int>(std::ceil(length / step)));
    samples.reserve(static_cast<size_t>(steps + 1));

    for (int i = 0; i <= steps; ++i) {
        const qreal t = static_cast<qreal>(i) / steps;
        samples.push_back(path.pointAtPercent(t).toPoint());
    }

    return samples;
}

bool isPointNearStroke(const QPainterPath& path, const QPoint& point, qreal tolerance)
{
    if (path.isEmpty()) {
        return false;
    }

    QPainterPathStroker stroker;
    stroker.setWidth(std::max(2.0 * tolerance, 2.0));
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);

    const QPainterPath stroke = stroker.createStroke(path);
    return stroke.contains(point);
}

} // namespace DiagramBezier
