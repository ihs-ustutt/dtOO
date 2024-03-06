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

message("**> Compile lib adjustOutletAngle")
