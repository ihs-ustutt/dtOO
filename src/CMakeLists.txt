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

add_subdirectory(geometryKernel)
add_subdirectory(generalKernel)
add_subdirectory(libraryKernel)
add_subdirectory(meshKernel)
add_subdirectory(renderKernel)
add_subdirectory(pluginKernel)
add_subdirectory(solverKernel)
add_subdirectory(applicationKernel)

set(DTOO_SOURCES ${DTOO_SOURCES} PARENT_SCOPE)
set(DTOO_HEADERS ${DTOO_HEADERS} PARENT_SCOPE)
