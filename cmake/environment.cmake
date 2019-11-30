# Описание файла: Переменные для конфигурации процесса сборки

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE INTERNAL "Configuration types" FORCE)
set(CMAKE_DEBUG_POSTFIX "d")

set(${PROJECT_NAME}_LIBRARY_NAME "${PROJECT_NAME}"                  CACHE STRING   "Name of ${PROJECT_NAME}'s library")
set(${PROJECT_NAME}_ROOT_DIR     "${CMAKE_SOURCE_DIR}"              CACHE PATH     "Path to root project directory")
set(${PROJECT_NAME}_SRC_DIR      "${${PROJECT_NAME}_ROOT_DIR}/src"  CACHE PATH     "Path to source directory")
set(${PROJECT_NAME}_UNITTEST_DIR "${${PROJECT_NAME}_ROOT_DIR}/test" CACHE PATH     "Path to unit-tests source directory")
set(${PROJECT_NAME}_BUILD_DIR    "${CMAKE_BINARY_DIR}"              CACHE INTERNAL "Build directory" FORCE)
set(${PROJECT_NAME}_TESTDATA_DIR "${${PROJECT_NAME}_ROOT_DIR}/data" CACHE PATH     "Path to test data directory")
set(${PROJECT_NAME}_DOC_DIR      "${${PROJECT_NAME}_ROOT_DIR}/dox"  CACHE PATH     "Path to documentation directory")
set(${PROJECT_NAME}_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}"          CACHE PATH     "Path to installation directory")

set(INSTALL_BINARY  "${${PROJECT_NAME}_INSTALL_DIR}/bin"     CACHE PATH "Path to install project binaries"      FORCE)
set(INSTALL_LIBRARY "${${PROJECT_NAME}_INSTALL_DIR}/lib"     CACHE PATH "Path to install project libraries"     FORCE)
set(INSTALL_INCLUDE "${${PROJECT_NAME}_INSTALL_DIR}/include" CACHE PATH "Path to install project includes"      FORCE)
set(INSTALL_DOC     "${${PROJECT_NAME}_INSTALL_DIR}/doc"     CACHE PATH "Path to install project documentation" FORCE)

# Force overriding CMAKE_INSTALL_PREFIX
set(CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_INSTALL_DIR}" CACHE INTERNAL "Install path prefix, prepended onto install directories" FORCE)

set(ENABLE_ALL_WARNINGS  True  CACHE BOOL "Enable warning level 4")
set(ENABLE_DOCUMENTATION False CACHE BOOL "Enable generation of documentation")
set(ENABLE_UNIT_TESTING  False CACHE BOOL "Enable generation of unit-tests")

# Third parties
set(3RDPARTY_DIR "" CACHE PATH "Third parties root directory")
