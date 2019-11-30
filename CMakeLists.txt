# Описание файла: Корневой CMake файла проекта

cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project("PolyIntersection" VERSION 0.1.0 LANGUAGES C CXX)

# Настройки проекта
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Необходимые переменные
include(environment)
include(macros)

# Включение всех предупреждений
if(${ENABLE_ALL_WARNINGS})
  enable_warnings()
endif()

# Генерация документации
if(${ENABLE_DOCUMENTATION})
  include(documentation)
endif()

# Тестирование
if(${ENABLE_UNIT_TESTING})
  include(googletest)
else()
  set(GTEST_FOUND 0)
endif()

add_subdirectory(src)
if(${BUILD_TESTING})
  add_subdirectory(test)
endif()
