#pragma once

#include <vector>

/// \brief Точка в двумерном пространстве
struct Point2D
{
  double x, y;
};

/// \brief Замкнутый многоугольник. N-угольник хранит ровно N точек.
typedef std::vector<Point2D> ClosedPolygon;
