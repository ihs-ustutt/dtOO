#
#
# sources
#
#
set( SOURCES surfaceComparison.h surfaceComparison.cpp )

#
#
# library
#
#
add_library(surfaceComparison SHARED ${SOURCES})

#
#
# linking
#
#
target_link_libraries( surfaceComparison dtOO )

#
#
# install
#
#
install(TARGETS surfaceComparison DESTINATION tools)

message("**> Compile lib surfaceComparison")