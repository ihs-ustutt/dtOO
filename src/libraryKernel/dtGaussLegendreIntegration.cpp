#include "dtGaussLegendreIntegration.h"

#define dt__defineGLIP( number ) \
	 constexpr float dtGaussLegendreIntegration::_GL_pt##number[]; \
	 constexpr float dtGaussLegendreIntegration::_GL_wt##number[]\

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
