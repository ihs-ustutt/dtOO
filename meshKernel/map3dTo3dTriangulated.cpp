#include "map3dTo3dTriangulated.h"
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
	map3dTo3dTriangulated::map3dTo3dTriangulated() {
	  GmshInitialize();
	}

	map3dTo3dTriangulated::~map3dTo3dTriangulated() {
		GmshFinalize();
	}
	
  void map3dTo3dTriangulated::init( 
    ::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init cardinalDirVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
    //
		// region
		//		
    ::QDomElement wElement = qtXmlPrimitive::getChild("analyticGeometry", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		//
		// get analyticGeometry, cast and store in region vector
		//
		dt__ptrAss(
			map3dTo3d const * mm3d,
			map3dTo3d::ConstDownCast( aG->get(label) )
		);
    _m3d.reset(mm3d->clone());
		
		//
		// get compound and put pieces as regions to gmsh model
		//
		_gm.reset( new dtGmshModel(getLabel()) );
//		vectorHandling< analyticGeometry const * > cI = mm3d->compoundInternal();		
//		dt__FORALL(cI, ii,
		  _gm->addRegionToGmshModel(_m3d.get());
//		  _gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshTransfinite();
//		);
	}
	
  void map3dTo3dTriangulated::makeGrid(void) {
    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }
    //
    // parse gmsh file if option exists
    //
    if ( hasOption("gmshMeshFile") ) {
      ParseFile( getOption("gmshMeshFile"), true, true );
    }		
		
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();

		//
		// mesh
		//
		_gm->mesh(3);
		
		//
		// notify observers
		//
		boundedVolume::postNotify();
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();		
		
	}
  
	void map3dTo3dTriangulated::makePreGrid(void) {
		boundedVolume::preNotify();
	}
  
	vectorHandling< renderInterface * > map3dTo3dTriangulated::getRender( void ) const {
		vectorHandling< renderInterface * > rV(1);
		
		rV[0] = _gm->toUnstructured3dMesh();
		
		return rV;
	}	
	
	dtGmshModel * map3dTo3dTriangulated::refDtGmshModel( void ) {
		return _gm.get();
	}
}
