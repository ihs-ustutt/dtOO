find_package(PythonInterp REQUIRED)
# component python could be labelled as e.g. python3 or python38
#   -> preliminary search for pyhton3 and python38
#   -> afterwards check with if statement
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonOCC REQUIRED)

find_package(PythonLibs 3 REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${PythonOCC_INCLUDE_DIR}/wrapper
  ${PythonOCC_INCLUDE_DIR}/headers
)

set(CMAKE_SWIG_FLAGS "")

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

#
#
# install
#
#
install(TARGETS ${SWIG_MODULE_dtOOPythonSWIG_REAL_NAME} DESTINATION tools)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtOOPythonSWIG.py DESTINATION tools)

message("**> Compile lib dtOOPythonSWIG")
