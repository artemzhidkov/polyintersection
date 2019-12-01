#include <PolyIntersection.h>
#include <BinaryTreeIntersector.h>

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

const double PolyIntersection::DEFAULT_TOLERANCE = 1.e-7;

// Проверка многоугольника на выпуклость.
// Все углы должны иметь один и тот же знак,
// сумма углов равна pi*(N-2)
static bool isConvex(const ClosedPolygon& thePoly, const double theTolerance)
{
  if (thePoly.size() <= 2)
    return false;

  // единичный вектор на отрезке, соединяющем точки
  auto direction = [&](const Point2D& theStart, const Point2D& theEnd)
  {
    Point2D dir = { theEnd.x - theStart.x, theEnd.y - theStart.y };
    double sqNorm = dir.x * dir.x + dir.y * dir.y;
    if (sqNorm < theTolerance * theTolerance)
      dir.x = dir.y = 0;
    else
    {
      sqNorm = sqrt(sqNorm);
      dir.x /= sqNorm;
      dir.y /= sqNorm;
    }
    return dir;
  };

  // вычисление угла по трём точкам в диапазоне (-pi, pi]
  auto angle = [&](const Point2D& theFirst, const Point2D& theSecond, const Point2D& theThird)
  {
    auto dir1 = direction(theFirst, theSecond);
    auto dir2 = direction(theSecond, theThird);
    double cosinus = dir1.x * dir2.x + dir1.y * dir2.y;
    double sinus = dir1.x * dir2.y - dir1.y * dir2.x;

    if (cosinus > -M_SQRT1_2 && cosinus < M_SQRT1_2)
    {
      if (sinus > 0)
        return acos(cosinus);
      else
        return -acos(cosinus);
    }
    else
    {
      if (cosinus > 0.0)
        return asin(sinus);
      else
      {
        if (sinus > 0)
          return M_PI - asin(sinus);
        else
          return -M_PI - asin(sinus);
      }
    }
    return 0.0;
  };

  auto prevNode = --thePoly.end();
  auto curNode = prevNode--;
  auto nextNode = thePoly.begin();

  double firstAngle = angle(*prevNode, *curNode, *nextNode);
  double totalAngle = firstAngle;
  double angleThreshold = theTolerance + M_PI * (thePoly.size() - 2);

  do {
    prevNode = curNode;
    curNode = nextNode++;
    auto curAngle = angle(*prevNode, *curNode, *nextNode);
    if (curAngle * firstAngle < -theTolerance)
      return false; // углы разных знаков
    totalAngle += curAngle;
    if (totalAngle > angleThreshold)
      return false; // многоугольник имеет самопересечения
  } while (nextNode != thePoly.end());
  // проверяем, что суммарный угол отличается от pi*(N-2) не более, чем на заданную точность
  return totalAngle + theTolerance * 2.0 < angleThreshold;
}

/// \brief Вспомогательный класс, хранящий пару многоугольников в виде связанного списка,
///        и позволяющий проводить разбиение сторон точками пересечения
class SplitPolygons
{
public:
  SplitPolygons(const ClosedPolygon& thePoly1, const ClosedPolygon& thePoly2)
    : myPoints(thePoly1), myPoly2Shift(thePoly1.size())
  {
    myPoints.insert(myPoints.end(), thePoly2.begin(), thePoly2.end());

    size_t i = 0;
    for (; i < thePoly1.size() - 1; ++i)
      myPoly1[i] = i + 1; // каждый узел ссылается на следующий
    myPoly1[i] = 0; // замыкаем полигон
    for (++i; i < myPoly2Shift + thePoly2.size() - 1; ++i)
      myPoly2[i] = i + 1; // каждый узел ссылается на следующий
    myPoly2[i] = myPoly2Shift; // замыкаем полигон
  }

  /// \brief Делим сегменты полигонов точкой пересечения
  /// \param[in] theSegOnPoly1 индекс сегмента первого многоугольника
  /// \param[in] theSegOnPoly2 индекс сегмента второго многоугольника
  /// \param[in] thePoint      координаты точки пересечения
  void Split(const size_t theSegOnPoly1, const size_t theSegOnPoly2, const Point2D& thePoint)
  {
    myPoints.push_back(thePoint);
    splitPoly(myPoly1, theSegOnPoly1, myPoints.size() - 1);
    splitPoly(myPoly2, theSegOnPoly2 + myPoly2Shift, myPoints.size() - 1);
  }

private:
  void splitPoly(std::unordered_map<size_t, size_t>& thePoly, const size_t theSegment, const size_t theInterPoint)
  {
    auto distance = [](const Point2D& p1, const Point2D& p2) {
      auto dx = p2.x - p1.x;
      auto dy = p2.y - p1.y;
      return dx * dx + dy * dy;
    };

    const Point2D& interPoint = myPoints[theInterPoint];
    size_t cur, next = theSegment;
    bool splitHere = false;

    do {
      cur = next;
      next = thePoly[cur];
      splitHere = next == theSegment + 1; // сегмент ещё не был поделён
      if (!splitHere)
      {
        Point2D startPoint = myPoints[cur];
        Point2D endPoint = myPoints[next];
        splitHere = distance(startPoint, interPoint) < distance(startPoint, endPoint);
      }
    } while (!splitHere);

    // делим сегмент
    thePoly[theInterPoint] = next;
    thePoly[cur] = theInterPoint;
  }

private:
  std::vector<Point2D> myPoints; ///< все доступные узлы
  size_t myPoly2Shift; ///< позиция первой точки второго многоугольника в массиве myPoints
  std::unordered_map<size_t, size_t> myPoly1; ///< список узлов и их соседей для первого многоугольника
  std::unordered_map<size_t, size_t> myPoly2; ///< список узлов и их соседей для второго многоугольника
};


PolyIntersection::PolyIntersection(const ClosedPolygon& thePoly1,
                                   const ClosedPolygon& thePoly2,
                                   const double theTolerance)
  : myDone(Status::NON_CONVEX)
{
  if (isConvex(thePoly1, theTolerance))
    Intersect(thePoly1, thePoly2, theTolerance);
  else if (isConvex(thePoly2, theTolerance))
    Intersect(thePoly2, thePoly1, theTolerance);
}

void PolyIntersection::Intersect(const ClosedPolygon& thePoly1,
                                 const ClosedPolygon& thePoly2,
                                 const double theTolerance)
{
  BinaryTree bTree2(thePoly2);
  BinaryTreeIntersector intersector(bTree2);

  SplitPolygons splitPoly(thePoly1, thePoly2);

  // ищем точки пересечения сторон многоугольников
  bool isInterseced = false;
  for (size_t curInd = thePoly1.size() - 1, nextInd = 0; nextInd < thePoly1.size(); curInd = nextInd++)
  {
    const std::unordered_map<int, Point2D>& points =
        intersector.Intersect(thePoly1[curInd], thePoly1[nextInd], theTolerance);
    for (auto pt : points)
    {
      splitPoly.Split(curInd, pt.first, pt.second);
      isInterseced = true;
    }
  }

  // TODO
}
