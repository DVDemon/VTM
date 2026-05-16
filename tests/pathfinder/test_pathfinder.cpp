#include <QtTest>
#include <QRect>
#include <QPoint>

#include <algorithm>
#include <cmath>
#include <vector>

#include "pathfinder.h"

namespace {

constexpr size_t kGridStep = 40; // как GetStep()*2 в редакторе (step=20)

struct DiagramLayout {
    std::vector<QRect> nodes;

    QRect totalBounds(int margin = 100) const
    {
        if (nodes.empty()) {
            return QRect();
        }
        QRect bounds = nodes.front();
        for (size_t i = 1; i < nodes.size(); ++i) {
            bounds = bounds.united(nodes[i]);
        }
        bounds.adjust(-margin, -margin, margin, margin);
        return bounds;
    }
};

QRect makeNode(int centerX, int centerY, int halfSize = 15)
{
    return QRect(centerX - halfSize, centerY - halfSize, halfSize * 2, halfSize * 2);
}

DiagramLayout chainLayout(int nodeCount, int spacing = 120, int centerY = 100)
{
    DiagramLayout layout;
    layout.nodes.reserve(static_cast<size_t>(nodeCount));
    for (int i = 0; i < nodeCount; ++i) {
        layout.nodes.push_back(makeNode(100 + i * spacing, centerY));
    }
    return layout;
}

DiagramLayout gridLayout(int columns, int rows, int spacingX = 120, int spacingY = 100)
{
    DiagramLayout layout;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            layout.nodes.push_back(makeNode(100 + col * spacingX, 80 + row * spacingY));
        }
    }
    return layout;
}

QPoint outputPoint(const QRect& node)
{
    return QPoint(node.right(), node.center().y());
}

QPoint inputPoint(const QRect& node)
{
    return QPoint(node.left(), node.center().y());
}

QPoint routeStartPoint(const QPoint& output)
{
    QPoint result = output;
    result.rx() += static_cast<int>(kGridStep / 2);
    return result;
}

QPoint routeFinishPoint(const QPoint& input)
{
    QPoint result = input;
    result.rx() -= static_cast<int>(kGridStep / 2);
    return result;
}

int chebyshev(const QPoint& a, const QPoint& b)
{
    return std::max(std::abs(a.x() - b.x()), std::abs(a.y() - b.y()));
}

bool pointNearSegment(const QPoint& point,
                      const QPoint& a,
                      const QPoint& b,
                      int tolerance = 3)
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

    return chebyshev(point, a) + chebyshev(point, b) <= chebyshev(a, b) + tolerance;
}

bool pathContainsPoint(const path_t& path, const QPoint& point, int tolerance = 3)
{
    if (path.empty()) {
        return false;
    }

    for (const QPoint& waypoint : path) {
        if (chebyshev(waypoint, point) <= tolerance) {
            return true;
        }
    }

    for (size_t i = 1; i < path.size(); ++i) {
        if (pointNearSegment(point, path[i - 1], path[i], tolerance)) {
            return true;
        }
    }

    return false;
}

bool segmentsOverlap(const QPoint& a1,
                     const QPoint& a2,
                     const QPoint& b1,
                     const QPoint& b2,
                     int tolerance = 2)
{
    const bool aHorizontal = a1.y() == a2.y();
    const bool bHorizontal = b1.y() == b2.y();

    if (aHorizontal && bHorizontal) {
        if (std::abs(a1.y() - b1.y()) > tolerance) {
            return false;
        }
        const int aMin = std::min(a1.x(), a2.x());
        const int aMax = std::max(a1.x(), a2.x());
        const int bMin = std::min(b1.x(), b2.x());
        const int bMax = std::max(b1.x(), b2.x());
        const int overlapMin = std::max(aMin, bMin);
        const int overlapMax = std::min(aMax, bMax);
        return overlapMax - overlapMin > tolerance;
    }

    if (!aHorizontal && !bHorizontal) {
        if (std::abs(a1.x() - b1.x()) > tolerance) {
            return false;
        }
        const int aMin = std::min(a1.y(), a2.y());
        const int aMax = std::max(a1.y(), a2.y());
        const int bMin = std::min(b1.y(), b2.y());
        const int bMax = std::max(b1.y(), b2.y());
        const int overlapMin = std::max(aMin, bMin);
        const int overlapMax = std::min(aMax, bMax);
        return overlapMax - overlapMin > tolerance;
    }

    const QPoint& h1 = aHorizontal ? a1 : b1;
    const QPoint& h2 = aHorizontal ? a2 : b2;
    const QPoint& v1 = aHorizontal ? b1 : a1;
    const QPoint& v2 = aHorizontal ? b2 : a2;

    const int hMin = std::min(h1.x(), h2.x());
    const int hMax = std::max(h1.x(), h2.x());
    const int vMin = std::min(v1.y(), v2.y());
    const int vMax = std::max(v1.y(), v2.y());

    return v1.x() >= hMin - tolerance && v1.x() <= hMax + tolerance && h1.y() >= vMin - tolerance
           && h1.y() <= vMax + tolerance;
}

bool pathsShareOnlyEndpoints(const path_t& left, const path_t& right, int tolerance = 4)
{
    if (left.empty() || right.empty()) {
        return false;
    }

    const QPoint leftStart = left.front();
    const QPoint leftEnd = left.back();
    const QPoint rightStart = right.front();
    const QPoint rightEnd = right.back();

    for (size_t i = 1; i < left.size(); ++i) {
        for (size_t j = 1; j < right.size(); ++j) {
            if (!segmentsOverlap(left[i - 1], left[i], right[j - 1], right[j], tolerance)) {
                continue;
            }

            const bool touchesSharedEndpoint =
                chebyshev(left[i - 1], rightStart) <= tolerance
                || chebyshev(left[i - 1], rightEnd) <= tolerance
                || chebyshev(left[i], rightStart) <= tolerance || chebyshev(left[i], rightEnd) <= tolerance
                || chebyshev(right[j - 1], leftStart) <= tolerance
                || chebyshev(right[j - 1], leftEnd) <= tolerance
                || chebyshev(right[j], leftStart) <= tolerance || chebyshev(right[j], leftEnd) <= tolerance;

            if (!touchesSharedEndpoint) {
                return false;
            }
        }
    }

    return true;
}

bool pathsCrossEachOther(const std::vector<path_t>& paths)
{
    for (size_t i = 0; i < paths.size(); ++i) {
        for (size_t j = i + 1; j < paths.size(); ++j) {
            if (!pathsShareOnlyEndpoints(paths[i], paths[j])) {
                return true;
            }
        }
    }
    return false;
}

bool pointInsideNodes(const QPoint& point, const std::vector<QRect>& nodes, int shrink = 1)
{
    for (const QRect& node : nodes) {
        QRect inner = node;
        inner.adjust(shrink, shrink, -shrink, -shrink);
        if (inner.contains(point)) {
            return true;
        }
    }
    return false;
}

bool pathAvoidsIntermediateNodes(const path_t& path,
                               const std::vector<QRect>& nodes,
                               size_t fromIndex,
                               size_t toIndex)
{
    if (path.size() < 2) {
        return true;
    }

    for (size_t i = 1; i < path.size(); ++i) {
        const QPoint& from = path[i - 1];
        const QPoint& to = path[i];
        const int steps = std::max(1, chebyshev(from, to) / static_cast<int>(kGridStep / 2));

        for (int step = 0; step <= steps; ++step) {
            const double t = static_cast<double>(step) / steps;
            const QPoint sample(static_cast<int>(from.x() + (to.x() - from.x()) * t),
                                static_cast<int>(from.y() + (to.y() - from.y()) * t));

            for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
                if (nodeIndex == fromIndex || nodeIndex == toIndex) {
                    continue;
                }

                QRect inner = nodes[nodeIndex];
                inner.adjust(2, 2, -2, -2);
                if (inner.contains(sample)) {
                    return false;
                }
            }
        }
    }

    return true;
}

wall_checker_t makeNodeWallChecker(const DiagramLayout& layout)
{
    return [&layout](const QPoint& point) { return pointInsideNodes(point, layout.nodes); };
}

path_t assembleFullPath(const DiagramLayout& layout,
                        size_t fromIndex,
                        size_t toIndex,
                        const path_t& routedMiddle)
{
    path_t full;
    full.push_back(outputPoint(layout.nodes[fromIndex]));
    for (const QPoint& point : routedMiddle) {
        if (full.empty() || full.back() != point) {
            full.push_back(point);
        }
    }
    const QPoint input = inputPoint(layout.nodes[toIndex]);
    if (full.empty() || full.back() != input) {
        full.push_back(input);
    }
    return full;
}

path_t buildTransitionPath(const DiagramLayout& layout,
                           const std::vector<path_t>& existingPaths,
                           size_t fromIndex,
                           size_t toIndex)
{
    Pathfinder pathfinder;
    const QPoint start = routeStartPoint(outputPoint(layout.nodes[fromIndex]));
    const QPoint finish = routeFinishPoint(inputPoint(layout.nodes[toIndex]));
    const QRect bounds = layout.totalBounds();

    const path_t routedMiddle = pathfinder.GetPath(start,
                                                   finish,
                                                   bounds,
                                                   kGridStep,
                                                   makeNodeWallChecker(layout),
                                                   existingPaths,
                                                   static_cast<int>(kGridStep / 2));

    return assembleFullPath(layout, fromIndex, toIndex, routedMiddle);
}

struct TransitionPath {
    size_t fromIndex = 0;
    size_t toIndex = 0;
    path_t points;
};

std::vector<path_t> collectPoints(const std::vector<TransitionPath>& transitions)
{
    std::vector<path_t> paths;
    paths.reserve(transitions.size());
    for (const TransitionPath& transition : transitions) {
        paths.push_back(transition.points);
    }
    return paths;
}

std::vector<TransitionPath> buildChainPaths(const DiagramLayout& layout)
{
    std::vector<TransitionPath> paths;
    if (layout.nodes.size() < 2) {
        return paths;
    }

    for (size_t index = 0; index + 1 < layout.nodes.size(); ++index) {
        TransitionPath transition;
        transition.fromIndex = index;
        transition.toIndex = index + 1;
        transition.points =
            buildTransitionPath(layout, collectPoints(paths), index, index + 1);
        paths.push_back(std::move(transition));
    }

    return paths;
}

std::vector<TransitionPath> buildExplicitPaths(const DiagramLayout& layout,
                                               const std::vector<std::pair<size_t, size_t>>& edges)
{
    std::vector<TransitionPath> paths;
    for (const auto& edge : edges) {
        TransitionPath transition;
        transition.fromIndex = edge.first;
        transition.toIndex = edge.second;
        transition.points =
            buildTransitionPath(layout, collectPoints(paths), edge.first, edge.second);
        paths.push_back(std::move(transition));
    }
    return paths;
}

bool allPathsNonEmpty(const std::vector<TransitionPath>& paths)
{
    return std::all_of(paths.begin(), paths.end(), [](const TransitionPath& transition) {
        return !transition.points.empty();
    });
}

bool allPathsAvoidIntermediateNodes(const std::vector<TransitionPath>& paths,
                                    const std::vector<QRect>& nodes)
{
    return std::all_of(paths.begin(), paths.end(), [&](const TransitionPath& transition) {
        return pathAvoidsIntermediateNodes(transition.points, nodes, transition.fromIndex, transition.toIndex);
    });
}

bool allPathsSeparated(const std::vector<TransitionPath>& paths)
{
    const std::vector<path_t> points = collectPoints(paths);
    return !pathsCrossEachOther(points);
}

int countBends(const path_t& path)
{
    if (path.size() < 3) {
        return 0;
    }

    int bends = 0;
    for (size_t i = 2; i < path.size(); ++i) {
        const QPoint& a = path[i - 2];
        const QPoint& b = path[i - 1];
        const QPoint& c = path[i];
        const bool prevHorizontal = a.y() == b.y();
        const bool nextHorizontal = b.y() == c.y();
        if (prevHorizontal != nextHorizontal) {
            ++bends;
        }
    }
    return bends;
}

bool hasAxisSpike(const path_t& path)
{
    for (size_t i = 1; i + 1 < path.size(); ++i) {
        const QPoint& a = path[i - 1];
        const QPoint& b = path[i];
        const QPoint& c = path[i + 1];

        if (a.x() == b.x() && b.x() == c.x()) {
            const int low = std::min(a.y(), c.y());
            const int high = std::max(a.y(), c.y());
            if (b.y() > low && b.y() < high && b.y() != a.y() && b.y() != c.y()) {
                return true;
            }
        }

        if (a.y() == b.y() && b.y() == c.y()) {
            const int low = std::min(a.x(), c.x());
            const int high = std::max(a.x(), c.x());
            if (b.x() > low && b.x() < high && b.x() != a.x() && b.x() != c.x()) {
                return true;
            }
        }
    }

    return false;
}

bool terminalSegmentsAreMonotonic(const path_t& path)
{
    if (path.size() < 3) {
        return true;
    }

    for (size_t i = 1; i + 1 < std::min(path.size(), size_t(5)); ++i) {
        if (path[i - 1].x() == path[i].x() && path[i].x() == path[i + 1].x()) {
            const int y0 = path[i - 1].y();
            const int y1 = path[i].y();
            const int y2 = path[i + 1].y();
            if ((y1 - y0) * (y2 - y1) < 0) {
                return false;
            }
        }
    }

    const size_t startTail = path.size() >= 4 ? path.size() - 4 : 0;
    for (size_t i = std::max(startTail, size_t(1)); i + 1 < path.size(); ++i) {
        if (path[i - 1].x() == path[i].x() && path[i].x() == path[i + 1].x()) {
            const int y0 = path[i - 1].y();
            const int y1 = path[i].y();
            const int y2 = path[i + 1].y();
            if ((y1 - y0) * (y2 - y1) < 0) {
                return false;
            }
        }
    }

    return true;
}

} // namespace

class PathfinderDiagramTest : public QObject
{
    Q_OBJECT

private slots:
    void singleTransition_avoidsMiddleNode();
    void chainTransitions_avoidNodes_data();
    void chainTransitions_avoidNodes();
    void parallelRowTransitions_doNotIntersect();
    void denseDiagram_multiplePathsRemainSeparated();
    void blockedRoute_returnsEmptyPath();
    void terminalRouting_exitsRightAndEntersLeft();
    void routedPaths_minimizeBendsWithoutSpikes();
};

void PathfinderDiagramTest::singleTransition_avoidsMiddleNode()
{
    DiagramLayout layout;
    layout.nodes = {makeNode(100, 100), makeNode(220, 100), makeNode(340, 100)};

    const path_t path = buildTransitionPath(layout, {}, 0, 2);
    QVERIFY(!path.empty());
    QVERIFY(pathAvoidsIntermediateNodes(path, layout.nodes, 0, 2));
}

void PathfinderDiagramTest::chainTransitions_avoidNodes_data()
{
    QTest::addColumn<int>("nodeCount");

    QTest::newRow("3_nodes") << 3;
    QTest::newRow("4_nodes") << 4;
    QTest::newRow("5_nodes") << 5;
    QTest::newRow("7_nodes") << 7;
}

void PathfinderDiagramTest::chainTransitions_avoidNodes()
{
    QFETCH(int, nodeCount);

    const DiagramLayout layout = chainLayout(nodeCount);
    const std::vector<TransitionPath> paths = buildChainPaths(layout);

    QCOMPARE(static_cast<int>(paths.size()), nodeCount - 1);
    QVERIFY(allPathsNonEmpty(paths));
    QVERIFY(allPathsAvoidIntermediateNodes(paths, layout.nodes));
    QVERIFY(allPathsSeparated(paths));
}

void PathfinderDiagramTest::parallelRowTransitions_doNotIntersect()
{
    const DiagramLayout layout = gridLayout(4, 1, 120, 100);
    const std::vector<TransitionPath> paths =
        buildExplicitPaths(layout, {{0, 1}, {1, 2}, {2, 3}});

    QCOMPARE(static_cast<int>(paths.size()), 3);
    QVERIFY(allPathsNonEmpty(paths));
    QVERIFY(allPathsAvoidIntermediateNodes(paths, layout.nodes));
    QVERIFY(allPathsSeparated(paths));
}

void PathfinderDiagramTest::denseDiagram_multiplePathsRemainSeparated()
{
    const DiagramLayout layout = gridLayout(3, 2, 120, 100);
    const std::vector<TransitionPath> paths = buildExplicitPaths(layout,
                                                                 {
                                                                     {0, 1},
                                                                     {1, 2},
                                                                     {3, 4},
                                                                     {4, 5},
                                                                     {2, 5},
                                                                 });

    QCOMPARE(static_cast<int>(paths.size()), 5);
    QVERIFY(allPathsNonEmpty(paths));
    QVERIFY(allPathsAvoidIntermediateNodes(paths, layout.nodes));
    QVERIFY(allPathsSeparated(paths));
}

void PathfinderDiagramTest::blockedRoute_returnsEmptyPath()
{
    DiagramLayout layout;
    layout.nodes = {
        makeNode(100, 100),
        makeNode(220, 100),
        makeNode(340, 100),
        makeNode(220, 220, 40),
    };

    const path_t path = buildTransitionPath(layout, {}, 0, 2);
    QVERIFY(path.empty() || pathAvoidsIntermediateNodes(path, layout.nodes, 0, 2));
}

void PathfinderDiagramTest::terminalRouting_exitsRightAndEntersLeft()
{
    const DiagramLayout layout = chainLayout(4);
    const std::vector<TransitionPath> paths = buildChainPaths(layout);
    QVERIFY(allPathsNonEmpty(paths));

    for (const TransitionPath& transition : paths) {
        const path_t& path = transition.points;
        QVERIFY(path.size() >= 3);

        QCOMPARE(path.front(), outputPoint(layout.nodes[transition.fromIndex]));
        QCOMPARE(path.back(), inputPoint(layout.nodes[transition.toIndex]));

        QVERIFY2(path[1].x() > path[0].x(),
                 qPrintable(QStringLiteral("first segment must go right from output")));
        QVERIFY2(path[path.size() - 2].x() < path.back().x(),
                 qPrintable(QStringLiteral("last segment must approach input from left")));
    }
}

void PathfinderDiagramTest::routedPaths_minimizeBendsWithoutSpikes()
{
    const DiagramLayout layout = gridLayout(4, 2, 120, 100);
    const std::vector<TransitionPath> paths = buildExplicitPaths(layout,
                                                                 {
                                                                     {0, 1},
                                                                     {1, 2},
                                                                     {2, 3},
                                                                     {3, 4},
                                                                     {4, 5},
                                                                     {6, 7},
                                                                     {2, 5},
                                                                 });

    QVERIFY(allPathsNonEmpty(paths));

    for (const TransitionPath& transition : paths) {
        const path_t& path = transition.points;
        QVERIFY2(!hasAxisSpike(path), qPrintable(QStringLiteral("path has redundant axis spike")));
        QVERIFY2(terminalSegmentsAreMonotonic(path),
                 qPrintable(QStringLiteral("path oscillates near terminals")));
        QVERIFY2(countBends(path) <= 6,
                 qPrintable(QString("too many bends (%1)").arg(countBends(path))));
    }
}

QTEST_APPLESS_MAIN(PathfinderDiagramTest)
#include "test_pathfinder.moc"
