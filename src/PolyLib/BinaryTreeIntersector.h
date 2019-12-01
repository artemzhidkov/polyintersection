#pragma once

#include <BinaryTree.h>

#include <list>
#include <unordered_map>

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
  /// \brief Поиск сторон многоугольника, которые потенциально пересекают заданный прямоугольник
  /// \param[in] theRect      целевой прямоугольник, с которым ищутся пересечения
  /// \param[in] theTreeNode  индекс узла дерева, для которого осуществляется проверка
  /// \param[in] theTolerance точность вычислений
  ///\param[out] theSegments  список начальных и конечных индексов сторон многоугольника, которые могут пересекать theRect
  void SearchSegments(const BndRect& theRect, const int theTreeNode, const double theTolerance,
                      std::list<std::pair<int, int> >& theSegments) const;

private:
  const BinaryTree& myTree;
  std::unordered_map<int, Point2D> myIntersectionPoints; ///< индекс сегмента и точки пересечения с ним
};
