/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtGaussLegendreIntegration.h"

#define dt__defineGLIP( number ) \
	 constexpr dtReal dtGaussLegendreIntegration::_GL_pt##number[]; \
	 constexpr dtReal dtGaussLegendreIntegration::_GL_wt##number[]\

namespace dtOO {
	 dt__defineGLIP(1);
	 dt__defineGLIP(2);
	 dt__defineGLIP(3);
	 dt__defineGLIP(4);
	 dt__defineGLIP(5);
	 dt__defineGLIP(6);
	 dt__defineGLIP(7);
	 dt__defineGLIP(8);
	 dt__defineGLIP(9);
	 dt__defineGLIP(10);
	 dt__defineGLIP(11);
	 dt__defineGLIP(12);
	 dt__defineGLIP(13);
	 dt__defineGLIP(14);
	 dt__defineGLIP(15);
	 dt__defineGLIP(16); 
	 dt__defineGLIP(20);
}
