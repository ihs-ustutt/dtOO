#include "dtMeshOperatorFactory.h"

#include <logMe/logMe.h>
#include "dtMeshOperator.h"
#include "dtMeshGEdge.h"
#include "dtMeshGFace.h"
#include "dtMeshGRegion.h"
#include "dtMeshGRegionWithBoundaryLayer.h"
#include "dtMeshAndOrientGEdge.h"
#include "dtMeshAndOrientGFace.h"
#include "dtMeshTransfiniteRecombineSelectionGFace.h"
#include "dtMeshGRegionTetgen.h"
#include "dtMeshFixedBoundaryElementGEdge.h"
#include "dtMeshTransfiniteGFace.h"

namespace dtOO {
  dtMeshOperatorFactory::dtMeshOperatorFactory() {
  }

  dtMeshOperatorFactory::~dtMeshOperatorFactory() {
  }

  dtMeshOperator * dtMeshOperatorFactory::create(std::string const str) {
    dt__info(create(), << "creating " << str <<  "...");
    
		if (str == "dtMeshGEdge") return new dtMeshGEdge();
    if (str == "dtMeshGFace") return new dtMeshGFace();
    if (str == "dtMeshGRegion") return new dtMeshGRegion();
    if (str == "dtMeshGRegionWithBoundaryLayer") {
      return new dtMeshGRegionWithBoundaryLayer();
    }
    if (str == "dtMeshAndOrientGEdge") return new dtMeshAndOrientGEdge();
    if (str == "dtMeshAndOrientGFace") return new dtMeshAndOrientGFace();
    if (str == "dtMeshTransfiniteRecombineSelectionGFace") {
      return new dtMeshTransfiniteRecombineSelectionGFace();
    }
    if ( str == "dtMeshGRegionTetgen" ) return new dtMeshGRegionTetgen();
    if ( str == "dtMeshFixedBoundaryElementGEdge" ) {
      return new dtMeshFixedBoundaryElementGEdge();
    }
    if ( str == "dtMeshTransfiniteGFace" ) return new dtMeshTransfiniteGFace();
    
    dt__throw(create(), << str <<  " could not be created");  
  }
}