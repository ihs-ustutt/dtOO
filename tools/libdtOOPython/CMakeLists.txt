find_package(PythonInterp REQUIRED)
# component python could be labelled as e.g. python3 or python38
#   -> preliminary search for pyhton3 and python38
#   -> afterwards check with if statement
find_package(
  Boost
  OPTIONAL_COMPONENTS 
    python${PYTHON_VERSION_MAJOR} 
    python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}
)
if (NOT Boost_LIBRARIES)
  message("**> Do not compile dtOOPython, because missing boost_python!")
  return()
endif (NOT Boost_LIBRARIES)
find_package(PythonLibs 3 REQUIRED)
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

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined,--disable-new-dtags")
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

message("**> Compile lib dtOOPython")
