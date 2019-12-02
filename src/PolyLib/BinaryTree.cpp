#include <BinaryTree.h>

#include <algorithm>
#include <cmath>

BinaryTree::BinaryTree(const ClosedPolygon& thePoly)
  : myPoly(thePoly)
{
  static const int MAX_DEPTH = 10;
  if (thePoly.size() < (1 << (MAX_DEPTH - 1)))
    myDepth = (size_t)log2(thePoly.size()) + 1;
  else
    myDepth = MAX_DEPTH;

  myNodes.reserve(1ULL << myDepth);

  Fill();
}

void BinaryTree::Fill()
{
  auto max = [](auto val1, auto val2) { return val1 > val2 ? val1 : val2; };

  // строим дерево
  myNodes.emplace_back(-1, -1, 0, (int)myPoly.size() - 1);
  auto itNode = myNodes.begin();
  for (int aDepth = 0; aDepth < myDepth; ++itNode)
  {
    // левый подузел
    int size = (itNode->mySegments[1] - itNode->mySegments[0]) >> 1;
    int start = itNode->mySegments[0];
    int end = start + size;
    myNodes.emplace_back(-1, -1, start, end);
    itNode->myChild[0] = (int)myNodes.size() - 1;
    // правый подузел
    start = end + 1;
    end = max(start + size, itNode->mySegments[1]);
    if (start < end)
    {
      myNodes.emplace_back(-1, -1, start, end);
      itNode->myChild[1] = (int)myNodes.size() - 1;
    }
    if (end == (int)myPoly.size() - 1)
      ++aDepth;
  }

  // вычисляем описанные прямоугольники
  auto itRev = myNodes.rbegin();
  for (; itRev != myNodes.rend(); ++itRev)
  {
    if (itRev->myChild[0] == -1 && itRev->myChild[1] == -1)
    {
      // нет дочерних узлов, вычисляем по координатам точек
      if (itRev->mySegments[1] == (int)myPoly.size() - 1)
        itRev->myBndRect.Add(myPoly[0]);
      std::for_each(myPoly.begin() + itRev->mySegments[0],
                    myPoly.begin() + itRev->mySegments[1] + 1,
                    [&itRev](const Point2D& thePoint) { itRev->myBndRect.Add(thePoint); }
      );
    }
    else
    {
      // вычисляем по дочерним узлам
      for (int i = 0; i < 2; ++i)
        if (itRev->myChild[i] != -1)
          itRev->myBndRect.Add(myNodes[itRev->myChild[i]].myBndRect);
    }
  }
}
