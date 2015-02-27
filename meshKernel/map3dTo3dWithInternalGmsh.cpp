#include "map3dTo3dWithInternalGmsh.h"
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
	map3dTo3dWithInternalGmsh::map3dTo3dWithInternalGmsh() : gmshBoundedVolume() {
	}

	map3dTo3dWithInternalGmsh::~map3dTo3dWithInternalGmsh() {
	}
	
  void map3dTo3dWithInternalGmsh::init( 
    QDomElement const & element,
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
    std::vector< QDomElement > wElement = qtXmlPrimitive::getChildVector("analyticGeometry", element);
		dt__FORALLITER(std::vector< QDomElement >, wElement, it) {
			std::string label = qtXmlPrimitive::getAttributeStr("label", *it);

			//
			// get analyticGeometry, cast and store in region vector
			//
			map3dTo3d const * mm3d = map3dTo3d::ConstDownCast( aG->get(label) );
			map2dTo3d const * mm2d = map2dTo3d::ConstDownCast( aG->get(label) );
			
			if (mm3d) {
				_m3d.reset( mm3d->clone() );
			}
			else if (mm2d) {
				_internal.push_back( mm2d->clone() );
			}
			

			//
			// set current model
			//
			GModel::setCurrent(_gm.get());

			_gm->addRegionToGmshModel(_m3d.get());
		}
	}
	
  void map3dTo3dWithInternalGmsh::makeGrid(void) {
		//
		// set current model
		//
		GModel::setCurrent(_gm.get());
		
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
		// destroy old mesh
		//
		_gm->deleteMesh();
		
		//
		// reset vertex and element numbering
		//
		_gm->setMaxVertexNumber(0); 
		_gm->setMaxElementNumber(0);
		
		//
		// meshing
		//
		_gm->mesh(0);
		_gm->mesh(1);
		_gm->mesh(2);
		_gm->mesh(3);
		
    //
		// force renumbering mesh in gmsh
		//
    _gm->indexMeshVertices(true, 0, true);
		
		//
		// update physicals
		//
		gmshBoundedVolume::updatePhysicals();
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();		
	}
  
	void map3dTo3dWithInternalGmsh::makePreGrid(void) {
		boundedVolume::notify();
	}
}
