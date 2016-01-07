#
#
# find packages
#
#
find_package(
  Boost 
  COMPONENTS 
  program_options
  REQUIRED
)

#
#
# executable
#
#
add_executable(dtOOToSAFE dtOOToSAFE.cpp)
add_executable(resultValueParseArg resultValueParseArg.cpp)

#
#
# linking
#
#
target_link_libraries(
  dtOOToSAFE 
  dtOO
  ${Boost_LIBRARIES}
)
target_link_libraries(
  resultValueParseArg 
  dtOO
  ${Boost_LIBRARIES}
)

#
#
# add install rpath of dtOO lib
#
#
set(
  CMAKE_EXE_LINKER_FLAGS 
  "-Wl,--no-undefined"
)
set(
  CMAKE_EXE_LINKER_FLAGS 
  "${CMAKE_EXE_LINKER_FLAGS},-rpath,${CMAKE_INSTALL_PREFIX}/lib"
)

install(TARGETS dtOOToSAFE DESTINATION bin)
install(TARGETS resultValueParseArg DESTINATION bin)