# Описание файла: Конфигурация библиотеки поиска пересечения плоских многоугольников

set(HEADERS Defines.h
            BinaryTree.h
            BinaryTreeIntersector.h
            BndRect.h
            ClosedPolygon.h
            PolyIntersection.h
            PolyIO.h
            PolyTools.h
)

set(SOURCES BinaryTree.cpp
            BinaryTreeIntersector.cpp
            BndRect.cpp
            PolyIntersection.cpp
            PolyIO.cpp
            PolyTools.cpp
)

add_definitions(-DPOLYLIB_EXPORTS)

add_library(${${PROJECT_NAME}_LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES})
target_compile_features(${${PROJECT_NAME}_LIBRARY_NAME} PUBLIC cxx_std_11)

install(TARGETS ${${PROJECT_NAME}_LIBRARY_NAME}
        RUNTIME DESTINATION ${INSTALL_BINARY}
        LIBRARY DESTINATION ${INSTALL_BINARY}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY})
install(FILES ${HEADERS} DESTINATION ${INSTALL_INCLUDE})
if(WIN32)
  install(FILES $<TARGET_PDB_FILE:${${PROJECT_NAME}_LIBRARY_NAME}>
          DESTINATION ${INSTALL_BINARY} OPTIONAL)
endif()
