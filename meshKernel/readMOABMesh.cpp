#include "readMOABMesh.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <moab/Core.hpp>
#include <moab/CN.hpp>
#include <iostream>

namespace dtOO {
	readMOABMesh::readMOABMesh() {
  _mb.reset( new moab::Core() );
	}

	readMOABMesh::~readMOABMesh() {
	}
	
  void readMOABMesh::init( 
		QDomElement const & element,
    baseContainer const * const bC,					
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< boundedVolume * > const * const depBVolP
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cValP, sFunP, depAGeoP, depBVolP);
		
		_fileName = getOption("mesh_file");
	}
	
  void readMOABMesh::makeGrid(void) {
		dt__THROW_IF(_mb == NULL, makeGrid());

		// Load the mesh from vtk file
		moab::ErrorCode rval;
		rval = _mb->delete_mesh();
		moab__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());		
		rval = _mb->load_mesh(_fileName.c_str());
		moab__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());		
	}
  
	void readMOABMesh::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > readMOABMesh::getRender( void ) const {
		vectorHandling< renderInterface * > rV(1);
		
		moab::ErrorCode rval;
		
		moab::Range verts;
		rval = _mb->get_entities_by_type(0, moab::MBVERTEX, verts);
		
		std::vector<double> xx(verts.size());
		std::vector<double> yy(verts.size());
		std::vector<double> zz(verts.size());
		rval = _mb->get_coords(verts, &xx[0], &yy[0], &zz[0]);
		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
		
		unstructured3dMesh * um = new unstructured3dMesh();
		rV[0] = um;
		um->addPoints(
		  vectorHandling<dtPoint3>(dtLinearAlgebra::toDtPoint3Vector(xx, yy, zz))
		);
		
	  moab::Range hex;
    rval = _mb->get_entities_by_type(0, moab::MBHEX, hex);
		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
		
		for (moab::Range::iterator it = hex.begin(); it != hex.end(); it++) {
			moab::EntityHandle const * conn;
			int nNodes;
			rval = _mb->get_connectivity(*it, conn, nNodes);
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());

//   		DTINFOWF(
//				makeGrid(), 
//				<< DTLOGEVAL(nNodes) << LOGDEL
//				<< DTLOGEVAL(conn) << LOGDEL
//			  << moab::CN::EntityTypeName(_mb->type_from_handle(*it)) << " " 
//				<< _mb->id_from_handle(*it) << " hex connectivity is: "
//			);
			vectorHandling< int > tmp(nNodes);
      for (int i = 0; i < nNodes; i++) tmp[i] = conn[i]-1;
			um->addElement(tmp);
    }
	
		int nMeshSets;
		rval = _mb->num_contained_meshsets(0, &nMeshSets, 0);
		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
		DTINFOWF( getRender(), << DTLOGEVAL(nMeshSets) );			
		
		moab::Range allSets;
    rval = _mb->get_entities_by_type(0, moab::MBENTITYSET, allSets);
		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
		for (moab::Range::iterator it=allSets.begin(); it!=allSets.end(); it++) {
		  moab::EntityHandle currentSet = *it;
			int dim1; int dim2; int dim3;
			rval = _mb->get_number_entities_by_dimension(currentSet, 1, dim1, true);
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
			rval = _mb->get_number_entities_by_dimension(currentSet, 2, dim2, true);
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
			rval = _mb->get_number_entities_by_dimension(currentSet, 3, dim3, true);
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
			DTINFOWF(
				getRender(), 
				<< DTLOGEVAL(dim1) << LOGDEL
				<< DTLOGEVAL(dim2) << LOGDEL
				<< DTLOGEVAL(dim3)
			); 
		} 		
		
		std::vector< moab::Tag > tag_handles;
		_mb->tag_get_tags(tag_handles);
		dt__FORALLITER(std::vector< moab::Tag >, tag_handles, it) {
			moab::Tag currentTag = *it;
			std::string tagName;
			rval = _mb->tag_get_name(currentTag, tagName);
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());				

			moab::Range quadEnts;
			rval
			=
			_mb->get_entities_by_type_and_tag(
				0, moab::MBQUAD, &currentTag, 0, 1, quadEnts
			);			
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());	
			
			moab::Range setEnts;
			rval
			=
			_mb->get_entities_by_type_and_tag(
				0, moab::MBENTITYSET, &currentTag, 0, 1, setEnts
			);			
			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());	
			
			DTINFOWF(
				getRender(), 
				<< logMe::dtFormat("%s has %i of type moab::MBENTITYSET.")
			    % tagName % setEnts.size() << LOGDEL
				<< logMe::dtFormat("%s has %i of type moab::MBQUAD.")
			    % tagName % quadEnts.size() << LOGDEL							
			); 
		}
		
		return rV;
	}	
}
