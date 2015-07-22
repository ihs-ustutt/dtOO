#include "map3dTo3dBlockGmsh.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <gmsh/Context.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/OpenFile.h>
#include <gmsh/GModel.h>

namespace dtOO {
	map3dTo3dBlockGmsh::map3dTo3dBlockGmsh() : gmshBoundedVolume() {
	}

	map3dTo3dBlockGmsh::~map3dTo3dBlockGmsh() {
	}
	
  void map3dTo3dBlockGmsh::init( 
    ::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init gmshBoundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
    //
		// region
		//		
    ::QDomElement wElement 
    = 
    qtXmlPrimitive::getChild("analyticGeometry", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		
		//
		// get analyticGeometry, cast and store in region vector
		//
		dt__ptrAss(
			map3dTo3d const * mm3d,
			map3dTo3d::ConstDownCast( aG->get(label) )
		);
    _m3d.reset( mm3d->clone() );
		
		//
		// set current model
		//
		::GModel::setCurrent(_gm.get());
		
		vectorHandling< analyticGeometry const * > cI = _m3d->compoundInternal();
		if (cI.size() == 0) {
			cI.push_back(_m3d.get());
		}
		dt__forAllIndex(cI, ii) {
			_gm->addRegionToGmshModel(map3dTo3d::ConstSecureCast(cI[ii]));
//		  _gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshUnstructured();
		}
	}
}
