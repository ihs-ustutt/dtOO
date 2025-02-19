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

#include "analyticGeometry.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <geoAlgoHeaven/dtPoint3Algo.h>

namespace dtOO {
  analyticGeometry::analyticGeometry() : labelHandling() {
		_resU 
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_u"
    );		
		_resV
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_v"
    );		
		_resW
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "geometry_render_resolution_w"
    );
    _characteristicLength = -1.;
    _boundingBoxValue = -1.;
  }

  analyticGeometry::analyticGeometry( 
    analyticGeometry const & orig 
  ) : labelHandling(orig) {
    _resU = orig._resU;
    _resV = orig._resV;
		_resW = orig._resW;
    _characteristicLength = orig._characteristicLength;
    _boundingBox = orig._boundingBox;
    _boundingBoxValue = orig._boundingBoxValue;
  }

  analyticGeometry::~analyticGeometry() {
  }

  dtPoint3 analyticGeometry::getPoint( dtReal const * const uvw ) const {
    dt__throwUnexpected(getPoint());
  }

  dtPoint3 analyticGeometry::getPointPercent( dtReal const * const uvw ) const {
    dtReal uvwP[dim()];
    dt__forFromToIndex(0, dim(), ii) {
      uvwP[ii] = getMin(ii) + uvw[ii] * (getMax(ii)-getMin(ii));
    }
    
    return getPoint(uvwP);
  }
  
	void analyticGeometry::setRenderResolution(
    dtInt const & dir, dtInt const & value
  ) const {
    switch (dir) {
      case 0:
				_resU = value;
        return;
      case 1:
				_resV = value;
        return;
      case 2:
				_resW = value;
        return;
      default:
        dt__throw(
          setRenderResolution(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }		
	}
	
	
	int analyticGeometry::getRenderResolution(int const & dir) const {
    switch (dir) {
      case 0:
				return _resU;
      case 1:
				return _resV;
      case 2:
				return _resW;
      default:
        dt__throw(
          getRenderResolution(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }		
	}

  void analyticGeometry::dump( void ) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< dumpToString()
		);
  }
	
	std::string analyticGeometry::dumpToString(void) const {
		std::stringstream ss;
		
		ss << dt__dumpToString( << "" );
		
		return ss.str();
	}

  bool analyticGeometry::isTransformed( void ) const {
		return false;
	}
    
	bool analyticGeometry::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticGeometry const * > 
  analyticGeometry::compoundInternal( void ) const {
		return vectorHandling< analyticGeometry const * >();
	}		
	
  std::pair< dtPoint3, dtPoint3 > analyticGeometry::boundingBox( 
    void 
  ) const {
    if (_characteristicLength < 0.) updateBoundingBox();
    
    return _boundingBox;
  }
  
  dtReal analyticGeometry::boundingBoxValue( void ) const {
    if (_characteristicLength < 0.) updateBoundingBox();
    
    return _boundingBoxValue;
  }

  void analyticGeometry::updateBoundingBox( void ) const {
    //
    // 1D
    //
    if (dim() == 1) {
      dtInt nOne = 3;
      dtReal uvwOne[3] = {0.00, 0.50, 1.00};
    
      std::vector< dtPoint3 > bb(nOne);
      dt__forFromToIndex(0, nOne, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwOne[ii]) );
      }
      _boundingBox = dtLinearAlgebra::boundingBox(bb);
      _boundingBoxValue 
      = 
      dtLinearAlgebra::distance(bb[1], bb[0]) 
      + 
      dtLinearAlgebra::distance(bb[2], bb[1]);
    }
    //
    // 2D
    //
    else if (dim() == 2) {
      dtInt nTwo = 9;
      dtReal uvwTwo[9][2] 
      = 
      {
        {0.00, 0.00}, {0.00, 0.50}, {0.00, 1.00},
        {0.50, 0.00}, {0.50, 0.50}, {0.50, 1.00},
        {1.00, 0.00}, {1.00, 0.50}, {1.00, 1.00}
      };
      
      std::vector< dtPoint3 > bb(nTwo);
      dt__forFromToIndex(0, nTwo, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwTwo[ii]) );
      }
      _boundingBox = dtLinearAlgebra::boundingBox(bb);
      
      _boundingBoxValue 
      = 
      dtLinearAlgebra::area(bb[0], bb[3], bb[4] , bb[1])
      +
      dtLinearAlgebra::area(bb[3], bb[6], bb[7] , bb[4])
      +
      dtLinearAlgebra::area(bb[4], bb[7], bb[8] , bb[5])
      +
      dtLinearAlgebra::area(bb[1], bb[4], bb[5] , bb[2]);
    }
    //
    // 3D
    //
    else if (dim() == 3) {
      dtInt nThree = 27;
      dtReal uvwThree[27][3] 
      = 
      {
        {0.00, 0.00, 0.00}, {0.00, 0.00, 0.50}, {0.00, 0.00, 1.00},
        {0.00, 0.50, 0.00}, {0.00, 0.50, 0.50}, {0.00, 0.50, 1.00},
        {0.00, 1.00, 0.00}, {0.00, 1.00, 0.50}, {0.00, 1.00, 1.00},
        {0.50, 0.00, 0.00}, {0.50, 0.00, 0.50}, {0.50, 0.00, 1.00},
        {0.50, 0.50, 0.00}, {0.50, 0.50, 0.50}, {0.50, 0.50, 1.00},
        {0.50, 1.00, 0.00}, {0.50, 1.00, 0.50}, {0.50, 1.00, 1.00},      
        {1.00, 0.00, 0.00}, {1.00, 0.00, 0.50}, {1.00, 0.00, 1.00},
        {1.00, 0.50, 0.00}, {1.00, 0.50, 0.50}, {1.00, 0.50, 1.00},
        {1.00, 1.00, 0.00}, {1.00, 1.00, 0.50}, {1.00, 1.00, 1.00}
      };

      std::vector< dtPoint3 > bb(nThree);
      dt__forFromToIndex(0, nThree, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwThree[ii]) );
      } 
      _boundingBox = dtLinearAlgebra::boundingBox(bb);

      _boundingBoxValue 
      = 
      dtLinearAlgebra::volume(
        bb[0], bb[9], bb[3] , bb[12], bb[1], bb[10], bb[4] , bb[13]
      )
      +
      dtLinearAlgebra::volume(
        bb[1], bb[10], bb[4] , bb[13], bb[2], bb[11], bb[5] , bb[14]
      )
      +
      dtLinearAlgebra::volume(
        bb[3], bb[12], bb[6] , bb[15], bb[4], bb[13], bb[7] , bb[16]
      )      
      +
      dtLinearAlgebra::volume(
        bb[4], bb[13], bb[7] , bb[16], bb[5], bb[14], bb[8] , bb[17]
      )      
      +
      dtLinearAlgebra::volume(
        bb[9], bb[18], bb[12] , bb[21], bb[10], bb[19], bb[13] , bb[22]
      )      
      +
      dtLinearAlgebra::volume(
        bb[10], bb[19], bb[13] , bb[22], bb[11], bb[20], bb[14] , bb[23]
      )      
      +
      dtLinearAlgebra::volume(
        bb[12], bb[21], bb[15] , bb[24], bb[13], bb[22], bb[16] , bb[25]
      )      
      +
      dtLinearAlgebra::volume(
        bb[13], bb[22], bb[16] , bb[25], bb[14], bb[23], bb[17] , bb[26]
      );        
    }
    else dt__throwUnexpected(updateBoundingBox());
    
    _characteristicLength 
    = 
    dtLinearAlgebra::length(_boundingBox.first - _boundingBox.second);
  }
  
  std::vector< dtPoint3 > analyticGeometry::cornerPoints( void ) const {
    //
    // 1D
    //
    if (dim() == 1) {
      dtInt nOne = 2;
      dtReal uvwOne[2] = {0.00, 1.00};
    
      std::vector< dtPoint3 > bb(nOne);
      dt__forFromToIndex(0, nOne, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwOne[ii]) );
      }
      
      return bb;
    }
    //
    // 2D
    //
    else if (dim() == 2) {
      dtInt nTwo = 4;
      dtReal uvwTwo[4][2] 
      = 
      { {0.00, 0.00}, {0.00, 1.00}, {1.00, 0.00}, {1.00, 1.00} };
      
      std::vector< dtPoint3 > bb(nTwo);
      dt__forFromToIndex(0, nTwo, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwTwo[ii]) );
      }
      
      return bb;
    }
    //
    // 3D
    //
    else if (dim() == 3) {
      dtInt nThree = 8;
      dtReal uvwThree[8][3] 
      = 
      {
        {0.00, 0.00, 0.00}, {0.00, 0.00, 1.00},
        {0.00, 1.00, 0.00}, {0.00, 1.00, 1.00},
        {1.00, 0.00, 0.00}, {1.00, 0.00, 1.00},
        {1.00, 1.00, 0.00}, {1.00, 1.00, 1.00}
      };

      std::vector< dtPoint3 > bb(nThree);
      dt__forFromToIndex(0, nThree, ii) {
        bb[ii] = getPointPercent( (dtReal*) &(uvwThree[ii]) );
      }

      return bb;      
    }
    else dt__throwUnexpected(updateBoundingBox());    
  }  
  
  dtReal analyticGeometry::characteristicLength( void ) const {
    if (_characteristicLength < 0.) updateBoundingBox();
    
    return _characteristicLength;
  }
  
  dtPoint3 analyticGeometry::getPoint( 
    std::vector< dtReal > const & uvw 
  ) const {
    return getPoint( &(uvw[0]) );
  }
  
  bool analyticGeometry::inXYZTolerance(
    dtPoint3 const & p0, dtPoint3 const & p1, 
    dtReal * const dist, bool output, dtReal inc 
  ) {
		dtReal xyzResolution 
		= 
		inc 
    *
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );    		
    
		dtVector3 distV = p0 - p1;
    *dist = dtLinearAlgebra::length(distV);
		if (*dist > xyzResolution) {
      if (output) {
        dt__warning(
          inXYZTolerance(), 
          << dt__point3d(p0) << std::endl
          << dt__point3d(p1) << std::endl
          << dt__eval( *dist )
        );
      }
      return false;
    }

    return true;    
  }

  bool analyticGeometry::inXYZTolerance(
    dtPoint3 const & p0, dtPoint3 const & p1
  ) {
    dtReal dist;
    return analyticGeometry::inXYZTolerance(p0, p1, &dist, false, 1.0);    
  }
  
  bool analyticGeometry::inXYZTolerance( 
    dtReal const dist, dtReal inc 
  ) {
		dtReal xyzResolution 
		= 
		inc 
    *    
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );    		
    
		if ( dist > xyzResolution ) return false;

    return true;    
  }  

  bool analyticGeometry::degenerated( void ) const {
    return inXYZTolerance( boundingBoxValue() );
  }
  
  bool analyticGeometry::inUVWTolerance(
    dtPoint3 const & p0, dtPoint3 const & p1
  ) {
		dtReal uvwResolution 
		=
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "uvw_resolution"
    );
    
		dtVector3 dist = p0 - p1;
		if (sqrt(dist.squared_length()) > uvwResolution) return false;

    return true;    
  }
  
  bool analyticGeometry::inUVWTolerance(
    dtPoint2 const & p0, dtPoint2 const & p1
  ) {
		dtReal uvwResolution 
		=
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "uvw_resolution"
    );
    
		dtVector2 dist = p0 - p1;
		if (sqrt(dist.squared_length()) > uvwResolution) return false;

    return true;    
  }

  bool analyticGeometry::inUVWTolerance(
    dtReal const & p0, dtReal const & p1
  ) {
		dtReal uvwResolution 
		=
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "uvw_resolution"
    );
    
		dtReal dist = p0 - p1;
		if ( fabs(dist) > uvwResolution ) return false;

    return true;    
  }  
  
  bool analyticGeometry::equal(const analyticGeometry & other) const {
    dt__throwIf( this->dim()!=other.dim(), equal() );
    
    std::vector< dtPoint3 > thisCP = this->cornerPoints();
    std::vector< dtPoint3 > otherCP = other.cornerPoints();
    
    std::vector< dtInt > matchCP;
    dt__forAllRefAuto(thisCP, aCP) {
      matchCP.push_back(
        dtLinearAlgebra::returnNearestPointIndexTo(aCP, otherCP)
      );
      
      if ( !inXYZTolerance(aCP, otherCP[matchCP.back()] ) ) return false;
    }
    
    progHelper::removeBastardTwins( matchCP );
    if ( matchCP.size() != thisCP.size() )  return false;
    
    return true;
  }
  
  bool analyticGeometry::equal( analyticGeometry const * const other) const {    
    return this->equal(*other);
  }

  void analyticGeometry::visitMe( dtPoint3Algo * d3algo ) const {
    d3algo->visit(this);
  }
  
  dt__C_addCloneForpVH(analyticGeometry);
}
