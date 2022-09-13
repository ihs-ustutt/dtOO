#include "map3dTo3dGmsh.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include "boundedVolumeFactory.h"

namespace dtOO {
  bool map3dTo3dGmsh::_registrated 
  =
  boundedVolumeFactory::registrate(
    dt__tmpPtr(map3dTo3dGmsh, new map3dTo3dGmsh())
  );
  
	map3dTo3dGmsh::map3dTo3dGmsh() : gmshBoundedVolume() {
	}

	map3dTo3dGmsh::~map3dTo3dGmsh() {
	}
  
  void map3dTo3dGmsh::init( 
    ::QDomElement const & element,
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV
	) {
    //
    // init gmshBoundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
    //
		// region
		//		
    std::vector< ::QDomElement > wEl 
    = 
    qtXmlPrimitive::getChildVector("analyticGeometry", element);

		//
		// set current model
		//
		::GModel::setCurrent( _gm );

		dt__forAllIndex(wEl, ii) {
      dt__ptrAss(
        map3dTo3d const * m3d,
        map3dTo3d::ConstDownCast( 
          aG->get( qtXmlPrimitive::getAttributeStr("label", wEl[ii]) ) 
        )
      );      
      dtInt vId;
			_gm->addIfRegionToGmshModel(m3d, &vId);

		}
	}
}
