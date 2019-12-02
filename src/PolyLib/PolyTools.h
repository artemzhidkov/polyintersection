#pragma once

#include <ClosedPolygon.h>

#include <memory>
#include <set>
#include <unordered_map>

namespace PolyTools
{
  /// \brief Проверка многоугольника на выпуклость.
  ///
  /// Все углы должны иметь один и тот же знак. Сумма углов равна pi*(N-2).
  bool isConvex(const ClosedPolygon& thePoly, const double theTolerance);

  /// \brief Проверка, что точка лежит внутри выпуклого многоугольника
  bool isInConvexPolygon(const Point2D& thePoint, const ClosedPolygon& thePoly, const double theTolerance);

  /// \brief Квадрат расстояния между точками
  double squareDistance(const Point2D& p1, const Point2D& p2);

  /// \brief Интерфейс для алгоритмов, работающих со структурой SplitPolygons
  class Algorithm
  {
  public:
    Algorithm() {}
    virtual ~Algorithm() {}

    /// \brief Инициализация алгоритма
    virtual void Init(const std::unordered_map<size_t, size_t>& thePoly1,
                      const std::unordered_map<size_t, size_t>& thePoly2,
                      const std::vector<Point2D>& theCoordinates,
                      const size_t thePoly1StartIndex,
                      const size_t thePoly2StartIndex)
    {
      myPolygons[0] = &thePoly1;
      myPolygons[1] = &thePoly2;
      myCoordinates = &theCoordinates;
      myStartIndices[0] = thePoly1StartIndex;
      myStartIndices[1] = thePoly2StartIndex;
    }

    /// \brief Выполнение алгоритма
    virtual void Perform() = 0;

  protected:
    std::unordered_map<size_t, size_t> const* myPolygons[2];
    std::vector<Point2D> const* myCoordinates;
    size_t myStartIndices[2];
  };

  /// \brief Алгоритм получения пересечения многоугольников
  class AlgoCommon : public Algorithm
  {
  public:
    AlgoCommon(const bool isFirstPointOut) : myFirstPointOut(isFirstPointOut) {}

    virtual void Perform() override;

    const std::list<ClosedPolygon>& Polygons() const { return myResults; }

  private:
    void closedPoly(const int thePolyID, const size_t theStartIndex);

  private:
    bool myFirstPointOut;
    std::list<ClosedPolygon> myResults;

    std::set<size_t> myProcessedPoints;
  };

  /// \brief Вспомогательный класс, хранящий пару многоугольников в виде связанного списка,
  ///        и позволяющий проводить разбиение сторон точками пересечения
  class SplitPolygons
  {
  public:
    SplitPolygons(const ClosedPolygon& thePoly1, const ClosedPolygon& thePoly2);

    /// \brief Делим сегменты полигонов точкой пересечения
    /// \param[in] theSegOnPoly1 индекс сегмента первого многоугольника
    /// \param[in] theSegOnPoly2 индекс сегмента второго многоугольника
    /// \param[in] thePoint      координаты точки пересечения
    void Split(const size_t theSegOnPoly1, const size_t theSegOnPoly2, const Point2D& thePoint);

    /// \brief Выполняет алгоритм на паре многоугольников (например, поиск общих частей)
    void PerformAlgorithm(std::shared_ptr<Algorithm> theAlgo) const;

  private:
    void splitPoly(std::unordered_map<size_t, size_t>& thePoly, const size_t theSegment, const size_t theInterPoint);

  private:
    std::vector<Point2D> myPoints; ///< все доступные узлы
    size_t myPoly2Shift; ///< позиция первой точки второго многоугольника в массиве myPoints
    std::unordered_map<size_t, size_t> myPoly1; ///< список узлов и их соседей для первого многоугольника
    std::unordered_map<size_t, size_t> myPoly2; ///< список узлов и их соседей для второго многоугольника
  };
}
