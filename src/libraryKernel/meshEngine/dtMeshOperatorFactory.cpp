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
#include "dtMeshFreeGradingGEdge.h"
#include "dtMeshTransfiniteGFace.h"
#include "dtMeshCustom2x3TransfiniteGFace.h"
#include "dtOptimizeMeshGRegion.h"
#include "dtMeshGFaceWithTransfiniteLayer.h"
#include "dtMeshLaplacianGFace.h"

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
    if ( str == "dtMeshFreeGradingGEdge" ) {
      return new dtMeshFreeGradingGEdge();
    }
    if ( str == "dtMeshTransfiniteGFace" ) return new dtMeshTransfiniteGFace();
    if ( str == "dtMeshCustom2x3TransfiniteGFace" ) {
      return new dtMeshCustom2x3TransfiniteGFace();
    }
    if (str == "dtOptimizeMeshGRegion") return new dtOptimizeMeshGRegion();
    if (str == "dtMeshGFaceWithTransfiniteLayer") {
      return new dtMeshGFaceWithTransfiniteLayer();
    }
    if (str == "dtMeshLaplacianGFace") return new dtMeshLaplacianGFace();
    
    dt__throw(create(), << str <<  " could not be created");  
  }
}