#include "map3dTo3dWithInternalBlockGmsh.h"
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/qtXmlBase.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <gmsh/Context.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/OpenFile.h>
#include <gmsh/GModel.h>

namespace dtOO {
	map3dTo3dWithInternalBlockGmsh::map3dTo3dWithInternalBlockGmsh() : gmshBoundedVolume() {
	}

	map3dTo3dWithInternalBlockGmsh::~map3dTo3dWithInternalBlockGmsh() {
	}
	
  void map3dTo3dWithInternalBlockGmsh::init( 
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
		// get geometries
		//		
    // 1) first element should be the outer channel
    // 2) next elements should be the internal blocks surrounding the 
    //    internal surface
    //
    std::vector< QDomElement > wElement 
		= 
		qtXmlPrimitive::getChildVector("analyticGeometry", element);
		dt__forAllIter(std::vector< QDomElement >, wElement, it) {
			//
			// get analyticGeometry, cast and store in region vector
			//
      dt__pH(analyticGeometry) aG_t( 
        dtXmlParserBase::createAnalyticGeometry(
          *it, (baseContainer*)bC, cV, aF, aG
        )
      );
      dt__ptrAss(
        map3dTo3d const * const mm3d,
        map3dTo3d::ConstDownCast(aG_t.get())
      );
      
			if (mm3d && _m3d.isNull()) _m3d.reset( mm3d->clone() );
			else _internal.push_back( mm3d->clone() );
		}	

		//
		// set current model
		//
		::GModel::setCurrent(_gm.get());
	
    //
    // add all faces of outer channel
    //
		std::vector< int > rId;
    rId.push_back(1);
    _gm->addIfRegionToGmshModel(_m3d.get(), &(rId.back()) );

		dtGmshRegion * const & volume = _gm->getDtGmshRegionByTag(1);
		dtGmshFace * const & hub = _gm->getDtGmshFaceByTag(1);
		dtGmshFace * const & shroud = _gm->getDtGmshFaceByTag(2);

    //
    // internal
    //		
		dt__forAllIter(dt__pVH(map3dTo3d), _internal, it) {
      map3dTo3d const & theRegion = *it;
      rId.push_back(1);      
      _gm->addIfRegionToGmshModel(&theRegion, &(rId.back()));
			//
			// create internal faces and edges
			//
			dt__pH(map1dTo3d) iE0(
        theRegion.segmentPercent(dtPoint3(0,1,0), dtPoint3(1,1,0))
      );
			dt__pH(map1dTo3d) iE1(
        theRegion.segmentPercent(dtPoint3(0,1,1), dtPoint3(1,1,1))
      );      

      //
      // internal edges (boundary edges between channel and internal blocks)
      //
			int iE0Tag;
			int iE1Tag;
			_gm->addIfEdgeToGmshModel(iE0.get(), &iE0Tag);
			hub->addEdge(_gm->getDtGmshEdgeByTag(iE0Tag), 1);
			_gm->addIfEdgeToGmshModel(iE1.get(), &iE1Tag);
			shroud->addEdge(_gm->getDtGmshEdgeByTag(iE1Tag), 1);

      //
      // internal faces (boundary faces between channel and internal blocks)
      //
      int iFTag;
			dt__pH(map2dTo3d) iF(
        theRegion.segmentPercent(
          dtPoint3(0,1,0), dtPoint3(1,1,0), 
          dtPoint3(1,1,1), dtPoint3(0,1,1)
        )
      );            
      _gm->addIfFaceToGmshModel(iF.get(), &iFTag);
      volume->addFace( _gm->getDtGmshFaceByTag(iFTag), 1);
		}
		
		//
		// correct transfinite surfaces and create region
		//
		std::list< ::GFace * > ff = _gm->faces();
		dt__forAllIter(std::list< ::GFace * >, ff, it) {
			dtGmshModel::cast2DtGmshFace(*it)->correctIfTransfinite();
		}	
	}
	
  void map3dTo3dWithInternalBlockGmsh::makeGrid(void) {
		//
		// set current model
		//
		::GModel::setCurrent(_gm.get());
		
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
		if ( !optionHandling::optionTrue("defer_mesh_0") ) _gm->meshPhysical(0);
		if ( !optionHandling::optionTrue("defer_mesh_1") ) _gm->meshPhysical(1);
		if ( !optionHandling::optionTrue("defer_mesh_2") ) _gm->meshPhysical(2);
		if ( !optionHandling::optionTrue("defer_mesh_3") ) _gm->meshPhysical(3);
		
		//
		// notify observers
		//
		boundedVolume::postNotify();
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();			
	}
  
	void map3dTo3dWithInternalBlockGmsh::makePreGrid(void) {
		bVOSubject::preNotify();
	}
}