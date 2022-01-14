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
    dt__forAllRefAuto(edges, aEdge) {
      aEdge->addFace(this);
    }
    std::vector< ::GEdge * > edgeV = progHelper::list2Vector(edges);
    GEdgeLoop theLoop( edgeV );
    theLoop.getEdges(l_edges);
    theLoop.getSigns(l_dirs);
    edgeLoops.push_back(theLoop);
    
    //
    // check
    //
    dt__forAllIndex(edgeV, ii) {
      dt__warnIfWithMessage(
        edgeV[ii] != l_edges[ii], 
        dtGmshFace(), 
        << "Edge[ " << ii << " ] not equal. Automatic change."
      );
      dt__warnIfWithMessage(
        ori[ii] != l_dirs[ii], 
        dtGmshFace(), 
        << "Orientation[ " << ii << " ] not equal. Automatic change."
      );      
    }
  }

  dtGmshFace::dtGmshFace(
    ::GModel *m, int tag, const std::list< ::GEdge * > &edges
  ) : GFace(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
    dt__forAllRefAuto(edges, aEdge) {
      aEdge->addFace(this);
    }
    std::vector< ::GEdge * > edgeV = progHelper::list2Vector(edges);
    GEdgeLoop theLoop( edgeV );
    theLoop.getEdges(l_edges);
    theLoop.getSigns(l_dirs);
    edgeLoops.push_back(theLoop);
  }  

  dtGmshFace::~dtGmshFace() {
    /* 
     * destructor ends in a segmentation fault, because if the model is
     * destroyed before the face no longer exists 
     */
//		dt__info(
//			~dtGmshFace(), << "Removing face tag = " << tag() << " on all edges."	
//		);
//		dt__forAllRefAuto(edges(), ee) ee->delFace(this);
//		   
//		dt__info(
//			~dtGmshFace(), << "Removing face tag = " << tag() << " on all regions."	
//		);
//		dt__forAllRefAuto(dtRegions(), rr) rr->deleteFace( this );
//    
//    dt__info(
//			~dtGmshFace(), << "Removing face tag = " << tag() << " on model."	
//		);    
//		if ( model() ) model()->remove(this);    
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
    dtVector3 ddU = _mm->firstDerU( (dtReal) param.x(), (dtReal) param.y() );
    dtVector3 ddV = _mm->firstDerV( (dtReal) param.x(), (dtReal) param.y() );
    
    return Pair<SVector3, SVector3>( 
      SVector3( (double) ddU.x(), (double) ddU.y(), (double) ddU.z()),
      SVector3( (double) ddV.x(), (double) ddV.y(), (double) ddV.z()) 
    );
  }

  void dtGmshFace::secondDer(
    const SPoint2 &param, SVector3 & dudu, SVector3 & dvdv, SVector3 & dudv
  ) const {
    dtVector3 ddUddU = _mm->secondDerUU( (dtReal) param.x(), (dtReal) param.y() );
    dtVector3 ddVddV = _mm->secondDerVV( (dtReal) param.x(), (dtReal) param.y() );    
    dtVector3 ddUddV = _mm->secondDerUV( (dtReal) param.x(), (dtReal) param.y() );    
    dudu = SVector3(ddUddU.x(), ddUddU.y(), ddUddU.z());
    dvdv = SVector3(ddVddV.x(), ddVddV.y(), ddVddV.z());
    dudv = SVector3(ddUddV.x(), ddUddV.y(), ddUddV.z()); 
  }

  dtVector3 dtGmshFace::normal( dtPoint2 const & uv ) const {
    ::SVector3 s3 = GFace::normal( ::SPoint2( uv.x(), uv.y() ) );  
    return dtVector3( s3.x(), s3.y(), s3.z() );
  }
  
  GPoint dtGmshFace::point(double par1, double par2) const {
    double pp[2] = {par1, par2};
    GPoint gp;
    if ( isnan(par1) || isnan(par2) ) {
      gp = GPoint(1.e21, 1.e21, 1.e21, this, pp);
      gp.setNoSuccess();
      dt__warning(
        point(), 
        << "par1 = " << par1 << ", par2 = " << par2 << std::endl
        << "Return GPoint with NoSuccess."
      );
    }
    else {
      dtPoint3 retPoint = _mm->getPoint(par1, par2);
      gp = GPoint(retPoint.x(), retPoint.y(), retPoint.z(), this, pp);
    }
    return gp;
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
    typedef std::vector< ::GEdge * >::const_iterator EIter;
    std::vector< int >::const_iterator OriIter = l_dirs.begin();

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
  
  GPoint dtGmshFace::closestPoint(
    const SPoint3 &queryPoint, const double initialGuess[2]
  ) const {
    SPoint2 p = GFace::parFromPoint(queryPoint, false);
    return point( p.x(), p.y() );
  }
   
  bool dtGmshFace::isClosed( int const dim ) const {
    if (dim == 0) {
      return _mm->isClosedU();
    }
    else if (dim == 1) {
      return _mm->isClosedV();
    }
    else dt__throw(
           isClosed(),
           << dt__eval(dim) << std::endl
           << "dim should be 0 or 1."
          );
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
		std::vector< dtGmshEdge * > ee = dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(ee.size()!=4, meshWNElements());
		
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
		std::vector< dtGmshEdge * > ee = dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(ee.size()!=4, meshWNElements());
		
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
		
		std::vector< dtGmshEdge * > ee = dtGmshModel::cast2DtGmshEdge(edges());		
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
			dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace( ee[0]->faces() ), aFace) {
				aFace->correctIfTransfinite();
			}
			dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace( ee[2]->faces() ), aFace) {
				aFace->correctIfTransfinite();
			}				
		}
		if (correct[1]) {
			dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace( ee[1]->faces() ), aFace) {
				aFace->correctIfTransfinite();
			}
			dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace( ee[3]->faces() ), aFace) {
				aFace->correctIfTransfinite();
			}					
		}		
	}	

  void dtGmshFace::meshUnstructured( void ) {
    this->meshAttributes.method = MESH_UNSTRUCTURED;
  }  
	
  std::vector< int > dtGmshFace::estimateTransfiniteNElements( 
	  dtReal const & uWidth, dtReal const & vWidth 
	) const {	
		std::vector< dtGmshEdge * > const ee 
    = 
    dtGmshModel::cast2DtGmshEdge(edges());
		
		//
		// only supported for 4-sided faces
		//
		dt__throwIf(ee.size()!=4, meshTransfiniteWNElements());
		
		//
		// set number of elements
		//
		bool toggle = false;
		std::vector< dtReal > average(2, 0.);
		dt__forAllRefAuto(ee, anEdge) {
			if ( !toggle) {
				average[0] = average[0] + anEdge->getMap1dTo3d()->length()/uWidth;
				toggle = true;
			}
			else {
				average[1] = average[1] + anEdge->getMap1dTo3d()->length()/vWidth;
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
		std::vector< ::GVertex * > VL0 = gf0->vertices();
		std::vector< ::GVertex * > VL1 = gf1->vertices();
		
		if (VL0.size() != VL1.size()) {
			return false;
		}
		
		int counter = 0;
		std::vector< ::GVertex * >::iterator V0_it;
		std::vector< ::GVertex * >::iterator V1_it;
		for (V0_it = VL0.begin(); V0_it != VL0.end(); ++V0_it) {
      for (V1_it = VL1.begin(); V1_it != VL1.end(); ++V1_it) {
				if ( dtGmshVertex::isEqual(*V0_it, *V1_it) ) {
					counter++;
				}
			}
		}
		
		if (VL0.size() == counter) {
      if ( 
        staticPropertiesHandler::getInstance()->optionTrue("isEqualExtendCheck") 
      ) {
        ::GPoint p0 
        = 
        gf0->point( 
          0.5 * ( gf0->parBounds(0).low() + gf0->parBounds(0).high() ),
          0.5 * ( gf0->parBounds(1).low() + gf0->parBounds(1).high() )                
        );
        double iG[] = {0.5,0.5};
        ::GPoint p1 = gf1->closestPoint( ::SPoint3(p0.x(), p0.y(), p0.z()), iG);
        
        dtReal const dist
        =
        dtLinearAlgebra::distance(
          dtPoint3(p0.x(), p0.y(), p0.z()), 
          dtPoint3(p1.x(), p1.y(), p1.z())
        );
        bool differentPoint
        =
        dist 
        >
        staticPropertiesHandler::getInstance()->getOptionFloat(
          "XYZ_resolution"
        );
        dt__debug(
          isEqual(), 
          << logMe::dtFormat("p0 = (%6.2f, %6.2f, %6.2f)") 
            % p0.x() % p0.y() % p0.z() 
          << std::endl
          << logMe::dtFormat("p1 = (%6.2f, %6.2f, %6.2f)") 
            % p1.x() % p1.y() % p1.z()
          << std::endl
          << "|p0 - p1| = " << dist
          << std::endl
          << "p0 != p1 = " << differentPoint
        );
        if ( differentPoint ) return false;
      }
			return true;
		}
		else {
			return false;
		}		
	}
	
	bool dtGmshFace::sortPredicate(::MVertex const * d1, ::MVertex const * d2) {
		return d1->getNum() < d2->getNum();
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
    std::vector< dtReal > const & grading, std::vector< dtReal > & type
  ) {
    dt__throwIf(grading.size() != 4, setGrading());
    dt__throwIf(type.size() != 4, setGrading());
		
//    dt__debug(
//      setGrading(),
//      << "Face[ " << tag() << " / " << getPhysicalString()
//      << " ] : grading = " << grading << ", type = " << type
//    );
    
    int ii = 0;
    dt__forAllRefAuto(dtEdges(), ee) {
      ee->setGrading(grading[ii], type[ii]);
      ii++;
		}
  }

  std::list< dtGmshVertex * > dtGmshFace::dtVertices( void ) const {
    return progHelper::vector2List(
      dtGmshModel::cast2DtGmshVertex(vertices())
    );
	}
	
  std::list< dtGmshEdge * > dtGmshFace::dtEdges( void ) const {
    return progHelper::vector2List(
      dtGmshModel::cast2DtGmshEdge(edges())
    );
	}  

  std::list< dtGmshRegion * > dtGmshFace::dtRegions( void ) const {
    return dtGmshModel::cast2DtGmshRegion( regions() );
	}
}
