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

//		dt__forFromToIter(
//			omConstEdgeI, 
//			om.edges_begin(), 
//			om.edges_end(),
//			eIt
//		) {
//			omVertexH const from 
//			= 
//		  om.from_vertex_handle( om.halfedge_handle(*eIt,0) );
//			omVertexH const to 
//		  = 
//			om.to_vertex_handle( om.halfedge_handle(*eIt,0) );
//			MVertex const * const mvFrom = om.at(from);
//			MVertex const * const mvTo = om.at(to);
//			_edgeElement.push_back( 
//				new MLine(const_cast< ::MVertex * >(mvFrom), const_cast< ::MVertex * >(mvTo))
//			);
//			moab::Range range
//			=
//			addElements( 
//				std::vector< ::MElement const * >(
//				  1, &(_edgeElement.back())
//				)
//			);
//			_edge_id_map[*eIt] = range.front();
//		}
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
    dt__debug( addVertices(), << "result = " << moab::ErrorCodeStr[ result ] );
  	moab__throwIf(
      (result != moab::MB_SUCCESS)
      &&
      (result != moab::MB_ALREADY_ALLOCATED), 
      addVertices()
    );
		
		//
		// allocate space and get pointer of vertices
		//
		int num_nodes = mv.size();
		std::vector<double*> coord_arrays;
		moab::EntityHandle handle = 0;
		result 
		= 
		_readUtilIface->get_node_coords(3, num_nodes, 1, handle, coord_arrays);
    dt__debug( addVertices(), << "result = " << moab::ErrorCodeStr[ result ] );
  	moab__throwIf(result != moab::MB_SUCCESS, addVertices());
		
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
      dt__warnIfWithMessage(
        !_node_id_map.insert(
          std::pair<long, moab::EntityHandle>(id, handle)
        ).second,
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
    dt__debug( addVertices(), << "result = " << moab::ErrorCodeStr[ result ] );
  	moab__throwIf(result != moab::MB_SUCCESS, addVertices());
		
		ids.clear();
		handles.clear();
	}
	
  moab::Range dtMoabCore::addElements( 
    std::vector< ::MElement const * > const & me 
  ) {
		dt__throwIf(me.size()==0, addElements());

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
    dt__debug( addElements(), << "result = " << moab::ErrorCodeStr[ result ] );
    moab__throwIf(
      (result != moab::MB_SUCCESS)
      &
      (result != moab::MB_ALREADY_ALLOCATED), 
      addElements()
    );
		
    std::vector< dtInt > elem_ids;
		moab::Range elements;    
    dt__forAllIndex(me, ii) {
      //
      // first element gives number of nodes per element
      //
      const unsigned long num_elem = 1;//me.size();
      const dtInt node_per_elem = me[ii]->getNumVertices();

      //
      // get type
      //
      moab::EntityType type;
      ::MQuadrangle const * quad = dynamic_cast< ::MQuadrangle const * >(me[ii]);
      ::MTetrahedron const * tet = dynamic_cast< ::MTetrahedron const * >(me[ii]);
      ::MHexahedron const * hex = dynamic_cast< ::MHexahedron const * >(me[ii]);
      ::MTriangle const * tri = dynamic_cast< ::MTriangle const * >(me[ii]);
      ::MLine const * line = dynamic_cast< ::MLine const * >(me[ii]);		
      dtInt elementDim = 0;
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
      else dt__throw(addElements, << "Unknown element type.");

      //
      // create connectivity list
      //
      std::vector< moab::EntityHandle > connectivity(num_elem*node_per_elem);
      std::vector< ::MVertex * > verts;
      const_cast< ::MElement * >(me[ii])->getVertices(verts);
      dt__forAllIndex(verts, jj) {
        connectivity[jj]	= _node_id_map[verts[jj]->getNum()];
      }

      // Create the element sequence
      moab::EntityHandle handle = 0;
      moab::EntityHandle * conn_array;
      dtInt prefStart = 1;
      result 
      = 
      _readUtilIface->get_element_connect(
        num_elem, node_per_elem, type, prefStart, handle, conn_array
      );
      moab__throwIfWithMessage(
        result != moab::MB_SUCCESS, 
        addElements(),
        << "result = " << moab::ErrorCodeStr[ result ] 
      );
    
      memcpy(
        conn_array, 
        &connectivity[0], connectivity.size() * sizeof(moab::EntityHandle)
      );

      //
      // notify MOAB of the new elements
      // add adjacencies to moab
      //
      result 
      = 
      _readUtilIface->update_adjacencies(
        handle, num_elem, node_per_elem, conn_array
      );
      moab__throwIfWithMessage(
        result != moab::MB_SUCCESS, 
        addElements(),
        << "result = " << moab::ErrorCodeStr[ result ] 
      );
      
      //
      // add handle to range and store element id
      //
      elem_ids.push_back(ii+1);
      elements.insert(handle);
      dt__warnIfWithMessage(
        !_element_id_map.insert(
          std::pair<long, moab::EntityHandle>(ii+1, handle)
        ).second,
        addElements(), 
        << "Duplicate face ID = " << ii+1
      );
    }
    
		//
		// store element IDs in tag
		//
		result = this->tag_set_data(globalId, elements, &elem_ids[0]);
    dt__debug( addElements(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addElements());

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
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< dtInt > val(field.size(), 0);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			field.refMesh().vertices_begin(), 
			field.refMesh().vertices_end(),
			vIt
		) {
			::MVertex const * mv = field.refMesh().at(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = (field.at(mv) ? 1 : 0);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
	}
	
	void dtMoabCore::addVertexField( dtOMVertexField< dtInt > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_INTEGER, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< dtInt > val(field.size(), 0.);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			field.refMesh().vertices_begin(), 
			field.refMesh().vertices_end(),
			vIt
		) {
			::MVertex const * mv = field.refMesh().at(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = field.at(mv);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
	}
	 
	void dtMoabCore::addVertexField( dtOMVertexField< dtReal > const & fF ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			fF.getLabel().c_str(), 1, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< double > val(fF.size(), 0.);
		std::vector< moab::EntityHandle > ent(fF.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			fF.refMesh().vertices_begin(), 
			fF.refMesh().vertices_end(),
			vIt
		) {
			::MVertex const * mv = fF.refMesh().at(*vIt);
			ent[ii] = _node_id_map[mv->getNum()];
			val[ii] = static_cast< double >(fF.at(mv));
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
	}
	
//	void dtMoabCore::addEdgeField( dtOMEdgeField<dtReal> const & eF ) {
//		moab::ErrorCode result;
//		
//		moab::Tag fieldTag;
//  	result 
//		= 
//		tag_get_handle(
//			eF.getLabel().c_str(), 1, moab::MB_TYPE_DOUBLE, 
//		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
//		);
//		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
//		
//		std::vector< double > val(eF.size(), 0.);
//		std::vector< moab::EntityHandle > ent(eF.size());
//		int ii = 0;
//		dt__forFromToIter(
//			omConstEdgeI, 
//			eF.refMesh().edges_begin(), 
//			eF.refMesh().edges_end(),
//			eIt
//		) {
//			ent[ii] = _edge_id_map[*eIt];
//			val[ii] = static_cast< double >(eF[*eIt]);
//		  ii++;
//		}
//		
//		result 
//		=
//		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
//		moab__throwIf(result != moab::MB_SUCCESS, addEdgeField());
//	}	

	void dtMoabCore::addVertexField( dtOMVertexField< dtVector3 > const & vF ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			vF.getLabel().c_str(), 3, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
		
		std::vector< double > val(vF.size()*3, 0.);
		std::vector< moab::EntityHandle > ent(vF.size());
		int ii = 0;
		dt__forFromToIter(
			omConstVertexI, 
			vF.refMesh().vertices_begin(), 
			vF.refMesh().vertices_end(),
			vIt
		) {
			::MVertex const * mv = vF.refMesh().at(*vIt);
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
    dt__debug( addVertexField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addVertexField());
	}	

	void dtMoabCore::addFaceField( dtOMFaceField< dtInt > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_INTEGER, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
		
		std::vector< dtInt > val(field.size(), 0.);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstFaceI, 
			field.refMesh().faces_begin(), 
			field.refMesh().faces_end(),
			it
		) {
//			::MVertex const * mv = field.refMesh().at(*vIt);
			ent[ii] = _element_id_map[ii+1];
			val[ii] = field.at(*it);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
	}  

	void dtMoabCore::addFaceField( dtOMFaceField< dtReal > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_DOUBLE, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
		
		std::vector< double > val(field.size(), 0.);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstFaceI, 
			field.refMesh().faces_begin(), 
			field.refMesh().faces_end(),
			it
		) {
//			::MVertex const * mv = field.refMesh().at(*vIt);
			ent[ii] = _element_id_map[ii+1];
			val[ii] = static_cast< double >(field.at(*it));
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
	}    
  
	void dtMoabCore::addFaceField( dtOMFaceField< bool > const & field ) {
		moab::ErrorCode result;
		
		moab::Tag fieldTag;
  	result 
		= 
		tag_get_handle(
			field.getLabel().c_str(), 1, moab::MB_TYPE_INTEGER, 
		  fieldTag, moab::MB_TAG_SPARSE|moab::MB_TAG_CREAT
		);
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
		
		std::vector< dtInt > val(field.size(), 0.);
		std::vector< moab::EntityHandle > ent(field.size());
		int ii = 0;
		dt__forFromToIter(
			omConstFaceI, 
			field.refMesh().faces_begin(), 
			field.refMesh().faces_end(),
			it
		) {
//			::MVertex const * mv = field.refMesh().at(*vIt);
			ent[ii] = _element_id_map[ii+1];
			val[ii] = (field.at(*it) ? 1 : 0);
		  ii++;
		}
		
		result 
		=
		tag_set_data(fieldTag, &(ent[0]), ent.size(), &(val[0]));
    dt__debug( addFaceField(), << "result = " << moab::ErrorCodeStr[ result ] );
		moab__throwIf(result != moab::MB_SUCCESS, addFaceField());
	}   
}
