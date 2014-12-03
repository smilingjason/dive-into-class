cmake_minimum_required (VERSION 2.8)

project (DiveIntoClass)

set (PLATFORM "linux")

# Reset default linker flags
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

#To be removed in final release version
add_definitions(-DDIVEINTOCLASS_DEBUG)

# Currently only support 32-bit platforms.
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  add_definitions (-m32)
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
  set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif ()

add_definitions (-g)
add_definitions (-std=c99)
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
add_executable(dic diveintoclass.c)
