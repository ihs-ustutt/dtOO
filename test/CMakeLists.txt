#
# baseSWIG
#
add_test(
  NAME baseSWIG 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/baseSWIG
)
set_tests_properties(
  baseSWIG 
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)

#
# naca
#
add_test(
  NAME naca 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/naca
)
set_tests_properties(
  naca
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
 
)
file(GLOB sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/naca/*.xz")
foreach(xzFilePath ${sources_list})
  get_filename_component(xzFile "${xzFilePath}" NAME)
  message("**> Add test naca-cmp-${xzFile}")
  add_test(
    NAME naca-cmp-${xzFile} 
    COMMAND sh "cmp.sh" "${xzFile}" 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/naca
  )
endforeach()
#
# transformerSWIG
#
add_test(
  NAME transformerSWIG 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/transformerSWIG
)
set_tests_properties(
  transformerSWIG 
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)

#
# analyticGeometrySWIG
#
add_test(
  NAME analyticGeometrySWIG 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/analyticGeometrySWIG
)
set_tests_properties(
  analyticGeometrySWIG 
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)

#
# constValueSWIG
#
add_test(
  NAME constValueSWIG 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/constValueSWIG
)
set_tests_properties(
  constValueSWIG 
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)

#
# canadaLight
#
add_test(
  NAME canadaLight 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/canadaLight
)
set_tests_properties(
  canadaLight 
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)

#
# tistos
#
add_test(
  NAME tistos 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tistos
)
set_tests_properties(
  tistos
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)
file(GLOB sources_list LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/test/tistos/*.xz")
foreach(xzFilePath ${sources_list})
  get_filename_component(xzFile "${xzFilePath}" NAME)
  message("**> Add test tistos-cmp-${xzFile}")
  add_test(
    NAME tistos-cmp-${xzFile} 
    COMMAND sh "cmp.sh" "${xzFile}" 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tistos
  )
endforeach()
#
# meshSimpleSurface
#
add_test(
  NAME meshSimpleSurface
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/meshSimpleSurface
)
set_tests_properties(
  meshSimpleSurface
  PROPERTIES
  ENVIRONMENT 
    "PYTHONPATH=$ENV{DTOO_EXTERNLIBS}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH};\
    LD_LIBRARY_PATH=$ENV{DTOO_EXTERNLIBS}/lib:$ENV{LD_LIBRARY_PATH}"
)
#
# dtClusteredSingletonState
#
add_test(
  NAME dtClusteredSingletonState
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dtClusteredSingletonState
)
set_tests_properties(
  dtClusteredSingletonState
  PROPERTIES
    ENVIRONMENT 
    "PYTHONPATH=${CMAKE_SOURCE_DIR}/scripts/python:$ENV{PYTHONPATH};"
)


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
