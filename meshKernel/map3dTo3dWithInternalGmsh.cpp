#include "map3dTo3dWithInternalGmsh.h"
#include <dtXmlParserDecorator/qtXmlBase.h>
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
		// get margin width, margin element size and blade element size
		//
		_bladeNormalWidth 
		= 
		qtXmlBase::getAttributeFloatMuParse("bladeNormalWidth", element, cV, aF);
		_bladeTangentialWidth 
		= 
	  qtXmlBase::getAttributeFloatMuParse("bladeTangentialWidth", element, cV, aF);
		
    //
		// get geometries
		//		
    std::vector< QDomElement > wElement 
		= 
		qtXmlPrimitive::getChildVector("analyticGeometry", element);
		dt__forAllIter(std::vector< QDomElement >, wElement, it) {
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
		}	

		//
		// set current model
		//
		::GModel::setCurrent(_gm.get());
	
		dt__pVH(map2dTo3d) m2dV;
		m2dV.push_back( _m3d->segmentConstWPercent(0.) );
		m2dV.push_back( _m3d->segmentConstWPercent(1.) );
		m2dV.push_back( _m3d->segmentConstVPercent(0.) );
		m2dV.push_back( _m3d->segmentConstVPercent(1.) );
		m2dV.push_back( _m3d->segmentConstUPercent(0.) );
		m2dV.push_back( _m3d->segmentConstUPercent(1.) );		
		std::vector< int > fId(6);
		for (int ii = 0; ii<6; ii++) {
			map2dTo3d const & thisMap = m2dV[ii];
			fId[ii] = 0;
			_gm->addIfFaceToGmshModel( &thisMap, &(fId[ii]) );
		}
		
		dtGmshFace * const & hub = _gm->getDtGmshFaceByTag(fId[0]);
		dtGmshFace * const & shroud = _gm->getDtGmshFaceByTag(fId[1]);
		map2dTo3d const * const & hubMap = hub->getMap2dTo3d();
		map2dTo3d const * const & shroudMap = shroud->getMap2dTo3d();
		
    //
    // internal
    //		
		dt__forAllIter(dt__pVH(map2dTo3d), _internal, it) {
			//
			// create internal faces and edges
			//
			map2dTo3d const & thisFace = *it;
			dt__pH(map1dTo3d) iE0(thisFace.segmentConstVPercent(0.));
			dt__pH(map1dTo3d) iE1(thisFace.segmentConstVPercent(1.));

			std::vector< int > eNEl;

			int iE0Tag;
			int iE1Tag;
			_gm->addIfEdgeToGmshModel(iE0.get(), &iE0Tag);
			hub->addEdge(_gm->getDtGmshEdgeByTag(iE0Tag), 1);
			_gm->addIfEdgeToGmshModel(iE1.get(), &iE1Tag);
			shroud->addEdge(_gm->getDtGmshEdgeByTag(iE1Tag), 1);
			
			//
			// add blade face
			// 
			int tmp;
			_gm->addIfFaceToGmshModel(&thisFace, &(tmp));
//  		eNEl = _gm->getDtGmshFaceByTag(tmp)->estimateTransfiniteNElements(
//			  _bladeTangentialWidth, _bladeNormalWidth			
//			);
//			_gm->getDtGmshFaceByTag(tmp)->meshTransfiniteWNElements(
//			  eNEl[0],eNEl[1]
//			);
//			_gm->getDtGmshFaceByTag(tmp)->meshAttributes.method = MESH_UNSTRUCTURED;
		}
		
		//
		// correct transfinite surfaces and create region
		//
		dtGmshRegion * gr = new dtGmshRegion(_gm.get(), _gm->getMaxRegionTag());
		std::list< ::GFace * > ff = _gm->faces();
		dt__forAllIter(std::list< ::GFace * >, ff, it) {
			dtGmshModel::cast2DtGmshFace(*it)->correctIfTransfinite();
			gr->addFace(*it, 1);
		}	
		_gm->add( gr );		
	}
	
  void map3dTo3dWithInternalGmsh::makeGrid(void) {
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
		
		_gm->writeMSH("dieterHerbert.msh");		
//		_gm->writeGEO("dieterHerbert.geo");				
	}
  
	void map3dTo3dWithInternalGmsh::makePreGrid(void) {
		bVOSubject::notify();
	}
}
