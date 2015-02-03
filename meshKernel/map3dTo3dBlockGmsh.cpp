#include "map3dTo3dBlockGmsh.h"
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
	map3dTo3dBlockGmsh::map3dTo3dBlockGmsh() {
	  GmshInitialize();
	}

	map3dTo3dBlockGmsh::~map3dTo3dBlockGmsh() {
		GmshFinalize();
	}
	
  void map3dTo3dBlockGmsh::init( 
    QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
    //
		// region
		//		
    QDomElement wElement = qtXmlPrimitive::getChild("analyticGeometry", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		
		//
		// get analyticGeometry, cast and store in region vector
		//
		dt__PTRASS(
			map3dTo3d const * mm3d,
			map3dTo3d::ConstDownCast( aG->get(label) )
		);
    _m3d.reset( mm3d->clone() );
		
		//
		// get compound and put pieces as regions to gmsh model
		//
		_gm.reset( new dtGmshModel() );

		//
		// set current model
		//
		GModel::setCurrent(_gm.get());
		
		vectorHandling< analyticGeometry const * > cI = _m3d->compoundInternal();
		if (cI.size() == 0) {
			cI.push_back(_m3d.get());
		}
		dt__FORALL(cI, ii,
		  _gm->addRegionToGmshModel(map3dTo3d::ConstSecureCast(cI[ii]));
		  _gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshTransfinite();
		);
	}
	
  void map3dTo3dBlockGmsh::makeGrid(void) {
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
		// mark as meshed
		//
		boundedVolume::setMeshed();		
	}
  
	void map3dTo3dBlockGmsh::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > map3dTo3dBlockGmsh::getRender( void ) const {
		if (mustExtRender()) return vectorHandling< renderInterface * >(0);
		vectorHandling< renderInterface * > rV(1);
		rV[0] = _gm->toUnstructured3dMesh();
		
		return rV;
	}	

	vectorHandling< renderInterface * > map3dTo3dBlockGmsh::getExtRender( void ) const {
		vectorHandling< renderInterface * > rV(1);
		rV[0] = _gm->toUnstructured3dMesh();
		
		return rV;
	}		
	
	std::vector< std::string > map3dTo3dBlockGmsh::getMeshTags( void ) const {
		std::vector< std::string > tags;
		tags.push_back("internal");

		return tags;
	}
		
	dtGmshModel * map3dTo3dBlockGmsh::getModel( void ) const {
		return _gm.get();
	}
}
