#include "map3dTo3dTriangulated.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <dtGmshRegion.h>
#include <dtGmshModel.h>
#include <dtGmshFace.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <functionHeaven/analyticFunction.h>
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
    QDomElement const & element,
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
    QDomElement wElement = qtXmlPrimitive::getChild("analyticGeometry", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		std::string pos = "volume";
    addId("region", pos);
		//
		// get analyticGeometry, cast and store in region vector
		//
		dt__PTRASS(
			map3dTo3d const * mm3d,
			map3dTo3d::ConstDownCast( aG->get(label) )
		);
    boundedVolume::getRefToMap3dTo3dHandling()[rStrToId(pos)] = mm3d->clone();
		
		//
		// get compound and put pieces as regions to gmsh model
		//
		_gm.reset( new dtGmshModel(getLabel()) );
//		vectorHandling< analyticGeometry const * > cI = mm3d->compoundInternal();		
//		dt__FORALL(cI, ii,
		  _gm->addRegionToGmshModel(mm3d);
//		  _gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshTransfinite();
//		);
		
		_gm->writeGEO( (getLabel()+".geo").c_str() );
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
//		CTX::instance()->lc = 10.;
		
//		_gm->writeGEO( (getLabel()+".geo").c_str() );
		
//		_gm->mesh(3);
//    _gm->meshVertex();
		
//    for (int ii=1; ii<=_gm->getNumRegions(); ii++) {
//		  _gm->meshRegion( ii );
//			_gm->writeMSH( (getLabel()+"_building.msh").c_str() );
//		}
		_gm->mesh(3);
		_gm->writeMSH( (getLabel()+".msh").c_str() );
		
	}
  
	void map3dTo3dTriangulated::makePreGrid(void) {
		boundedVolume::notify();
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
