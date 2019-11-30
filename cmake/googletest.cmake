# Описание файла: Скрипт конфигурации Google Test

if(NOT "${ENABLE_UNIT_TESTING}")
  message(STATUS "Unit testing disabled.")
  unset(${PROJECT_NAME}_UNITTEST_DIR CACHE)
  return()
endif()

set(3RDPARTY_GOOGLETEST_DIR_DESCRIPTION "Path to Google Test root directory")

if(NOT 3RDPARTY_GOOGLETEST_DIR)
  # Директории поиска утилит тестирования
  set(3RDPARTY_GOOGLETEST_DIR "" CACHE PATH "${3RDPARTY_GOOGLETEST_DIR_DESCRIPTION}")

  if(3RDPARTY_DIR)
    # поиск gtest.h в директории сторонних библиотек
    file(GLOB_RECURSE loc_GTEST_INCLUDE "${3RDPARTY_DIR}/*/gtest.h")
    foreach(loc_INC ${loc_GTEST_INCLUDE})
      string(REGEX MATCH "(gtest/gtest.h)$" loc_INC_FOUND "${loc_INC}")
      if(loc_INC_FOUND)
        set(loc_GTEST_INCLUDE ${loc_INC})
        break()
      endif()
    endforeach()

    if(loc_GTEST_INCLUDE)
      get_filename_component(loc_GTEST_DIR "${loc_GTEST_INCLUDE}" DIRECTORY)
      get_filename_component(loc_GTEST_DIR "${loc_GTEST_DIR}/../.." ABSOLUTE)
      set(3RDPARTY_GOOGLETEST_DIR "${loc_GTEST_DIR}" CACHE PATH "${3RDPARTY_GOOGLETEST_DIR_DESCRIPTION}" FORCE)
    endif()
  endif()

endif()

if(3RDPARTY_GOOGLETEST_DIR)
  set(GTEST_ROOT "${3RDPARTY_GOOGLETEST_DIR}")
endif()

find_package(GTest 1.8.0)
if(GTEST_FOUND)
  message(STATUS "Google Test library FOUND")
  enable_testing()
  set(BUILD_TESTING ON)
else()
  message(WARNING "GTest ${GTEST_MINIMAL_VERSION} has not been found. Please set 3RDPARTY_GOOGLETEST_DIR to be able to run unit tests.")
endif()
