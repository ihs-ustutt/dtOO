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
		moab::ErrorCode rval = _mb->load_mesh(_fileName.c_str());
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
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
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
		
		unstructured3dMesh * um = new unstructured3dMesh();
		rV[0] = um;
		um->addPoints(
		  vectorHandling<dtPoint3>(dtLinearAlgebra::toDtPoint3Vector(xx, yy, zz))
		);
		
	  moab::Range hex;
    rval = _mb->get_entities_by_type(0, moab::MBHEX, hex);
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
		
		for (moab::Range::iterator it = hex.begin(); it != hex.end(); it++) {
			moab::EntityHandle const * conn;
			int nNodes;
			rval = _mb->get_connectivity(*it, conn, nNodes);
			dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());

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
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
		DTINFOWF( getRender(), << DTLOGEVAL(nMeshSets) );			
		
		int dim1; int dim2; int dim3;
    rval = _mb->get_number_entities_by_dimension(0, 1, dim1, true);
		rval = _mb->get_number_entities_by_dimension(0, 2, dim2, true);
		rval = _mb->get_number_entities_by_dimension(0, 3, dim3, true); 
		DTINFOWF(
			getRender(), 
			<< DTLOGEVAL(dim1) << LOGDEL
			<< DTLOGEVAL(dim2) << LOGDEL
			<< DTLOGEVAL(dim3)
		);
		
    rval = _mb->get_number_entities_by_dimension(1, 1, dim1, true);
		rval = _mb->get_number_entities_by_dimension(1, 2, dim2, true);
		rval = _mb->get_number_entities_by_dimension(1, 3, dim3, true);
		DTINFOWF(
			getRender(), 
			<< DTLOGEVAL(dim1) << LOGDEL
			<< DTLOGEVAL(dim2) << LOGDEL
			<< DTLOGEVAL(dim3)
		); 
		return rV;
	}	
}
