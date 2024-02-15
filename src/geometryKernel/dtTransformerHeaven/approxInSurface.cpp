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

#include "approxInSurface.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/dtPoint3_map1dTo3dEquidistantPoint.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool approxInSurface::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(approxInSurface, new approxInSurface())
  );
  
  approxInSurface::approxInSurface() : dtTransformer() {
  }

	approxInSurface::approxInSurface( approxInSurface const & orig ) {
	  _m2d.reset( orig._m2d->clone() );
		_nPoints = orig._nPoints;
		_order = orig._order;
	}
	
  approxInSurface::~approxInSurface() {
  }

  dtTransformer * approxInSurface::clone( void ) const {
	  return new approxInSurface(*this);
	}
	
  dtTransformer * approxInSurface::create( void ) const {
		return new approxInSurface();
	}
	
  lvH_analyticGeometry approxInSurface::apply( lvH_analyticGeometry const * const aGeoVecP ) const {
    lvH_analyticGeometry retAGeo;
    for (int ii=0; ii<aGeoVecP->size(); ii++) {
      //
      // cast analyticGeometry
      //
      dt__ptrAss(
			  map1dTo3d const * const m1d, 
				map1dTo3d::ConstDownCast(aGeoVecP->at(ii))
			);

			
			std::vector< dtPoint3 > ppXYZ
			=
			dtPoint3_map1dTo3dEquidistantPoint(m1d, _nPoints).result();
			
			std::vector< dtPoint2 > ppUV(ppXYZ.size());
			for (int ii=0; ii<ppUV.size(); ii++) {
				ppUV[ii] = _m2d->reparamOnFace( ppXYZ[ii] );
			}
		
			dt__info(
				apply(),
				<< "Approx " << dt__eval(m1d->getLabel()) << " on " 
				<<  dt__eval(_m2d->getLabel()) << "."
			);
			
      //
      // push approx geometry in vector
      //      
			dt__pH(dtCurve2d) dtC2d(
				bSplineCurve2d_pointConstructOCC(ppUV, _order).result()
			);
			dt__pH(vec2dCurve2dOneD) v2d1d(new vec2dCurve2dOneD(dtC2d.get()) );
			retAGeo.push_back( new vec2dOneDInMap2dTo3d(v2d1d.get(), _m2d.get()) );
			
			retAGeo.back()->setLabel(m1d->getLabel());
    }
    return retAGeo;
  }

  bool approxInSurface::isNecessary( void ) const {
    return true;
  }

  void approxInSurface::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    if (dtXmlParserBase::hasAttribute( "part_label", *tE) ) {
			_m2d.reset(			
				map2dTo3d::ConstDownCast(
					aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))->clone()
				)
			);
    }
    else {
      dt__throw(
				init(), 
				<< dt__eval(dtXmlParserBase::hasAttribute( "part_label", *tE))
			);
    }
		
    if (dtXmlParserBase::hasAttribute( "number_points", *tE) ) {
			_nPoints 
			= 
			dtXmlParserBase::getAttributeIntMuParse("number_points", *tE, cV, aF);
    }
    else {
      dt__throw(
				init(), 
				<< dt__eval(dtXmlParserBase::hasAttribute( "number_points", *tE))
			);
    }		

    if (dtXmlParserBase::hasAttribute( "order", *tE) ) {
			_order 
			= 
			dtXmlParserBase::getAttributeIntMuParse("order", *tE, cV, aF);
    }
    else {
      dt__throw(
				init(), 
				<< dt__eval(dtXmlParserBase::hasAttribute( "order", *tE))
			);
    }				
  }
}
