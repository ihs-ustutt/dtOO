#
#
# sources
#
#
set( SOURCES adjustOutletAngle.h adjustOutletAngle.cpp )

#
#
# library
#
#
add_library(adjustOutletAngle SHARED ${SOURCES})

#
#
# linking
#
#
target_link_libraries( adjustOutletAngle dtOO )

#
#
# install
#
#
install(TARGETS adjustOutletAngle DESTINATION tools)