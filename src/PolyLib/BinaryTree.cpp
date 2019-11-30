#include <BinaryTree.h>

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
  auto max = [](auto val1, auto val2) { return val1 > val2 ? val1 : val2; };

  int size = 1 << (myDepth - 1);
  myChildIndices.emplace_back(-1, -1, 0, thePoly.size() - 1);
  myBndRect.emplace_back(0, 0, 0, 0);
  auto itChild = myChildIndices.begin();
  auto itRect = myBndRect.begin();
  for (; size > 1; ++itChild, ++itRect)
  {
    // левый подузел
    int start = (*itChild)[2];
    int end = start + size;
    myChildIndices.emplace_back(-1, -1, start, end);
    myBndRect.emplace_back(0, 0, 0, 0);
    (*itChild)[0] = myChildIndices.size() - 1;
    // правый подузел
    start = end + 1;
    end = max(start + size, (*itChild)[3]);
    if (start < end)
    {
      myChildIndices.emplace_back(-1, -1, start, end);
      myBndRect.emplace_back(0, 0, 0, 0);
      (*itChild)[1] = myChildIndices.size() - 1;
    }
  }
}
