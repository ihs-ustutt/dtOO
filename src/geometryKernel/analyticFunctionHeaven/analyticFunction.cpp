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

#include "analyticFunction.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <attributionHeaven/vectorFunctionDist.h>
#include <gslMinFloatAttr.h>

namespace dtOO {
	analyticFunction::analyticFunction() : optionHandling(), labelHandling() {

	}

	analyticFunction::analyticFunction(
    const analyticFunction& orig
  ) : optionHandling(orig), labelHandling(orig) {

	}

	analyticFunction::~analyticFunction() {
	}

  analyticFunction * analyticFunction::weakClone( void ) const {
    return clone();
  }

  analyticFunction * analyticFunction::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
    dt__throwUnexpected(cloneTransformed());
  }
      
  void analyticFunction::dump(void) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);		
	}
	
	std::string analyticFunction::dumpToString(void) const {
		return std::string("");
	}	
  
  aFX analyticFunction::xMin( void ) const {
    aFX ret(xDim(), 0.);
    
    dt__forFromToIndex(0, xDim(), ii) ret[ii] = xMin(ii);
    return ret;        
  }
  
  aFX analyticFunction::xMax( void ) const {
    aFX ret(xDim(), 0.);
    
    dt__forFromToIndex(0, xDim(), ii) ret[ii] = xMax(ii);
    return ret;            
  }
  
	aFX analyticFunction::x_percent( aFX const & xx ) const {
    aFX ret(xDim(), 0.);
    
    for (int ii=0; ii<xDim(); ii++) {
      ret[ii] = xMin(ii) +  (xMax(ii) - xMin(ii)) * xx[ii];    
    }
    return ret;    
  }
  
  aFX analyticFunction::percent_x( aFX const & xx ) const {
    aFX ret(xDim(), 0.);
    
    for (int ii=0; ii<xDim(); ii++) {
      ret[ii] = (xx[ii] - xMin(ii)) / (xMax(ii) - xMin(ii));    
    }
    return ret;
  }
    
  aFX analyticFunction::invY(aFY const & yy) const {
    gslMinFloatAttr md(
      dt__pH(vectorFunctionDist)( new vectorFunctionDist(yy, this) ),
      ::std::vector(this->xDim(), 0.5),
      ::std::vector(this->xDim(), 0.001),
      staticPropertiesHandler::getInstance()->getOptionFloat("invY_precision")
    );
    md.perform();
    dt__throwIf(!md.converged(), invY());
    return x_percent(md.result());
  }

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction * > const & analyticFunction::vecRef( 
    void 
  ) const {
		dt__throwUnexpected(vecRef);
	}			

	std::map< int, dtInt > const & analyticFunction::mapRef(
    void 
  ) const {
		dt__throwUnexpected(mapRef);
	}			
  
	bool analyticFunction::isTransformed( void ) const {
		return false;
	}
  
  std::pair< aFY, aFY > analyticFunction::yBoundingBox( void ) const {
    return std::pair< aFY, aFY >(
      aFY(yDim(), std::numeric_limits<dtReal>::min()),
      aFY(yDim(), std::numeric_limits<dtReal>::max())
    );
  } 
	
  aFX analyticFunction::aFXZeroD( void ) {
		return aFX(0, 0.);
	}
  
  aFX analyticFunction::aFXOneD( dtReal const & x0 ) {
		aFX ret = aFX(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFX analyticFunction::aFXTwoD( dtReal const & x0, dtReal const & x1 ) {
		aFX ret = aFX(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	} 
	
  aFX analyticFunction::aFXThreeD( 
    dtReal const & x0, dtReal const & x1, dtReal const & x2 
  ) {
		aFX ret = aFX(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}
  
  aFX analyticFunction::aFXTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFXTwoD(pp.x(), pp.y());
	} 
  
  aFX analyticFunction::aFXThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFXThreeD(pp.x(), pp.y(), pp.z());
	}  
  
  aFY analyticFunction::aFYZeroD( void ) {
		return aFY(0, 0.);
	}
  
  aFY analyticFunction::aFYOneD( dtReal const & x0 ) {
		aFY ret = aFY(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFY analyticFunction::aFYTwoD( dtReal const & x0, dtReal const & x1 ) {
		aFY ret = aFY(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	}
	
  aFY analyticFunction::aFYThreeD( 
    dtReal const & x0, dtReal const & x1, dtReal const & x2 
  ) {
		aFY ret = aFY(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}  
  
  aFY analyticFunction::aFYTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFYTwoD(pp.x(), pp.y());
	} 
  
  aFY analyticFunction::aFYThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFYThreeD(pp.x(), pp.y(), pp.z());
	}    

  aFY analyticFunction::aFY_aFX( aFX const & xx ) {
    return aFY(xx);
  }
  
  aFX analyticFunction::aFX_aFY( aFY const & yy ) {
    return aFX(yy);
  }
  
  dtReal analyticFunction::distance( aFX const & x0, aFX const & x1 ) {
    dt__throwIf(x0.size()!=x1.size(), length());
    
    dtReal dist = 0.;
    dt__forAllIndex(x0, ii) dist = dist + (x1[ii] - x0[ii])*(x1[ii] - x0[ii]);
    
    return sqrt(dist);
  }
  
  std::vector< dtReal > const analyticFunction::operator()(
    dtReal const & xx
  ) const {
    dt__throwIf(xDim()!=1, operator());  
    return this->Y( aFXOneD(xx) ).stdVector();
  }
  
  std::vector< dtReal > const analyticFunction::operator()(
    dtReal const & xx, dtReal const & yy
  ) const {
    dt__throwIf(xDim()!=2, operator());
    return this->Y( aFXTwoD(xx, yy) ).stdVector();
  }

  std::vector< dtReal > const analyticFunction::operator()(
    dtReal const & xx, dtReal const & yy, dtReal const & zz
  ) const {
    dt__throwIf(xDim()!=3, operator());
    return this->Y( aFXThreeD(xx, yy, zz) ).stdVector();
  }  
    
  dt__C_addCloneForpVH(analyticFunction);  
}
