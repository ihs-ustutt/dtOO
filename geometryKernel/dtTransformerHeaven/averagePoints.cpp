#include "averagePoints.h"
#include <analyticFunctionHeaven/scaFunction.h>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {

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
			float xi = toTrans->at(ii)->x();
			float yi = toTrans->at(ii)->y();

			float xj = toTrans->at((toTrans->size()) - ii-1)->x();
			float yj = toTrans->at((toTrans->size()) - ii-1)->y();

			float xx = (xi+xj)/2;
			float yy = (yi+yj)/2;

			transP2.push_back( new dtPoint2(xx, yy) );
		}
    
		return transP2;
  }

  bool averagePoints::isNecessary( void ) const {
    return true;
  }

	void averagePoints::init( 
		QDomElement const * tE, 
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
		
	}	
}

