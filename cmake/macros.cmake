# Описание файла: Вспомогательные макро-команды

# Включает все доступные предупреждения компилятора
function(enable_warnings)

  if("_${CMAKE_CXX_COMPILER_ID}_" STREQUAL "_MSVC_")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
  elseif("_${CMAKE_CXX_COMPILER_ID}_" STREQUAL "_GNU_")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()
  get_property(docString CACHE CMAKE_CXX_FLAGS PROPERTY HELPSTRING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "${docString}" FORCE)

endfunction(enable_warnings)
