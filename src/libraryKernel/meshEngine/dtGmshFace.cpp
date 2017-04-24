#include "dtGmshFace.h"

#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshModel.h"
#include "dtOMMesh.h"
#include "dtGmshRegion.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include <gmsh/MElement.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MVertex.h>
#include <list>
#include <map>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/intHandling.h>

#define __caCThis const_cast< dtGmshFace * >(this)

namespace dtOO {    
  dtGmshFace::dtGmshFace(::GModel *m, int tag) 
  : GFace(m, tag), 
    _geomType( ::GEntity::GeomType::Unknown ) {
    
  }
	
  dtGmshFace::dtGmshFace(
    ::GModel *m, int tag, 
    const std::list< ::GEdge * > &edges, const std::vector< int > &ori 
  ) : GFace(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
    edgeLoops.push_back(GEdgeLoop(edges));
    typedef std::list< ::GEdge * >::const_iterator EIter;
    int ii = 0;
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge *e = *ei;
      l_edges.push_back(e);
      e->addFace(this);
      l_dirs.push_back(ori[ii]);
      ii++;
    }
  }

  dtGmshFace::dtGmshFace(
    ::GModel *m, int tag, const std::list< ::GEdge * > &edges
  ) : GFace(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
    edgeLoops.push_back(::GEdgeLoop(edges));
    typedef std::list< ::GEdge * >::const_iterator EIter;
    for (EIter ei=edges.begin(); ei != edges.end(); ++ei) {
      ::GEdge *e = *ei;
      l_edges.push_back(e);
      e->addFace(this);
      l_dirs.push_back(1);
    }
  }  

  dtGmshFace::~dtGmshFace() {
    /* 
     * destructor ends in a segmentation fault, because if the model is
     * destroyed before the face no longer exists 
     */
//		dt__info(
//			~dtGmshFace(), << "Removing face tag = " << tag() << " on all edges."	
//		);
//		std::list< ::GEdge * > ee = edges();
//		dt__forAllIter(std::list< ::GEdge * >, ee, it) {
//			(*it)->delFace(this);
//		}
//		this->model()->remove(this);
//		std::list< ::GRegion * > rr = regions();
//		dt__forAllIter(std::list< ::GRegion * >, rr, it) {
//			std::list< ::GFace * > ff = (*it)->faces();
//			std::list< ::GFace * >::iterator itF = std::find(ff.begin(), ff.end(), this);
//			if(itF != ff.end()) ff.erase(itF);		
//		}		
  }
  
  dtGmshModel const & dtGmshFace::refDtGmshModel( void ) const {
    dt__ptrAss(
      dtGmshModel const * const gm, dtGmshModel::ConstDownCast(model())
    );
    
    return *gm;
  }
  
  ::GEntity::GeomType dtGmshFace::geomType( void ) const {
    return _geomType;
  }
  
  void dtGmshFace::setGeomType( ::GEntity::GeomType const & gT ) {
    _geomType = gT;
  }  
  
  dtGmshModel & dtGmshFace::refDtGmshModel( void ) {
    dt__ptrAss(dtGmshModel * gm, dtGmshModel::DownCast(model()));
    
    return *gm;
  }
  
  Range<double> dtGmshFace::parBounds(int i) const {
    if (i == 0) {
      return Range<double>(_mm->getUMin(), _mm->getUMax());
    }    
    if (i == 1) {
      return Range<double>(_mm->getVMin(), _mm->getVMax());
    }
    else {
      dt__throw(parBounds(),
              << dt__eval(i) << std::endl
              << "i should be 0 or 1.");
    }
  }

  Pair<SVector3, SVector3> dtGmshFace::firstDer(const SPoint2 &param) const {
    dtVector3 ddU = _mm->firstDerU( (float) param.x(), (float) param.y() );
    dtVector3 ddV = _mm->firstDerV( (float) param.x(), (float) param.y() );
    
    return Pair<SVector3, SVector3>( 
      SVector3( (double) ddU.x(), (double) ddU.y(), (double) ddU.z()),
      SVector3( (double) ddV.x(), (double) ddV.y(), (double) ddV.z()) 
    );
  }

  void dtGmshFace::secondDer(
    const SPoint2 &param, SVector3 *dudu, SVector3 *dvdv, SVector3 *dudv
  ) const {
    dtVector3 ddUddU = _mm->secondDerUU( (float) param.x(), (float) param.y() );
    dtVector3 ddVddV = _mm->secondDerVV( (float) param.x(), (float) param.y() );    
    dtVector3 ddUddV = _mm->secondDerUV( (float) param.x(), (float) param.y() );    
    *dudu = SVector3(ddUddU.x(), ddUddU.y(), ddUddU.z());
    *dvdv = SVector3(ddVddV.x(), ddVddV.y(), ddVddV.z());
    *dudv = SVector3(ddUddV.x(), ddUddV.y(), ddUddV.z()); 
  }

  dtVector3 dtGmshFace::normal( dtPoint2 const & uv ) const {
    ::SVector3 s3 = GFace::normal( ::SPoint2( uv.x(), uv.y() ) );  
    return dtVector3( s3.x(), s3.y(), s3.z() );
  }
  
  GPoint dtGmshFace::point(double par1, double par2) const {
    double pp[2] = {par1, par2};
    
    dtPoint3 retPoint = _mm->getPoint(par1, par2);
    
    return GPoint(retPoint.x(), retPoint.y(), retPoint.z(), this, pp);
  }

  SPoint2 dtGmshFace::reparamOnFace(dtPoint3 const ppXYZ) const {
    dtPoint2 ppUV = _mm->reparamOnFace( ppXYZ );
    
    return SPoint2(ppUV.x(), ppUV.y());
  }

  SPoint2 dtGmshFace::reparamOnFace(::GVertex const * gv) const {
    dtPoint3 pp(gv->x(), gv->y(), gv->z());
    
    return reparamOnFace(pp);
  }  
  
  void dtGmshFace::setMap2dTo3d( map2dTo3d const * const base ) {
    _geomType = ::GEntity::GeomType::ParametricSurface;
    _mm.reset( base->clone() );
  }

  map2dTo3d const * dtGmshFace::getMap2dTo3d( void ) const {
    return _mm.get();
  }
  
  void dtGmshFace::addEdge( ::GEdge * edge, int const ori ) {
    l_edges.push_back( edge );
    edge->addFace(this);
    l_dirs.push_back( ori );
    edgeLoops.clear();
    edgeLoops.push_back( ::GEdgeLoop(l_edges) );
  }  

  int dtGmshFace::edgeOrientation( ::GEdge * edge ) const {
    typedef std::list< ::GEdge * >::const_iterator EIter;
    std::list< int >::const_iterator OriIter = l_dirs.begin();

    for (EIter ei = l_edges.begin(); ei != l_edges.end(); ++ei) {
      if (*ei == edge) return *OriIter;
      ++OriIter;
    }    
  }    
	
  void dtGmshFace::addEdgeLoop( std::list< ::GEdge * > edgeL ) {
    addEdge( edgeL.front(), 1);
    ::GVertex * gv = edgeL.front()->getEndVertex();
    edgeL.erase( edgeL.begin() );
    for (int ii=0;edgeL.size();ii++) {
      for(
        std::list< ::GEdge * >::iterator it = edgeL.begin(); 
        it != edgeL.end(); 
        ++it 
      ) {
        if ( (*it)->getBeginVertex() == gv ) {
          addEdge(*it, 1);
          gv = (*it)->getEndVertex();
          edgeL.erase(it);
          break;
        }
        if ( (*it)->getEndVertex() == gv ) {
          addEdge(*it, -1);
          gv = (*it)->getBeginVertex();
          edgeL.erase(it);
          break;
        }
      }
    }
    if (edgeL.size() != 0) {
      dt__throw(addEdgeLoop(),
              << dt__eval( edgeL.size() ) );
    }
  }  
  
  SPoint2 dtGmshFace::parFromPoint(const SPoint3 &p, bool onSurface) const {
    dtPoint3 pp( p.x(), p.y(), p.z() );
    
    return reparamOnFace( pp );
  }
  
  std::list< ::GEdge * > dtGmshFace::edges( void ) const { 
    return l_edges;
  }
  
  bool dtGmshFace::isClosed( int const dim ) const {
    if (dim == 0) {
      return _mm->isClosedU();
    }
    else if (dim == 1) {
      return _mm->isClosedV();
    }
    else {
      dt__throw(isClosed(),
              << dt__eval(dim) << std::endl
              << "dim should be 0 or 1.");
    }    
  }
  
  void dtGmshFace::meshTransfinite( void ) {
      this->meshAttributes.method = MESH_TRANSFINITE;
  }
	
	void dtGmshFace::meshRecombine( void ) {
		this->meshAttributes.recombine = 1;
	}
  
  void dtGmshFace::meshWNElements( 
    int const & nElementsU, int const & nElementsV 
  ) {	
		std::list< dtGmshEdge * > eeList = dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(eeList.size()!=4, meshWNElements());
		std::vector< dtGmshEdge * > ee = progHelper::list2Vector(eeList);
		
		//
		// set number of elements
		//
		ee[0]->meshTransfiniteWNElements(1, 1., nElementsU);
    ee[1]->meshTransfiniteWNElements(1, 1., nElementsV);
		ee[2]->meshTransfiniteWNElements(1, 1., nElementsU);
    ee[3]->meshTransfiniteWNElements(1, 1., nElementsV);		
	}

  void dtGmshFace::meshWNElements( 
    int const & nElements0, int const & nElements1, 
    int const & nElements2, int const & nElements3 
  ) {	
		std::list< dtGmshEdge * > eeList = dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(eeList.size()!=4, meshWNElements());
		std::vector< dtGmshEdge * > ee = progHelper::list2Vector(eeList);
		
		//
		// set number of elements
		//
		ee[0]->meshTransfiniteWNElements(1, 1., nElements0);
    ee[1]->meshTransfiniteWNElements(1, 1., nElements1);
		ee[2]->meshTransfiniteWNElements(1, 1., nElements2);
    ee[3]->meshTransfiniteWNElements(1, 1., nElements3);		
	}  
	
  void dtGmshFace::correctIfTransfinite( void ) {
		//
		// only correct transfinite surfaces
		//
		if (GFace::meshAttributes.method != MESH_TRANSFINITE) return;
		
		std::list< dtGmshEdge * > eeList = dtGmshModel::cast2DtGmshEdge(edges());
    std::vector< dtGmshEdge * > ee = progHelper::list2Vector(eeList);		
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(ee.size()!=4, meshTransfiniteWNElements());

		std::vector< bool > correct(2, false);
    if (
		  ee[0]->meshAttributes.nbPointsTransfinite !=
			ee[2]->meshAttributes.nbPointsTransfinite
		) {
			correct[0] = true;
		}		
    if (
		  ee[1]->meshAttributes.nbPointsTransfinite !=
			ee[3]->meshAttributes.nbPointsTransfinite
		) {
			correct[1] = true;
		}			
		
		//
		// correct number of elements
		//
		ee[0]->meshAttributes.nbPointsTransfinite
		= 
		std::max(
		  ee[0]->meshAttributes.nbPointsTransfinite, 
			ee[2]->meshAttributes.nbPointsTransfinite
		);
		ee[1]->meshAttributes.nbPointsTransfinite 
		= 
		std::max(
		  ee[1]->meshAttributes.nbPointsTransfinite, 
			ee[3]->meshAttributes.nbPointsTransfinite
		);
		ee[2]->meshAttributes.nbPointsTransfinite 
		= 
		ee[0]->meshAttributes.nbPointsTransfinite;
		ee[3]->meshAttributes.nbPointsTransfinite
		= 
		ee[1]->meshAttributes.nbPointsTransfinite;
		
		if (correct[0]) {
			std::list< dtGmshFace * > faces0 
      = 
      dtGmshModel::cast2DtGmshFace(ee[0]->faces());
			std::list< dtGmshFace * > faces2 
      = 
      dtGmshModel::cast2DtGmshFace(ee[2]->faces());
			dt__forAllIter(std::list< dtGmshFace * >, faces0, it) {
				(*it)->correctIfTransfinite();
			}
			dt__forAllIter(std::list< dtGmshFace * >, faces2, it) {
				(*it)->correctIfTransfinite();
			}				
		}
		if (correct[1]) {
			std::list< dtGmshFace * > faces1 
      = 
      dtGmshModel::cast2DtGmshFace(ee[1]->faces());
			std::list< dtGmshFace * > faces3 
      = 
      dtGmshModel::cast2DtGmshFace(ee[3]->faces());
			dt__forAllIter(std::list< dtGmshFace * >, faces1, it) {
				(*it)->correctIfTransfinite();
			}
			dt__forAllIter(std::list< dtGmshFace * >, faces3, it) {
				(*it)->correctIfTransfinite();
			}				
		}		
	}	

  void dtGmshFace::meshUnstructured( void ) {
    this->meshAttributes.method = MESH_UNSTRUCTURED;
  }  
	
  std::vector< int > dtGmshFace::estimateTransfiniteNElements( 
	  float const & uWidth, float const & vWidth 
	) const {	
		std::list< dtGmshEdge * > const ee = dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(ee.size()!=4, meshTransfiniteWNElements());
		
		//
		// set number of elements
		//
		bool toggle = false;
		std::vector< float > average(2, 0.);
		dt__forAllConstIter(std::list< dtGmshEdge * >, ee, it) {
			if ( !toggle) {
				average[0] = average[0] + (*it)->getMap1dTo3d()->length()/uWidth;
				toggle = true;
			}
			else {
				average[1] = average[1] + (*it)->getMap1dTo3d()->length()/vWidth;
				toggle = false;
			}			
		}

    std::vector< int > nEl(2);
    nEl[0] = std::max(intHandling::round(average[0]/2.), 1); 		
		nEl[1] = std::max(intHandling::round(average[1]/2.), 1); 		
		
		return nEl;
	}	
	
	bool dtGmshFace::isEqual( ::GFace const * const gf ) const {
    return isEqual(this, gf);
	}

  bool dtGmshFace::isEqual( 
    ::GFace const * const gf0, ::GFace const * const gf1 
  ) {	
		std::list< ::GVertex * > VL0 = gf0->vertices();
		std::list< ::GVertex * > VL1 = gf1->vertices();
		
		if (VL0.size() != VL1.size()) {
			return false;
		}
		
		int counter = 0;
		std::list< ::GVertex * >::iterator V0_it;
		std::list< ::GVertex * >::iterator V1_it;
		for (V0_it = VL0.begin(); V0_it != VL0.end(); ++V0_it) {
      for (V1_it = VL1.begin(); V1_it != VL1.end(); ++V1_it) {
				if ( dtGmshVertex::isEqual(*V0_it, *V1_it) ) {
					counter++;
				}
			}
		}
		
		if (VL0.size() == counter) {
			return true;
		}
		else {
			return false;
		}		
	}
	
	bool dtGmshFace::sortPredicate(::MVertex const * d1, ::MVertex const * d2) {
		return d1->getNum() < d2->getNum();
	}
	
	twoDArrayHandling< ::MVertex * > dtGmshFace::reconstructEdgesFromSurfaceMesh( 
    void 
  ) const {
    std::map< ::MVertex *, std::vector< ::MElement * >  > e_v;
		std::map< ::MVertex *, std::vector< ::MElement * >  >::iterator e_vIt;
		std::map< ::MElement *, std::vector< ::MVertex * >  > v_e;
		std::map< ::MElement *, std::vector< ::MVertex * >  >::iterator v_eIt;
		
		//
		// create e_v => in: vertex out: element
		// create v_e => in: element out: vertex
		//
		for (int ii=0; ii<__caCThis->getNumMeshElements(); ii++) {
			::MElement * me = __caCThis->getMeshElement(ii);
			std::vector< ::MVertex * > verts;
			me->getVertices(verts);
			for (int jj=0; jj<verts.size(); jj++) {
				e_vIt = e_v.find( verts[jj] );
				if (e_vIt == e_v.end()) {
					e_v[verts[jj]] 
					= 
					std::vector< ::MElement * >(1,me);
				}
				else e_vIt->second.push_back(me);
				v_eIt = v_e.find( me );
				if (v_eIt == v_e.end()) {
					v_e[me] = std::vector< ::MVertex * >(1,verts[jj]);
				}
				else v_eIt->second.push_back(verts[jj]);				
			}
		}
		
//		dt__info(reconstructEdgesFromSurfaceMesh(), << logMe::mapToTable(e_v));
//		dt__info(reconstructEdgesFromSurfaceMesh(), << logMe::mapToTable(v_e));
		
		std::vector< ::MVertex * > startVerts;
		for (e_vIt = e_v.begin(); e_vIt != e_v.end(); ++e_vIt) {
			if (e_vIt->second.size() == 1) {
        startVerts.push_back(e_vIt->first);
			}
		}
		std::sort(startVerts.begin(), startVerts.end(), dtGmshFace::sortPredicate);
		
		twoDArrayHandling< ::MVertex * > recEdges;
		
		while ( !startVerts.empty() ) {
			//
			// set start vertex
			//
			e_vIt = e_v.find(startVerts.back()); 
			
			//
			// initialize reconstructed edge vector
			//
			recEdges.push_back( std::vector< ::MVertex * >(0) );
			std::vector< ::MVertex * > & recEdge = recEdges.back();
			recEdge.push_back(e_vIt->first);
			::MElement * wE;
			wE = e_vIt->second[0];

			//
			// perform reconstruction
			//
			do {
				//
				// get a reference to the vertices
				//
				std::vector< ::MVertex * > & pV = v_e[wE];
				//
				// iterate over all vertices
				//
				bool endVertex = false;
				::MVertex * toSave = NULL;
				for (int ii=0; ii<pV.size(); ii++) {
					if (pV[ii] == recEdge.back()) continue;
					
					//
					// found an internal edge vertex
					//
					if ( e_v[pV[ii]].size() == 2) {
				    toSave = pV[ii];						
//						dt__info(
//							reconstructEdgesFomSurface(),
//							<< logMe::dtFormat("Internal vertex %d") % recEdge.back()->getNum()
//						);
					}
					//
					// found the end vertex
					//
					else if ( e_v[pV[ii]].size() == 1) {
				    toSave = pV[ii];						
						endVertex = true;
//						dt__info(
//							reconstructEdgesFomSurface(),
//							<< logMe::dtFormat("End vertex %d") % recEdge.back()->getNum()
//						);
						break;
					}
				}
				//
				// check if there is something to save
				//
				if (toSave) {
					//
					// save vertex in edge reconstruction
					//
					recEdge.push_back(toSave);
					if (!endVertex) {
						if (e_v[toSave][0] == wE) wE = e_v[toSave][1];
						else wE = e_v[toSave][0];
						//
						// erase internal mapping from vertex to element
						//
						e_v[toSave].clear();
					}
				}
			}
			while ( e_v[recEdge.back()].size() != 1 );
						
			//
			// delete start vertex if there is no connection to another end vertex
			//
			if (recEdge.size() == 1) {
				e_v[recEdge[0]].clear();
				recEdges.erase( progHelper::prior(recEdges.end()) );
				startVerts.erase( progHelper::prior(startVerts.end()) );
			}			
			if (recEdge.size() == 2) {
				dt__throwIf(recEdge[0]!=recEdge[1], reconstructEdgesFromSurfaceMesh());
				e_v[recEdge[0]].clear();
				recEdges.erase( progHelper::prior(recEdges.end()) );
				startVerts.erase( progHelper::prior(startVerts.end()));
			}
		}

//	  dt__info( 
//		  reconstructFromEdges(), 
//			<< logMe::dtFormat("%d edges reconstructed.") % recEdges.size()
//		);    		
		for (
			twoDArrayHandling< ::MVertex * >::iterator0 it = recEdges.begin(); 
			it!=recEdges.end(); 
			++it 
		) {
			dt__info(
				reconstructEdgesFromSurfaceMesh(), 
				<< logMe::dtFormat(
				  "Edge from vertex %d to vertex %d reconstructed with %d vertices"
				) % it->at(0)->getNum() % it->back()->getNum() % it->size()
			); 	
		}		
		
		return recEdges;
	}
	
	std::vector< const ::MVertex * > dtGmshFace::getMeshVertices( void ) const {	
		int num_vertices  = __caCThis->getNumMeshVertices();
		dt__throwIf(num_vertices == 0, getMesVertices());

		std::vector< ::MVertex const * > mv;
		mv.reserve(num_vertices);
    for (int ii=0; ii<num_vertices; ii++) {
			mv.push_back( __caCThis->getMeshVertex(ii) );
		}
		
		return mv;
	}
	
	void dtGmshFace::getMeshVerticesAndElements(
		std::vector< ::MVertex const * > * const mv, 
		std::vector< ::MElement const * > * const me
	) const {	
		int num_elements  = __caCThis->getNumMeshElements();
		
		dt__throwIf(num_elements == 0, getMeshVerticesAndElements());
		
		int num_vertices_per_element 
		= 
		__caCThis->getMeshElement(0)->getNumVertices();
		
		//
		// reserve size
		//
		mv->reserve(mv->size() + num_elements*num_vertices_per_element);
		me->reserve(me->size() + num_elements);
		
    for (int ii=0; ii<num_elements; ii++) {
			me->push_back( __caCThis->getMeshElement(ii) );
			
			std::vector< ::MVertex * > vertices;
			const_cast< ::MElement *>(me->back())->getVertices(vertices);
			dt__forAllIter(std::vector< ::MVertex * >, vertices, aV) {
        mv->push_back(*aV);	
      }
		}		
	}
	
	dtOMMesh * dtGmshFace::getOMMesh( void ) const {
		dtOMMesh * om = new dtOMMesh();
			
    for (int ii=0; ii<__caCThis->getNumMeshElements(); ii++) {
			std::vector< MVertex * > vertices;
			MElement * me = __caCThis->getMeshElement(ii);
			me->getVertices(vertices);
			om->addFace(me);
		}
		
		return om;
	}
	
	void dtGmshFace::addElement( ::MElement * me ) {
		::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(me);
		::MTriangle * mtri = dynamic_cast< ::MTriangle * >(me);
		
		if (mquad) addQuadrangle(mquad);
		else if (mtri) addTriangle(mtri);
		else dt__throw(addElement(), << "Try to add unsupported element.");
	}

	void dtGmshFace::addGEntity( ::GEntity * const gEnt ) {
		//
		// cast
		//
		dtGmshEdge * ge = dtGmshEdge::DownCast(gEnt);
		dtGmshRegion * gr = dtGmshRegion::DownCast(gEnt);
		dt__throwIf( (ge==NULL)&&(gr==NULL), addGEntity() );

		if (ge) {
			// add this face to edge
      ge->addFace(this);
      // add edge to this face
			this->addEdge(ge, 1);
		}
		else {
			// add this face to region
      gr->addFace(this, 1);
      // add region to this face
			if ( (getRegion(0)!=gr) && (getRegion(1)!=gr) ) addRegion(gr);
		}
	}	
  
  std::string dtGmshFace::getPhysicalString( void ) const {
    dt__ptrAss( 
      dtGmshModel const * const gm, 
      dtGmshModel::ConstDownCast(__caCThis->model()) 
    );
    return gm->getPhysicalString(this);
  }

  void dtGmshFace::setGrading(
    std::vector< float > const & grading, std::vector< float > & type
  ) {
    dt__throwIf(grading.size() != 4, setGrading());
    dt__throwIf(type.size() != 4, setGrading());
		
    std::list< GEdge * > ee = edges();
    int ii = 0;
    dt__forAllIter(std::list< GEdge * >, ee, it) {
      if (type[ii] != 0) {
        (*it)->meshAttributes.coeffTransfinite = grading[ii];
        (*it)->meshAttributes.typeTransfinite = type[ii];
      }
      ii++;
		}
  }

  std::list< dtGmshVertex * > dtGmshFace::dtVertices( void ) const {
    return refDtGmshModel().cast2DtGmshVertex( vertices() );
	}
	
  std::list< dtGmshEdge * > dtGmshFace::dtEdges( void ) const {
    return refDtGmshModel().cast2DtGmshEdge( edges() );
	}  
}
