#include <BndRect.h>

#include <climits>

static auto min = [](auto val1, auto val2) { return val1 < val2 ? val1 : val2; };
static auto max = [](auto val1, auto val2) { return val1 > val2 ? val1 : val2; };

BndRect::BndRect()
  : myMinCorner({ DBL_MAX, DBL_MAX }),
    myMaxCorner({ -DBL_MAX, -DBL_MAX })
{
}

void BndRect::Add(const Point2D& thePoint)
{
  myMinCorner.x = min(myMinCorner.x, thePoint.x);
  myMinCorner.y = min(myMinCorner.y, thePoint.y);
  myMaxCorner.x = max(myMaxCorner.x, thePoint.x);
  myMaxCorner.y = max(myMaxCorner.y, thePoint.y);
}

void BndRect::Add(const BndRect& theRect)
{
  myMinCorner.x = min(myMinCorner.x, theRect.myMinCorner.x);
  myMinCorner.y = min(myMinCorner.y, theRect.myMinCorner.y);
  myMaxCorner.x = max(myMaxCorner.x, theRect.myMaxCorner.x);
  myMaxCorner.y = max(myMaxCorner.y, theRect.myMaxCorner.y);
}

bool BndRect::IsIntersected(const BndRect& theRect, const double theTolerance) const
{
  bool isOut = theRect.myMinCorner.x > myMaxCorner.x + theTolerance ||
               theRect.myMinCorner.y > myMaxCorner.y + theTolerance ||
               theRect.myMaxCorner.x + theTolerance < myMinCorner.x ||
               theRect.myMaxCorner.y + theTolerance < myMinCorner.y;
  return !isOut;
}
