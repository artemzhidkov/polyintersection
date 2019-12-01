#include <PolyTools.h>

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>


// единичный вектор на отрезке, соединяющем точки
static auto direction = [](const Point2D& theStart, const Point2D& theEnd, const double theTolerance)
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

bool PolyTools::isConvex(const ClosedPolygon& thePoly, const double theTolerance)
{
  if (thePoly.size() <= 2)
    return false;

  // вычисление угла по трём точкам в диапазоне (-pi, pi]
  auto angle = [&](const Point2D& theFirst, const Point2D& theSecond, const Point2D& theThird)
  {
    auto dir1 = direction(theFirst, theSecond, theTolerance);
    auto dir2 = direction(theSecond, theThird, theTolerance);
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

bool PolyTools::isInConvexPolygon(const Point2D& thePoint, const ClosedPolygon& thePoly, const double theTolerance)
{
  auto prev = --thePoly.end();
  for (auto cur = thePoly.begin(); cur != thePoly.end(); prev = cur++)
  {
    auto dirSeg = direction(*prev, *cur, theTolerance);
    auto dirSP = direction(*prev, thePoint, theTolerance);
    if (dirSeg.x * dirSP.y - dirSeg.y * dirSP.x < -theTolerance)
      return false; // точка лежит справа от сегмента (вне выпуклого многоугольника)
  }
  return true;
}

double PolyTools::squareDistance(const Point2D& p1, const Point2D& p2)
{
  auto dx = p2.x - p1.x;
  auto dy = p2.y - p1.y;
  return dx * dx + dy * dy;
};




PolyTools::SplitPolygons::SplitPolygons(const ClosedPolygon& thePoly1, const ClosedPolygon& thePoly2)
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

void PolyTools::SplitPolygons::Split(const size_t theSegOnPoly1, const size_t theSegOnPoly2, const Point2D& thePoint)
{
  myPoints.push_back(thePoint);
  splitPoly(myPoly1, theSegOnPoly1, myPoints.size() - 1);
  splitPoly(myPoly2, theSegOnPoly2 + myPoly2Shift, myPoints.size() - 1);
}

void PolyTools::SplitPolygons::splitPoly(std::unordered_map<size_t, size_t>& thePoly, const size_t theSegment, const size_t theInterPoint)
{
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
      splitHere = squareDistance(startPoint, interPoint) < squareDistance(startPoint, endPoint);
    }
  } while (!splitHere);

  // делим сегмент
  thePoly[theInterPoint] = next;
  thePoly[cur] = theInterPoint;
}

void PolyTools::SplitPolygons::PerformAlgorithm(std::shared_ptr<Algorithm> theAlgo) const
{
  theAlgo->Init(myPoly1, myPoly2, myPoints, 0, myPoly2Shift);
  theAlgo->Perform();
}



void PolyTools::AlgoCommon::Perform()
{
  int polyID = 1;
  size_t curPoint = myStartIndices[polyID];
  while (myFirstPointOut)
  {
    // ищем точку пересечения многоугольников
    curPoint = myPolygons[polyID]->at(curPoint);
    myFirstPointOut = myPolygons[1 - polyID]->find(curPoint) == myPolygons[1 - polyID]->end();
  }

  // собираем многоугольики
  while (curPoint != myStartIndices[polyID])
  {
    if (myProcessedPoints.find(curPoint) == myProcessedPoints.end())
      closedPoly(polyID, curPoint);
    curPoint = myPolygons[polyID]->at(curPoint);
  }
}

void PolyTools::AlgoCommon::closedPoly(const int thePolyID, const size_t theStartIndex)
{
  myResults.emplace_back();
  auto newPoly = myResults.back();

  int polyID = thePolyID;
  size_t curIndex = theStartIndex;
  do {
    myProcessedPoints.insert(curIndex);
    newPoly.push_back(myCoordinates[curIndex]);

    curIndex = myPolygons[polyID]->at(curIndex);
    if (myPolygons[1 - polyID]->find(curIndex) != myPolygons[1 - polyID]->end())
      polyID = 1 - polyID; // точка пересечения => меняем многоугольник, по которому идём
  } while (curIndex != theStartIndex);
}
