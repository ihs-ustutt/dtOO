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
#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	dtGmshRegion::dtGmshRegion(::GModel *m, int tag) : GRegion(m, tag) {
	  _status = ::GEntity::MeshGenerationStatus::PENDING;	
	}
	
  dtGmshRegion::dtGmshRegion(::GModel *m, int tag, const std::list< ::GFace * > &faces, const std::vector<int> &ori )
    : GRegion(m, tag) {
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

  dtGmshRegion::dtGmshRegion(::GModel *m, int tag, const std::list< dtGmshFace* > &faces, const std::vector<int> &ori )
    : GRegion(m, tag) {
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
		for (std::list< ::GFace * >::iterator f_it = fl.begin(); f_it != fl.end(); ++f_it) {
			std::list< ::GEdge * > el = (*f_it)->edges();
			for (std::list< ::GEdge * >::iterator e_it = el.begin(); e_it != el.end(); ++e_it) {
				(*e_it)->meshAttributes.method = MESH_UNSTRUCTURED;
			}
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
		
		float xyzRes 
		= 
		staticPropertiesHandler::getInstance()->getOptionFloat(
      "xyz_resolution"
    );
		int counter = 0;
		std::list< ::GVertex * >::iterator V0_it;
		std::list< ::GVertex * >::iterator V1_it;
		for (V0_it = VL0.begin(); V0_it != VL0.end(); ++V0_it) {
			dtPoint3 v0((*V0_it)->x(), (*V0_it)->y(), (*V0_it)->z());
      for (V1_it = VL1.begin(); V1_it != VL1.end(); ++V1_it) {
				dtPoint3 v1((*V1_it)->x(), (*V1_it)->y(), (*V1_it)->z());
				if ( dtLinearAlgebra::distance(v0, v1) < xyzRes ) {
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
		
    std::list< GEdge * > ee = edges();
//		for (int ii=0; ii<grading.size(); ii++) {
    int ii = 0;
    dt__forAllIter(std::list< GEdge * >, ee, it) {
      if (type[ii]!=2.) (*it)->meshAttributes.coeffTransfinite = grading[ii];
			else (*it)->meshAttributes.coeffTransfinite = grading[ii];
			(*it)->meshAttributes.typeTransfinite = type[ii];
      ii++;
		}
  }	
	
  void dtGmshRegion::setGrading( 
	  float const & gU, float const & gV, float const & gW,
	  float const & tU, float const & tV, float const & tW
  ) {
    std::vector< float > grading(12);
		std::vector< float > type(12);
		//
		// types
		//
		type[0] = tU; type[2] = tU;
		type[6] = tU; type[4] = tU;
		type[3] = tV; type[1] = tV;
		type[5] = tV; type[7] = tV;		
		type[8] = tW; type[9] = tW;
		type[10] = tW; type[11] = tW;				
    //
		// gradings
		//
		if (tU!=2.) {
			grading[0] = gU; grading[2] = 1./gU;
			grading[6] = 1./gU; grading[4] = gU;
		}
		else {
			grading[0] = gU; grading[2] = gU;
			grading[6] = gU; grading[4] = gU;			
		}
		if (tV!=2.) {
			grading[3] = 1./gV; grading[1] = gV;
			grading[5] = gV; grading[7] = 1./gV;		
		}
		else {
			grading[3] = gV; grading[1] = gV;
			grading[5] = gV; grading[7] = gV;					
		}
		grading[8] = gW; grading[9] = gW;
		grading[10] = gW; grading[11] = gW;						
		
		setGrading(grading, type);
  }	  
}
