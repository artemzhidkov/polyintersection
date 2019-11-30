# Описание файла: Скрипт конфигурации Doxygen

if(NOT "${ENABLE_DOCUMENTATION}")
  message(STATUS "Documentation disabled.")
  unset(${PROJECT_NAME}_DOC_DIR CACHE)
  unset(INSTALL_DOC CACHE)
  return()
endif()

set(3RDPARTY_DOXYGEN_DIR_DESCRIPTION  "Path to Doxygen root directory")
set(3RDPARTY_GRAPHVIZ_DIR_DESCRIPTION "Path to Graphviz root directory")

if(NOT "${3RDPARTY_DOXYGEN_DIR}" OR NOT "${3RDPARTY_GRAPHVIZ_DIR}")
  # Директории поиска утилит генерации документации
  set(3RDPARTY_DOXYGEN_DIR "" CACHE PATH "${3RDPARTY_DOXYGEN_DIR_DESCRIPTION}")
  set(3RDPARTY_GRAPHVIZ_DIR "" CACHE PATH "${3RDPARTY_GRAPHVIZ_DIR_DESCRIPTION}")

  if(3RDPARTY_DIR)
    # Поиск директорий по ключевым словам
    set(loc_DOCS "DOXYGEN" "GRAPHVIZ")
    foreach(doc ${loc_DOCS})
      file(GLOB loc_FOUND "${3RDPARTY_DIR}/${doc}*")
      foreach(p ${loc_FOUND})
        if(IS_DIRECTORY "${p}")
          set(CMAKE_PREFIX_PATH "${p}" "${CMAKE_PREFIX_PATH}")
        endif()
      endforeach()
    endforeach()
  endif()

else()
  set(CMAKE_PREFIX_PATH "${3RDPARTY_DOXYGEN_DIR}" "${3RDPARTY_GRAPHVIZ_DIR}" "${CMAKE_PREFIX_PATH}")
endif()

find_package(Doxygen 1.8.9)
if(${DOXYGEN_FOUND})
  message(STATUS "Doxygen version ${DOXYGEN_VERSION} FOUND")
  if(NOT "${3RDPARTY_DOXYGEN_DIR}")
    get_filename_component(loc_PATH "${DOXYGEN_EXECUTABLE}" DIRECTORY)
    get_filename_component(loc_PATH "${loc_PATH}/.." ABSOLUTE)
    set(3RDPARTY_DOXYGEN_DIR "${loc_PATH}" CACHE PATH "${3RDPARTY_DOXYGEN_DIR_DESCRIPTION}" FORCE)
  endif()

  if(NOT "${3RDPARTY_GRAPHVIZ_DIR}" AND DOXYGEN_DOT_EXECUTABLE)
    get_filename_component(loc_PATH "${DOXYGEN_DOT_EXECUTABLE}" DIRECTORY)
    get_filename_component(loc_PATH "${loc_PATH}/.." ABSOLUTE)
    set(3RDPARTY_GRAPHVIZ_DIR "${loc_PATH}" CACHE PATH "${3RDPARTY_GRAPHVIZ_DIR_DESCRIPTION}" FORCE)
  endif()

  # setup Doxygen
  set(DOXYFILE_IN ${${PROJECT_NAME}_DOC_DIR}/Doxyfile.in)
  set(DOXYFILE    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  set(DOXY_INPUT  ${${PROJECT_NAME}_SRC_DIR})
  set(DOXY_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doc)
  set(DOXY_MDPAGES  ${${PROJECT_NAME}_ROOT_DIR})
  set(DOXY_MAINPAGE ${${PROJECT_NAME}_ROOT_DIR}/README.md)

  configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)

  add_custom_target(Documentation ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
                    SOURCES ${DOXYFILE}
                    VERBATIM)
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/html"
          DESTINATION ${INSTALL_DOC}
          COMPONENT "Documentation")
  install(FILES "${${PROJECT_NAME}_DOC_DIR}/doc.html"
          DESTINATION ${INSTALL_DOC}
          COMPONENT "Documentation")
  
else()
  message(WARNING "Doxygen ${DOXYGEN_MINIMAL_VERSION} has not been found. Unable to generate documentation")
endif()
