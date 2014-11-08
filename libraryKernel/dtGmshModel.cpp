#include "dtGmshModel.h"

#include <logMe/logMe.h>

#include <gmsh/GEdge.h>
#include <gmsh/GFace.h>
#include <gmsh/GVertex.h>
#include <gmsh/GRegion.h>

#include "dtGmshRegion.h"
#include "dtGmshRegionHex.h"
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include "dtGmshVertex.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <interfaceHeaven/ptrHandling.h>

#include <gmsh/meshGEdge.h>
#include <gmsh/meshGFace.h>
#include <gmsh/meshGRegion.h>

#define __caCThis \
  const_cast< dtGmshModel * >(this)

namespace dtOO {
  dtGmshModel::dtGmshModel(std::string name) : GModel(name){
    _bV = NULL;
  }

  dtGmshModel::~dtGmshModel() {
  }
  
  void dtGmshModel::attachToBoundedVolume( boundedVolume const * const bV) {
    _bV = bV;
  }
  
  boundedVolume const * dtGmshModel::getAttachedBoundedVolume( void ) {
    return _bV;
  }
  
  void dtGmshModel::addVertexToGmshModel( dtPoint3 const & vertex, int const tag ) {
    GModel::add( new dtGmshVertex(this, tag) );

    dtGmshVertex * gV;
    dt__MUSTDOWNCAST( GModel::getVertexByTag(tag), dtGmshVertex, gV );
    gV->setPosition(vertex); 
  }
	
  void dtGmshModel::addIfVertexToGmshModel( dtPoint3 const & vertex, int * const tag ) {
		*tag = this->getNumVertices()+1;
    dtGmshVertex * gv = new dtGmshVertex(this, *tag);
		gv->setPosition(vertex); 
		
		int tTag = alreadyInModel(gv);
		
		if (tTag) {
			delete gv;
			*tag = tTag;
		}
		else {
			this->add(gv);
		}
  }	

  void dtGmshModel::addVertexToGmshModel( dtPoint3 const * const vertex, int const tag ) {
    addVertexToGmshModel(*vertex, tag);
  }  
  
  void dtGmshModel::addEdgeToGmshModel( 
    map1dTo3d const * const edge, 
    int const tag, 
    int const from, 
    int const to 
  ) {
      GModel::add( 
        new dtGmshEdge(
          this, tag, GModel::getVertexByTag(from), GModel::getVertexByTag(to)
        )
      );    

      dtGmshEdge * gE;
      dt__MUSTDOWNCAST(GModel::getEdgeByTag(tag), dtGmshEdge, gE);
      
      gE->setMap1dTo3d( edge );
  }  

  void dtGmshModel::addIfEdgeToGmshModel( 
    map1dTo3d const * const edge, 
    int * const tag, 
    int const from, 
    int const to 
  ) {
		*tag = this->getNumEdges()+1;
		dtGmshEdge * ge = new dtGmshEdge(
			this, *tag, GModel::getVertexByTag(from), GModel::getVertexByTag(to)
		);
		ge->setMap1dTo3d( edge );
		
		int tTag = alreadyInModel(ge);
		if (tTag) {
			delete ge;
			*tag = tTag;
			if ( GModel::getEdgeByTag(*tag)->getBeginVertex()->tag() != from ) {
				*tag = -tTag;
			}
		}
		else {
			this->add(ge);
		}
  }  	
	
//	void dtGmshModel::addRegionToGmshModel( 
//	  map3dTo3d const * const vol, int const tag, int const vS, int const fS
//	) {
//		this->addVertexToGmshModel( vol->getPoint( 0, 0, 0 ), vS );
//		this->addVertexToGmshModel( vol->getPoint( 1, 0, 0 ), vS+1 );		
//		this->addVertexToGmshModel( vol->getPoint( 1, 1, 0 ), vS+2 );
//		this->addVertexToGmshModel( vol->getPoint( 0, 1, 0 ), vS+3 );		
//		this->addVertexToGmshModel( vol->getPoint( 0, 0, 1 ), vS+4 );
//		this->addVertexToGmshModel( vol->getPoint( 1, 0, 1 ), vS+5 );		
//		this->addVertexToGmshModel( vol->getPoint( 1, 1, 1 ), vS+6 );
//		this->addVertexToGmshModel( vol->getPoint( 0, 1, 1 ), vS+7 );
		
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(0,0,0), dtPoint3(1,0,0) ), fS, vS, vS+1
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(1,0,0), dtPoint3(1,1,0) ), fS+1, vS+1, vS+2
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(1,1,0), dtPoint3(0,1,0) ), fS+2, vS+2, vS+3
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(0,1,0), dtPoint3(0,0,0) ), fS+3, vS+3, vS
//		);
//		
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(0,0,1), dtPoint3(1,0,1) ), fS+4, vS+4, vS+5
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(1,0,1), dtPoint3(1,1,1) ), fS+5, vS+5, vS+6
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(1,1,1), dtPoint3(0,1,1) ), fS+6, vS+6, vS+7
//		);
//		this->addEdgeToGmshModel(
//		  vol->segmentPercent( dtPoint3(0,1,1), dtPoint3(0,0,1) ), fS+7, vS+7, vS+4
//		);		
//	}
  
	dtGmshRegion * dtGmshModel::addRegionToGmshModel( map3dTo3d const * const vol ) {
	  std::vector< int > vId(8,0);// = GModel::getNumVertices()+1;
		std::vector< int > eId(12, 0);// = GModel::getNumEdges()+1;
//		int fId = GModel::getNumFaces()+1;
		int rId = GModel::getNumRegions()+1;
		
    dtPoint3 p0(0. ,0. ,0.);
		dtPoint3 p1(1. ,0. ,0.);
		dtPoint3 p2(1. ,1. ,0.);
		dtPoint3 p3(0. ,1. ,0.);
		dtPoint3 p4(0. ,0. ,1.);
		dtPoint3 p5(1. ,0. ,1.);
		dtPoint3 p6(1. ,1. ,1.);
		dtPoint3 p7(0. ,1. ,1.);
		
		this->addIfVertexToGmshModel( vol->getPointPercent(p0), &(vId[0]) );
		this->addIfVertexToGmshModel( vol->getPointPercent(p1), &(vId[1]) );		
		this->addIfVertexToGmshModel( vol->getPointPercent(p2), &(vId[2]) );
		this->addIfVertexToGmshModel( vol->getPointPercent(p3), &(vId[3]) );		
		this->addIfVertexToGmshModel( vol->getPointPercent(p4), &(vId[4]) );
		this->addIfVertexToGmshModel( vol->getPointPercent(p5), &(vId[5]) );		
		this->addIfVertexToGmshModel( vol->getPointPercent(p6), &(vId[6]) );
		this->addIfVertexToGmshModel( vol->getPointPercent(p7), &(vId[7]) );	
		
		this->addIfEdgeToGmshModel(vol->segmentPercent(p0, p1), &(eId[0]), vId[0], vId[1]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p1, p2), &(eId[1]), vId[1], vId[2]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p2, p3), &(eId[2]), vId[2], vId[3]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p3, p0), &(eId[3]), vId[3], vId[0]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p4, p5), &(eId[4]), vId[4], vId[5]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p5, p6), &(eId[5]), vId[5], vId[6]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p6, p7), &(eId[6]), vId[6], vId[7]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p7, p4), &(eId[7]), vId[7], vId[4]);		
		this->addIfEdgeToGmshModel(vol->segmentPercent(p0, p4), &(eId[8]), vId[0], vId[4]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p1, p5), &(eId[9]), vId[1], vId[5]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p2, p6), &(eId[10]), vId[2], vId[6]);
		this->addIfEdgeToGmshModel(vol->segmentPercent(p3, p7), &(eId[11]), vId[3], vId[7]);
		
		std::list<GEdge*> ge;
		std::vector<int> fori;
		std::vector<int> eori(4);
	  std::list< dtGmshFace * > gf;

		int fId = GModel::getNumFaces()+1;		
		ge.push_back( this->getEdgeByTag(abs(eId[0])));
		ge.push_back( this->getEdgeByTag(abs(eId[1])));
		ge.push_back( this->getEdgeByTag(abs(eId[2])));
		ge.push_back( this->getEdgeByTag(abs(eId[3])));
		eori[0] = ( eId[0] < 0 ? -1 : 1);
		eori[1] = ( eId[1] < 0 ? -1 : 1);
		eori[2] = ( eId[2] < 0 ? -1 : 1);
		eori[3] = ( eId[3] < 0 ? -1 : 1);
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {		
			this->add( gf.back() );
			gf.back()->setMap2dTo3d(
				vol->segmentPercent(p0, p1, p2, p3)
			);
		}
    ge.clear();		
//		fori.clear();
//		eori.clear();
		fId = GModel::getNumFaces()+1;
		ge.push_back( this->getEdgeByTag(abs(eId[4])));
		ge.push_back( this->getEdgeByTag(abs(eId[5])));
		ge.push_back( this->getEdgeByTag(abs(eId[6])));
		ge.push_back( this->getEdgeByTag(abs(eId[7])));
		eori[0] = ( eId[4] < 0 ? -1 : 1);
		eori[1] = ( eId[5] < 0 ? -1 : 1);
		eori[2] = ( eId[6] < 0 ? -1 : 1);
		eori[3] = ( eId[7] < 0 ? -1 : 1);		
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {		
			this->add( gf.back() );
			gf.back()->setMap2dTo3d( 
				vol->segmentPercent(p4, p5, p6, p7)
			);		
		}
    ge.clear();		
//		fori.clear();
//		eori.clear();
		fId = GModel::getNumFaces()+1;
		ge.push_back( this->getEdgeByTag(abs(eId[0])));
		ge.push_back( this->getEdgeByTag(abs(eId[9])));
		ge.push_back( this->getEdgeByTag(abs(eId[4])));
		ge.push_back( this->getEdgeByTag(abs(eId[8])));
//		eori.push_back(1);
//		eori.push_back(1);
//		eori.push_back(-1);
//		eori.push_back(-1);
		eori[0] = ( eId[0] < 0 ? -1 : 1);
		eori[1] = ( eId[9] < 0 ? -1 : 1);
		eori[2] = ( eId[4] > 0 ? -1 : 1);
		eori[3] = ( eId[8] > 0 ? -1 : 1);				
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {		
			this->add( gf.back() );
			gf.back()->setMap2dTo3d( 
				vol->segmentPercent(p0, p1, p5, p4)
			);		
		}
    ge.clear();	
//		fori.clear();
//		eori.clear();
		fId = GModel::getNumFaces()+1;
		ge.push_back( this->getEdgeByTag(abs(eId[2])));
		ge.push_back( this->getEdgeByTag(abs(eId[10])));
		ge.push_back( this->getEdgeByTag(abs(eId[6])));
		ge.push_back( this->getEdgeByTag(abs(eId[11])));
//		eori.push_back(-1);
//		eori.push_back(1);
//		eori.push_back(1);
//		eori.push_back(-1);		
		eori[0] = ( eId[2] > 0 ? -1 : 1);
		eori[1] = ( eId[10] < 0 ? -1 : 1);
		eori[2] = ( eId[6] < 0 ? -1 : 1);
		eori[3] = ( eId[11] > 0 ? -1 : 1);				
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {		
			this->add( gf.back() );
			gf.back()->setMap2dTo3d( 
				vol->segmentPercent(p3, p2, p6, p7)
			);		
		}
    ge.clear();	
//		fori.clear();
//		eori.clear();
		fId = GModel::getNumFaces()+1;
		ge.push_back( this->getEdgeByTag(abs(eId[3])));
		ge.push_back( this->getEdgeByTag(abs(eId[8])));
		ge.push_back( this->getEdgeByTag(abs(eId[7])));
		ge.push_back( this->getEdgeByTag(abs(eId[11])));
//		eori.push_back(1);
//		eori.push_back(1);
//		eori.push_back(-1);
//		eori.push_back(-1);	
		eori[0] = ( eId[3] < 0 ? -1 : 1);
		eori[1] = ( eId[8] < 0 ? -1 : 1);
		eori[2] = ( eId[7] > 0 ? -1 : 1);
		eori[3] = ( eId[11] > 0 ? -1 : 1);			
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {
			this->add( gf.back() );
			gf.back()->setMap2dTo3d( 
				vol->segmentPercent(p3, p0, p4, p7)
			);		
		}
    ge.clear();	
//		fori.clear();
//		eori.clear();
		fId = GModel::getNumFaces()+1;
		ge.push_back( this->getEdgeByTag(abs(eId[1])));
		ge.push_back( this->getEdgeByTag(abs(eId[9])));
		ge.push_back( this->getEdgeByTag(abs(eId[5])));
		ge.push_back( this->getEdgeByTag(abs(eId[10])));
//		eori.push_back(-1);
//		eori.push_back(1);
//		eori.push_back(1);
//		eori.push_back(-1);	
		eori[0] = ( eId[1] > 0 ? -1 : 1);
		eori[1] = ( eId[9] < 0 ? -1 : 1);
		eori[2] = ( eId[5] < 0 ? -1 : 1);
		eori[3] = ( eId[10] > 0 ? -1 : 1);					
		gf.push_back( new dtGmshFace(this, fId, ge, eori) );
		fori.push_back(1);
		if ( alreadyInModel(gf.back()) ) {
		  gf.back() = getDtGmshFaceByTag(alreadyInModel(gf.back()));
			fori.back() = -1;
		}
		else {
			this->add( gf.back() );
			gf.back()->setMap2dTo3d( 
				vol->segmentPercent(p2, p1, p5, p6)
			);	
		}
		
		this->add( new dtGmshRegion(this, rId, gf, fori) );
	}
	
  dtGmshRegion * dtGmshModel::getDtGmshRegionByTag( int const tag ) const {
    GRegion * region = GModel::getRegionByTag(tag);
    dtGmshRegion * gRegion;
    dt__MUSTDOWNCASTWM(region, dtGmshRegion, gRegion, 
            << "Cannot convert region " << DTLOGEVAL(tag) << "." << LOGDEL
            << DTLOGEVAL(region)
    );
    
    return gRegion;    
  }
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByTag( int const tag ) const {
    GFace * face = GModel::getFaceByTag(tag);
    dtGmshFace * gFace;
    dt__MUSTDOWNCASTWM(face, dtGmshFace, gFace, 
            << "Cannot convert face " << DTLOGEVAL(tag) << "." << LOGDEL
            << DTLOGEVAL(face)
    );
    
    return gFace;    
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByTag( int const tag ) const {
    GEdge * edge = GModel::getEdgeByTag(tag);
    dtGmshEdge * gEdge;
    dt__MUSTDOWNCASTWM(edge, dtGmshEdge, gEdge,
            << "Cannot convert edge " << DTLOGEVAL(tag) << "." << LOGDEL
            << DTLOGEVAL(edge)
    );
    
    return gEdge;    
  }

  int dtGmshModel::getDtGmshEdgeTagByFromTo( int const from, int const to ) const {
    dtGmshVertex * gv = getDtGmshVertexByTag(from);
    
    std::list<GEdge*> edges = gv->edges();
    std::list<GEdge*>::iterator it = edges.begin();
    for (it; it != edges.end(); ++it) {
      if ( (*it)->getBeginVertex()->tag() == to ) {
        return (*it)->tag();
      }
      else if ( (*it)->getEndVertex()->tag() == to ) {
        return -(*it)->tag();
      }
    }
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByFromTo( int const from, int const to ) const {
    return getDtGmshEdgeByTag( abs(getDtGmshEdgeTagByFromTo(from, to)) );
  }  

  dtGmshVertex * dtGmshModel::getDtGmshVertexByTag( int const tag ) const {
    GVertex * vertex = GModel::getVertexByTag(tag);
    dtGmshVertex * gVertex;
    dt__MUSTDOWNCASTWM(vertex, dtGmshVertex, gVertex,
            << "Cannot convert vertex " << DTLOGEVAL(tag) << "." << LOGDEL
            << DTLOGEVAL(vertex)            
    );
    
    return gVertex;    
  }  

  dtGmshRegion * dtGmshModel::cast2DtGmshRegion( GRegion * gr ) const{
    dtGmshRegion * ret;
    dt__MUSTDOWNCAST(gr, dtGmshRegion, ret);
    
    return ret;
  }
  
  dtGmshFace * dtGmshModel::cast2DtGmshFace( GFace * gf ) const{
    dtGmshFace * ret;
    dt__MUSTDOWNCAST(gf, dtGmshFace, ret);
    
    return ret;
  }
  
  dtGmshEdge * dtGmshModel::cast2DtGmshEdge( GEdge * ge ) const {
    dtGmshEdge * ret;
    dt__MUSTDOWNCAST(ge, dtGmshEdge, ret);
    
    return ret;    
  }
  
  dtGmshVertex * dtGmshModel::cast2DtGmshVertex( GVertex * gv ) const {
    dtGmshVertex * ret;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, ret);
    
    return ret;    
  }

  void dtGmshModel::createEdge(int const geI, int const gfI, int const v0, int const v1) {
    dtGmshFace * gf = getDtGmshFaceByTag(gfI);
    
    SPoint2 v0UV = gf->reparamOnFace( getVertexByTag(v0) );
    SPoint2 v1UV = gf->reparamOnFace( getVertexByTag(v1) );

    ptrHandling< map1dTo3d > mm(
      gf->getMap2dTo3d()->pickLinearUV(v0UV.x(), v0UV.y(), v1UV.x(), v1UV.y())
    );
    
    addEdgeToGmshModel(mm.get(), geI, v0, v1);
  }
  
  void dtGmshModel::createEmbeddedEdge(int const geI, int const gfI, int const v0, int const v1) {
    createEdge(geI, gfI, v0, v1);
    getDtGmshFaceByTag(gfI)->addEmbeddedEdge( getEdgeByTag(geI) );
  }
  
  dtPoint3 dtGmshModel::cast2DtPoint3( GVertex * gv ) const {
    return cast2DtGmshVertex(gv)->cast2DtPoint3();
  }

  std::list< dtGmshEdge * > dtGmshModel::parallelEdges( dtGmshEdge const * const ge ) const {
    dtGmshRegionHex * dtgrh = NULL;
    std::list<GFace*> facesContain;
//    std::list<dtGmshRegion*>::iterator dtgr_it;
    for (riter r_it = __caCThis->firstRegion(); r_it != __caCThis->lastRegion(); ++r_it) {
      std::list<GFace*> gf = (*r_it)->faces();
      std::list<GFace*>::iterator f_it;
      for (f_it = gf.begin(); f_it != gf.end(); ++f_it) {
        if ( (*f_it)->containsEdge(ge->tag()) ) {
          dt__MUSTDOWNCASTWM(
            getDtGmshRegionByTag( (*r_it)->tag() ),
            dtGmshRegionHex,
            dtgrh,
            << "parallelEdges() only supported in purely dtGmshRegionHex meshes."
          );
//          break;
          facesContain.push_back(*f_it);
        }
      }
      if (dtgrh) break;
    }
    
//    DTBUFFERINIT();
//    dt__FORALL(dtgr, ii,
//      DTBUFFER( << DTLOGEVAL( dtgr[ii]->tag() ) << LOGDEL);
//    );
//    DTINFOWF_BUFFER(parallelEdges());
  }
  
  void dtGmshModel::meshEdgeTransfiniteFromTo(
    int const from, int const to, 
    int const type, float const coeff, 
    int const nEl) {
    
    int eTag = getDtGmshEdgeTagByFromTo(from, to);
    dtGmshEdge * dtge = getDtGmshEdgeByTag( abs(eTag) );
    
    if (eTag > 0) {
      dtge->meshTransfiniteWNElements(type, coeff, nEl);
    }
    else {
      dtge->meshTransfiniteWNElements(type, 1./coeff, nEl);
    }
  }
  void dtGmshModel::meshVertex( int const tag ) {
    if (tag == 0) {
      this->mesh(0);
    }
    else {
      DTINFOWF(
        meshVertex(), 
        << DTLOGEVAL(tag) << LOGDEL
        << "Should be 0.");
    }
  }
  
  void dtGmshModel::meshEdge( int const tag ) {
    dtGmshEdge * dtge = getDtGmshEdgeByTag(tag);   
    meshGEdge me;
    if ( dtge->meshStatistics.status == GEntity::PENDING ) me(dtge);
  }
  
  void dtGmshModel::meshFace( int const tag ) {
    dtGmshFace * dtgf = getDtGmshFaceByTag(tag);
    
    std::list<GEdge*> ee = dtgf->edges();
    std::list<GEdge*>::iterator e_it = ee.begin();
    for ( e_it; e_it != ee.end(); ++e_it) {
      meshEdge( (*e_it)->tag() );
    }
    
    meshGFace mf;
    if ( dtgf->meshStatistics.status == GEntity::PENDING ) {
      dtgf->makeSuitable();
      mf(dtgf);
    }
  }
  
  void dtGmshModel::meshRegion( int const tag ) {
    dtGmshRegion * dtgr = getDtGmshRegionByTag(tag);
    
    std::list<GFace*> ff = dtgr->faces();
    std::list<GFace*>::iterator f_it = ff.begin();
    for ( f_it; f_it != ff.end(); ++f_it) {
      meshFace( (*f_it)->tag() );
    }
    
    std::vector<GRegion*> delauny;
    meshGRegion mr( delauny );
    mr(dtgr);    
  }

  int dtGmshModel::alreadyInModel( GVertex const * const gv ) const {
		for (GModel::viter vIt = __caCThis->firstVertex(); vIt != __caCThis->lastVertex(); ++vIt) {
			if ( dtGmshVertex::isEqual(gv, *vIt) ) {
				DTINFOWF(
					update(),
					<< "duplicate vertex = " << gv->tag() << " equal to vertex tag = " << (*vIt)->tag()
				);				
				return (*vIt)->tag();
			}
		}
		return 0;
	}

  int dtGmshModel::alreadyInModel( GEdge const * const ge ) const {
		for (GModel::eiter eIt = __caCThis->firstEdge(); eIt != __caCThis->lastEdge(); ++eIt) {
			if ( dtGmshEdge::isEqual(ge, *eIt) ) {
				DTINFOWF(
					update(),
					<< "duplicate edge = " << ge->tag() << " equal to edge tag = " << (*eIt)->tag()
				);				
				return (*eIt)->tag();
			}
		}
		return 0;
	}
	
  int dtGmshModel::alreadyInModel( GFace const * const gf ) const {
		for (GModel::fiter fIt = __caCThis->firstFace(); fIt != __caCThis->lastFace(); ++fIt) {
			if ( dtGmshFace::isEqual(gf, *fIt) ) {
				DTINFOWF(
					update(),
					<< "duplicate face = " << gf->tag() << " equal to face tag = " << (*fIt)->tag()
				);				
				return (*fIt)->tag();
			}
		}
		return 0;
	}	
}
