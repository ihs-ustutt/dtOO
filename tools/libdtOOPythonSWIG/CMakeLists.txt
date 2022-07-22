option(DTOO_USE_PYTHONOCC "Use PythonOCC" OFF)

find_package(PythonInterp REQUIRED)

find_package(SWIG 4.0.2 REQUIRED)
include(${SWIG_USE_FILE})

if (DTOO_USE_PYTHONOCC)
  find_package(PythonOCC REQUIRED)
  add_definitions(-DDTOO_HAS_PYTHONOCC)
  include_directories(
    ${PythonOCC_INCLUDE_DIR}/wrapper
    ${PythonOCC_INCLUDE_DIR}/headers
  )
  set(CMAKE_SWIG_FLAGS "-DDTOO_HAS_PYTHONOCC")
endif (DTOO_USE_PYTHONOCC)

find_package(PythonLibs 3 REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_PATH}
)

#
#
# sources
#
#
set_source_files_properties( dtOOPythonSWIG.i PROPERTIES CPLUSPLUS ON )
set_source_files_properties( dtOOPythonSWIG.i PROPERTIES SWIG_FLAGS "" )
set( SOURCES dtOOPythonSWIG.i )

#
#
# library
#
#
SWIG_ADD_LIBRARY(dtOOPythonSWIG TYPE SHARED LANGUAGE python SOURCES ${SOURCES})

#
#
# linking
#
#
SWIG_LINK_LIBRARIES(dtOOPythonSWIG ${PYTHON_LIBRARIES} dtOO)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined,--disable-new-dtags")
set(
  CMAKE_SHARED_LINKER_FLAGS 
  "${CMAKE_SHARED_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib"
)

#
#
# dependencies
#  see https://gitlab.kitware.com/cmake/cmake/-/issues/17918#note_401481
#
# get internal dependencies
get_property(
  dependencies 
  TARGET ${SWIG_MODULE_dtOOPythonSWIG_REAL_NAME} 
  PROPERTY MANUALLY_ADDED_DEPENDENCIES
)
# add external project dependency on internal targets
foreach (dependency IN LISTS dependencies)
  add_dependencies (${dependency} createInclude)
endforeach()

#
#
# install
#
#
install(TARGETS ${SWIG_MODULE_dtOOPythonSWIG_REAL_NAME} DESTINATION tools)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtOOPythonSWIG.py DESTINATION tools)

message("**> Compile lib dtOOPythonSWIG")
