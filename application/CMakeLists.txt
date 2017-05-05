#------------------------------------------------------------------------------#
#
# add directories
#
#------------------------------------------------------------------------------#
if (DTOO_USE_MPI)
  add_subdirectory(runCase)
endif (DTOO_USE_MPI)
add_subdirectory(built4CubanLinx)
add_subdirectory(createState)
add_subdirectory(listLabel)
add_subdirectory(listState)
add_subdirectory(stateToCSV)
add_subdirectory(dtOOToSAFE)
