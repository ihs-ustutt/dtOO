#include "map2dTo3dTriangulated.h"
#include <meshEngine/dtGmshModel.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/Gmsh.h>
#include <gmsh/OpenFile.h>

namespace dtOO {
	map2dTo3dTriangulated::map2dTo3dTriangulated() : gmshBoundedVolume() {
	}

	map2dTo3dTriangulated::~map2dTo3dTriangulated() {
	}
	
  void map2dTo3dTriangulated::init( 
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
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
					
    //
		// region
		//		
    std::vector< ::QDomElement > wElementVec 
		= 
		qtXmlPrimitive::getChildVector("analyticGeometry", element);
    
		dt__forAllConstIter(std::vector< ::QDomElement >, wElementVec, it) { 
			//
			// get analyticGeometry, cast and store in region vector
			//
			dt__ptrAss(
				map2dTo3d const * mm2d,
				map2dTo3d::ConstDownCast( 
				  aG->get(qtXmlPrimitive::getAttributeStr("label", *it)) 
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

		//
		// mesh
		//
		if ( !optionHandling::optionTrue("defer_mesh_0") ) _gm->meshPhysical(0);
		if ( !optionHandling::optionTrue("defer_mesh_1") ) _gm->meshPhysical(1);
		if ( !optionHandling::optionTrue("defer_mesh_2") ) _gm->meshPhysical(2);

		//
		// notify observers
		//
		boundedVolume::postNotify();
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();		
	}
  
	void map2dTo3dTriangulated::makePreGrid(void) {
		boundedVolume::preNotify();
	}  
}
