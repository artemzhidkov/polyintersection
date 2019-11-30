# Описание файла: Конфигурация приложения для запуска алгоритмов

set(APP_NAME "${${PROJECT_NAME}_LIBRARY_NAME}App")

add_executable(${APP_NAME} "PolyApp.cpp")
target_include_directories(${APP_NAME} PRIVATE "../PolyLib")
target_link_libraries(${APP_NAME} ${${PROJECT_NAME}_LIBRARY_NAME})

install(TARGETS ${APP_NAME} DESTINATION ${INSTALL_BINARY})
if(WIN32)
  install(FILES $<TARGET_PDB_FILE:${APP_NAME}>
          DESTINATION ${INSTALL_BINARY} OPTIONAL)
endif()
