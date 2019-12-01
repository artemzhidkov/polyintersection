#include <BinaryTreeIntersector.h>

#include <cmath>

// Вычисляет точку пересечения отрезков.
// Возвращает true, если отрезки пересекаются.
static bool intersectSegments(const Point2D& theStart1, const Point2D& theEnd1,
                              const Point2D& theStart2, const Point2D& theEnd2,
                              Point2D& theIntersection,
                              const double theTolerance)
{
  // Положение точки на отрезке вычисляется по формуле
  // P(t) = S * (1 - t) + E * t, t in [0, 1]
  // S - начальная точка отрезка, E - конечная.

  // Вычисляем параметр точки пересечения на каждом отрезке
  Point2D dir1 = { theEnd1.x - theStart1.x, theEnd1.y - theStart1.y };
  Point2D dir2 = { theEnd2.x - theStart2.x, theEnd2.y - theStart2.y };
  double det = dir1.x * dir2.y - dir1.y * dir2.x;
  if (fabs(det) < theTolerance * theTolerance)
    return false; // отрезки параллельны
  Point2D dirSS = { theStart2.x - theStart1.x, theStart2.y - theStart1.y };

  double param1 = (dirSS.x * dir2.y - dirSS.y * dir2.x) / det;
  double param2 = (dirSS.x * dir1.y - dirSS.y * dir1.x) / det;
  if (param1 < -theTolerance || param1 > 1.0 + theTolerance ||
      param2 < -theTolerance || param2 > 1.0 + theTolerance)
    return false; // точка пересечения не лежит на одном из отрезков

  theIntersection.x = theStart1.x + dir1.x * param1;
  theIntersection.y = theStart1.y + dir1.y * param1;
  return true;
}


BinaryTreeIntersector::BinaryTreeIntersector(const BinaryTree& thePolyBT)
  : myTree(thePolyBT)
{}

const std::unordered_map<int, Point2D>& BinaryTreeIntersector::Intersect(
    const Point2D& theStart,
    const Point2D& theEnd,
    const double theTolerance)
{
  myIntersectionPoints.clear();

  BndRect segmentBnd;
  segmentBnd.Add(theStart);
  segmentBnd.Add(theEnd);

  // ищем список потенциально пересекаемых сторон
  std::list<std::pair<int, int> > segments;
  SearchSegments(segmentBnd, 0, theTolerance, segments);

  size_t nbNodes = myTree.myPoly.size();

  // вычисляем точки пересечения
  Point2D intPoint;
  for (auto seg : segments)
    for (int id = seg.first; id <= seg.second; ++id)
    {
      if (intersectSegments(myTree.myPoly[id], myTree.myPoly[(id + 1) % nbNodes],
                            theStart, theEnd,
                            intPoint, theTolerance))
        myIntersectionPoints[id] = intPoint;
    }

  return myIntersectionPoints;
}

void BinaryTreeIntersector::SearchSegments(const BndRect& theRect,
                                           const int theTreeNode,
                                           const double theTolerance,
                                           std::list<std::pair<int, int> >& theSegments) const
{
  auto node = myTree.myNodes[theTreeNode];
  if (node.myBndRect.IsIntersected(theRect, theTolerance))
  {
    if (node.myChild[0] == -1 && node.myChild[1] == -1)
    {
      // лист дерева, берём индексы отрезков, принадлежащих ему
      theSegments.emplace_back(node.mySegments[0], node.mySegments[1]);
    }
    else
    {
      if (node.myChild[0] >= 0)
        SearchSegments(theRect, node.myChild[0], theTolerance, theSegments);
      if (node.myChild[1] >= 0)
        SearchSegments(theRect, node.myChild[1], theTolerance, theSegments);
    }
  }
}
