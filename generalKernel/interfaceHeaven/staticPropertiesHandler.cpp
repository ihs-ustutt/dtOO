#include "staticPropertiesHandler.h"

#include <logMe/logMe.h>

namespace dtOO {
  staticPropertiesHandler * staticPropertiesHandler::_pH = NULL;
  
  staticPropertiesHandler::staticPropertiesHandler() : optionHandling() {
    setOption("reparamOnFace_precision", "1.e-2");
    setOption("reparamInVolume_precision", "1.e-2");
    setOption("reparamOnFace_precisionXYZ", "1.e-1");
    setOption("reparamInVolume_precisionXYZ", "1.e-1");		
    setOption("xyz_resolution", "1.e-2");
		setOption("uv_resolution", "1.e-2");
    setOption("point_render_diameter", "1.");
    setOption("vector_render_size", "1.");
    setOption("snapTol", "1.e-2");
    setOption("function_render_resolution_u", "10");
		setOption("function_render_resolution_v", "10");
		setOption("function_render_resolution_w", "10");
    setOption("geometry_render_resolution_u", "10");
		setOption("geometry_render_resolution_v", "10");
		setOption("geometry_render_resolution_w", "10");		
  }

  staticPropertiesHandler * staticPropertiesHandler::getInstance( void ) {
    if (!_pH) {
      _pH = new staticPropertiesHandler();
    }
    return _pH;
  }
  staticPropertiesHandler::~staticPropertiesHandler() {

  }
}
