#ifndef DIAGRAMBEZIER_H
#define DIAGRAMBEZIER_H

#include <QPainterPath>
#include <QPoint>
#include <vector>

namespace DiagramBezier {

/** Align stub points beside ports to the port center Y. */
void snapPortHeights(std::vector<QPoint>& points);

/** Smooth cubic Bézier connector through orthogonal routing waypoints. */
QPainterPath buildConnectorPath(const std::vector<QPoint>& points);

/** Polyline approximation for hit-testing and overlap checks. */
std::vector<QPoint> flattenPath(const QPainterPath& path, qreal maxSegmentLength = 8.0);

bool isPointNearStroke(const QPainterPath& path, const QPoint& point, qreal tolerance);

} // namespace DiagramBezier

#endif // DIAGRAMBEZIER_H
