# Описание файла: Юнит-тесты для библиотеки поиска пересечений многоугольников

set(TEST_NAME "${${PROJECT_NAME}_LIBRARY_NAME}Test")

# необходимо для корректной линковки с GTest
add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY=1)

add_executable(${TEST_NAME} PolyLibTest.cpp)
target_include_directories(${TEST_NAME} PUBLIC ${GTEST_INCLUDE_DIRS} ${${PROJECT_NAME}_SRC_DIR}/PolyLib)
target_link_libraries(${TEST_NAME} ${GTEST_BOTH_LIBRARIES} ${${PROJECT_NAME}_LIBRARY_NAME})

gtest_add_tests(${TEST_NAME} "" AUTO)
