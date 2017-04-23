#include "dtGmshModel.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include <gmsh/HighOrder.h>
#include <gmsh/GEdge.h>
#include <gmsh/GFace.h>
#include <gmsh/GVertex.h>
#include <gmsh/GRegion.h>

#include "dtGmshRegion.h"
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include "dtGmshVertex.h"
#include "discrete3dPoints.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <interfaceHeaven/ptrHandling.h>
#include <unstructured3dMesh.h>
#include <unstructured3dSurfaceMesh.h>
#include "dtMeshGEdge.h"
#include "dtMeshGFace.h"
#include "dtMeshGRegion.h"
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MPrism.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <cgnslib.h>

#define __caCThis \
  const_cast< dtGmshModel * >(this)

#define __cgnsCheck(cmd) \
  if (cmd) dt__throw(__cgnsCheck(), << dt__eval(cg_get_error()))

namespace dtOO {
  dtGmshModel::dtGmshModel(std::string name) : GModel(name){
    _debug = "";
    _fmTwin = NULL;
  }

  dtGmshModel::dtGmshModel(
    std::string name, ::FieldManager * fm
  ) : GModel(name){
    _debug = "";
    
    //
    // temporarly store field
    //
    _fmTwin = GModel::_fields;
    
    //
    // set given field to model
    //
    GModel::_fields = fm;
  }
  
  dtGmshModel::~dtGmshModel() {
    //
    // if initialized with field, change to old one before destruction
    //
    if (_fmTwin) GModel::_fields = _fmTwin;
  }
  
  void dtGmshModel::addIfVertexToGmshModel( 
    dtPoint3 const & vertex, int * const tag 
  ) {
		*tag = this->getMaxVertexTag()+1;
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
		*tag = this->getMaxEdgeTag()+1;
		dtGmshEdge * ge = new dtGmshEdge(
			this, *tag, ::GModel::getVertexByTag(from), ::GModel::getVertexByTag(to)
		);
		ge->setMap1dTo3d( edge );
		
		int tTag = alreadyInModel(ge);
		if (tTag) {
			delete ge;
			*tag = tTag;
			if ( ::GModel::getEdgeByTag(*tag)->getBeginVertex()->tag() != from ) {
				*tag = -tTag;
			}
		}
		else {
			this->add(ge);
		}
  }
	
  void dtGmshModel::addIfEdgeToGmshModel(
    map1dTo3d const * const edge, int * const tag
  ) {
		*tag = this->getMaxEdgeTag()+1;		
		
		std::vector< int > vId(2);
		addIfVertexToGmshModel(edge->getPointPercent(0.), &(vId[0]) );
		addIfVertexToGmshModel(edge->getPointPercent(1.), &(vId[1]) );
		
		addIfEdgeToGmshModel(edge, tag, vId[0], vId[1]);
	}
	
  void dtGmshModel::addIfFaceToGmshModel( 
    map2dTo3d const * const face, int * const tag,
		std::list< ::GEdge * > const & edges, std::vector< int > const & ori
  ) {
		*tag = this->getMaxFaceTag()+1;	
		
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
	 
  void dtGmshModel::addIfFaceToGmshModel( 
    map2dTo3d const * const face, int * const tag,
		int const & eId0, int const & eId1, int const & eId2, int const & eId3
  ) {
		std::list< ::GEdge * > ge;
		ge.push_back( this->getEdgeByTag(abs(eId0)) );
		ge.push_back( this->getEdgeByTag(abs(eId1)) );
		ge.push_back( this->getEdgeByTag(abs(eId2)) );
		ge.push_back( this->getEdgeByTag(abs(eId3)) );
		std::vector<int> ori(4);
		ori[0] = ( eId0 < 0 ? -1 : 1);
		ori[1] = ( eId1 < 0 ? -1 : 1);
		ori[2] = ( eId2 < 0 ? -1 : 1);
		ori[3] = ( eId3 < 0 ? -1 : 1);  
		
		addIfFaceToGmshModel(face, tag, ge, ori);
  }
	
  void dtGmshModel::addIfFaceToGmshModel(
    map2dTo3d const * const face, int * const tag
  ) {
		*tag = this->getMaxFaceTag()+1;		
		
    dtPoint2 p0(0., 0.);
		dtPoint2 p1(1., 0.);
		dtPoint2 p2(1., 1.);
		dtPoint2 p3(0., 1.);		
		std::vector< int > eId(4);
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentPercent(p0, p1)), &(eId[0]) 
    );
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentPercent(p1, p2)), &(eId[1]) 
    );
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentPercent(p2, p3)), &(eId[2]) 
    );
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentPercent(p3, p0)), &(eId[3]) 
    );
		
  	addIfFaceToGmshModel(face, tag, eId[0], eId[1], eId[2], eId[3]);
	}

  void dtGmshModel::addIfRegionToGmshModel(
    map3dTo3d const * const region, int * const tag,
		std::list< ::GFace * > const & faces, std::vector< int > const & ori    
  ) {
		*tag = this->getMaxRegionTag()+1;	
		
		dtGmshRegion * gr = new dtGmshRegion(this, *tag, faces, ori);
		
		int tTag = alreadyInModel(gr);
		if (tTag) {
			delete gr;
			*tag = tTag;
		}
		else {
			this->add(gr);
		}    
  }

  void dtGmshModel::addIfRegionToGmshModel( 
    map3dTo3d const * const region, int * const tag,
		int const & fId0, int const & fId1, 
    int const & fId2, int const & fId3, 
    int const & fId4, int const & fId5
  ) {
		std::list< ::GFace * > gf;
		gf.push_back( this->getFaceByTag(abs(fId0)) );
		gf.push_back( this->getFaceByTag(abs(fId1)) );
		gf.push_back( this->getFaceByTag(abs(fId2)) );
		gf.push_back( this->getFaceByTag(abs(fId3)) );
    gf.push_back( this->getFaceByTag(abs(fId4)) );
    gf.push_back( this->getFaceByTag(abs(fId5)) );
		std::vector<int> ori(6);
		ori[0] = ( fId0 < 0 ? -1 : 1);
		ori[1] = ( fId1 < 0 ? -1 : 1);
		ori[2] = ( fId2 < 0 ? -1 : 1);
		ori[3] = ( fId3 < 0 ? -1 : 1);  
    ori[4] = ( fId4 < 0 ? -1 : 1);  
    ori[5] = ( fId5 < 0 ? -1 : 1);  
		
		addIfRegionToGmshModel(region, tag, gf, ori);
  }
    
  void dtGmshModel::addIfRegionToGmshModel(
    map3dTo3d const * const region, int * const tag
  ) {
		*tag = this->getMaxRegionTag()+1;		
		std::vector< int > fId(6);
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstWPercent(0)), &(fId[0])
		);						
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstWPercent(1)), &(fId[1])
		);
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstVPercent(0)), &(fId[2])
		);
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstVPercent(1)), &(fId[3])
		);
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstUPercent(0)), &(fId[4])
		);
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstUPercent(1)), &(fId[5])
		);    

  	addIfRegionToGmshModel(
      region, tag, fId[0], fId[1], fId[2], fId[3], fId[4], fId[5]
    );    
  }  
			
  dtGmshRegion * dtGmshModel::getDtGmshRegionByTag( int const tag ) const {
    ::GRegion * region = ::GModel::getRegionByTag(abs(tag));
    dt__ptrAss(dtGmshRegion * gRegion, dtGmshRegion::DownCast(region));
    
    return gRegion;    
  }
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByTag( int const tag ) const {
    ::GFace * face = ::GModel::getFaceByTag(abs(tag));
    dt__ptrAss(dtGmshFace * gFace, dtGmshFace::DownCast(face));
    
    return gFace;    
  }
	
  ::GEntity * dtGmshModel::getGEntityByPhysical( 
    std::string const & physical 
  ) const {
    dt__forFromToIndex(0, 4, dim) {
      dtGmshModel::intGEntityVMap ge_number;      
      GModel::getPhysicalGroups(dim, ge_number);     
      //
      // for each physical group
      //
      dt__forAllConstIter(dtGmshModel::intGEntityVMap, ge_number, nIt) {
        if ( getPhysicalName(dim, nIt->first) == physical ) {
          dt__throwIfWithMessage( 
            nIt->second.size()!=1, 
            getGEntityByPhysical(),
            << dt__eval(nIt->second.size())
          );
          return nIt->second[0];
        }
      }
    }
    dt__throw(
      getGEntityByPhysical(), 
      << "No GEntity with physical " << physical
    );
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByPhysical( 
    std::string const & physical 
  ) const {
    int pN = __caCThis->getPhysicalNumber(1, physical);
		intGEntityVMap gE_pN;
		__caCThis->getPhysicalGroups(1, gE_pN);
		
		dt__throwIf(gE_pN[pN].size()!=1, getDtGmshEdgeByPhysical());
		
		return cast2DtGmshEdge(gE_pN[pN][0]);
  }	
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByPhysical( 
    std::string const & physical 
  ) const {
    int pN = __caCThis->getPhysicalNumber(2, physical);
		intGEntityVMap gE_pN;
		__caCThis->getPhysicalGroups(2, gE_pN);
		
		dt__throwIf(gE_pN[pN].size()!=1, getDtGmshFaceByPhysical());
		
		return cast2DtGmshFace(gE_pN[pN][0]);
  }	
  
  dtGmshRegion * dtGmshModel::getDtGmshRegionByPhysical( 
    std::string const & physical 
  ) const {
    int pN = __caCThis->getPhysicalNumber(3, physical);
		intGEntityVMap gE_pN;
		__caCThis->getPhysicalGroups(3, gE_pN);
		
		dt__throwIf(gE_pN[pN].size()!=1, getDtGmshRegionByPhysical());
		
		return cast2DtGmshRegion(gE_pN[pN][0]);
  }	  
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByTag( int const tag ) const {
    ::GEdge * edge = ::GModel::getEdgeByTag(abs(tag));
    dt__ptrAss(dtGmshEdge * gEdge, dtGmshEdge::DownCast(edge));
    
    return gEdge;    
  }

  int dtGmshModel::getDtGmshEdgeTagByFromTo( 
    int const from, int const to 
  ) const {
    dtGmshVertex * gv = getDtGmshVertexByTag(from);
    
    std::list< ::GEdge * > edges = gv->edges();
    std::list< ::GEdge * >::iterator it = edges.begin();
    for (it; it != edges.end(); ++it) {
      if ( (*it)->getBeginVertex()->tag() == to ) {
        return -((*it)->tag());
      }
      else if ( (*it)->getEndVertex()->tag() == to ) {
        return (*it)->tag();
      }
    }
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByFromTo( 
    int const from, int const to 
  ) const {
    return getDtGmshEdgeByTag( abs(getDtGmshEdgeTagByFromTo(from, to)) );
  }  

  dtGmshVertex * dtGmshModel::getDtGmshVertexByTag( int const tag ) const {
    ::GVertex * vertex = ::GModel::getVertexByTag(tag);
   
    dt__ptrAss( dtGmshVertex * gVertex, dtGmshVertex::DownCast(vertex) );
    
    return gVertex;    
  }  

  dtGmshRegion * dtGmshModel::cast2DtGmshRegion( ::GEntity * gr ){
    dtGmshRegion * ret;
    dt__mustCast(gr, dtGmshRegion, ret);
    
    return ret;
  }
  
  dtGmshFace * dtGmshModel::cast2DtGmshFace( ::GEntity * gf ){
    dtGmshFace * ret;
    dt__mustCast(gf, dtGmshFace, ret);
    
    return ret;
  }
  
  dtGmshEdge * dtGmshModel::cast2DtGmshEdge( ::GEntity * ge ) {
    dtGmshEdge * ret;
    dt__mustCast(ge, dtGmshEdge, ret);
    
    return ret;    
  }
  
  std::list< dtGmshEdge * > dtGmshModel::cast2DtGmshEdge( 
    std::list< ::GEdge * > edges 
  ) {
		std::list< dtGmshEdge * > retEdges;
		dt__forAllIter(std::list< ::GEdge * >, edges, it) {
			retEdges.push_back( cast2DtGmshEdge(*it) );
		}
		return retEdges;
	}
	
  std::list< dtGmshFace * > dtGmshModel::cast2DtGmshFace( 
    std::list< ::GFace * > faces 
  ) {
		std::list< dtGmshFace * > ret;
		dt__forAllIter(std::list< ::GFace * >, faces, it) {
			ret.push_back( cast2DtGmshFace(*it) );
		}
		return ret;
	}

  std::list< dtGmshRegion * > dtGmshModel::cast2DtGmshRegion( 
    std::list< ::GRegion * > regions 
  ) {
		std::list< dtGmshRegion * > ret;
		dt__forAllIter(std::list< ::GRegion * >, regions, it) {
			ret.push_back( cast2DtGmshRegion(*it) );
		}
		return ret;
	}	  
	
  dtGmshVertex * dtGmshModel::cast2DtGmshVertex( ::GEntity * gv ) {
    dtGmshVertex * ret;
    dt__mustCast(gv, dtGmshVertex, ret);
    
    return ret;    
  }

  std::list< dtGmshVertex * > dtGmshModel::cast2DtGmshVertex( 
    std::list< ::GVertex * > vertices 
  ) {
		std::list< dtGmshVertex * > retVertices;
		dt__forAllIter(std::list< ::GVertex * >, vertices, it) {
			retVertices.push_back( cast2DtGmshVertex(*it) );
		}
		return retVertices;
	}
  
  dtPoint3 dtGmshModel::extractPosition( ::GVertex const * const gv ) {
    return cast2DtGmshVertex(
			const_cast< ::GVertex * >(gv)
		)->cast2DtPoint3();
  }

  dtPoint3 dtGmshModel::extractPosition( ::MVertex const * const mv ) {
    return dtPoint3(
			const_cast< ::MVertex * >(mv)->x(), 
			const_cast< ::MVertex * >(mv)->y(), 
			const_cast< ::MVertex * >(mv)->z()
		);
  }
	
	void dtGmshModel::setPosition( ::MVertex * mv, dtPoint3 const & pp ) {
		mv->setXYZ(pp.x(), pp.y(), pp.z());
	}
  
  void dtGmshModel::add( ::GEntity * ge) {
    if ( dtGmshVertex::DownCast(ge) ) {
      GModel::add( dtGmshVertex::SecureCast(ge) );
    }
    else if ( dtGmshEdge::DownCast(ge) ) {
      GModel::add( dtGmshEdge::SecureCast(ge) );
    }
    else if ( dtGmshFace::DownCast(ge) ) {
      GModel::add( dtGmshFace::SecureCast(ge) );
    }
    else if ( dtGmshRegion::DownCast(ge) ) {
      GModel::add( dtGmshRegion::SecureCast(ge) );
    }    
    else dt__throwUnexpected(add);
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
  
  void dtGmshModel::prepareToMesh( void ) {
    // Change any high order elements back into first order ones    
    // Initialize pseudo random mesh generator with the same seed
    srand(1);
    SetOrder1(this);    
  }

  int dtGmshModel::alreadyInModel( ::GVertex const * const gv ) const {
		for (
      ::GModel::viter vIt = __caCThis->firstVertex(); 
      vIt != __caCThis->lastVertex(); 
      ++vIt
    ) {
			if ( dtGmshVertex::isEqual(gv, *vIt) ) {
				dt__debug(
					alreadyInModel(),
					<< "duplicate vertex = " << gv->tag() 
          << " equal to vertex tag = " << (*vIt)->tag()
				);				
				return (*vIt)->tag();
			}
		}
		return 0;
	}

  int dtGmshModel::alreadyInModel( ::GEdge const * const ge ) const {
		for (
      ::GModel::eiter eIt = __caCThis->firstEdge(); 
      eIt != __caCThis->lastEdge(); 
      ++eIt
    ) {
			if ( dtGmshEdge::isEqual(ge, *eIt) ) {
				dt__debug(
					alreadyInModel(),
					<< "duplicate edge = " << ge->tag() 
          << " equal to edge tag = " << (*eIt)->tag()
				);				
				return (*eIt)->tag();
			}
		}
		return 0;
	}
	
  int dtGmshModel::alreadyInModel( ::GFace const * const gf ) const {
		for (
      ::GModel::fiter fIt = __caCThis->firstFace(); 
      fIt != __caCThis->lastFace(); 
      ++fIt
    ) {
			if ( dtGmshFace::isEqual(gf, *fIt) ) {
				dt__debug(
					alreadyInModel(),
					<< "duplicate face = " << gf->tag() 
          << " equal to face tag = " << (*fIt)->tag()
				);				
				return (*fIt)->tag();
			}
		}
		return 0;
	}

  int dtGmshModel::alreadyInModel( ::GRegion const * const gr ) const {
		for (
      ::GModel::riter rIt = __caCThis->firstRegion(); 
      rIt != __caCThis->lastRegion(); 
      ++rIt
    ) {
			if ( dtGmshRegion::isEqual(gr, *rIt) ) {
				dt__debug(
					alreadyInModel(),
					<< "duplicate region = " << gr->tag() 
          << " equal to region tag = " << (*rIt)->tag()
				);				
				return (*rIt)->tag();
			}
		}
		return 0;
	}	  
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( void ) const {
		//
		// set current model
		// 
		dt__info( 
      toUnstructured3dMesh(), 
      << dt__eval(::GModel::setCurrent(__caCThis)) 
    );
			
		//
		// get all entities
		//
		std::vector< ::GEntity * > ent;
		this->getEntities(ent);
		vectorHandling< dtPoint3 > pp(__caCThis->getNumMeshVertices());		
		std::map< int, int > vLoc_num;
		
		int counter = 0;
		for( int ii=0; ii<ent.size(); ii++ ) {
      for( int jj=0; jj<ent[ii]->getNumMeshVertices(); jj++ ) {
				::MVertex const * const mv = ent[ii]->getMeshVertex(jj);				
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
			::GVertex * gv = dynamic_cast< ::GVertex * >(ent[ii]);
			::GEdge * ge = dynamic_cast< ::GEdge * >(ent[ii]);
			::GFace * gf = dynamic_cast< ::GFace * >(ent[ii]);
			::GRegion * gr = dynamic_cast< ::GRegion * >(ent[ii]);

      if (gv) {
				unsigned nEl = gv->getNumMeshElements();
				int nElTot = nEl;
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GVertex>:" << std::endl
					<< "points = " << nElTot << std::endl
					<< dt__eval(nElTot)
				);		
			}		
      else if (ge) {
				unsigned nEl = ge->getNumMeshElements();
				int nElTot = nEl;
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GEdge>:" << std::endl
					<< "lines = " << nElTot << std::endl
					<< dt__eval(nElTot)
				);		
			}					
      else if (gf) {
				std::vector< unsigned > nEl(3,0);
				gf->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2];
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GFace>:" << std::endl
					<< "triangles = " << nEl[0] << std::endl
					<< "quadrangles = " << nEl[1] << std::endl
					<< "polygons = " << nEl[2] << std::endl
					<< dt__eval(nElTot)
				);		
			}			
			else if (gr) {
				std::vector< unsigned > nEl(5,0);
				ent[ii]->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2] + nEl[3] + nEl[4];
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GRegion>:" << std::endl
					<< "tetrahedra = " << nEl[0] << std::endl
					<< "hexahedra = " << nEl[1] << std::endl
					<< "prisms = " << nEl[2] << std::endl
					<< "pyramids = " << nEl[3] << std::endl
					<< "polyhedra = " << nEl[4] << std::endl
					<< dt__eval(nElTot)
				);		
			}
		}
		//
		// write only 3d elements
		//
    for (
			::GModel::riter r_it = __caCThis->firstRegion(); 
			r_it != __caCThis->lastRegion(); 
			++r_it
		) {
      ::GRegion * gr = *r_it;
			unsigned nElemTot = gr->getNumMeshElements();
      for( unsigned ii=0; ii<nElemTot; ii++ ) {
        ::MElement * me = gr->getMeshElement(ii);
        ::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
        ::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
        //
        // tetrahedron
        //
        if ( mtet ) {
				  vectorHandling< int > vertsIndex(4);					
          std::vector< ::MVertex * > verts;
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
          std::vector< ::MVertex * > verts;
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
	  std::vector< ::MElement const * > const & elements
	) {
		std::vector< ::MVertex const * > vertices;
		for( int ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			std::vector< ::MVertex * > verts;
			me->getVertices(verts);
			for( int ii=0; ii<verts.size(); ii++ ) vertices.push_back(verts[ii]);			
		}		
		progHelper::removeBastardTwins(vertices);
		return dtGmshModel::toUnstructured3dMesh(vertices, elements);
	}	
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( 
	  std::vector< ::MVertex const * > const & vertices, 
    std::vector< ::MElement const * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, int > vLoc_num;
		for( int ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
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
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
			::MPyramid * mpyr = dynamic_cast< ::MPyramid * >(me);
			::MPrism * mpri = dynamic_cast< ::MPrism * >(me);			
			::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
			//
			// tetrahedron
			//
			if ( mtet ) {
				vectorHandling< int > vertsIndex(4);					
				std::vector< ::MVertex * > verts;
				mtet->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}     
			//
			// pyramid
			//
			else if ( mpyr ) {
				vectorHandling< int > vertsIndex(5);
				std::vector< ::MVertex * > verts;
				mpyr->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				vertsIndex[4] = vLoc_num[verts[4]->getNum()];
				um->addElement(vertsIndex);
			}  
			//
			// prism
			//
			else if ( mpri ) {
				vectorHandling< int > vertsIndex(6);
				std::vector< ::MVertex * > verts;
				mpri->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				vertsIndex[4] = vLoc_num[verts[4]->getNum()];
				vertsIndex[5] = vLoc_num[verts[5]->getNum()];
				um->addElement(vertsIndex);
			}  			
			//
			// hexahedron
			//
			else if ( mhex ) {
				vectorHandling< int > vertsIndex(8);
				std::vector< ::MVertex * > verts;
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
	  std::vector< ::MVertex const * > const & vertices, 
		std::vector< ::MElement const * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, int > vLoc_num;
		for( int ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
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
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(me);
			::MTriangle * mtri = dynamic_cast< ::MTriangle * >(me);
			
			//
			// quadrangle
			//
			if ( mquad ) {
				vectorHandling< int > vertsIndex(4);					
				std::vector< ::MVertex * > verts;
				mquad->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}    
			else if ( mtri ) {
				vectorHandling< int > vertsIndex(3);					
				std::vector< ::MVertex * > verts;
				mtri->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				um->addElement(vertsIndex);
			}     				
		}		
		
		return um;		
	}	

  discrete3dPoints * dtGmshModel::toDiscrete3dPoints( 
	  std::vector< ::MVertex const * > const & vertices
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		for( int ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< float >(mv->x()), 
				static_cast< float >(mv->y()), 
				static_cast< float >(mv->z())
			); 			
		}
		
		return new discrete3dPoints(pp);
	}	
	
  renderInterface * dtGmshModel::toAdequateSurfaceRenderInterface( 
		std::vector< ::MElement const * > const & elements
	) {
		std::vector< ::MVertex const * > vertices;
		for( int ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			std::vector< ::MVertex * > verts;
			me->getVertices(verts);
			for( int ii=0; ii<verts.size(); ii++ ) vertices.push_back(verts[ii]);			
		}		
    progHelper::removeBastardTwins(vertices);		
    
		return dtGmshModel::toAdequateSurfaceRenderInterface(vertices, elements);
	}		
	
  renderInterface * dtGmshModel::toAdequateSurfaceRenderInterface( 
	  std::vector< ::MVertex const * > const & vertices, 
		std::vector< ::MElement const * > const & elements
	) {
		if (elements.size() != 0) {
			return toUnstructured3dSurfaceMesh(vertices, elements);
		}
		else return toDiscrete3dPoints(vertices);
	}		
	
	void dtGmshModel::dtReadCGNS(const std::string &name) {		
			std::vector< dtGmshFace * >  faces;
			std::vector< dtGmshRegion * >  regions;
							
			typedef std::map<int, ::MVertex*> vertexMap_t;
			typedef std::map<int, ::MElement*> elementMap_t;		
			
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
			dt__info(dtReadCGNS(), << "Found " << nBases << " base(s).");
			dt__throwIf(nBases > 1, dtReadCGNS());
			int index_base = 1;
			
			//
			// number of zones
			//
			int nZones;
			__cgnsCheck(cg_nzones(index_file, index_base, &nZones));
			dt__info(dtReadCGNS(), << "Found " << nZones << " zone(s).");

			for (int index_zone = 1; index_zone <= nZones; index_zone++) {
			  dt__info(dtReadCGNS(), << "Reading zone " << index_zone);
				
				ZoneType_t zoneType;
				__cgnsCheck(cg_zone_type(index_file, index_base, index_zone, &zoneType));

				dt__throwIf(zoneType==ZoneTypeNull, dtReadCGNS() );
				dt__throwIf(zoneType==ZoneTypeUserDefined, dtReadCGNS() );
				dt__throwIf(zoneType==Structured, dtReadCGNS() );
				if ( zoneType == Unstructured ) {
					dt__info(dtReadCGNS(), << "Unstructured zone detected.");
					//
					// read zone info
					//				
					cgsize_t zoneSizes[3];				
					char zoneName[35];
					__cgnsCheck(
            cg_zone_read(
              index_file, index_base, index_zone, zoneName, zoneSizes
            )
          );
					int nNodes = static_cast< int >(zoneSizes[0]);
					int nCells = static_cast< int >(zoneSizes[1]);
					dt__info(
					  dtReadCGNS(), 
						<< dt__eval(zoneName) << std::endl
						<< dt__eval(zoneSizes[0]) << std::endl
						<< dt__eval(zoneSizes[1]) << std::endl
						<< dt__eval(zoneSizes[2]) << std::endl
						<< dt__eval(nNodes) << std::endl
						<< dt__eval(nCells)
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
						dt__info(
							dtReadCGNS(),
							<< "Reading coordinate " << iCoord+1 << " : " << coordName
						);

						cgsize_t irmin  = 1;
						cgsize_t irmax = zoneSizes[0];
						switch(dataType) {
							case RealSingle:
    						dt__info(dtReadCGNS(), << "Type is float");
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
								dt__info(dtReadCGNS(), << "Type is double");
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
						::MVertex* mv 
            = 
            new ::MVertex(
              nodes[iNode][0], nodes[iNode][1], nodes[iNode][2], 0, vNum
            );
						minVertex = std::min(minVertex, vNum);
						maxVertex = std::max(maxVertex, vNum);
						vertexMap[vNum] = mv;
					}
	
			  //
			  // read sections
				//
				int nSec;
				__cgnsCheck( cg_nsections(index_file, index_base, index_zone, &nSec) );
				dt__info(dtReadCGNS(), << "Found " << nSec << " sections.");
				for (int index_section = 1; index_section <= nSec; index_section++) {
					char secName[30];
					ElementType_t elementType;
					cgsize_t bounds[2];
					int nBoundary;
					int parentFlag = 0;
					__cgnsCheck(
						cg_section_read(
							index_file, index_base, index_zone, index_section,
							secName, &elementType, 
              &(bounds[0]), &(bounds[1]), &nBoundary, &parentFlag
						)
					);
					dt__info(
					  dtReadCGNS(), 
						<< dt__eval(secName) << std::endl
						<< dt__eval(bounds[0]) << std::endl
						<< dt__eval(bounds[1]) << std::endl
						<< dt__eval(nBoundary) << std::endl
						<< dt__eval(parentFlag) 
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

					dt__info(
						dtReadCGNS(), 
						<< dt__eval(elementDataSize) << std::endl
						<< dt__eval(elementType) << std::endl
            << dt__eval(ElementTypeName[elementType])
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
								std::vector< ::MVertex * > locVertices(4, NULL);
								locVertices[0] = vertexMap[elementData[counter+0]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+2]];
								locVertices[3] = vertexMap[elementData[counter+3]];
								::MQuadrangle * me = new ::MQuadrangle(locVertices, eNum, 0);
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
								std::vector< ::MVertex * > locVertices(8, NULL);
								locVertices[0] = vertexMap[elementData[counter+2]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+5]];
								locVertices[3] = vertexMap[elementData[counter+6]];
								locVertices[4] = vertexMap[elementData[counter+3]];
								locVertices[5] = vertexMap[elementData[counter+0]];
								locVertices[6] = vertexMap[elementData[counter+4]];
								locVertices[7] = vertexMap[elementData[counter+7]];
								::MHexahedron * me = new ::MHexahedron(locVertices, eNum, 0);
								minElement = std::min(minElement, eNum);
								maxElement = std::max(maxElement, eNum);
								elementMap[eNum] = me;
								regions.back()->addHexahedron(me);
							}
							break;
						default:
							dt__throw(dtReadCGNS(), << ElementTypeName[elementType]);
					}
				}				
			}
		}
		
		__cgnsCheck( cg_close(index_file) );
		
		dt__throwIf( vertexMap.size() != vNum, dtReadCGNS() );
		dt__throwIf( elementMap.size() != eNum, dtReadCGNS() );
		
		dt__forAllIter(vertexMap_t, vertexMap, it) {
			::MVertex * mv = it->second;
			mv->setEntity(regions.back());
			regions.back()->addMeshVertex(mv);
		}
	}	

	void dtGmshModel::clearModel( void ) {
	  this->destroy(true);
		this->deleteMesh();
	}	

	int dtGmshModel::getMaxVertexTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::viter v_it=GModel::vertices.begin(); 
      v_it!=GModel::vertices.end(); 
      ++v_it 
    ) if ( (*v_it)->tag() > maxTag ) maxTag = (*v_it)->tag();
		return maxTag;
	}
	
	int dtGmshModel::getMaxEdgeTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::eiter e_it= GModel::edges.begin(); 
      e_it!=GModel::edges.end(); 
      ++e_it 
    ) if ( (*e_it)->tag() > maxTag ) maxTag = (*e_it)->tag();
		return maxTag;
	}
	
	int dtGmshModel::getMaxFaceTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::fiter f_it = GModel::faces.begin(); 
      f_it!=GModel::faces.end(); 
      ++f_it 
    ) if ( (*f_it)->tag() > maxTag ) maxTag = (*f_it)->tag();
		return maxTag;
	}	
	
	int dtGmshModel::getMaxRegionTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::riter r_it = GModel::regions.begin(); 
      r_it!=GModel::regions.end(); 
      ++r_it 
    ) if ( (*r_it)->tag() > maxTag ) maxTag = (*r_it)->tag();
		return maxTag;
	}		
	
  std::list< ::GVertex * > dtGmshModel::vertices( void ) const {
		std::list< ::GVertex * > ll;
    for( 
      ::GModel::viter it = GModel::vertices.begin(); 
      it!=GModel::vertices.end(); 
      ++it ) ll.push_back( *it );
		
    return ll;
	}
	
  std::list< ::GEdge * > dtGmshModel::edges( void ) const {
		std::list< ::GEdge * > ll;
    for( 
      ::GModel::eiter it = GModel::edges.begin(); 
      it!=GModel::edges.end(); 
      ++it 
    ) ll.push_back( *it );
    
		return ll;		
	}
		
	std::list< ::GFace * > dtGmshModel::faces( void ) const {
		std::list< ::GFace * > faceL;
    for( 
      ::GModel::fiter f_it = GModel::faces.begin(); 
      f_it!=GModel::faces.end(); 
      ++f_it 
    ) faceL.push_back( *f_it );

		return faceL;
	}
	
	std::list< ::GRegion * > dtGmshModel::regions( void ) const {
		std::list< ::GRegion * > regionL;
    for( 
      ::GModel::riter r_it = GModel::regions.begin(); 
      r_it!=GModel::regions.end(); 
      ++r_it 
    ) regionL.push_back( *r_it );

		return regionL;
	}

  void dtGmshModel::untagPhysical( ::GEntity * const ge ) {
		dt__throwIfWithMessage(
      ge->physicals.size()!=1, 
      untagPhysical(),
      << getPhysicalNames(ge->dim(), ge->physicals)
    );
		
		ge->physicals.clear();
	}
	
  std::string dtGmshModel::getPhysicalString( 
    ::GEntity const * const ge 
  ) const {
		std::vector< int > pInt 
    = 
    const_cast< ::GEntity * >(ge)->getPhysicalEntities();
    
    if(pInt.size() == 0) return std::string("");
    
    dt__throwIfWithMessage(
      pInt.size()!=1, 
      getPhysicalString(),
      << dt__eval(pInt) << std::endl
      << dt__eval( getPhysicalNames(ge->dim(), pInt) )
    );
    
		return ::GModel::getPhysicalName(ge->dim(), pInt[0]);
	}
	
  std::vector< std::string > dtGmshModel::getPhysicalNames(
    int const & dim, std::vector< int > const & number
  ) const {
    std::vector< std::string > names;
    dt__forAllRefAuto(number, aNumber) {
      names.push_back( 
        ::GModel::getPhysicalName(dim, aNumber)
      );
    }
    return names;
  }

  void dtGmshModel::tagPhysical(
    ::GEntity * const ge, std::string const & pName
  ) {		
    //
    // check if already tagged with same string
    //
		std::vector< int > pInt = ge->getPhysicalEntities();
    dt__forAllRefAuto(pInt, anInt) {
      if ( anInt == GModel::setPhysicalName(pName, ge->dim()) ) {
        dt__warning(
          tagPhysical(),
          << "Physical > " << pName << " < already exists."
        );
        return;
      }
    }
    
    ge->addPhysicalEntity( GModel::setPhysicalName(pName, ge->dim()) );
	}
	  
  int dtGmshModel::getPhysicalNumber(
    const int &dim, const std::string &name
  ) const {
    return 
      const_cast< dtGmshModel * >(
        this
      )->GModel::getPhysicalNumber(dim, name);
  }
  
  void dtGmshModel::removeEmptyPhysicals( void ) {
    dtGmshModel::intGEntityVMap ge_number;
    //
    // for each dimension
    //
    dt__forFromToIndex(0, 4, dim) {
      GModel::getPhysicalGroups(dim, ge_number);     
      //
      // for each physical group
      //
      dt__forAllConstIter(dtGmshModel::intGEntityVMap, ge_number, nIt) {
        if (nIt->second.empty()) GModel::deletePhysicalGroup(dim, nIt->first);
      }
    }
  }
  
  void dtGmshModel::setDebug( std::string const debug ) {
    _debug = debug;
  }
  
  std::map< ::GEntity *, ::GEntity * > dtGmshModel::createTwin( 
    int const dim0Tag, int const dim1Tag, int const dim2Tag, int const dim3Tag 
  ) const {
    std::map< dtGmshVertex *, dtGmshVertex * > dim0 = createVertexTwins(dim0Tag);
    std::map< dtGmshEdge *, dtGmshEdge * > dim1 = createEdgeTwins(dim1Tag);
    std::map< dtGmshFace *, dtGmshFace * > dim2 = createFaceTwins(dim2Tag);
    std::map< dtGmshRegion *, dtGmshRegion * > dim3 = createRegionTwins(dim3Tag);
    
    updateAssociations( dim0, dim1, dim2, dim3 );
    
    std::map< ::GEntity *, ::GEntity * > retMap;
    dt__forAllRefAuto(dim0, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim1, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim2, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim3, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    
    return retMap;
  }
 
  std::map< dtGmshVertex *, dtGmshVertex * > 
  dtGmshModel::createVertexTwins( int startTag ) const {
    std::map< dtGmshVertex *, dtGmshVertex * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshVertex(vertices()), aGV ) {
      //
      // clone
      //
      startTag++;
      dtGmshVertex * twin = new dtGmshVertex( NULL, startTag );
      twin->setPosition( dtGmshModel::extractPosition( aGV ) );
      newOld[ aGV ] = twin;
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGV->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }
  
  std::map< dtGmshEdge *, dtGmshEdge * >
  dtGmshModel::createEdgeTwins( int startTag ) const {
    std::map< dtGmshEdge *, dtGmshEdge * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshEdge(edges()), aGE ) {
      //
      // clone
      //
      startTag++;
      dtGmshEdge * twin = new dtGmshEdge( NULL, startTag );
      newOld[ aGE ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGE->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  std::map< dtGmshFace *, dtGmshFace * >
  dtGmshModel::createFaceTwins( int startTag ) const {
    std::map< dtGmshFace *, dtGmshFace * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace(faces()), aGF ) {
      //
      // clone
      //
      startTag++;      
      dtGmshFace * twin = new dtGmshFace( NULL, startTag );
      newOld[ aGF ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGF->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  std::map< dtGmshRegion *, dtGmshRegion * >
  dtGmshModel::createRegionTwins( int startTag ) const {
    std::map< dtGmshRegion *, dtGmshRegion * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshRegion(regions()), aGR ) {
      //
      // clone
      //
      startTag++; 
      dtGmshRegion * twin = new dtGmshRegion( NULL, startTag );
      newOld[ aGR ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGR->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  void dtGmshModel::updateAssociations( 
    std::map< dtGmshVertex *, dtGmshVertex * > & dim0, 
    std::map< dtGmshEdge *, dtGmshEdge * > & dim1,
    std::map< dtGmshFace *, dtGmshFace * > & dim2,
    std::map< dtGmshRegion *, dtGmshRegion * > & dim3
  ) const {
    //
    // vertex associations
    //
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshVertex(vertices()), aGV ) {
      dtGmshVertex * clone = dim0[ aGV ];
      // update edge associations
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshEdge(aGV->edges()), aGE 
      ) {
        clone->addGEntity( dim1[ aGE ] );
      }
    }

    //
    // edge associations
    //
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshEdge(edges()), aGE ) {
      dtGmshEdge * clone = dim1[ aGE ];
      // update begin and end vertex
      clone->setBeginVertex( 
        dim0[ dtGmshVertex::DownCast( aGE->getBeginVertex()) ] 
      );
      clone->setEndVertex( 
        dim0[ dtGmshVertex::DownCast( aGE->getEndVertex()) ] 
      );      
      // update vertices
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshVertex(aGE->vertices()), aGV 
      ) {
        clone->addVertex( dim0[ aGV ] );
      }
      // update faces
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshFace(aGE->faces()), aGF
      ) {
        clone->addFace( dim2[ aGF ] );
      }      
    }

    //
    // face associations
    //
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace(faces()), aGF ) {
      dtGmshFace * clone = dim2[ aGF ];
      // update edges
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshEdge(aGF->edges()), aGE
      ) {
        clone->addEdge( dim1[ aGE ], aGF->edgeOrientation( aGE ) );
      }
      // update regions
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshRegion(aGF->regions()), aGR
      ) {
        clone->addRegion( dim3[ aGR ] );
      }      
    }    

    //
    // region associations
    //
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshRegion(regions()), aGR ) {
      dtGmshRegion * clone = dim3[ aGR ];
      // update faces
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshFace(aGR->faces()), aGF
      ) {
        clone->addFace( dim2[ aGF ], aGR->faceOrientation( aGF ) );
      }
    }        
  }
}