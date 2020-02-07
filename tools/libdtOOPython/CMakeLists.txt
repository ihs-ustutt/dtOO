find_package(Boost COMPONENTS python3)
find_package(PythonLibs 3.0 REQUIRED)
FIND_package(PythonInterp 3.0)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${Boost_INCLUDE_DIRS}
)

#
#
# sources
#
#
set( SOURCES dtOOPython.cpp )

#
#
# library
#
#
add_library("dtOOPython" SHARED ${SOURCES})

#
#
# linking
#
#
target_link_libraries( dtOOPython dtOO ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(
  CMAKE_SHARED_LINKER_FLAGS 
  "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib"
)

#
#
# install
#
#
install(TARGETS dtOOPython DESTINATION tools)
