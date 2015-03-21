#include "map3dTo3dWithInternalGmsh.h"
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshMeshGFaceExtrude.h>
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/intHandling.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map2dTo3d_constructMarginFaces.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_closeGapsArithmetic.h>
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
		_marginWidth = optionHandling::getOptionFloat("marginWidth");
		_marginNormalWidth = optionHandling::getOptionFloat("marginNormalWidth");
		_marginTangentialWidth = optionHandling::getOptionFloat("marginTangentialWidth");
		_bladeNormalWidth = optionHandling::getOptionFloat("bladeNormalWidth");
		_bladeTangentialWidth = optionHandling::getOptionFloat("bladeTangentialWidth");
		_normalAddNumberIntegrationPoints 
		= 
		optionHandling::getOptionInt("normalAddNumberIntegrationPoints");
		
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
		twoDArrayHandling< int > fId(6,0);
		for (int ii = 0; ii<6; ii++) {
			dt__pVH(map2dTo3d) margin 
			= 
			map2dTo3d_constructMarginFaces(&(m2dV[ii]), _marginWidth).result();
			
			dt__forAllIter(dt__pVH(map2dTo3d), margin, it) {
				map2dTo3d const & thisMap = *it;
				fId[ii].push_back(0);
				_gm->addIfFaceToGmshModel( &thisMap, &(fId[ii].back()) );
			}
			
			//
			// set number of elements
			//
      for (int jj=1; jj<5; jj++) {	
				std::vector< int > eNEl
				=
	      _gm->getDtGmshFaceByTag(fId[ii][jj])->estimateTransfiniteNElements(
  				_marginTangentialWidth, _marginNormalWidth
				);
				_gm->getDtGmshFaceByTag(fId[ii][jj])->meshTransfiniteWNElements(eNEl[0], eNEl[1]);
		  }
		}
		
		dtGmshFace * const & hub = _gm->getDtGmshFaceByTag(fId[0][0]);
		dtGmshFace * const & shroud = _gm->getDtGmshFaceByTag(fId[1][0]);
		map2dTo3d const * const & hubMap = hub->getMap2dTo3d();
		map2dTo3d const * const & shroudMap = shroud->getMap2dTo3d();
		
		dt__pVH(map1dTo3d) bL0;
		dt__pVH(map1dTo3d) bL1;
		
		//
		// add normal to internal faces
		//		
		dt__forAllIter(dt__pVH(map2dTo3d), _internal, it) {
			map2dTo3d const & thisFace = *it;
			
			dt__pH(map1dTo3d) iE0(thisFace.segmentConstVPercent(0.));
			dt__pH(map1dTo3d) iE1(thisFace.segmentConstVPercent(1.));
			
			int nPoints
			=
			intHandling::round(iE0->length()/_marginTangentialWidth);
			bL0.push_back(
				map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(
					iE0.get(), hubMap, _marginWidth, 
					nPoints, _normalAddNumberIntegrationPoints, 1 
				).result() 
			);
			nPoints
			=
			intHandling::round(iE1->length()/_marginTangentialWidth);			
			bL1.push_back(
			  map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(
			    iE1.get(), shroudMap, _marginWidth, 
					nPoints, _normalAddNumberIntegrationPoints, 1 
			  ).result() 
			);
		}
			
		//
		// close gaps
		//
		bL0 = map1dTo3d_closeGapsArithmetic(bL0).result();
		bL1 = map1dTo3d_closeGapsArithmetic(bL1).result();
		
		dt__forAllIter(dt__pVH(map2dTo3d), _internal, it) {
			//
			// create internal faces and edges
			//
			map2dTo3d const & thisFace = *it;
			int ii = it - _internal.begin();
			dt__pH(map1dTo3d) iE0(thisFace.segmentConstVPercent(0.));
			dt__pH(map1dTo3d) iE1(thisFace.segmentConstVPercent(1.));
			
			dt__pVH(map1dTo3d) bLN0;
			bLN0.push_back(
				hubMap->segment(
					hubMap->reparamOnFace( iE0->getPointPercent(0.) ),
					hubMap->reparamOnFace( bL0[ii].getPointPercent(0.) )
				)
			);
			bLN0.push_back(
				hubMap->segment(
					hubMap->reparamOnFace( iE0->getPointPercent(1.) ),
					hubMap->reparamOnFace( bL0[ii].getPointPercent(1.) )
				)
			);			
			dt__pVH(map1dTo3d) bLN1;
			bLN1.push_back(
				shroudMap->segment(
					shroudMap->reparamOnFace( iE1->getPointPercent(0.) ),
					shroudMap->reparamOnFace( bL1[ii].getPointPercent(0.) )
				)
			);
			bLN1.push_back(
				shroudMap->segment(
					shroudMap->reparamOnFace( iE1->getPointPercent(1.) ),
					shroudMap->reparamOnFace( bL1[ii].getPointPercent(1.) )
				)
			);

			//
			// add hub bL edge
			//
			std::vector< int > bLLoop0(4);
			_gm->addIfEdgeToGmshModel(iE0.get(),  &(bLLoop0[0]));
			_gm->addIfEdgeToGmshModel(&(bLN0[1]), &(bLLoop0[1]));
			_gm->addIfEdgeToGmshModel(&(bL0[ii]), &(bLLoop0[2]));
			_gm->addIfEdgeToGmshModel(&(bLN0[0]), &(bLLoop0[3]));
			hub->addEdge( _gm->getDtGmshEdgeByTag(bLLoop0[2]), 1);
			bLLoop0[2] = bLLoop0[2]*(-1);
			bLLoop0[3] = bLLoop0[3]*(-1);
			
			std::vector< int > bLLoop1(4);
			_gm->addIfEdgeToGmshModel(iE1.get(),  &(bLLoop1[0]));
			_gm->addIfEdgeToGmshModel(&(bLN1[1]), &(bLLoop1[1]));			
			_gm->addIfEdgeToGmshModel(&(bL1[ii]), &(bLLoop1[2]));
			_gm->addIfEdgeToGmshModel(&(bLN1[0]), &(bLLoop1[3]));
			shroud->addEdge( _gm->getDtGmshEdgeByTag(bLLoop1[2]), 1);
			bLLoop1[2] = bLLoop1[2]*(-1);
			bLLoop1[3] = bLLoop1[3]*(-1);

			int tmp;
			std::vector< int > eNEl;
			_gm->addIfFaceToGmshModel(
			  hubMap, &tmp, 
				bLLoop0[0], bLLoop0[1], bLLoop0[2], bLLoop0[3]
			);
  		eNEl = _gm->getDtGmshFaceByTag(tmp)->estimateTransfiniteNElements(
			  _bladeTangentialWidth, _bladeNormalWidth			
			);
			_gm->getDtGmshFaceByTag(tmp)->meshTransfiniteWNElements(
			  eNEl[0],eNEl[1]
			);
			
			_gm->addIfFaceToGmshModel(
			  shroudMap, &tmp, 
				bLLoop1[0], bLLoop1[1], bLLoop1[2], bLLoop1[3]
			);
  		eNEl = _gm->getDtGmshFaceByTag(tmp)->estimateTransfiniteNElements(
			  _bladeTangentialWidth, _bladeNormalWidth			
			);
			_gm->getDtGmshFaceByTag(tmp)->meshTransfiniteWNElements(
			  eNEl[0],eNEl[1]
			);			
			
			
			//
			// add blade face
			// 
			_gm->addIfFaceToGmshModel(&thisFace, &(tmp));
  		eNEl = _gm->getDtGmshFaceByTag(tmp)->estimateTransfiniteNElements(
			  _bladeTangentialWidth, _bladeNormalWidth			
			);
			_gm->getDtGmshFaceByTag(tmp)->meshTransfiniteWNElements(
			  eNEl[0],eNEl[1]
			);
		}
		
//		//
//		// remove old faces
//		//
//		for (int ii = 0; ii<6; ii++) delete _gm->getDtGmshFaceByTag(ii+1);	
		
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
		_gm->meshPhysical(0);
		_gm->meshPhysical(1);
		_gm->meshPhysical(2);
		
		std::list< dtGmshFace * > llF;
//		llF.push_back(_gm->getDtGmshFaceByPhysical("FACE_31"));
		llF.push_back(_gm->getDtGmshFaceByPhysical("FACE_32"));
		llF.push_back(_gm->getDtGmshFaceByPhysical("FACE_33"));
		dtGmshMeshGFaceExtrude()(llF);
		
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
		boundedVolume::notify();
	}
}
