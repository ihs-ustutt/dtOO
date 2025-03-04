#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

function(DTOO_ADDTESTWLABEL TNAME)
  file(GLOB sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/${TNAME}/*.xz")
  foreach(xzFilePath ${sources_list})
    get_filename_component(xzFile "${xzFilePath}" NAME)
    string(REGEX MATCH "gcc-[^ $!~]+" gccVer ${xzFile})
    message("**> Add test ${TNAME}-cmp-${xzFile} / gccVer=${gccVer}")
    add_test(
      NAME ${TNAME}-cmp-${xzFile} 
      COMMAND sh "cmp.sh" "${xzFile}" 
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TNAME}
    )
    set_tests_properties(${TNAME}-cmp-${xzFile} PROPERTIES DEPENDS "${TNAME}")
    set_property(TEST ${TNAME}-cmp-${xzFile}  PROPERTY LABELS ${gccVer})
  endforeach()
endfunction()

set( 
  TESTENVLIST
  PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:${CMAKE_INSTALL_PREFIX}/scripts/python:$ENV{PYTHONPATH}
  LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}
  OSLO_LOCK_PATH=./oslo-lock
)

#
# baseSWIG
#
add_test(
  NAME baseSWIG 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/baseSWIG
)
set_tests_properties(baseSWIG PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST baseSWIG  PROPERTY LABELS base-test)

#
# naca
#
add_test(
  NAME naca 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/naca
)
set_tests_properties(naca PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST naca  PROPERTY LABELS base-test)
DTOO_ADDTESTWLABEL("naca")

#
# transformerSWIG
#
add_test(
  NAME transformerSWIG 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/transformerSWIG
)
set_tests_properties(transformerSWIG PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST transformerSWIG  PROPERTY LABELS base-test)

#
# analyticGeometrySWIG
#
add_test(
  NAME analyticGeometrySWIG 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/analyticGeometrySWIG
)
set_tests_properties(
  analyticGeometrySWIG PROPERTIES ENVIRONMENT "${TESTENVLIST}"
)
set_property(TEST analyticGeometrySWIG  PROPERTY LABELS base-test)

#
# constValueSWIG
#
add_test(
  NAME constValueSWIG 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/constValueSWIG
)
set_tests_properties(constValueSWIG PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST constValueSWIG  PROPERTY LABELS base-test)

#
# canadaLight
#
add_test(
  NAME canadaLight 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/canadaLight
)
set_tests_properties(canadaLight PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST canadaLight PROPERTY LABELS base-test)

#
# tistos
#
add_test(
  NAME tistos 
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tistos
)
set_tests_properties(tistos PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST tistos PROPERTY LABELS base-test)
DTOO_ADDTESTWLABEL("tistos")

#
# meshSimpleSurface
#
add_test(
  NAME meshSimpleSurface
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/meshSimpleSurface
)
set_tests_properties(meshSimpleSurface PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST meshSimpleSurface PROPERTY LABELS base-test)
DTOO_ADDTESTWLABEL("meshSimpleSurface")
#
# meshPrismBlock
#
add_test(
  NAME meshPrismBlock
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/meshPrismBlock
)
set_tests_properties(meshPrismBlock PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST meshPrismBlock PROPERTY LABELS base-test)
DTOO_ADDTESTWLABEL("meshPrismBlock")

#
# dtClusteredSingletonState
#
add_test(
  NAME dtClusteredSingletonState
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dtClusteredSingletonState
)
set_tests_properties(dtClusteredSingletonState PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST dtClusteredSingletonState PROPERTY LABELS base-test)

#
# partRotatingMap2dTo3d
#
add_test(
  NAME partRotatingMap2dTo3d
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/partRotatingMap2dTo3d
)
set_tests_properties(
  partRotatingMap2dTo3d PROPERTIES ENVIRONMENT "${TESTENVLIST}"
)
set_property(TEST partRotatingMap2dTo3d PROPERTY LABELS base-test)

#
# pyDtOO-dtField
#
add_test(
  NAME pyDtOO-dtField
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pyDtOO-dtField
)
set_tests_properties(pyDtOO-dtField PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST pyDtOO-dtField PROPERTY LABELS base-test)

#
# write-runData
#
add_test(
  NAME write-runData
  COMMAND ${Python3_EXECUTABLE} "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/write-runData
)
set_tests_properties(write-runData PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST write-runData PROPERTY LABELS base-test)

#
# pyDtOO-doctest
#
add_test(
  NAME pyDtOO-doctest
  COMMAND 
    "${Python3_EXECUTABLE}" 
    "-m" "doctest" 
    "${CMAKE_SOURCE_DIR}/scripts/python/pyDtOO/dtClusteredSingletonState.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pyDtOO-doctest
)
set_tests_properties(pyDtOO-doctest PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST pyDtOO-doctest PROPERTY LABELS base-test)

#
# interTransMesh
#
add_test(
  NAME interTransMesh
  COMMAND 
    "${Python3_EXECUTABLE}" 
    "-m" "doctest" 
    "${CMAKE_SOURCE_DIR}/scripts/python/pyDtOO/dtClusteredSingletonState.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interTransMesh
)
set_tests_properties(interTransMesh PROPERTIES ENVIRONMENT "${TESTENVLIST}")
set_property(TEST interTransMesh PROPERTY LABELS base-test)

file(GLOB sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/*")
foreach(dirPath ${sources_list})
    IF(IS_DIRECTORY ${dirPath})
      get_filename_component(dir "${dirPath}" NAME)
      message("**> Add target cp-${dir}-test")
      add_custom_target(
        cp-${dir}-test ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/test/${dir}
          ${CMAKE_CURRENT_BINARY_DIR}/${dir}
      )
    ENDIF()
endforeach()
