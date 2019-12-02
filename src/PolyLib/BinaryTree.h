#pragma once

#include <BndRect.h>

/// \brief Представление полигона в виде бинарного дерева
///
/// Каждый узел (поддерево) хранит описанный прямоугольник для сегментов, составляющих данное поддерево.
/// Также хранятся индексы правой и левой ветки и индексы стартового и конечного сегментов.
class BinaryTree
{
public:
  BinaryTree(const ClosedPolygon& thePoly);

private:
  /// \brief Заполнение дерева
  void Fill();

private:
  struct Node
  {
    int myChild[2];    ///< индексы дочерних узлов (-1, если узла нет), leftChild и rightChild
    int mySegments[2]; ///< индексы сегментов, попадающих в данный узел (first, last)
    BndRect myBndRect; ///< координаты описанного прямоугольника

    Node(const int theLeftChild, const int theRightChild, const int theStart, const int theEnd)
    {
      myChild[0] = theLeftChild;
      myChild[1] = theRightChild;
      mySegments[0] = theStart;
      mySegments[1] = theEnd;
    }
  };

  std::vector<Node> myNodes; ///< узлы дерева
  size_t myDepth; ///< глубина дерева (максимальное значение 10, соответствует 1023 узлам дерева)
  const ClosedPolygon& myPoly; ///< многоугольник,Ю для которого построено дерево

  friend class BinaryTreeIntersector;
};
