#pragma once

#include <BinaryTree.h>

/// \brief Алгоритм вычисления точек пересечения отрезка и полигона, представленного в виде бинарного дерева. 
///
/// Возвращает пары следующего вида: координаты точки пересечения и индекс стороны многоугольника
class BinaryTreeIntersector
{
public:
  BinaryTreeIntersector(const BinaryTree& thePolyBT);

  /// \brief Поиск пересечения с отрезком с заданной точностью
  void Intersect(const Point2D& theStart, const Point2D& theEnd, const double theTolerance);

private:
  const BinaryTree& myTree;
};
