#
# base
#
add_test(
  NAME base 
  COMMAND python3 "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/base
)
set_tests_properties(
  base 
  PROPERTIES
  ENVIRONMENT PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH}
)

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
  ENVIRONMENT PYTHONPATH=$ENV{DTOO_EXTERNLIBS=}/tools:${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH}
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
  ENVIRONMENT PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH}
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