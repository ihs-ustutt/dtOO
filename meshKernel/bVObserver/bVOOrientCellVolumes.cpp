#include "bVOOrientCellVolumes.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <gmsh/MElement.h>

namespace dtOO {  
  bVOOrientCellVolumes::bVOOrientCellVolumes() {
  }

  bVOOrientCellVolumes::~bVOOrientCellVolumes() {
    
  }
  
  void bVOOrientCellVolumes::bVOOrientCellVolumes::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		boundedVolume * attachTo
  ) {		
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver name="bVOOrientCellVolumes" positive="true"/>									
		_positive
		= 
		qtXmlBase::getAttributeBool("positive", element);		
  }
  
  void bVOOrientCellVolumes::postUpdate( void ) {
    //
    // model check
    //
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		
    //
    // counter
    //
    int gCount = 0;
    int pCount = 0;
    int nCount = 0;
    
    //
    // revert element if necessary
    //
    std::list< GRegion * > reg = gm->regions();
    dt__forAllConstIter(std::list< GRegion * >, reg, it) {
      if (_positive) {
        dt__forFromToIndex( 0, (*it)->getNumMeshElements(), ii) {
          gCount++;
          if ( (*it)->getMeshElement(ii)->getVolume() < 0.) {
            nCount++;
            (*it)->getMeshElement(ii)->reverse();
          }
          else {
            pCount++;
          }
        }
      }
      else {
        dt__forFromToIndex( 0, (*it)->getNumMeshElements(), ii) {
          gCount++;
          if ( (*it)->getMeshElement(ii)->getVolume() > 0.) {
            pCount++;
            (*it)->getMeshElement(ii)->reverse();
          }
          else {
            nCount++;
          }
        }
      }
    }
    dt__info(
      postUpdate(), 
      << " _positive = " << _positive << std::endl
      << "Number of cells: " << gCount << std::endl
      << "Positive number of cells: " << pCount << std::endl
      << "Negative number of cells: " << nCount
    );
  }
}
