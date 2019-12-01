#include <BinaryTree.h>

#include <climits>
#include <cmath>

BinaryTree::BinaryTree(const ClosedPolygon& thePoly)
{
  static const int MAX_DEPTH = 10;
  if (thePoly.size() < (1 << (MAX_DEPTH - 1)))
    myDepth = (size_t)log2(thePoly.size()) + 1;
  else
    myDepth = MAX_DEPTH;

  myChildIndices.reserve(1 << myDepth);
  myBndRect.reserve(1 << myDepth);

  Fill(thePoly);
}

void BinaryTree::Fill(const ClosedPolygon& thePoly)
{
  auto min = [](auto val1, auto val2) { return val1 < val2 ? val1 : val2; };
  auto max = [](auto val1, auto val2) { return val1 > val2 ? val1 : val2; };

  // строим дерево
  myChildIndices.emplace_back(-1, -1, 0, thePoly.size() - 1);
  myBndRect.emplace_back(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
  auto itChild = myChildIndices.begin();
  auto itRect = myBndRect.begin();
  for (int aDepth = 0; aDepth < myDepth; ++itChild, ++itRect)
  {
    // левый подузел
    int size = ((*itChild)[3] - (*itChild)[2]) >> 1;
    int start = (*itChild)[2];
    int end = start + size;
    myChildIndices.emplace_back(-1, -1, start, end);
    myBndRect.emplace_back(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
    (*itChild)[0] = myChildIndices.size() - 1;
    // правый подузел
    start = end + 1;
    end = max(start + size, (*itChild)[3]);
    if (start < end)
    {
      myChildIndices.emplace_back(-1, -1, start, end);
      myBndRect.emplace_back(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
      (*itChild)[1] = myChildIndices.size() - 1;
    }
    if (end == thePoly.size() - 1)
      ++aDepth;
  }

  // вычисляем описанные прямоугольники
  auto itChildRev = myChildIndices.rbegin();
  auto itRectRev = myBndRect.rbegin();
  for (; itChildRev != myChildIndices.rend(); ++itChildRev, ++itRectRev)
  {
    if ((*itChildRev)[0] == -1 && (*itChildRev)[1] == -1)
    {
      // нет дочерних узлов, вычисляем по координатам точек
      if ((*itChildRev)[3] == thePoly.size() - 1)
      {
        (*itRectRev)[0] = (*itRectRev)[2] = thePoly[0].x;
        (*itRectRev)[1] = (*itRectRev)[3] = thePoly[0].y;
      }
      for_each(thePoly + (*itChildRev)[2], thePoly + (*itChildRev)[3] + 1,
               [&itRectRev](const Point2D& thePoint) {
                 (*itRectRev)[0] = min((*itRectRev)[0], thePoint.x); 
                 (*itRectRev)[1] = min((*itRectRev)[1], thePoint.y); 
                 (*itRectRev)[2] = max((*itRectRev)[2], thePoint.x); 
                 (*itRectRev)[3] = max((*itRectRev)[3], thePoint.y); 
               }
      )
    }
    else
    {
      // вычисляем по дочерним узлам
      for (int i = 0; i < 2; ++i)
      {
        if ((*itChildRev)[i] != -1)
        {
          for (int j = 0; j < 4; ++j)
            (*itRectRev)[j] = min((*itRectRev)[j], myBndRect[(*itChildRev)[i]][j]); 
        }
      }
    }
  }
}
