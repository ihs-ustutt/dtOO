#include "dtGmshModel.h"

#include <logMe/logMe.h>
#include <progHelper.h>

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
#include <unstructured3dMesh.h>
#include <unstructured3dSurfaceMesh.h>
#include <gmsh/meshGEdge.h>
#include <gmsh/meshGFace.h>
#include <gmsh/meshGRegion.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <cgnslib.h>

#define __caCThis \
  const_cast< dtGmshModel * >(this)

#define __cgnsCheck(cmd) \
  if (cmd) dt__THROW(__cgnsCheck(), << DTLOGEVAL(cg_get_error()))

namespace dtOO {
  dtGmshModel::dtGmshModel(std::string name) : GModel(name){
  }

  dtGmshModel::~dtGmshModel() {
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

  void dtGmshModel::addIfFaceToGmshModel( 
    map2dTo3d const * const face, int * const tag,
		std::list< GEdge * > const & edges, std::vector< int > const & ori
  ) {
		*tag = this->getNumFaces()+1;	
		
		dtGmshFace * gf = new dtGmshFace(this, *tag, edges, ori);
		gf->setMap2dTo3d(face);
		
		int tTag = alreadyInModel(gf);
		if (tTag) {
			delete gf;
			*tag = tTag;
		}
		else {
			this->add(gf);
		}
  }	
	 
	/**
   * @todo What if region is not 6-sided?
   */
	dtGmshRegion * dtGmshModel::addRegionToGmshModel( map3dTo3d const * const vol ) {
	  std::vector< int > vId(8,0);// = GModel::getNumVertices()+1;
		std::vector< int > eId(12, 0);// = GModel::getNumEdges()+1;
//		int fId = GModel::getNumFaces()+1;
		int rId = GModel::getNumRegions()+1;
		
    dtPoint3 p0(0., 0., 0.);
		dtPoint3 p1(1., 0., 0.);
		dtPoint3 p2(1., 1., 0.);
		dtPoint3 p3(0., 1., 0.);
		dtPoint3 p4(0., 0., 1.);
		dtPoint3 p5(1., 0., 1.);
		dtPoint3 p6(1., 1., 1.);
		dtPoint3 p7(0., 1., 1.);
		
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
			gf.back()->setMap2dTo3d(vol->segmentPercent(p2, p1, p5, p6));	
		}
				
		dtGmshRegion * gr = new dtGmshRegionHex(this, rId, eId, gf, fori);
		this->add(gr);		
		
		return gr;
	}
	
  dtGmshRegion * dtGmshModel::getDtGmshRegionByTag( int const tag ) const {
    GRegion * region = GModel::getRegionByTag(tag);
    dt__PTRASS(dtGmshRegion * gRegion, dtGmshRegion::DownCast(region));
    
    return gRegion;    
  }
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByTag( int const tag ) const {
    GFace * face = GModel::getFaceByTag(tag);
    dt__PTRASS(dtGmshFace * gFace, dtGmshFace::DownCast(face));
    
    return gFace;    
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByTag( int const tag ) const {
    GEdge * edge = GModel::getEdgeByTag(tag);
    dt__PTRASS(dtGmshEdge * gEdge, dtGmshEdge::DownCast(edge));
    
    return gEdge;    
  }

  int dtGmshModel::getDtGmshEdgeTagByFromTo( int const from, int const to ) const {
    dtGmshVertex * gv = getDtGmshVertexByTag(from);
    
    std::list<GEdge*> edges = gv->edges();
    std::list<GEdge*>::iterator it = edges.begin();
    for (it; it != edges.end(); ++it) {
      if ( (*it)->getBeginVertex()->tag() == to ) {
        return -((*it)->tag());
      }
      else if ( (*it)->getEndVertex()->tag() == to ) {
        return (*it)->tag();
      }
    }
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByFromTo( int const from, int const to ) const {
    return getDtGmshEdgeByTag( abs(getDtGmshEdgeTagByFromTo(from, to)) );
  }  

  dtGmshVertex * dtGmshModel::getDtGmshVertexByTag( int const tag ) const {
    GVertex * vertex = GModel::getVertexByTag(tag);
   
    dt__PTRASS( dtGmshVertex * gVertex, dtGmshVertex::DownCast(vertex) );
    
    return gVertex;    
  }  

  dtGmshRegion * dtGmshModel::cast2DtGmshRegion( GEntity * gr ){
    dtGmshRegion * ret;
    dt__MUSTDOWNCAST(gr, dtGmshRegion, ret);
    
    return ret;
  }
  
  dtGmshFace * dtGmshModel::cast2DtGmshFace( GEntity * gf ){
    dtGmshFace * ret;
    dt__MUSTDOWNCAST(gf, dtGmshFace, ret);
    
    return ret;
  }
  
  dtGmshEdge * dtGmshModel::cast2DtGmshEdge( GEntity * ge ) {
    dtGmshEdge * ret;
    dt__MUSTDOWNCAST(ge, dtGmshEdge, ret);
    
    return ret;    
  }
  
  dtGmshVertex * dtGmshModel::cast2DtGmshVertex( GEntity * gv ) {
    dtGmshVertex * ret;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, ret);
    
    return ret;    
  }

  dtPoint3 dtGmshModel::cast2DtPoint3( GVertex * gv ) {
    return cast2DtGmshVertex(gv)->cast2DtPoint3();
  }

  dtPoint3 dtGmshModel::cast2DtPoint3( MVertex * mv ) {
    return dtPoint3(mv->x(), mv->y(), mv->z());
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
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( void ) const {
		//
		// set current model
		// 
		DTINFOWF( toUnstructured3dMesh(), << DTLOGEVAL(GModel::setCurrent(__caCThis)) );
			
		//
		// get all entities
		//
		std::vector< GEntity * > ent;
		this->getEntities(ent);
		vectorHandling< dtPoint3 > pp(__caCThis->getNumMeshVertices());		
		std::map< int, int > vLoc_num;
		
		int counter = 0;
		for( int ii=0; ii<ent.size(); ii++ ) {
      for( int jj=0; jj<ent[ii]->getNumMeshVertices(); jj++ ) {
				MVertex const * const mv = ent[ii]->getMeshVertex(jj);				
				pp[counter]
				=
				dtPoint3(
					static_cast< float >(mv->x()), 
					static_cast< float >(mv->y()), 
					static_cast< float >(mv->z())
				); 				
				vLoc_num[mv->getNum()] = counter;
				counter++;
			}
		}	
    
		//
		// create unstructured mesh and set points
		//
		unstructured3dMesh * um = new unstructured3dMesh();		
		um->addPoints(pp);
		
		for( int ii=0; ii<ent.size(); ii++ ) {
			GVertex * gv = dynamic_cast< GVertex * >(ent[ii]);
			GEdge * ge = dynamic_cast< GEdge * >(ent[ii]);
			GFace * gf = dynamic_cast< GFace * >(ent[ii]);
			GRegion * gr = dynamic_cast< GRegion * >(ent[ii]);

      if (gv) {
				unsigned nEl = gv->getNumMeshElements();
				int nElTot = nEl;
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GVertex>:" << LOGDEL
					<< "points = " << nElTot << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}		
      else if (ge) {
				unsigned nEl = ge->getNumMeshElements();
				int nElTot = nEl;
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GEdge>:" << LOGDEL
					<< "lines = " << nElTot << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}					
      else if (gf) {
				std::vector< unsigned > nEl(3,0);
				gf->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2];
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GFace>:" << LOGDEL
					<< "triangles = " << nEl[0] << LOGDEL
					<< "quadrangles = " << nEl[1] << LOGDEL
					<< "polygons = " << nEl[2] << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}			
			else if (gr) {
				std::vector< unsigned > nEl(5,0);
				ent[ii]->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2] + nEl[3] + nEl[4];
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GRegion>:" << LOGDEL
					<< "tetrahedra = " << nEl[0] << LOGDEL
					<< "hexahedra = " << nEl[1] << LOGDEL
					<< "prisms = " << nEl[2] << LOGDEL
					<< "pyramids = " << nEl[3] << LOGDEL
					<< "polyhedra = " << nEl[4] << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}
		}
		//
		// write only 3d elements
		//
    for (
			GModel::riter r_it = __caCThis->firstRegion(); 
			r_it != __caCThis->lastRegion(); 
			++r_it
		) {
      GRegion * gr = *r_it;
			unsigned nElemTot = gr->getNumMeshElements();
      for( unsigned ii=0; ii<nElemTot; ii++ ) {
        MElement * me = gr->getMeshElement(ii);
        MTetrahedron * mtet = dynamic_cast< MTetrahedron * >(me);
        MHexahedron * mhex = dynamic_cast< MHexahedron * >(me);
        //
        // tetrahedron
        //
        if ( mtet ) {
				  vectorHandling< int > vertsIndex(4);					
          std::vector< MVertex * > verts;
          mtet->getVertices(verts);        
          vertsIndex[0] = vLoc_num[verts[0]->getNum()];
          vertsIndex[1] = vLoc_num[verts[1]->getNum()];
          vertsIndex[2] = vLoc_num[verts[2]->getNum()];
          vertsIndex[3] = vLoc_num[verts[3]->getNum()];
					um->addElement(vertsIndex);
        }     
        //
        // hexahedron
        //
        else if ( mhex ) {
					vectorHandling< int > vertsIndex(8);
          std::vector< MVertex * > verts;
					mhex->getVertices(verts); 
          vertsIndex[0] = vLoc_num[verts[4]->getNum()];
          vertsIndex[1] = vLoc_num[verts[5]->getNum()];
          vertsIndex[2] = vLoc_num[verts[1]->getNum()];
          vertsIndex[3] = vLoc_num[verts[0]->getNum()];
          vertsIndex[4] = vLoc_num[verts[7]->getNum()];
          vertsIndex[5] = vLoc_num[verts[6]->getNum()];
          vertsIndex[6] = vLoc_num[verts[2]->getNum()];
          vertsIndex[7] = vLoc_num[verts[3]->getNum()];          
					um->addElement(vertsIndex);
        }  
      }
    }
		
		return um;
	}
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( 
	  std::vector< MVertex * > const & vertices, std::vector< MElement * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, int > vLoc_num;
		for( int ii=0; ii<vertices.size(); ii++ ) {
			MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< float >(mv->x()), 
				static_cast< float >(mv->y()), 
				static_cast< float >(mv->z())
			); 				
			vLoc_num[mv->getNum()] = ii;
		}

		unstructured3dMesh * um = new unstructured3dMesh();		
		um->addPoints(pp);
		
		for( int ii=0; ii<elements.size(); ii++ ) {
			MElement * me = elements[ii];
			MTetrahedron * mtet = dynamic_cast< MTetrahedron * >(me);
			MHexahedron * mhex = dynamic_cast< MHexahedron * >(me);
			//
			// tetrahedron
			//
			if ( mtet ) {
				vectorHandling< int > vertsIndex(4);					
				std::vector< MVertex * > verts;
				mtet->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}     
			//
			// hexahedron
			//
			else if ( mhex ) {
				vectorHandling< int > vertsIndex(8);
				std::vector< MVertex * > verts;
				mhex->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[4]->getNum()];
				vertsIndex[1] = vLoc_num[verts[5]->getNum()];
				vertsIndex[2] = vLoc_num[verts[1]->getNum()];
				vertsIndex[3] = vLoc_num[verts[0]->getNum()];
				vertsIndex[4] = vLoc_num[verts[7]->getNum()];
				vertsIndex[5] = vLoc_num[verts[6]->getNum()];
				vertsIndex[6] = vLoc_num[verts[2]->getNum()];
				vertsIndex[7] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}  
		}		
		
		return um;
	}	
	
  unstructured3dSurfaceMesh * dtGmshModel::toUnstructured3dSurfaceMesh( 
	  std::vector< MVertex * > const & vertices, std::vector< MElement * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, int > vLoc_num;
		for( int ii=0; ii<vertices.size(); ii++ ) {
			MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< float >(mv->x()), 
				static_cast< float >(mv->y()), 
				static_cast< float >(mv->z())
			); 			
			vLoc_num[mv->getNum()] = ii;
		}

		unstructured3dSurfaceMesh * um = new unstructured3dSurfaceMesh();		
		um->addPoints(pp);
		
		for( int ii=0; ii<elements.size(); ii++ ) {
			MElement * me = elements[ii];
			MQuadrangle * mquad = dynamic_cast< MQuadrangle * >(me);
			MTriangle * mtri = dynamic_cast< MTriangle * >(me);
			
			//
			// quadrangle
			//
			if ( mquad ) {
				vectorHandling< int > vertsIndex(4);					
				std::vector< MVertex * > verts;
				mquad->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}    
			else if ( mtri ) {
				vectorHandling< int > vertsIndex(3);					
				std::vector< MVertex * > verts;
				mtri->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				um->addElement(vertsIndex);
			}     				
		}		
		
		return um;		
	}	
	
	void dtGmshModel::dtReadCGNS(const std::string &name) {		
			std::vector< dtGmshFace * >  faces;
			std::vector< dtGmshRegion * >  regions;
							
			typedef std::map<int, MVertex*> vertexMap_t;
			typedef std::map<int, MElement*> elementMap_t;		
			
			int vNum = 0;	
			vertexMap_t vertexMap;
			int minVertex = 1;
			int maxVertex = 0;		

			int eNum = 0;
			elementMap_t elementMap;			
			int minElement = 1;
			int maxElement = 0;
				
			int fNum = 0;
			int rNum = 0;

			//
			// open cgns file
			//
			int index_file;
			__cgnsCheck(cg_open(name.c_str(), CG_MODE_READ, &index_file));

			//
			// get number of bases
			//
			int nBases;
			__cgnsCheck(cg_nbases(index_file, &nBases));
			DTINFOWF(dtReadCGNS(), << "Found " << nBases << " base(s).");
			dt__THROW_IF(nBases > 1, dtReadCGNS());
			int index_base = 1;
			
			//
			// number of zones
			//
			int nZones;
			__cgnsCheck(cg_nzones(index_file, index_base, &nZones));
			DTINFOWF(dtReadCGNS(), << "Found " << nZones << " zone(s).");

			for (int index_zone = 1; index_zone <= nZones; index_zone++) {
			  DTINFOWF(dtReadCGNS(), << "Reading zone " << index_zone);
				
				ZoneType_t zoneType;
				__cgnsCheck(cg_zone_type(index_file, index_base, index_zone, &zoneType));

				dt__THROW_IF(zoneType==ZoneTypeNull, dtReadCGNS() );
				dt__THROW_IF(zoneType==ZoneTypeUserDefined, dtReadCGNS() );
				dt__THROW_IF(zoneType==Structured, dtReadCGNS() );
				if ( zoneType == Unstructured ) {
					DTINFOWF(dtReadCGNS(), << "Unstructured zone detected.");
					//
					// read zone info
					//				
					cgsize_t zoneSizes[3];				
					char zoneName[35];
					__cgnsCheck(cg_zone_read(index_file, index_base, index_zone, zoneName, zoneSizes));
					int nNodes = static_cast< int >(zoneSizes[0]);
					int nCells = static_cast< int >(zoneSizes[1]);
					DTINFOWF(
					  dtReadCGNS(), 
						<< DTLOGEVAL(zoneName) << LOGDEL
						<< DTLOGEVAL(zoneSizes[0]) << LOGDEL
						<< DTLOGEVAL(zoneSizes[1]) << LOGDEL
						<< DTLOGEVAL(zoneSizes[2]) << LOGDEL
						<< DTLOGEVAL(nNodes) << LOGDEL
						<< DTLOGEVAL(nCells)
					);
					
					//
					// read coordinates
					//				
					int nCoords;
					__cgnsCheck(cg_ncoords(index_file, index_base, index_zone, &nCoords));		
					DataType_t dataType;
					char coordName[35];
					void* coord;
					twoDArrayHandling< double > nodes(nNodes, nCoords);

					for ( int iCoord = 0; iCoord < nCoords; iCoord++ ) {
						__cgnsCheck(
							cg_coord_info(
								index_file, index_base, index_zone, 
								iCoord+1, &dataType, coordName
							)
						);
						DTINFOWF(
							dtReadCGNS(),
							<< "Reading coordinate " << iCoord+1 << " : " << coordName
						);

						cgsize_t irmin  = 1;
						cgsize_t irmax = zoneSizes[0];
						switch(dataType) {
							case RealSingle:
    						DTINFOWF(dtReadCGNS(), << "Type is float");
								coord = new float[nNodes];
								__cgnsCheck(
									cg_coord_read(
										index_file, index_base, index_zone, 
										coordName, dataType, &irmin, &irmax, coord
									)
								);
								for (int iNode = 0; iNode < nNodes; iNode++) {
									nodes[iNode][iCoord] = (double)((float*)coord)[iNode];
								}
								delete [] (float*)coord;
								break;
							case RealDouble:
								DTINFOWF(dtReadCGNS(), << "Type is double");
								coord = new double[nNodes];
								__cgnsCheck(
									cg_coord_read(
										index_file, index_base, index_zone, 
										coordName, dataType, &irmin, &irmax, coord
									)
								);
								for (int iNode = 0; iNode < nNodes; iNode++) {
									nodes[iNode][iCoord] = ((double*) coord)[iNode];
								}
								delete [] (double*)coord;
								break;
						}
					}	

					//
					// create vertices
					//		
					for (int iNode = 0; iNode < nNodes; iNode++) {
						vNum++;
						MVertex* mv = new MVertex(nodes[iNode][0], nodes[iNode][1], nodes[iNode][2], 0, vNum);
						minVertex = std::min(minVertex, vNum);
						maxVertex = std::max(maxVertex, vNum);
						vertexMap[vNum] = mv;
					}
	
			  //
			  // read sections
				//
				int nSec;
				__cgnsCheck( cg_nsections(index_file, index_base, index_zone, &nSec) );
				DTINFOWF(dtReadCGNS(), << "Found " << nSec << " sections.");
				for (int index_section = 1; index_section <= nSec; index_section++) {
					char secName[30];
					ElementType_t elementType;
					cgsize_t bounds[2];
					int nBoundary;
					int parentFlag = 0;
					__cgnsCheck(
						cg_section_read(
							index_file, index_base, index_zone, index_section,
							secName, &elementType, &(bounds[0]), &(bounds[1]), &nBoundary, &parentFlag
						)
					);
					DTINFOWF(
					  dtReadCGNS(), 
						<< DTLOGEVAL(secName) << LOGDEL
						<< DTLOGEVAL(bounds[0]) << LOGDEL
						<< DTLOGEVAL(bounds[1]) << LOGDEL
						<< DTLOGEVAL(nBoundary) << LOGDEL
						<< DTLOGEVAL(parentFlag) 
					);					
           
					//
					// read element data
					//
					cgsize_t elementDataSize;				
					__cgnsCheck( 
						cg_ElementDataSize(
							index_file, index_base, index_zone, index_section,
							&elementDataSize
						)
					);
					std::vector< cgsize_t > elementData(elementDataSize);					
					__cgnsCheck(
						cg_elements_read(
							index_file, index_base, index_zone, index_section,
							&(elementData[0]), NULL
						)
					);

					DTINFOWF(
						dtReadCGNS(), 
						<< DTLOGEVAL(elementDataSize) << LOGDEL
						<< DTLOGEVAL(elementType) << LOGDEL
            << DTLOGEVAL(ElementTypeName[elementType])
					);
					int nElements;
					int tmpC = 0;
					switch(elementType) {
						/*        gmsh                              cgns                   
						 *         v                                 v                        
						 *         ^                                 ^                         
						 *         |                                 |                      
						 *   3-----------2                     3-----------2                  
						 *   |     |     |                     |     |     |                
						 *   |     |     |           >>>       |     |     |               
						 *   |     +---- | --> u     >>>       |     +---- | --> u              
						 *   |           |           >>>       |           |                   
						 *   |           |                     |           |              
						 *   0-----------1                     0-----------1           
						 */									
						case QUAD_4:
							fNum++;
							faces.push_back( new dtGmshFace(this, fNum) );
							nElements = elementDataSize/4;
							//
							// create elements
							//
							tmpC = 0;
							faces.back()->addPhysicalEntity(
							  setPhysicalName(secName, 2, 0)
							);
							this->add( faces.back() );
							for (int iEl = bounds[0]; iEl<=bounds[1]; iEl++) {
								eNum++;								
								int counter = (tmpC)*4;
								tmpC++;
								std::vector< MVertex * > locVertices(4, NULL);
								locVertices[0] = vertexMap[elementData[counter+0]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+2]];
								locVertices[3] = vertexMap[elementData[counter+3]];
								MQuadrangle * me = new MQuadrangle(locVertices, eNum, 0);
								minElement = std::min(minElement, eNum);
								maxElement = std::max(maxElement, eNum);
								elementMap[eNum] = me;
								faces.back()->addQuadrangle(me);
							}
							break;
						/*         gmsh                           cgns
						 *          v                              v
						 *   3----------2                   7----------6
						 *   |\     ^   |\                  |\     ^   |\
						 *   | \    |   | \                 | \    |   | \
						 *   |  \   |   |  \                |  \   |   |  \
						 *   |   7------+---6        >>>    |   8------+---5
						 *   |   |  +-- |-- | -> u   >>>    |   |  +-- |-- | -> u
						 *   0---+---\--1   |        >>>    3---+---\--2   |
						 *    \  |    \  \  |                \  |    \  \  |
						 *     \ |     \  \ |                 \ |     \  \ |
						 *      \|      w  \|                  \|      w  \|
						 *       4----------5                   4----------1
						 */													
						case HEXA_8:
							rNum++;
							regions.push_back( new dtGmshRegion(this, rNum) );
							regions.back()->addPhysicalEntity(
							  setPhysicalName(secName, 3, 0)
							);							
							this->add( regions.back() );
							nElements = elementDataSize/4;							
							nElements = elementDataSize/8;
							//
							// create elements
							//
							tmpC = 0;
							for (int iEl = bounds[0]; iEl<= bounds[1]; iEl++) {						
								eNum++;								
								int counter = (tmpC)*8;
								tmpC++;
								std::vector< MVertex * > locVertices(8, NULL);
								locVertices[0] = vertexMap[elementData[counter+2]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+5]];
								locVertices[3] = vertexMap[elementData[counter+6]];
								locVertices[4] = vertexMap[elementData[counter+3]];
								locVertices[5] = vertexMap[elementData[counter+0]];
								locVertices[6] = vertexMap[elementData[counter+4]];
								locVertices[7] = vertexMap[elementData[counter+7]];
								MHexahedron * me = new MHexahedron(locVertices, eNum, 0);
								minElement = std::min(minElement, eNum);
								maxElement = std::max(maxElement, eNum);
								elementMap[eNum] = me;
								regions.back()->addHexahedron(me);
							}
							break;
						default:
							dt__THROW(dtReadCGNS(), << ElementTypeName[elementType]);
					}
				}				
			}
		}
		
		__cgnsCheck( cg_close(index_file) );
		
		dt__THROW_IF( vertexMap.size() != vNum, dtReadCGNS() );
		dt__THROW_IF( elementMap.size() != eNum, dtReadCGNS() );
		
		dt__FORALLITER(vertexMap_t, vertexMap, it) {
			MVertex * mv = it->second;
			mv->setEntity(regions.back());
			regions.back()->addMeshVertex(mv);
		}
	}	

	void dtGmshModel::clearModel( void ) {
	  this->destroy(true);
		this->deleteMesh();
	}	
}
