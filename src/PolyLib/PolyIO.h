#pragma once

#include <Defines.h>
#include <ClosedPolygon.h>

#include <string>

/// \brief Чтение/запись вершин полигона из файлов или входных.выходных потоков.
///
/// Многоугольники читаются/пишутся в текстовом виде в формате:
/// x1 y1 x2 y2 ... xN yN
///
/// Разделителями выступают пробелы, символы табуляции и перехода на новую строку.
namespace PolyIO
{
  /// \brief Чтение многоугольника из файла.
  /// \param[in]  theFilename имя файла для чтения
  /// \param[out] thePolygon  многоугольник
  /// \return \c true, если файл прочитан без ошибок.
  PolyIntersection_EXPORT bool Read(const std::string& theFilename, ClosedPolygon& thePolygon);

  /// \brief Чтение многоугольника из входного потока.
  /// \param[in]  theStream  входной поток для чтения
  /// \param[out] thePolygon многоугольник
  /// \return \c true, если многоугольник прочитан без ошибок.
  PolyIntersection_EXPORT bool Read(std::istream& theStream, ClosedPolygon& thePolygon);

  /// \brief Запись многоугольника в файл.
  /// \param[in]  theFilename имя файла для записи
  /// \param[out] thePolygon  многоугольник
  /// \return \c true, если файл записан без ошибок.
  PolyIntersection_EXPORT bool Write(const std::string& theFilename, const ClosedPolygon& thePolygon);

  /// \brief Запись многоугольника в выходной поток.
  /// \param[in]  theStream  выходной поток для записи
  /// \param[out] thePolygon многоугольник
  /// \return \c true, если многоугольник записан без ошибок.
  PolyIntersection_EXPORT bool Write(std::ostream& theStream, const ClosedPolygon& thePolygon);
};
