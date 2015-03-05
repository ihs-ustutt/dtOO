#include "map2dTo3dTriangulated.h"
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
	map2dTo3dTriangulated::map2dTo3dTriangulated() : gmshBoundedVolume() {
	}

	map2dTo3dTriangulated::~map2dTo3dTriangulated() {
	}
	
  void map2dTo3dTriangulated::init( 
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
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
//		//
//		// create model
//		//
//	  _gm.reset( new dtGmshModel(getLabel()) );
			
    //
		// region
		//		
    std::vector< QDomElement > wElementVec 
		= 
		qtXmlPrimitive::getChildVector("analyticGeometry", element);
    
		dt__FORALLCONSTITER(std::vector< QDomElement >, wElementVec, it) { 
			QDomElement const & wElement = *it;
			
			//
			// get analyticGeometry, cast and store in region vector
			//
			dt__PTRASS(
				map2dTo3d const * mm2d,
				map2dTo3d::ConstDownCast( 
				  aG->get(qtXmlPrimitive::getAttributeStr("label", wElement)) 
				)
			);

			//
			// get face and add to gmsh model
			//
			int fId;
			_gm->addIfFaceToGmshModel(mm2d, &fId);
		}
	}
	
  void map2dTo3dTriangulated::makeGrid(void) {
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

		_gm->mesh(2);
//		_gm->writeMSH( (getLabel()+".msh").c_str() );
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();		
		
	}
  
	void map2dTo3dTriangulated::makePreGrid(void) {
		boundedVolume::notify();
		
		//
		// update physicals
		//
		gmshBoundedVolume::updatePhysicals();
	}
  
//	vectorHandling< renderInterface * > map2dTo3dTriangulated::getRender( void ) const {
//		vectorHandling< renderInterface * > rV(1);
//		
//		rV[0] = _gm->toUnstructured3dMesh();
//		
//		return rV;
//	}	
//	
//	dtGmshModel * map2dTo3dTriangulated::refDtGmshModel( void ) {
//		return _gm.get();
//	}
}
