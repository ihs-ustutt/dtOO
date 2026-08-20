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

# Test with build.py script
function(DTOO_ADD_BUILD_TEST TNAME)
    add_test(
        NAME ${TNAME}
        COMMAND ${Python3_EXECUTABLE} "build.py"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TNAME}
    )
    set_tests_properties(${TNAME} PROPERTIES ENVIRONMENT "${TESTENVLIST}")
    set_property(TEST ${TNAME} PROPERTY LABELS base-test)
endfunction()

# Test with doctest
function(DTOO_ADD_DOCTEST TESTNAME SCRIPT WDIRPOSTFIX)
    add_test(
        NAME ${TESTNAME}
        COMMAND ${Python3_EXECUTABLE} -m doctest ${SCRIPT} -v
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${WDIRPOSTFIX}
    )
    set_tests_properties(${TESTNAME} PROPERTIES ENVIRONMENT "${TESTENVLIST}")
    set_property(TEST ${TESTNAME} PROPERTY LABELS base-test)
endfunction()

function(APPLY_TEST_TO stringA stringB result)
    string(REGEX MATCH "~([^~.]+)" match "${stringA}")
    if(NOT match)
        set(${result} TRUE PARENT_SCOPE)
        return()
    endif()
    set(substring "${CMAKE_MATCH_1}")
    string(FIND "${stringB}" "${substring}" position)
    if(position GREATER_EQUAL 0)
        set(${result} TRUE PARENT_SCOPE)
    else()
        set(${result} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Add additional mesh check to test
function(DTOO_ADDTESTWLABEL TNAME)
    file(GLOB sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/${TNAME}/*.xz")
    file(GLOB cmp_sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/${TNAME}/*cmp*")

    foreach(xzFilePath ${sources_list})
      foreach(cmpTestPath ${cmp_sources_list})
        get_filename_component(xzFile "${xzFilePath}" NAME)
        get_filename_component(cmpTest "${cmpTestPath}" NAME)
        APPLY_TEST_TO("${cmpTest}" "${xzFile}" APPLY_TO)
        if (APPLY_TO)
          string(REGEX MATCH "gcc-[^ $!~]+" gccVer ${xzFile})
          message("**> Add test ${TNAME}-${cmpTest}-${xzFile} / gccVer=${gccVer}")
          add_test(
            NAME ${TNAME}-${cmpTest}-${xzFile}
            COMMAND ./${cmpTest} "${xzFile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TNAME}
          )
          set_tests_properties(
            ${TNAME}-${cmpTest}-${xzFile} PROPERTIES 
            DEPENDS "${TNAME}"
            LABELS ${gccVer}
            ENVIRONMENT "${TESTENVLIST}"
          )
        else()
          message("**> Skip test ${TNAME}-${cmpTest}-${xzFile} / gccVer=${gccVer}")
        endif()

      endforeach()
    endforeach()
endfunction()

# copy all subdirs in ROOT
function(dtoo_copy_all_subdirs ROOT)
    file(GLOB _entries LIST_DIRECTORIES true "${ROOT}/*")
    foreach(_path ${_entries})
        if(IS_DIRECTORY ${_path})
            get_filename_component(_name "${_path}" NAME)
            message("**> Add target cp-${_name}-test from ${ROOT}")
            add_custom_target(cp-${_name}-test ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${ROOT}/${_name}"
                        "${CMAKE_CURRENT_BINARY_DIR}/${_name}"
            )
        endif()
    endforeach()
endfunction()

# copy one subdir DIR from BASE
function(dtoo_copy_one_dir BASE DIR)
    set(_src "${BASE}/${DIR}")
    if(IS_DIRECTORY "${_src}")
      message("**> Add target cp-${DIR}-test from ${BASE}")
        add_custom_target(cp-${DIR}-test ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${_src}"
                    "${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
        )
    endif()
endfunction()

# common test environment
set(TESTENVLIST
    PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:${CMAKE_INSTALL_PREFIX}/scripts/python:$ENV{PYTHONPATH}
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}
    OSLO_LOCK_PATH=./oslo-lock
)

# Tests with build.py script
DTOO_ADD_BUILD_TEST(baseSWIG)
DTOO_ADD_BUILD_TEST(naca)
DTOO_ADDTESTWLABEL("naca")
DTOO_ADD_BUILD_TEST(transformerSWIG)
DTOO_ADD_BUILD_TEST(analyticGeometrySWIG)
DTOO_ADD_BUILD_TEST(constValueSWIG)
DTOO_ADD_BUILD_TEST(canadaLight)
DTOO_ADD_BUILD_TEST(tistos)
DTOO_ADDTESTWLABEL("tistos")
DTOO_ADD_BUILD_TEST(meshSimpleSurface)
DTOO_ADDTESTWLABEL("meshSimpleSurface")
DTOO_ADD_BUILD_TEST(meshPrismBlock)
DTOO_ADDTESTWLABEL("meshPrismBlock")
DTOO_ADD_BUILD_TEST(dtClusteredSingletonState)
DTOO_ADD_BUILD_TEST(partRotatingMap2dTo3d)
DTOO_ADD_BUILD_TEST(pyDtOO-dtField)
DTOO_ADD_BUILD_TEST(write-runData)

# Tests as doctest
DTOO_ADD_DOCTEST(
  dtClusteredSingletonState-dt
  "${CMAKE_SOURCE_DIR}/scripts/python/pyDtOO/dtClusteredSingletonState.py"
  ""
)
DTOO_ADD_DOCTEST(
  dtValueField
  "${CMAKE_SOURCE_DIR}/scripts/python/pyDtOO/dtValueField.py"
  ""
)
DTOO_ADD_DOCTEST(
  dtField
  "${CMAKE_SOURCE_DIR}/scripts/python/pyDtOO/dtField.py"
  ""
)
DTOO_ADD_DOCTEST(
  interTransMesh
  "${CMAKE_SOURCE_DIR}/test/interTransMesh/interTransMesh.py"
  ""
)
DTOO_ADD_DOCTEST(floatAtt "floatAtt.py" "/floatAtt")
DTOO_ADD_DOCTEST(simpleAxialRunner "simpleAxialRunner.py" "/simpleAxialRunner")
DTOO_ADD_DOCTEST(meshCuboid "meshCuboid.py" "/meshCuboid")
DTOO_ADDTESTWLABEL("meshCuboid")
DTOO_ADD_DOCTEST(multipleBounded "multipleBounded.py" "/multipleBounded")
DTOO_ADDTESTWLABEL("multipleBounded")
DTOO_ADD_DOCTEST(
  labeledVectorHandlingSWIG 
  "labeledVectorHandlingSWIG.py" 
  "/labeledVectorHandlingSWIG"
)
DTOO_ADD_DOCTEST(trans4SidedFace "trans4SidedFace.py" "/trans4SidedFace")
DTOO_ADD_DOCTEST(hydFoilOpt "build.py" "/hydFoilOpt")

# Copy directories
dtoo_copy_all_subdirs("${CMAKE_SOURCE_DIR}/test")
dtoo_copy_one_dir("${CMAKE_SOURCE_DIR}/demo" "hydFoilOpt")
