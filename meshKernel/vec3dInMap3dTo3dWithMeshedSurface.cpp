#include "vec3dInMap3dTo3dWithMeshedSurface.h"

#include <logMe/logMe.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <interfaceHeaven/twoDArrayHandling.h>

#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <unstructured3dMesh.h>

#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/OpenFile.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/vec3dTransVolThreeD.h>
#include <functionHeaven/vec3dBoxThreeD.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_geomCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_geomSurfaceTranslateConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>

namespace dtOO {
	vec3dInMap3dTo3dWithMeshedSurface::vec3dInMap3dTo3dWithMeshedSurface() : gmshBoundedVolume() {
	}

	vec3dInMap3dTo3dWithMeshedSurface::~vec3dInMap3dTo3dWithMeshedSurface() {
	}
	
  void vec3dInMap3dTo3dWithMeshedSurface::init( 
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
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
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
		// boundedVolume
		//		
    wElement = qtXmlPrimitive::getChild("analyticFunction", element);
    label = qtXmlPrimitive::getAttributeStr("boundedVolume_label", wElement);
		
		//
		// get boundedVolume
		//
    _meshedBV = bV->get(label);
    _meshedFaceTag = qtXmlPrimitive::getAttributeStr("reconstruct", wElement);
	}
	
  void vec3dInMap3dTo3dWithMeshedSurface::makeGrid(void) {				
		//
		// set current model
		//
		::GModel::setCurrent(_gm.get());
		_gm->clearModel();
		
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
		
		dt__WARN_IFWMAS(
			!_meshedBV->isMeshed(), 
			return;, 
			makeGrid(), 
			<< logMe::dtFormat("%s is not yet meshed.") % _meshedBV->getLabel()
		);
		
		dtGmshFace const * gf = _meshedBV->getFace(_meshedFaceTag);
		
		twoDArrayHandling< ::MVertex * > mv = gf->reconstructEdgesFromSurfaceMesh();
		twoDArrayHandling< dtPoint3 > mvUVW;//(mv.size(0));
		mvUVW.resize(mv.size(0));
		
		//
		// reconstruct edge vertices
		//
		int ii = 0;
		int jj = 0;
		dt__pVH(dtCurve) dtC;//(mv.size(0));
		std::vector< dtPoint3 > ultraBoxPoints;//(mv.size(0));
		for (
			twoDArrayHandling< ::MVertex * >::iterator0 it0 = mv.begin();
			it0 != mv.end();
			++it0
		) {
			mvUVW[ii].resize(mv[ii].size());
			jj = 0;
			for (
				twoDArrayHandling< ::MVertex * >::iterator1 it1 = it0->begin();
				it1 != it0->end();
				++it1
			) {			
			  mvUVW[ii][jj]
				= 
				_m3d->reparamInVolume( dtGmshModel::cast2DtPoint3(*it1) );
//				DTINFOWF(
//					makeGrid(), 
//					<< logMe::dtFormat("mvUVW[%d][%d] = %f, %f, %f") 
//					% ii % jj % mvUVW[ii][jj].x() % mvUVW[ii][jj].y() % mvUVW[ii][jj].z()
//				);
				jj++;
			}
			
			std::pair< dtPoint3, dtPoint3 > bb = dtLinearAlgebra::boundingBox(mvUVW[ii]);
			ultraBoxPoints.push_back(bb.first);
			ultraBoxPoints.push_back(bb.second);
			float eps = staticPropertiesHandler::getInstance()->getOptionFloat("uv_resolution");
			if ( dtLinearAlgebra::isStraightLine(bb, eps) ) {
				dtC.push_back( 
				  trimmedCurve_twoPointsConnectConstructOCC(
				    mvUVW[ii].front(), mvUVW[ii].back()).result() 
				);
			}
			else {
			  dtC.push_back( 
				  bSplineCurve_pointInterpolateConstructOCC(mvUVW[ii]).result()
				);
			}
			ii++;
		}
		
		//
		// create coupling surface
		//
		vectorHandling< dtSurface const * > dtS(2);
		dtS[0] = geomSurface_geomCurveFillConstructOCC(dtC).result();
		//dtS[1] = geomSurface_geomSurfaceTranslateConstructOCC(dtS[0], _vv).result();
		dtSurface * wS = dtS[0]->clone();
		std::vector< float > scale(3, 1.);
		std::vector< float > trans(3, 0.);
		
		if (hasOption("extent_to")) {
			std::string opt = getOption("extent_to");
			if (opt == "vMin") {
				scale[1] = 0.;
				trans[1] = _m3d->getVMin();
			}
			else if (opt == "vMax") {
				scale[1] = 0.;
				trans[1] = _m3d->getVMax();				
			}
			else {
				dt__THROW(
					makeGrid(), 
					<< DTLOGEVAL(opt) << LOGDEL
					<< "Unknown option."
				);
			}
		}
		for (int ii=0; ii<wS->nControlPointsU(); ii++) {
			for (int jj=0; jj<wS->nControlPointsV(); jj++) {
				dtPoint3 cP = wS->controlPoint(ii, jj);
				cP 
				= 
				dtPoint3(
					cP.x()*scale[0] + trans[0], 
					cP.y()*scale[1] + trans[1], 
					cP.z()*scale[2] + trans[2]
				);
				wS->setControlPoint(ii,jj, cP);
			}
		}
		dtS[1] = wS;
		
		//
		// do skinning with surface
		//
		vectorHandling< dtSurface * > dtSSkin 
		=
		bSplineSurfaces_bSplineSurfaceSkinConstructOCC(dtS).result();
		
		//
		// create transfinite volume
		//
		ptrHandling<vec3dSurfaceTwoD> v3d2d0(new vec3dSurfaceTwoD(dtSSkin[0])); // north
		ptrHandling<vec3dSurfaceTwoD> v3d2d1(new vec3dSurfaceTwoD(dtSSkin[1])); // west
		ptrHandling<vec3dSurfaceTwoD> v3d2d2(new vec3dSurfaceTwoD(dtSSkin[2])); // south
		ptrHandling<vec3dSurfaceTwoD> v3d2d3(new vec3dSurfaceTwoD(dtSSkin[3])); // east
		ptrHandling<vec3dSurfaceTwoD> v3d2d4(new vec3dSurfaceTwoD(dtSSkin[4])); // front
		ptrHandling<vec3dSurfaceTwoD> v3d2d5(new vec3dSurfaceTwoD(dtSSkin[5])); // back
		dt__pH(vec3dThreeD) v3d3d( 
			new vec3dTransVolThreeD( 
				v3d2d0.get(), v3d2d1.get(), v3d2d2.get(), 
				v3d2d3.get(), v3d2d4.get(), v3d2d5.get()
			)
		);		

		//
		// create reconstructed face
		//
		_recFace.reset( new vec3dTwoDInMap3dTo3d(
		  dt__pH(vec3dTwoD)(new vec3dSurfaceTwoD(dtS[0])).get(), _m3d.get()
		) );
		
		//
		// create reconstructed volume
		//
		_recVol.reset( new vec3dThreeDInMap3dTo3d(v3d3d.get(), _m3d.get()) );
		
		//
		// create couplingBox
		//
		std::pair< dtPoint3, dtPoint3 > ultraBox = dtLinearAlgebra::boundingBox(ultraBoxPoints);
		DTINFOWF(
			makeGrid(),
			<< logMe::dtFormat("ultraBox(%f, %f, %f : %f, %f, %f)")
						% ultraBox.first.x() % ultraBox.first.y() % ultraBox.first.z()
						% ultraBox.second.x() % ultraBox.second.y() % ultraBox.second.z()
		);
		_couplingBox.reset(
		  new vec3dThreeDInMap3dTo3d(
		    dt__pH(vec3dThreeD)(new vec3dBoxThreeD(ultraBox.first, ultraBox.second) ).get(),
				_m3d.get()
		  ) 
		);
		
		//
		// free memory
		//
		dtS.destroy();
		dtSSkin.destroy();
		
    //
		// add reconstructed volume
		//
		_gm->addRegionToGmshModel( _recVol.get() );
//		_gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshTransfinite();

	  //
		// call observers
		//
		boundedVolume::notify();
				
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();
		
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
		// update physical labels
		//
		gmshBoundedVolume::updatePhysicals();
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();
	}
  
	void vec3dInMap3dTo3dWithMeshedSurface::makePreGrid(void) {
	}

	vectorHandling< renderInterface * > vec3dInMap3dTo3dWithMeshedSurface::getExtRender( void ) const {
		::GModel::setCurrent(_gm.get());
		
		std::string toRender = extRenderWhat();

		//
		// give objects according to extent tags if necessary
		//
    if (toRender == "reconstructedFace") {
		  return _recFace->getRender();
		}		
    else if (toRender == "reconstructedVolume") {
		  return _recVol->getRender();
		}
    else if (toRender == "couplingBox") {
		  return _couplingBox->getRender();
		}
		else {	
		  return gmshBoundedVolume::getExtRender();
		}
	}		
	
	std::vector< std::string > vec3dInMap3dTo3dWithMeshedSurface::getMeshTags( void ) const {
		std::vector< std::string > tags = gmshBoundedVolume::getMeshTags();

		//
		// extent tags
		//		
    tags.push_back("reconstructedFace");
		tags.push_back("reconstructedVolume");
		tags.push_back("couplingBox");

		return tags;
	}	
}
