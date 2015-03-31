#include "dtMoabCore.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <gmsh/MElement.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MVertex.h>
#include <gmsh/MLine.h>
#include "dtGmshFace.h"
#include "dtOMVertexField.h"

#include <moab/ReadUtilIface.hpp>


namespace dtOO {
	dtMoabCore::dtMoabCore() : moab::Core() {
		query_interface(_readUtilIface);
	}
	
	/**
	 * @todo Implementation of edges is not very well. Should not be done by
	 * additionally add ::MLine elements.
   */
	dtMoabCore::dtMoabCore( dtOMMesh const & om ) {
		query_interface(_readUtilIface);
		
		//
		// vertices
		//
		std::vector< ::MVertex const * > vertices(om.n_vertices());		
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			om.vertices_begin(), 
			om.vertices_end(),
			vIt
		) {
			vertices[ii] = om.at(*vIt);
		  ii++;
		}
		addVertices(vertices);
		
		//
		// elements
		//
		ii = 0;
		std::vector< ::MElement const * > elements(om.n_faces());		
		dt__forFromToIter(
			omConstFaceI, 
			om.faces_begin(), 
			om.faces_end(),
			fIt
		) {
			elements[ii] = om.at(*fIt);
		  ii++;
		}
		addElements(elements);

		dt__forFromToIter(
			omConstEdgeI, 
			om.edges_begin(), 
			om.edges_end(),
			eIt
		) {
			omVertexH const from 
			= 
		  om.from_vertex_handle( om.halfedge_handle(*eIt,0) );
			omVertexH const to 
		  = 
			om.to_vertex_handle( om.halfedge_handle(*eIt,0) );
			MVertex const * const mvFrom = om.at(from);
			MVertex const * const mvTo = om.at(to);
			_edgeElement.push_back( 
				new MLine(const_cast< ::MVertex * >(mvFrom), const_cast< ::MVertex * >(mvTo))
			);
			moab::Range range
			=
			addElements( 
				std::vector< ::MElement const * >(
				  1, &(_edgeElement.back())
				)
			);
			_edge_id_map[*eIt] = range.front();
		}
	}

	dtMoabCore::~dtMoabCore() {
		if (_readUtilIface) {
			release_interface(_readUtilIface);
			_readUtilIface = NULL;
		}		
	}
	
  void dtMoabCore::addVertices( std::vector< ::MVertex const * > const & mv ) {
		//
		// get global id tag
		//
		moab::Tag globalId;
		int zero = 0;
		moab::ErrorCode result 
		= 
		this->tag_get_handle(
			"GLOBAL_ID", 1, moab::MB_TYPE_INTEGER,
			globalId, 
			moab::MB_TAG_DENSE | moab::MB_TAG_CREAT, &zero
		);
  	moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());
		
		//
		// allocate space and get pointer of vertices
		//
		int num_nodes = mv.size();
		std::vector<double*> coord_arrays;
		moab::EntityHandle handle = 0;
		result 
		= 
		_readUtilIface->get_node_coords(3, num_nodes, 1, handle, coord_arrays);
  	moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());
		
		//
		// insert vertices
		//
		double * x = coord_arrays[0];
		double * y = coord_arrays[1];
    double * z = coord_arrays[2];
		for (long i = 0; i < num_nodes; ++i, ++handle) {
			long id = mv[i]->getNum();
			x[i] = mv[i]->x();
			y[i] = mv[i]->y();
			z[i] = mv[i]->z();

				//
				// ignore duplicate nodes
				// 
				dt__WARN_IFWM(
					!_node_id_map.insert(std::pair<long, moab::EntityHandle>(id, handle)).second,
					addVertices(), 
					<< "Duplicate node ID = " << id
				);
			}

		//
		// create reverse map from handle to id
		//
		std::vector<int> ids(num_nodes);
		std::vector<int>::iterator id_iter = ids.begin();
		std::vector<moab::EntityHandle> handles(num_nodes);
		std::vector<moab::EntityHandle>::iterator h_iter = handles.begin();
		for (std::map<long, moab::EntityHandle>::iterator i = _node_id_map.begin();
				i != _node_id_map.end(); ++i, ++id_iter, ++h_iter) {
			*id_iter = i->first;
			*h_iter = i->second;
		}
		
		//
		// store IDs in global id tag
		//
		result = this->tag_set_data(globalId, &handles[0], num_nodes, &ids[0]);
  	moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());
		
		ids.clear();
		handles.clear();
	}
	
  moab::Range dtMoabCore::addElements( std::vector< ::MElement const * > const & me ) {
		dt__THROW_IF(me.size()==0, addElements());

		//
		// get global id tag
		//		
		moab::ErrorCode result;
		moab::Tag globalId;
		int zero = 0;
		result 
		= 
		this->tag_get_handle(
			"GLOBAL_ID", 1, moab::MB_TYPE_INTEGER,
			globalId, 
			moab::MB_TAG_DENSE | moab::MB_TAG_CREAT, &zero
		);
    moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());
		
		//
		// first element gives number of nodes per element
		//
		const unsigned long num_elem = me.size();
		const int node_per_elem = me[0]->getNumVertices();

		//
		// get type
		//
		moab::EntityType type;
		::MQuadrangle const * quad = dynamic_cast< ::MQuadrangle const * >(me[0]);
		::MTetrahedron const * tet = dynamic_cast< ::MTetrahedron const * >(me[0]);
		::MHexahedron const * hex = dynamic_cast< ::MHexahedron const * >(me[0]);
		::MTriangle const * tri = dynamic_cast< ::MTriangle const * >(me[0]);
		::MLine const * line = dynamic_cast< ::MLine const * >(me[0]);		
		int elementDim = 0;
		if (quad) {
			elementDim = 2;
			type = moab::MBQUAD;
		}
		else if (tet) {
			elementDim = 3;
			type = moab::MBTET;
		}
		else if (hex) {
			elementDim = 3;
			type = moab::MBHEX;
		}
		else if (tri) {
			elementDim = 2;
			type = moab::MBTRI;
		}		
		else if (line) {
			elementDim = 1;
			type = moab::MBEDGE;
		}				
		else dt__THROW(addElements, "Unknown element type.");

		//
		// create connectivity list
		//
		std::vector< moab::EntityHandle > connectivity(num_elem*node_per_elem);
		std::vector< int > elem_ids; 
		elem_ids.resize(num_elem);
		int counter = 0;
		int elCount;
    this->get_number_entities_by_dimension(0, elementDim, elCount, true);
		dt__forAllConstIter(std::vector< ::MElement const * >, me, it) {
			::MElement const * thisElement = *it;
			elem_ids.push_back(elCount+1);
			elCount++;
			std::vector< ::MVertex * > verts;
			const_cast< ::MElement * >(thisElement)->getVertices(verts);
			dt__forAllIndex(verts, ii) {
				connectivity[counter]	= _node_id_map[verts[ii]->getNum()];
				counter++;
			}
		}

		// Create the element sequence
		moab::EntityHandle handle = 0;
		moab::EntityHandle * conn_array;
		result 
		= 
		_readUtilIface->get_element_connect(
			num_elem, node_per_elem, type, 1, handle, conn_array
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());

		memcpy(conn_array, &connectivity[0], connectivity.size() * sizeof(moab::EntityHandle));

		//
	  // notify MOAB of the new elements
		// add adjacencies to moab
		//
		result 
		= 
		_readUtilIface->update_adjacencies(handle, num_elem, node_per_elem, conn_array);
		moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());

		//
		// store element IDs
		//
		moab::Range elements(handle, handle + num_elem - 1);
		result = this->tag_set_data(globalId, elements, &elem_ids[0]);
		moab__THROW_IF(result != moab::MB_SUCCESS, makeGrid());

		//
		// return new added range
		//
		return elements;
	}
	
	void dtMoabCore::addVertexField( dtOMVertexField< bool > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_INTEGER, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< int > val(field.size(), 0);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			field.refMesh().vertices_begin(), 
			field.refMesh().vertices_end(),
			vIt
		) {
			::MVertex * mv = field.refMesh().requestMVertex(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = (field.at(mv) ? 1 : 0);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
	}
	
	void dtMoabCore::addVertexField( dtOMVertexField< int > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_INTEGER, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< int > val(field.size(), 0.);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			field.refMesh().vertices_begin(), 
			field.refMesh().vertices_end(),
			vIt
		) {
			::MVertex * mv = field.refMesh().requestMVertex(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = field.at(mv);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
	}
	
	void dtMoabCore::addVertexField( dtOMVertexField<float> const & fF ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			fF.getLabel().c_str(), 1, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< double > val(fF.size(), 0.);
		std::vector< moab::EntityHandle > ent(fF.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			fF.refMesh().vertices_begin(), 
			fF.refMesh().vertices_end(),
			vIt
		) {
			::MVertex * mv = fF.refMesh().requestMVertex(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = static_cast< double >(fF.at(mv));
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
	}
	
	void dtMoabCore::addEdgeField( dtOMEdgeField<float> const & eF ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			eF.getLabel().c_str(), 1, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< double > val(eF.size(), 0.);
		std::vector< moab::EntityHandle > ent(eF.size());
		int ii = 0;
		dt__forFromToIter(
			omConstEdgeI, 
			eF.refMesh().edges_begin(), 
			eF.refMesh().edges_end(),
			eIt
		) {
			ent[ii] = _edge_id_map[*eIt];
			val[ii] = static_cast< double >(eF[*eIt]);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
		moab__THROW_IF(result != moab::MB_SUCCESS, addEdgeField());
	}	

	void dtMoabCore::addVertexField( dtOMVertexField< dtVector3 > const & vF ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			vF.getLabel().c_str(), 3, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< double > val(vF.size()*3, 0.);
		std::vector< moab::EntityHandle > ent(vF.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			vF.refMesh().vertices_begin(), 
			vF.refMesh().vertices_end(),
			vIt
		) {
			::MVertex * mv = vF.refMesh().requestMVertex(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			dtVector3 const & vec = vF.at(mv);
			val[ii*3+0] = vec.x();
			val[ii*3+1] = vec.y();
			val[ii*3+2] = vec.z();
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
		moab__THROW_IF(result != moab::MB_SUCCESS, addVertexField());
	}	
}
