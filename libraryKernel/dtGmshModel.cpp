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
#include <gmsh/meshGEdge.h>
#include <gmsh/meshGFace.h>
#include <gmsh/meshGRegion.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>

#define __caCThis \
  const_cast< dtGmshModel * >(this)

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
		
		std::vector< std::string > fNames(6,"");
		fNames[0] = "south"; fNames[1] = "north"; fNames[2] = "front";
		fNames[3] = "back"; fNames[4] = "west"; fNames[5] = "east";
		int counter = 0;
		for (std::list< dtGmshFace* >::iterator it=gf.begin(); it!=gf.end(); ++it) {
			counter++;
			(*it)->addPhysicalEntity( 
				this->setPhysicalName( 
					vol->getLabel()+"_GFace_"+fNames[counter], 2, 0
				)							
			);
		}
		
		this->add( new dtGmshRegionHex(this, rId, eId, gf, fori) );
		this->getRegionByTag(rId)->addPhysicalEntity( 			
			this->setPhysicalName(
				vol->getLabel()+"_GRegion_"+stringPrimitive::intToString(rId), 3, 0
			)
    );
		
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
   
    dt__PTRASS( dtGmshVertex * gVertex, dtGmshVertex::DownCast(vertex) );
    
    return gVertex;    
  }  

  dtGmshRegion * dtGmshModel::cast2DtGmshRegion( GRegion * gr ){
    dtGmshRegion * ret;
    dt__MUSTDOWNCAST(gr, dtGmshRegion, ret);
    
    return ret;
  }
  
  dtGmshFace * dtGmshModel::cast2DtGmshFace( GFace * gf ){
    dtGmshFace * ret;
    dt__MUSTDOWNCAST(gf, dtGmshFace, ret);
    
    return ret;
  }
  
  dtGmshEdge * dtGmshModel::cast2DtGmshEdge( GEdge * ge ) {
    dtGmshEdge * ret;
    dt__MUSTDOWNCAST(ge, dtGmshEdge, ret);
    
    return ret;    
  }
  
  dtGmshVertex * dtGmshModel::cast2DtGmshVertex( GVertex * gv ) {
    dtGmshVertex * ret;
    dt__MUSTDOWNCAST(gv, dtGmshVertex, ret);
    
    return ret;    
  }

  dtPoint3 dtGmshModel::cast2DtPoint3( GVertex * gv ) {
    return cast2DtGmshVertex(gv)->cast2DtPoint3();
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
		GModel::setCurrent(__caCThis);
			
		//
		// get all entities
		//
		std::vector< GEntity * > ent;
		this->getEntities(ent);
		vectorHandling< dtPoint3 > pp(__caCThis->getNumMeshVertices());		
		
		for( int ii=0; ii<ent.size(); ii++ ) {
      for( int jj=0; jj<ent[ii]->getNumMeshVertices(); jj++ ) {
				MVertex const * const mv = ent[ii]->getMeshVertex(jj);				
				pp[mv->getNum()-1]
				=
				dtPoint3(
					static_cast< float >(mv->x()), 
					static_cast< float >(mv->y()), 
					static_cast< float >(mv->z())
				); 				
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
				unsigned nEl = ent[ii]->getNumMeshElements();
				int nElTot = nEl;
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GVertex>:" << LOGDEL
					<< "points = " << nEl << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}		
      else if (ge) {
				unsigned nEl = ent[ii]->getNumMeshElements();
				int nElTot = nEl;
				DTINFOWF(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GEdge>:" << LOGDEL
					<< "lines = " << nEl << LOGDEL
					<< DTLOGEVAL(nElTot)
				);		
			}					
      else if (gf) {
				std::vector< unsigned > nEl(3,0);
				ent[ii]->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2] + nEl[3] + nEl[4];
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
          vertsIndex[0] = verts[0]->getNum()-1;
          vertsIndex[1] = verts[1]->getNum()-1;
          vertsIndex[2] = verts[2]->getNum()-1;
          vertsIndex[3] = verts[3]->getNum()-1;
					um->addElement(vertsIndex);
        }     
        //
        // hexahedron
        //
        else if ( mhex ) {
					vectorHandling< int > vertsIndex(8);
          std::vector< MVertex * > verts;
					mhex->getVertices(verts); 
          vertsIndex[0] = verts[4]->getNum()-1;
          vertsIndex[1] = verts[5]->getNum()-1;
          vertsIndex[2] = verts[1]->getNum()-1;
          vertsIndex[3] = verts[0]->getNum()-1;
          vertsIndex[4] = verts[7]->getNum()-1;
          vertsIndex[5] = verts[6]->getNum()-1;
          vertsIndex[6] = verts[2]->getNum()-1;
          vertsIndex[7] = verts[3]->getNum()-1;          
					um->addElement(vertsIndex);
        }  
      }
    }
		
		return um;
	}
}
