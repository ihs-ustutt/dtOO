find_package(Python REQUIRED COMPONENTS Interpreter Development)
# component python could be labelled as e.g. python3 or python38
#   -> preliminary search for pyhton3 and python38
#   -> afterwards check with if statement
find_package(
  Boost
  OPTIONAL_COMPONENTS 
    python${Python_VERSION_MAJOR} 
    python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}
)
if (NOT Boost_LIBRARIES)
  message("**> Do not compile dtOOPython, because missing boost_python!")
  return()
endif (NOT Boost_LIBRARIES)
include_directories(
  ${Python_INCLUDE_DIRS}
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
target_link_libraries( dtOOPython dtOO ${Boost_LIBRARIES} ${Python_LIBRARIES})

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
