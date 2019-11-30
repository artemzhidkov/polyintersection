#pragma once

#include <ClosedPolygon.h>

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
  void Fill(const ClosedPolygon& thePoly);

private:
  std::vector<int[4]> myChildIndices; ///< индексы дочерних узлов (-1, если узла нет) и сегментов, попадающих в данный узел (left, right, first, last)
  std::vector<double[4]> myBndRect;   ///< координаты описанного прямоугольника (xMin, yMin, xMax, yMax)
  size_t myDepth; ///< глубина дерева (максимальное значение 10, соответствует 1023 узлам дерева)
};
