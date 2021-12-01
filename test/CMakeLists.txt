#
# base
#
add_test(
  NAME base 
  COMMAND ipython "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/base
)
set_tests_properties(
  base 
  PROPERTIES
  ENVIRONMENT PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH}
)
add_custom_target(
  cp-base-test ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/test/base
    ${CMAKE_CURRENT_BINARY_DIR}/base
)

#
# naca
#
add_test(
  NAME naca 
  COMMAND ipython "build.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/naca
)
set_tests_properties(
  naca
  PROPERTIES
  ENVIRONMENT PYTHONPATH=${CMAKE_INSTALL_PREFIX}/tools:$ENV{PYTHONPATH}
)
add_custom_target(
  cp-naca-test ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/test/naca
    ${CMAKE_CURRENT_BINARY_DIR}/naca
)