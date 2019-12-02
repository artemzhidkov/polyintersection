#pragma once

#include <vector>

/// \brief Точка в двумерном пространстве
struct Point2D
{
  double x, y;

  Point2D(const double theX = 0.0, const double theY = 0.0) : x(theX), y(theY) {}
};

/// \brief Замкнутый многоугольник. N-угольник хранит ровно N точек.
typedef std::vector<Point2D> ClosedPolygon;
