#include <PolyIntersection.h>
#include <PolyIO.h>

#include <iostream>
#include <list>
#include <sstream>

void printHelp(const char* theExeName)
{
  std::cout << "Run executable with arguments:" << std::endl
            << theExeName << " file1 file2 result" << std::endl
            << "       file1 - contains points of the first polygon" << std::endl
            << "       file2 - contains points of the second polygon" << std::endl
            << "       result - store result polygons (generate several indexed files if many polygons)" << std::endl;
}

int main(int argc, char** argv)
{
  bool isPrintHelp = argc != 4;
  if (!isPrintHelp)
  {
    std::string arg1(argv[1]);
    isPrintHelp = arg1 == "--help" || arg1 == "-h" || arg1 == "/?";
  }

  if (isPrintHelp)
  {
    printHelp(argv[0]);
    return -1;
  }

  ClosedPolygon poly1, poly2;
  PolyIO::Read(argv[1], poly1);
  PolyIO::Read(argv[2], poly2);

  PolyIntersection inter(poly1, poly2);
  PolyIntersection::Status state = inter.IsDone();
  if (state == PolyIntersection::Status::FAILED)
  {
    std::cout << "ERROR: Algorithm FAILED!" << std::endl;
    return -1;
  }
  else if (state == PolyIntersection::Status::NON_CONVEX)
  {
    std::cout << "ERROR: Both input polygons are non convex!" << std::endl;
    return -1;
  }

  const std::list<ClosedPolygon>& res = inter.Results();
  if (res.empty())
    std::cout << "Original polygons are not intersected" << std::endl;
  else
  {
    std::cout << "Number of intersection parts is " << res.size() << std::endl;

    std::string resFile(argv[3]);
    std::string resFileExt;
    size_t dotPos = resFile.find_last_of('.');
    if (dotPos != std::string::npos)
    {
      resFileExt = resFile.substr(dotPos);
      resFile.erase(dotPos);
    }

    int index = 0;
    for (auto poly : res)
    {
      std::ostringstream outFileName;
      outFileName << resFile;
      if (res.size() > 1)
        outFileName << ++index;
      outFileName << resFileExt;

      PolyIO::Write(outFileName.str(), poly);
    }
  }

  return 0;
}
