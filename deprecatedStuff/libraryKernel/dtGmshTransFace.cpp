#include "dtGmshTransFace.h"

#include <gmsh/GVertex.h>
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include <gmsh/MVertex.h>
#include "dtGmshEdge.h"
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <functionHeaven/vec3dOneD.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>

#include <logMe/logMe.h>
#include <progHelper.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_geomCurveFillConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>

namespace dtOO {
  dtGmshTransFace::dtGmshTransFace(::GModel *m, int tag, const std::list< ::GEdge * > &edges, const std::vector< int > & ori) 
    : dtGmshFace(m, tag, edges, ori) {
    vectorHandling< map1dTo3d const * > curve;
    typedef std::list< ::GEdge * >::const_iterator EIter;    
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge * ge = *ei;
      dtGmshEdge const * dge;
      dt__MUSTDOWNCAST(ge, dtGmshEdge const, dge);
      curve.push_back( dge->getMap1dTo3d()->clone() );
      ge->addFace(this);
    }
    
    blendSurf(curve);
    curve.destroy();
  }
  
  dtGmshTransFace::dtGmshTransFace(::GModel *m, int tag, const std::list< ::GEdge * > &edges) 
    : dtGmshFace(m, tag, edges) {
    vectorHandling< map1dTo3d const * > curve;
    typedef std::list< ::GEdge * >::const_iterator EIter;    
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge * ge = *ei;
      dtGmshEdge const * dge;
      dt__MUSTDOWNCAST(ge, dtGmshEdge const, dge);
      curve.push_back( dge->getMap1dTo3d()->clone() );
      ge->addFace(this);
    }  
    
    blendSurf( curve );
    curve.destroy();
  }
  
  dtGmshTransFace::~dtGmshTransFace() {
  }
  
  /**
   * @todo: Validate reparamInVolume function precision. What happens if reparametrizing
   *        is not precise enough?
   */
  void dtGmshTransFace::blendSurf( vectorHandling< map1dTo3d const * > & curve ) {
    splineCurve3d const * sC[4];
    vec3dOneDInMap3dTo3d const * v3d1dV[4];
    dt__FORALL(curve, ii,
      sC[ii] = splineCurve3d::ConstDownCast(curve[ii]);
      v3d1dV[ii] = vec3dOneDInMap3dTo3d::ConstDownCast(curve[ii]);
    );
    
    if (sC[0] && sC[1] && sC[2] && sC[3]) {
      dt__THROW(
        blendSurf(), 
        << "Necessary to create dtGmshTransFace via blending in physical space." << LOGDEL
        << "Not yet supported."
      );
    }
    else {
      DTINFOWF(
        blendSurf(), 
        << "Create dtGmshTransFace via blending in parameter space, " << LOGDEL
        << "but some edges have to be reparametrized in map3dTo3d." << LOGDEL
        << DTLOGEVAL(sC[0]) << LOGDEL
        << DTLOGEVAL(sC[1]) << LOGDEL
        << DTLOGEVAL(sC[2]) << LOGDEL 
        << DTLOGEVAL(sC[3]) << LOGDEL
        << DTLOGEVAL(v3d1dV[0]) << LOGDEL 
        << DTLOGEVAL(v3d1dV[1]) << LOGDEL
        << DTLOGEVAL(v3d1dV[2]) << LOGDEL
        << DTLOGEVAL(v3d1dV[3])
      );
      
      //
      // respline splineCurve3d in parameter space of map3dTo3d
      //
      map3dTo3d const * m3d;
      for (int ii=0; ii<4; ii++ ) {
        if (v3d1dV[ii]) {
          m3d = v3d1dV[ii]->refToMap3dTo3d();
          break;
        }        
      }
			std::list<::GVertex*> verts = this->vertices();
			std::list<::GVertex*>::iterator theIter;
			std::vector<dtPoint3> vertUVW;
      for ( theIter=verts.begin(); theIter != verts.end(); theIter++ ) {
				vertUVW.push_back( 
					m3d->reparamInVolume(
						dtPoint3( (*theIter)->x(), (*theIter)->y(), (*theIter)->z() )
					)
				);
      }
			ptrHandling< dtCurve > dtC;
			ptrHandling< vec3dOneD > v3d1d;
			//0				
			dtC.reset( 
			  trimmedCurve_twoPointsConnectConstructOCC(vertUVW[0], vertUVW[1]).result() 
			);
			v3d1d.reset( new vec3dCurveOneD(dtC.get()) );
			curve[0] = new vec3dOneDInMap3dTo3d(v3d1d.get(), m3d);
			v3d1dV[0] 
			= 
			vec3dOneDInMap3dTo3d::ConstSecureCast(curve[0]);
			//1
			dtC.reset( 
			  trimmedCurve_twoPointsConnectConstructOCC(vertUVW[1], vertUVW[2]).result() 
			);
			v3d1d.reset( new vec3dCurveOneD(dtC.get()) );
			curve[1] = new vec3dOneDInMap3dTo3d(v3d1d.get(), m3d);
			v3d1dV[1] 
			= 
			vec3dOneDInMap3dTo3d::ConstSecureCast(curve[1]);
			//2				
			dtC.reset( 
			  trimmedCurve_twoPointsConnectConstructOCC(vertUVW[2], vertUVW[3]).result() 
			);
			v3d1d.reset( new vec3dCurveOneD(dtC.get()) );
			curve[2] = new vec3dOneDInMap3dTo3d(v3d1d.get(), m3d);
			v3d1dV[2] 
			= 
			vec3dOneDInMap3dTo3d::ConstSecureCast(curve[2]);
			//3				
			dtC.reset( 
			  trimmedCurve_twoPointsConnectConstructOCC(vertUVW[3], vertUVW[0]).result() 
			);
			v3d1d.reset( new vec3dCurveOneD(dtC.get()) );
			curve[3] = new vec3dOneDInMap3dTo3d(v3d1d.get(), m3d);
			v3d1dV[3] 
			= 
			vec3dOneDInMap3dTo3d::ConstSecureCast(curve[3]);
      
      
      vectorHandling< dtCurve const * > tmp(4);
//			splineCurve3d const * s3d;
      for (int ii=0;ii<4;ii++) {
//				dt__PTRASS(
//					s3d, 
//					splineCurve3d::ConstDownCast(m1dInm3d[ii]->getConstPtrToMap1dTo3d())
//				);
				tmp[ii] 
				= 
				vec3dCurveOneD::ConstDownCast(v3d1dV[ii]->refToVec3dOneD())->ptrDtCurve();
			}
			ptrHandling<dtSurface> dtS(
			  geomSurface_geomCurveFillConstructOCC(tmp).result()
			);

      ptrHandling< vec3dTwoD > v3d2d( 
			  new vec3dSurfaceTwoD(dtS.get()) 
			);
      v3d2d->setLabel( "GFace::tag()_"+stringPrimitive().intToString(::GFace::tag()) );
			
      ptrHandling< vec3dTwoDInMap3dTo3d > surf( 
        new vec3dTwoDInMap3dTo3d( v3d2d.get(), m3d ) 
      );
      setMap2dTo3d( surf.get() );
    }
  }
  
  void dtGmshTransFace::updateFace( void ) {
    DTINFOWF(updateFace(), << "Updating ...");
    
    vectorHandling< map1dTo3d const * > curve;
    std::list< ::GEdge * > edges = ::GFace::edges();
    std::list< ::GEdge * >::iterator it;
    for (it=edges.begin(); it != edges.end(); ++it) {
      ::GEdge const * ge = *it;
      dtGmshEdge const * dge;
      dt__MUSTDOWNCAST(ge, dtGmshEdge const, dge);
      curve.push_back( dge->getMap1dTo3d()->clone() );
    }
    
    blendSurf(curve);
    curve.destroy();    
  }
  
  void dtGmshTransFace::makeSuitable( void ) {
    DTINFOWF(
      makeSuitable(), 
      << "Create dtGmshTransFace via blending in parameter space, " << LOGDEL
      << "based on meshed bounding edges."
    );

    //
    // respline splineCurve3d in parameter space of map3dTo3d
    //
    map3dTo3d const * m3d = NULL;
    std::list< ::GEdge * > ee = this->edges();
    std::list< ::GEdge * >::iterator e_it;// = ee.begin();
    for (e_it = ee.begin(); e_it != ee.end(); ++e_it) {
      dtGmshEdge * dtge;
      dt__CANDOWNCAST(*e_it, dtGmshEdge, dtge);
      vec3dOneDInMap3dTo3d const * v1dIn3d 
			= 
			vec3dOneDInMap3dTo3d::ConstDownCast(dtge->getMap1dTo3d());
      if (!m3d && v1dIn3d) {        
        m3d = v1dIn3d->refToMap3dTo3d();
        break;
      }
    }

    if (!m3d) {
      dt__THROW(makeSuitable(), 
        << DTLOGEVAL(m3d) << LOGDEL 
        << "No mapping map3dTo3d.");
    }
		
    int counter = 0;
    vectorHandling< dtCurve const * > tmp(4);    
    for (e_it = ee.begin(); e_it != ee.end(); ++e_it) {      
      std::vector<dtPoint3> pp;
      ::GVertex * gv = (*e_it)->getBeginVertex();
      pp.push_back( m3d->reparamInVolume(dtPoint3(gv->x(), gv->y(), gv->z())) );
      for (int ii=0;ii<(*e_it)->getNumMeshVertices();ii++) {
        ::MVertex * mv = (*e_it)->getMeshVertex(ii);
        pp.push_back( m3d->reparamInVolume(dtPoint3(mv->x(), mv->y(), mv->z())) );
      }        
      gv = (*e_it)->getEndVertex();
      pp.push_back( m3d->reparamInVolume(dtPoint3(gv->x(), gv->y(), gv->z())) );
			tmp[counter] = bSplineCurve_pointConstructOCC(pp, 1).result();
      counter++;
    }
//
		ptrHandling< dtSurface > dtS(
		  bSplineSurface_bSplineCurveFillConstructOCC(tmp).result()
		);
    tmp.destroy();
    ptrHandling< vec3dTwoD > v3d2d( new vec3dSurfaceTwoD(dtS.get()) );
    v3d2d->setLabel( "GFace::tag()_"+stringPrimitive().intToString(::GFace::tag()) );

    ptrHandling< vec3dTwoDInMap3dTo3d > surf( 
      new vec3dTwoDInMap3dTo3d( v3d2d.get(), m3d ) 
    );
    setMap2dTo3d( surf.get() );
  }
  
}
