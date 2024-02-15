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

SET(HEADERS
  dtArg.h
)
SET(SOURCES
  dtArg.cpp
)

APPEND_DTOO_SOURCE(src/applicationKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/applicationKernel "${HEADERS}")
