/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "map3dTo3dWithInternalGmsh.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include "boundedVolumeFactory.h"

namespace dtOO {
  bool map3dTo3dWithInternalGmsh::_registrated 
  =
  boundedVolumeFactory::registrate(
    dt__tmpPtr(map3dTo3dWithInternalGmsh, new map3dTo3dWithInternalGmsh())
  );
  
	map3dTo3dWithInternalGmsh::map3dTo3dWithInternalGmsh() : gmshBoundedVolume() {
	}

	map3dTo3dWithInternalGmsh::~map3dTo3dWithInternalGmsh() {
	}
  
  void map3dTo3dWithInternalGmsh::init( 
    ::QDomElement const & element,
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV
	) {
    //
    // init gmshBoundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
//		//
//		// get margin width, margin element size and blade element size
//		//
//		_bladeNormalWidth 
//		= 
//		qtXmlBase::getAttributeFloatMuParse("bladeNormalWidth", element, cV, aF);
//		_bladeTangentialWidth 
//		= 
//	  qtXmlBase::getAttributeFloatMuParse("bladeTangentialWidth", element, cV, aF);
		
    //
		// get geometries
		//		
    std::vector< ::QDomElement > wElement 
		= 
		qtXmlPrimitive::getChildVector("analyticGeometry", element);
		dt__forAllIter(std::vector< ::QDomElement >, wElement, it) {
			//
			// get analyticGeometry, cast and store in region vector
			//
      dt__pH(analyticGeometry) aG_t( 
        dtXmlParserBase::createAnalyticGeometry(
          *it, (baseContainer*)bC, cV, aF, aG
        )
      );
			map3dTo3d const * const mm3d = map3dTo3d::ConstDownCast(aG_t.get());
			map2dTo3d const * const mm2d = map2dTo3d::ConstDownCast(aG_t.get());
			
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
		::GModel::setCurrent( _gm );
	
		dt__pVH(map2dTo3d) m2dV;
		m2dV.push_back( _m3d->segmentConstWPercent(0.) );
		m2dV.push_back( _m3d->segmentConstWPercent(1.) );
		m2dV.push_back( _m3d->segmentConstVPercent(0.) );
		m2dV.push_back( _m3d->segmentConstVPercent(1.) );
		m2dV.push_back( _m3d->segmentConstUPercent(0.) );
		m2dV.push_back( _m3d->segmentConstUPercent(1.) );		
		std::vector< dtInt > fId(6);
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

			std::vector< dtInt > eNEl;

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
		dtGmshRegion * gr = new dtGmshRegion( _gm , _gm->getMaxRegionTag()+1);
		std::list< ::GFace * > ff = _gm->faces();
		dt__forAllIter(std::list< ::GFace * >, ff, it) {
			dtGmshModel::cast2DtGmshFace(*it)->correctIfTransfinite();
			gr->addFace(*it, 1);
			(*it)->addRegion(gr);
		}	
		_gm->add( gr );		
	}
}
