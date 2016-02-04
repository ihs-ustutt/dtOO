#include "analyticGeometry.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

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
  }

  analyticGeometry::analyticGeometry( 
    analyticGeometry const & orig 
  ) : labelHandling(orig) {
    _resU = orig._resU;
    _resV = orig._resV;
		_resW = orig._resW;
    _characteristicLength = orig._characteristicLength;
    _boundingBox = orig._boundingBox;
  }

  analyticGeometry::~analyticGeometry() {
  }

  dtPoint3 analyticGeometry::getPointPercent( float const * const uvw ) const {
    float uvwP[dim()];
    dt__forFromToIndex(0, dim(), ii) {
      uvwP[ii] = getMin(ii) + uvw[ii] * (getMax(ii)-getMin(ii));
    }
    
    return getPoint(uvwP);
  }
  
	void analyticGeometry::setRenderResolution(
    int const & dir, int const & value
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

	vectorHandling< analyticGeometry const * > analyticGeometry::compoundInternal(
    void 
  ) const {
		return vectorHandling< analyticGeometry const * >();
	}		
	
  std::pair< dtPoint3, dtPoint3 > analyticGeometry::boundingBox( 
    void 
  ) const {
    if (_characteristicLength < 0.) updateBoundingBox();
    
    return _boundingBox;
  }
  
  void analyticGeometry::boundingBox( 
    std::pair< dtPoint3, dtPoint3 > boundingBox
  ) {
    _boundingBox = boundingBox;

    _characteristicLength 
    = 
    dtLinearAlgebra::length(_boundingBox.first - _boundingBox.second);    
  }
    
  void analyticGeometry::updateBoundingBox( void ) const {
    //
    // 1D
    //
    if (dim() == 1) {
      int nOne = 3;
      float uvwOne[3] = {0.00, 0.50, 1.00};
    
      std::vector< dtPoint3 > bb(nOne);
      dt__forFromToIndex(0, nOne, ii) {
        bb[ii] = getPoint( (float*) &(uvwOne[ii]) );
      }
      
      _boundingBox = dtLinearAlgebra::boundingBox(bb);
    }
    //
    // 2D
    //
    else if (dim() == 2) {
      int nTwo = 9;
      float uvwTwo[9][2] 
      = 
      {
        {0.00, 0.00}, {0.00, 0.50}, {0.00, 1.00},
        {0.50, 0.00}, {0.50, 0.50}, {0.50, 1.00},
        {1.00, 0.00}, {1.00, 0.50}, {1.00, 1.00}
      };
      
      std::vector< dtPoint3 > bb(nTwo);
      dt__forFromToIndex(0, nTwo, ii) {
        bb[ii] = getPoint( (float*) &(uvwTwo[ii]) );
      }
      
      _boundingBox = dtLinearAlgebra::boundingBox(bb);
    }
    //
    // 3D
    //
    else if (dim() == 3) {
      int nThree = 27;
      float uvwThree[27][3] 
      = 
      {
        {0.00, 0.00, 0.00}, {0.00, 0.00, 0.50}, {0.00, 0.00, 1.00},
        {0.00, 0.50, 0.00}, {0.00, 0.50, 0.50}, {0.00, 0.50, 1.00},
        {0.00, 1.00, 0.00}, {0.00, 1.00, 0.50}, {0.00, 1.00, 1.00},
        {0.00, 0.00, 0.00}, {0.50, 0.00, 0.50}, {0.50, 0.00, 1.00},
        {0.50, 0.50, 0.00}, {0.50, 0.50, 0.50}, {0.50, 0.50, 1.00},
        {0.50, 1.00, 0.00}, {0.50, 1.00, 0.50}, {0.50, 1.00, 1.00},      
        {1.00, 0.00, 0.00}, {1.00, 0.00, 0.50}, {1.00, 0.00, 1.00},
        {1.00, 0.50, 0.00}, {1.00, 0.50, 0.50}, {1.00, 0.50, 1.00},
        {1.00, 1.00, 0.00}, {1.00, 1.00, 0.50}, {1.00, 1.00, 1.00}
      };

      std::vector< dtPoint3 > bb(nThree);
      dt__forFromToIndex(0, nThree, ii) {
        bb[ii] = getPoint( (float*) &(uvwThree[ii]) );
      }
      
      _boundingBox = dtLinearAlgebra::boundingBox(bb);
    }
    else dt__throwUnexpected(updateBoundingBox());
    
    _characteristicLength 
    = 
    dtLinearAlgebra::length(_boundingBox.first - _boundingBox.second);
  }  
  
  float analyticGeometry::characteristicLength( void ) const {
    if (_characteristicLength < 0.) updateBoundingBox();
    
    return _characteristicLength;
  }
  
  bool analyticGeometry::inXYZTolerance(
    dtPoint3 const & p0, dtPoint3 const & p1, bool output, float inc
  ) {
		float xyzResolution 
		= 
		inc 
    *
    staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );    		
    
		dtVector3 dist = p0 - p1;
		if (sqrt(dist.squared_length()) > xyzResolution) {		
      if (output) {
        dt__warning(
          inXYZTolerance(), 
          << dt__point3d(p0) << std::endl
          << dt__point3d(p1) << std::endl
          << dt__eval( dtLinearAlgebra::length( dist) )
        );
      }
      return false;
    }

    return true;    
  }  
  
  analyticGeometry * new_clone(analyticGeometry const & aG) {
    return aG.clone();
  }	
}