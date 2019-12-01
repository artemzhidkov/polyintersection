#pragma once

#include <ClosedPolygon.h>

/// \brief Описанный прямоугольник на плоскости, заданный диагональю.
///
/// Прредоставляет интерфейс для добавления новых точек либо объединения с другим описанным прямоугольником.
/// В этом случае, границы прямоугольника расширяются.
class BndRect
{
public:
  BndRect();

  /// \brief Добавление точки
  void Add(const Point2D& thePoint);
  /// \brief Объединение с другим прямоугольником
  void Add(const BndRect& theRect);

  /// \brief Проверка, пересекаются ли прямоугольники с заданной точностью
  bool IsIntersected(const BndRect& theRect, const double theTolerance) const;

private:
  Point2D myMinCorner; ///< узел с минимальными координатами
  Point2D myMaxCorner; ///< узел с максимальными координатами
};
