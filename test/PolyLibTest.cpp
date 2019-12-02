#include <PolyLibTestConfig.h>

#include <PolyIntersection.h>
#include <PolyIO.h>

// GoogleTest includes
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#include <gtest/gtest.h>

/// \brief Класс для тестирования алгоритма пересечения многоугольников
class TestIntersection : public ::testing::Test
{
public:
  /// \brief Запуск теста
  /// \param[in] theFileName1 имя файла, содержащего первый многоугольник
  /// \param[in] theFileName2 имя файла, содержащего второй многоугольник
  void Perform(const std::string& theFileName1, const std::string& theFileName2)
  {
    ClosedPolygon poly1, poly2;
    ASSERT_TRUE(PolyIO::Read(TEST_DATA_PATH + theFileName1, poly1));
    ASSERT_TRUE(PolyIO::Read(TEST_DATA_PATH + theFileName2, poly2));

    PolyIntersection inter(poly1, poly2);
    ASSERT_EQ(inter.IsDone(), PolyIntersection::Status::DONE);

    myResults = inter.Results();
  }

  /// \brief Проверка количества многоугольников и их размерности
  void CheckResults(const std::list<size_t>& theNbNodes) const
  {
    ASSERT_EQ(myResults.size(), theNbNodes.size());
    auto itPoly = myResults.begin();
    auto itNbNodes = theNbNodes.begin();
    for (; itPoly != myResults.end(); ++itPoly, ++itNbNodes)
      ASSERT_EQ(itPoly->size(), *itNbNodes);
  }

  /// \brief Проверка количества многоугольников и координат узлов
  void CheckResults(const std::list<ClosedPolygon>& theReference) const
  {
    static const double TOLERANCE = 1.e-7;

    ASSERT_EQ(myResults.size(), theReference.size());
    auto itPoly = myResults.begin();
    auto itRef = theReference.begin();
    for (; itPoly != myResults.end(); ++itPoly, ++itRef)
    {
      ASSERT_EQ(itPoly->size(), itRef->size());
      for (auto itPolyPnt = itPoly->begin(), itRefPnt = itRef->begin(); itPolyPnt != itPoly->end(); ++itPolyPnt, ++itRefPnt)
      {
        EXPECT_NEAR(itPolyPnt->x, itRefPnt->x, TOLERANCE);
        EXPECT_NEAR(itPolyPnt->y, itRefPnt->y, TOLERANCE);
      }
    }
  }

private:
  std::list<ClosedPolygon> myResults;
};


TEST_F(TestIntersection, NotIntersected1)
{
  Perform("rect1x1.txt", "corner.txt");
  CheckResults(std::list<size_t>());
}

TEST_F(TestIntersection, NotIntersected2)
{
  Perform("rect1x1.txt", "almost-ring.txt");
  CheckResults(std::list<size_t>());
}

TEST_F(TestIntersection, FirstInside)
{
  Perform("rect1x1.txt", "octo3x3.txt");

  ClosedPolygon ref;
  PolyIO::Read(TEST_DATA_PATH + "rect1x1.txt", ref);
  CheckResults({ref});
}

TEST_F(TestIntersection, SecondInside)
{
  Perform("octo3x3.txt", "rect1x1.txt");

  ClosedPolygon ref;
  PolyIO::Read(TEST_DATA_PATH + "rect1x1.txt", ref);
  CheckResults({ ref });
}

TEST_F(TestIntersection, Intersection1)
{
  Perform("triangle1.txt", "non-convex.txt");
  CheckResults({ 10 });
}

TEST_F(TestIntersection, Intersection2)
{
  Perform("triangle2.txt", "non-convex.txt");
  CheckResults({ 8 });
}

TEST_F(TestIntersection, Intersection3)
{
  Perform("triangle3.txt", "non-convex.txt");
  CheckResults({ 8 });
}

TEST_F(TestIntersection, Intersection4)
{
  Perform("triangle4.txt", "non-convex.txt");
  CheckResults({ 7, 4 });
}

TEST_F(TestIntersection, Intersection5)
{
  Perform("triangle5.txt", "non-convex.txt");
  CheckResults({ 6, 4 });
}

TEST_F(TestIntersection, Intersection6)
{
  Perform("triangle6.txt", "non-convex.txt");
  CheckResults({ 3, 4, 3 });
}


//=============================================================================
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
