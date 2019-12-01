#include <PolyIntersection.h>
#include <BinaryTreeIntersector.h>
#include <PolyTools.h>

#include <algorithm>

const double PolyIntersection::DEFAULT_TOLERANCE = 1.e-7;

PolyIntersection::PolyIntersection(const ClosedPolygon& thePoly1,
                                   const ClosedPolygon& thePoly2,
                                   const double theTolerance)
  : myDone(Status::NON_CONVEX)
{
  if (PolyTools::isConvex(thePoly1, theTolerance))
    Intersect(thePoly1, thePoly2, theTolerance);
  else if (PolyTools::isConvex(thePoly2, theTolerance))
    Intersect(thePoly2, thePoly1, theTolerance);
}

void PolyIntersection::Intersect(const ClosedPolygon& thePoly1,
                                 const ClosedPolygon& thePoly2,
                                 const double theTolerance)
{
  BinaryTree bTree2(thePoly2);
  BinaryTreeIntersector intersector(bTree2);

  PolyTools::SplitPolygons splitPoly(thePoly1, thePoly2);

  // ищем точки пересечения сторон многоугольников
  bool isIntersected = false;
  for (size_t curInd = thePoly1.size() - 1, nextInd = 0; nextInd < thePoly1.size(); curInd = nextInd++)
  {
    const std::unordered_map<int, Point2D>& points =
        intersector.Intersect(thePoly1[curInd], thePoly1[nextInd], theTolerance);
    for (auto pt : points)
    {
      splitPoly.Split(curInd, pt.first, pt.second);
      isIntersected = true;
    }
  }

  if (isIntersected)
  {
    // собираем общие части
    bool isOut = !PolyTools::isInConvexPolygon(thePoly2[0], thePoly1, theTolerance);
    std::shared_ptr<PolyTools::AlgoCommon> commonParts(new PolyTools::AlgoCommon(isOut));
    splitPoly.PerformAlgorithm(commonParts);

    myResults = commonParts->Polygons();
  }
  else
  { // стороны многоугольников не имеют пересечений
    if (PolyTools::isInConvexPolygon(thePoly2[0], thePoly1, theTolerance))
      myResults.push_back(thePoly2); // второй многоугольник внутри первого
    else
    {
      // проверяем чётность пересечений со вторым многоугольником произвольного луча, выходящего из вершины первого многоугольника
      BndRect bndRect;
      bndRect.Add(thePoly1[0]);
      for (auto pt : thePoly2)
        bndRect.Add(pt);

      // ищем самый удалённый угол прямоугольника
      double maxDist = 0.0;
      Point2D remotePoint;
      Point2D corners[] = {bndRect.MinCorner(), bndRect.MaxCorner()};
      for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
        {
          double dist = PolyTools::squareDistance(thePoly1[0], {corners[i].x, corners[j].y});
          if (dist > maxDist)
          {
            maxDist = dist;
            remotePoint = {corners[i].x, corners[j].y};
          }
        }

      // пересекаем луч в направлении самого удалённого угла прямоугольника
      const std::unordered_map<int, Point2D>& points =
          intersector.Intersect(thePoly1[0], remotePoint, theTolerance);
      if (points.size() % 2)
        myResults.push_back(thePoly1); // нечётное количество пересечений => первый многоугольник внутри второго
    }
  }
  myDone = Status::DONE;
}
