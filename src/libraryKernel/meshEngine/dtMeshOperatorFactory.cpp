#include "dtMeshOperatorFactory.h"

#include <logMe/logMe.h>
#include "dtMeshOperator.h"
#include "dtMeshGEdge.h"
#include "dtMeshGFace.h"
#include "dtMeshGRegion.h"
#include "dtMeshGRegionWithBoundaryLayer.h"
#include "dtMeshGRegionWithOneLayer.h"
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
#include "dtMeshGVertex.h"

namespace dtOO {
  dt__pVH(dtMeshOperator) dtMeshOperatorFactory::_product;
  
  dtMeshOperatorFactory::dtMeshOperatorFactory() {
  }

  dtMeshOperatorFactory::~dtMeshOperatorFactory() {

  }

  bool dtMeshOperatorFactory::registrate( dtMeshOperator const * const reg ) {
    dt__forAllRefAuto( _product, aProd ) {
      if ( aProd.virtualClassName() == reg->virtualClassName() ) {
        return false;
      }
    }  
    _product.push_back( reg->create() );
    
    return true;
  }
  
  dtMeshOperator * dtMeshOperatorFactory::create(char const * const str) {
    return create( std::string(str) );
  }

  dtMeshOperator * dtMeshOperatorFactory::create( std::string const str ) {
    dt__forAllRefAuto( _product, aProd ) {
      //
      // check virtual class name
      //
      if ( aProd.virtualClassName() == str ) {
        return aProd.create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aProd.factoryAlias(), anAlias) {
        if ( anAlias == str ) return aProd.create();
      }
    }

    std::vector< std::string > av;
    dt__forAllRefAuto( _product, aProd ) {
      av.push_back( aProd.virtualClassName() );
      dt__forAllRefAuto(aProd.factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias); 
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented builder:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );           
  }
}
