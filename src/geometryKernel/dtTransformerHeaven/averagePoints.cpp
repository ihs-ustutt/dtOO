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

#include "averagePoints.h"
#include <analyticFunctionHeaven/scaFunction.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool averagePoints::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(averagePoints, new averagePoints())
  );

  averagePoints::averagePoints() {
  }

  averagePoints::~averagePoints() {
  }

	averagePoints::averagePoints( averagePoints const & orig ) {
		
	}
	
	dtTransformer * averagePoints::clone( void ) const {
		return new averagePoints(*this);
	}
	
  dtTransformer * averagePoints::create( void ) const {
		return new averagePoints();
	}
	
  std::vector< dtPoint2 * > 
	averagePoints::apply( std::vector< dtPoint2 * > const * const toTrans ) const {
    std::vector< dtPoint2 * > transP2;
    if ((toTrans->size()%2) == 1) {
			dt__throw(apply(),
							<< "Point vector has not correct size." << std::endl
							<< dt__eval(toTrans->size()) );
    }
		for (int ii=0; ii<=(((toTrans->size())/2)-1);(ii=ii+1)) {		
			dtReal xi = toTrans->at(ii)->x();
			dtReal yi = toTrans->at(ii)->y();

			dtReal xj = toTrans->at((toTrans->size()) - ii-1)->x();
			dtReal yj = toTrans->at((toTrans->size()) - ii-1)->y();

			dtReal xx = (xi+xj)/2;
			dtReal yy = (yi+yj)/2;

			transP2.push_back( new dtPoint2(xx, yy) );
		}
    
		return transP2;
  }

  bool averagePoints::isNecessary( void ) const {
    return true;
  }

	void averagePoints::init( 
		::QDomElement const * tE, 
		baseContainer * const bC,
		lvH_constValue const * const cValP,
		lvH_analyticFunction const * const sFunP,
		lvH_analyticGeometry const * const depAGeoP 
	) {
		
	}	
}
