#pragma once

#include <ClosedPolygon.h>
#include <Defines.h>

#include <list>

/// \brief Алгоритм пересечения многоугольников.
///
/// Основан на алгоритме Вейлера-Атертона (https://www.cs.drexel.edu/~david/Classes/CS430/HWs/p214-weiler.pdf).
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
