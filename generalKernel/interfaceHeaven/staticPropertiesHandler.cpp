#include "staticPropertiesHandler.h"
#include "systemHandling.h"

#include <logMe/logMe.h>
#include <omp.h>

namespace dtOO {
  staticPropertiesHandler * staticPropertiesHandler::_pH = NULL;
  
  staticPropertiesHandler::staticPropertiesHandler() : optionHandling() {
    setOption("reparamOnFace_precision", "1.e-2");
    setOption("reparamInVolume_precision", "1.e-2");
    setOption("reparamOnFace_precisionXYZ", "1.e-1");
    setOption("reparamInVolume_precisionXYZ", "1.e-1");		
    setOption("invY_precision", "1.e-2");
    setOption("xyz_resolution", "1.e-2");
		setOption("uvw_resolution", "1.e-2");
    setOption("point_render_diameter", "1.");
    setOption("vector_render_size", "1.");
    setOption("function_render_resolution_u", "10");
		setOption("function_render_resolution_v", "10");
		setOption("function_render_resolution_w", "10");
    setOption("geometry_render_resolution_u", "10");
		setOption("geometry_render_resolution_v", "10");
		setOption("geometry_render_resolution_w", "10");		
		setOption("root_printLevel", "0");
    setOption("root_maxIterations", "1000");
    setOption("root_maxFunctionCalls", "10000");
    setOption("workingDirectory", systemHandling::currentDirectory());
    setOption("reparamOnFace_minimizer", ":Minuit2:kMigrad:");
    setOption("reparamInVolume_minimizer", ":Minuit2:kMigrad:");
    setOption("ompNumThreads", "1");
  }

  staticPropertiesHandler * staticPropertiesHandler::getInstance( void ) {
    if (!_pH) _pH = new staticPropertiesHandler();
    return _pH;
  }
  staticPropertiesHandler::~staticPropertiesHandler() {

  }
  
  void staticPropertiesHandler::init(::QDomElement const * const wElement) {
    optionHandling::init(wElement);
    
    omp_set_num_threads( getOptionInt("ompNumThreads") );
    dt__info(
      init(),
      << dt__eval(omp_get_num_threads()) << std::endl
      << dt__eval(omp_get_thread_limit()) << std::endl
      << dt__eval(omp_get_max_threads())
      )
  }
}
