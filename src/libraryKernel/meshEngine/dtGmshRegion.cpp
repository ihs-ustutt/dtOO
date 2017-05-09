#include "dtGmshRegion.h"

#include <gmsh/GmshDefines.h>
#include <gmsh/GFace.h>
#include <gmsh/GModel.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MPrism.h>
#include <gmsh/MHexahedron.h>
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <progHelper.h>

#define __caCThis const_cast< dtGmshRegion * >(this)

namespace dtOO {
	dtGmshRegion::dtGmshRegion(::GModel *m, int tag) 
    : GRegion(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
	  _status = ::GEntity::MeshGenerationStatus::PENDING;
	}
	
  dtGmshRegion::dtGmshRegion(
    ::GModel *m, int tag, 
    const std::list< ::GFace * > &faces, 
    const std::vector<int> &ori 
  ) 
    : GRegion(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
    typedef std::list< ::GFace * >::const_iterator FIter;
    int ii = 0;
    for (FIter fi=faces.begin(); fi != faces.end(); ++fi) {
      ::GFace *f = *fi;
      l_faces.push_back(f);
      f->addRegion(this);
      l_dirs.push_back( ori[ii] );
      ii++;
    }
		_status = ::GEntity::MeshGenerationStatus::PENDING;
  }

  dtGmshRegion::dtGmshRegion(
    ::GModel *m, int tag, 
    const std::list< dtGmshFace* > &faces, 
    const std::vector<int> &ori 
  ) 
    : GRegion(m, tag),
      _geomType( ::GEntity::GeomType::Unknown ) {
    typedef std::list< dtGmshFace* >::const_iterator FIter;
    int ii = 0;
    for (FIter fi=faces.begin(); fi != faces.end(); ++fi) {
      ::GFace * f = static_cast< dtGmshFace* >(*fi);
      l_faces.push_back(f);
      f->addRegion(this);
      l_dirs.push_back( ori[ii] );
      ii++;
    }
		_status = ::GEntity::MeshGenerationStatus::PENDING;
  }
  
  dtGmshModel const & dtGmshRegion::refDtGmshModel( void ) const {
    dt__ptrAss(
      dtGmshModel const * const gm,
      dtGmshModel::ConstDownCast(model())
    );
    
    return *gm;
  }
  
  ::GEntity::GeomType dtGmshRegion::geomType( void ) const {
    return _geomType;
  }
  
  void dtGmshRegion::setGeomType( ::GEntity::GeomType const & gT ) {
    _geomType = gT;
  }
    
   
  void dtGmshRegion::meshTransfinite( void ) {
    this->meshAttributes.method = MESH_TRANSFINITE;
//    this->meshAttributes.recombine3D = 1;
//		std::list< ::GFace * > fl = faces();
//		for (std::list< ::GFace * >::iterator f_it = fl.begin(); f_it != fl.end(); ++f_it) {
//			std::list< ::GEdge * > el = (*f_it)->edges();
//			for (std::list< ::GEdge * >::iterator e_it = el.begin(); e_it != el.end(); ++e_it) {
//				(*e_it)->meshAttributes.method = MESH_TRANSFINITE;
//				(*e_it)->meshAttributes.nbPointsTransfinite = 2;
//				(*e_it)->meshAttributes.coeffTransfinite = 1.;
//				(*e_it)->meshAttributes.typeTransfinite = 1.;				
//			}
//			(*f_it)->meshAttributes.method = MESH_TRANSFINITE;
//			(*f_it)->meshAttributes.recombine = 1;
//		}
  }
  
  void dtGmshRegion::meshTransfiniteRecursive( void ) {
    this->meshAttributes.method = MESH_TRANSFINITE;
		dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace( this->faces() ), aFace ) {
      aFace->meshTransfinite();
		}
  }
  
  void dtGmshRegion::meshWNElements( 
    int const & nElementsU, int const & nElementsV, int const & nElementsW
  ) {
		//
		// only supported for 6-sided dtGmshRegions
		//    
		std::vector< dtGmshFace * > ff 
    = 
    progHelper::list2Vector( dtGmshModel::cast2DtGmshFace(faces()) );
		dt__throwIf(ff.size()!=6, meshWNElements());
		
		//
		// set number of elements
		//
		ff[0]->meshWNElements(nElementsU, nElementsV);
    ff[1]->meshWNElements(nElementsU, nElementsV);
		ff[2]->meshWNElements(nElementsU, nElementsW);
    ff[3]->meshWNElements(nElementsU, nElementsW);    
		ff[4]->meshWNElements(nElementsV, nElementsW);
    ff[5]->meshWNElements(nElementsV, nElementsW);	    
  }  
  void dtGmshRegion::meshRecombine( void ) {
    this->meshAttributes.recombine3D = 1;
  }  
	
  void dtGmshRegion::meshRecombineRecursive( void ) {
    this->meshAttributes.recombine3D = 1;
		std::list< ::GFace * > fl = faces();
    dt__forAllIter(std::list< ::GFace * >, fl, f_it) {
			(*f_it)->meshAttributes.recombine = 1;
		}    
  }  
  
  void dtGmshRegion::meshUnstructured( void ) {
    this->meshAttributes.method = MESH_UNSTRUCTURED;
    this->meshAttributes.recombine3D = 0;
		std::list< ::GFace * > fl = faces();
		for (
      std::list< ::GFace * >::iterator f_it = fl.begin(); 
      f_it != fl.end(); 
      ++f_it
    ) {
			std::list< ::GEdge * > el = (*f_it)->edges();
			for (
        std::list< ::GEdge * >::iterator e_it = el.begin(); 
        e_it != el.end(); 
        ++e_it
      ) (*e_it)->meshAttributes.method = MESH_UNSTRUCTURED;
			(*f_it)->meshAttributes.method = MESH_UNSTRUCTURED;
			(*f_it)->meshAttributes.recombine = 0;
		}
  }
	
  void dtGmshRegion::addFace( ::GFace * face, int const ori ) {
	  if (std::find(l_faces.begin(), l_faces.end(), face) == l_faces.end()) {
		  l_faces.push_back(face);
			l_dirs.push_back( ori );
		}
  }  	
	
  int dtGmshRegion::faceOrientation( ::GFace * face ) const {
    typedef std::list< ::GFace * >::const_iterator FIter;
    std::list< int >::const_iterator OriIter = l_dirs.begin();

    for (FIter fi = l_faces.begin(); fi != l_faces.end(); ++fi) {
      if (*fi == face) return *OriIter;
      ++OriIter;
    }    
  }      
  
	void dtGmshRegion::addGEntity( ::GEntity * const gEnt ) {
		//
		// cast
		//		
		dt__ptrAss(dtGmshFace * gf, dtGmshFace::DownCast(gEnt));
		
		// add this region to face
		gf->addRegion(this);
		// add face to this region
		addFace(gf, 1);
	}
	
	void dtGmshRegion::addElement( ::MElement * me ) {
	  ::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
		::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
		::MPyramid * mpyr = dynamic_cast< ::MPyramid * >(me);
		::MPrism * mpri = dynamic_cast< ::MPrism * >(me);
    //
    // tetrahedron
    //
    if (mtet) addTetrahedron(mtet);
    else if (mhex) addHexahedron(mhex);
    else if (mpyr) addPyramid(mpyr);
    else if (mpri) addPrism(mpri);
    else dt__throwUnexpected(addElement());
	}
  
	bool dtGmshRegion::isEqual( ::GRegion const * const gr ) const {
    return isEqual(this, gr);
	}
  
  bool dtGmshRegion::isEqual( 
    ::GRegion const * const gr0, ::GRegion const * const gr1 
  ) {	
		std::list< ::GVertex * > VL0 = gr0->vertices();
		std::list< ::GVertex * > VL1 = gr1->vertices();
		
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
  
  void dtGmshRegion::replaceFace( 
    ::GFace const * const toReplace, ::GFace * const with 
  ) {
    std::list< ::GFace * > faces = l_faces;
    dt__forAllIter(std::list< GFace * >, faces, it) {
      if ( (*it) == toReplace ) (*it) = with;
    }
    replaceFaces(faces);
  }  

	std::string dtGmshRegion::dumpToString( void ) const {
		std::stringstream ss;
		
    std::list< ::GFace * > ff = faces(); 
    dt__forAllConstIter(std::list< ::GFace * >, ff, it) {
		  ss << "face: " << refDtGmshModel().getPhysicalString(*it) << std::endl;
		}
		return ss.str();
	}	  

  void dtGmshRegion::setGrading( 
    std::vector< float > const & grading, std::vector< float > & type 
  ) {
    dt__throwIf(grading.size() != 12, setGrading());
    dt__throwIf(type.size() != 12, setGrading());
		
    dt__debug(
      setGrading(),
      << "Region[ " << tag() << " / " << getPhysicalString()
      << " ] : grading = " << grading << ", type = " << type
    );
    
    int ii = 0;
    dt__forAllRefAuto(dtEdges(), ee) {
      ee->setGrading( grading[ii], type[ii] );
      ii++;
		}
  }	
  
  std::list< dtGmshFace const * > dtGmshRegion::constFaceList(
    std::vector< std::string > const & label
  ) const {
		std::list< dtGmshFace const * > faceList;
		dt__forAllConstIter(std::vector< std::string >, label, it) {
		  faceList.push_back( refDtGmshModel().getDtGmshFaceByPhysical(*it) );
		}
    
    return faceList;
  }    

  std::list< dtGmshFace * > dtGmshRegion::faceList(
    std::vector< std::string > const & label
  ) {
		std::list< dtGmshFace * > faceList;
		dt__forAllConstIter(std::vector< std::string >, label, it) {
		  faceList.push_back( refDtGmshModel().getDtGmshFaceByPhysical(*it) );
		}
    
    return faceList;
  }  

  std::string dtGmshRegion::getPhysicalString( void ) const {
    dt__ptrAss( 
      dtGmshModel const * const gm, 
      dtGmshModel::ConstDownCast(__caCThis->model()) 
    );
    return gm->getPhysicalString(this);
  }  

  std::list< dtGmshFace * > dtGmshRegion::dtFaces( void ) const {
    return refDtGmshModel().cast2DtGmshFace( faces() );
	}    

  std::list< dtGmshEdge * > dtGmshRegion::dtEdges( void ) const {
    return refDtGmshModel().cast2DtGmshEdge( edges() );
	}      
}
