#include "dtPoint3_vec3dTwoD.h"

#include <logMe/dtMacros.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>

namespace dtOO {
  dtPoint3_vec3dTwoD::dtPoint3_vec3dTwoD(
      vec3dTwoD const * const v3d, int const & nU, int const & nV
  ) {
		_pp.resize(nU, nV);

    twoDArrayHandling< dtPoint2 > uGrid = dtLinearAlgebra::unitGrid(nU, nV);
		dt__forAllIndex(uGrid, ii) {
      dt__forAllIndex(uGrid[ii], jj) {
			  _pp[ii][jj] 
        = 
        v3d->YdtPoint3Percent( analyticFunction::aFXTwoD(uGrid[ii][jj]) );
      }
		}				
	}
  
	dtPoint3_vec3dTwoD::~dtPoint3_vec3dTwoD() {
		
	}
	
	twoDArrayHandling< dtPoint3 > dtPoint3_vec3dTwoD::result( void ) {
		return _pp;
	}
}

