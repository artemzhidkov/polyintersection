#pragma once

#include <ClosedPolygon.h>
#include <Defines.h>

#include <list>

/// \brief Алгоритм пересечения многоугольников.
///
/// Основан на алгоритме Сазерленда-Ходжмана (https://dl.acm.org/citation.cfm?id=360802),
/// существенным ограничением, которого является требование выпуклости одного из многоугольников.
class PolyIntersection
{
  /// Точность операций с плавающей точкой по умолчанию
  static const double DEFAULT_TOLERANCE;

public:
  /// \brief Статус алгоритма
  enum class Status
  {
    DONE = 0,  ///< выполнен успешно
    FAILED,    ///< ошибка работы алгоритма
    NON_CONVEX ///< оба многоугольника невыпуклые
  };

  PolyIntersection() = delete;
  PolyIntersection(const PolyIntersection&) = delete;

  /// \brief Конструирование и запуск алгоритма
  PolyIntersection_EXPORT
  PolyIntersection(const ClosedPolygon& thePoly1,
                   const ClosedPolygon& thePoly2,
                   const double theTolerance = DEFAULT_TOLERANCE);

protected:
  /// \brief Пересечение многоугольников.
  /// \param[in] thePoly1 выпуклый многоугольник
  /// \param[in] thePoly2 невыпуклый многоугольник (в общем случае)
  void Intersect(const ClosedPolygon& thePoly1,
                 const ClosedPolygon& thePoly2,
                 const double theTolerance);

private:
  Status myDone; ///< Статус работы алгоритма
  std::list<ClosedPolygon> myResults; ///< Результаты пересечения
};
