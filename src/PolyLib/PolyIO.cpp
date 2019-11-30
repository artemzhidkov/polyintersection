#include <PolyIO.h>

#include <fstream>

bool PolyIO::Read(const std::string& theFilename, ClosedPolygon& thePolygon)
{
  std::ifstream aFileStream(theFilename, std::ifstream::in);
  bool isOk = aFileStream.is_open();
  if (isOk)
  {
    isOk = Read(aFileStream, thePolygon);
    aFileStream.close();
  }
  return isOk;
}

bool PolyIO::Read(std::istream& theStream, ClosedPolygon& thePolygon)
{
  bool isOk = true;
  double x, y;
  while (!theStream.eof()) {
    theStream >> x;
    if (theStream.eof()) {
      // количество чисел - нечётное
      isOk = false;
      break;
    }
    theStream >> y;
    thePolygon.emplace_back(x, y);
  }
  return isOk;
}

bool PolyIO::Write(const std::string& theFilename, const ClosedPolygon& thePolygon)
{
  std::ofstream aFileStream(theFilename, std::ifstream::out);
  bool isOk = aFileStream.is_open();
  if (isOk)
  {
    isOk = Write(aFileStream, thePolygon);
    aFileStream.close();
  }
  return isOk;
}

bool PolyIO::Write(std::ostream& theStream, const ClosedPolygon& thePolygon)
{
  for (auto pnt : thePolygon)
    theStream << pnt.x << " " << pnt.y << std::endl;
  return true;
}
