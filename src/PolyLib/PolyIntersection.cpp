#include <PolyIntersection.h>
#include <BinaryTree.h>

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
  // ищем точки пересечения сторон многоугольников
}
