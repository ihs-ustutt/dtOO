#include "readMOABMesh.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <moab/Core.hpp>
#include <moab/CN.hpp>
#include <iostream>

#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <gmsh/MVertex.h>
#include <gmsh/MQuadrangle.h>

namespace dtOO {
	readMOABMesh::readMOABMesh() {
  _mb.reset( new moab::Core() );
	}

	readMOABMesh::~readMOABMesh() {
    
	}
	
  void readMOABMesh::init( 
		::QDomElement const & element,
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
		dt__throwIf(_mb == NULL, makeGrid());

    //
    // create error status
    //
    moab::ErrorCode rval;
    
    //
		// destroy mesh
    //
		rval = _mb->delete_mesh();
		moab__throwIf(rval != moab::MB_SUCCESS, makeGrid());
    
    //
    // load mesh
    //
		rval = _mb->load_mesh(_fileName.c_str());
		moab__throwIf(rval != moab::MB_SUCCESS, makeGrid());	

    //
    // convert mesh to gmsh format
    //    
    convertToGmsh();
    
    //
    // call observers
    //
		boundedVolume::postNotify();
		
    //
    // mark as meshed
    //
    boundedVolume::setMeshed();			    
	}
  
	void readMOABMesh::makePreGrid(void) {
    //
    // call observers
    //
		boundedVolume::preNotify();
	}
  
  vectorHandling< renderInterface * > readMOABMesh::getRender( void ) const {
    return vectorHandling< renderInterface * >(0);
  }
  
 vectorHandling< renderInterface * > readMOABMesh::getExtRender( 
    void 
  ) const {
		vectorHandling< renderInterface * > rV;
    
    if ( _ff_string.find(extRenderWhat()) != _ff_string.end()) {
      dtGmshFace * gf = _ff_string.find( extRenderWhat() )->second;
			std::vector< ::MElement const * > elTwoD;
			for (int jj=0;jj<gf->getNumMeshElements(); jj++) {
			  elTwoD.push_back( gf->getMeshElement(jj) );	
			}
			rV.push_back(
			  dtGmshModel::toAdequateSurfaceRenderInterface(elTwoD)
			);
	  }

		return rV;
	}
    
  
	void readMOABMesh::convertToGmsh( void ) {
    //
    // create type-string map
    //
    std::map< moab::EntityType, std::string > typeString;
    typeString[ moab::MBVERTEX ] = "moab::MBVERTEX";
    typeString[ moab::MBEDGE ] = "moab::MBEDGE";
    typeString[ moab::MBTRI ] = "moab::MBTRI";
    typeString[ moab::MBQUAD ] = "moab::MBQUAD";
    typeString[ moab::MBPOLYGON ] = "moab::MBPOLYGON";
    typeString[ moab::MBTET ] = "moab::MBTET";
    typeString[ moab::MBPYRAMID ] = "moab::MBPYRAMID";
    typeString[ moab::MBPRISM ] = "moab::MBPRISM";
    typeString[ moab::MBKNIFE ] = "moab::MBKNIFE";
    typeString[ moab::MBHEX ] = "moab::MBHEX";
    typeString[ moab::MBPOLYHEDRON ] = "moab::MBPOLYHEDRON";
    typeString[ moab::MBENTITYSET ] = "moab::MBENTITYSET";  
    
    //
    // create error status
    //		
		moab::ErrorCode rval;
		
//    //
//    // create a range
//    //
//		moab::Range aRange;
    
    //
    // logContainer
    //
    logContainer< readMOABMesh > logC(logINFO, "getRender()");
    
    //
    // output
    //
    dt__forAllRefAuto(typeString, aTypeString) {
		  moab::Range aRange;
      rval = _mb->get_entities_by_type(0, aTypeString.first, aRange);
      moab__throwIf(rval != moab::MB_SUCCESS, getRender());
      if ( !aRange.empty() ) { 
        logC() 
          << aRange.size() << " elements of type " << aTypeString.second
          << std::endl;
      }
    }
    
    //
    // create vertices
    //
    _mv_MOAB = createVertices(*_mb);
    
    _ff_string = createFaces(*_mb, _mv_MOAB);
    
//    //
//    // get vertices
//    //
//    aRange.clear();
//		rval = _mb->get_entities_by_type(0, moab::MBVERTEX, aRange);
//		moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//    
//    //
//    // store coordinates in vector
//    //
//		std::vector<double> xx(aRange.size());
//		std::vector<double> yy(aRange.size());
//		std::vector<double> zz(aRange.size());
//		rval = _mb->get_coords(aRange, &xx[0], &yy[0], &zz[0]);
//		moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//		
//    //
//    // create unstructured mesh
//    //
//		unstructured3dMesh * um = new unstructured3dMesh();
//    
//    //
//    // store vertices in unstructured mesh
//    //
//		um->addPoints(
//		  vectorHandling<dtPoint3>(dtLinearAlgebra::toDtPoint3Vector(xx, yy, zz))
//		);
//		
//    //
//    // hexahedrons
//    //
//    aRange.clear();    
//    rval = _mb->get_entities_by_type(0, moab::MBHEX, aRange);
//		moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//		
//    // store in unstructured mesh
//		for (moab::Range::iterator it = aRange.begin(); it != aRange.end(); it++) {
//			moab::EntityHandle const * conn;
//			int nNodes;
//			rval = _mb->get_connectivity(*it, conn, nNodes);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//
//			vectorHandling< int > tmp(nNodes);
//      dt__forAllIndex(tmp, i) tmp[i] = conn[i]-1;
//			um->addElement(tmp);
//    }
//	       
//		int nMeshSets;
//		rval = _mb->num_contained_meshsets(0, &nMeshSets, 0);
//		moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//
//    logC() 
//      << logMe::dtFormat("[ x ] --- MeshSets ( %3i ) --- ") % nMeshSets 
//      << std::endl;    
//		
//		moab::Range allSets;
//    rval = _mb->get_entities_by_type(0, moab::MBENTITYSET, allSets, true);
//		moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//		
//    int meshSetCounter = 0;
//		for (moab::Range::iterator it=allSets.begin(); it!=allSets.end(); it++) {
//		  moab::EntityHandle currentSet = *it;
//      
//      //
//      // get dimension of entities
//      //
//			std::vector< int > dim(3,0);
//			rval = _mb->get_number_entities_by_dimension(currentSet, 1, dim[0], true);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());		
//			rval = _mb->get_number_entities_by_dimension(currentSet, 2, dim[1], true);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());		
//			rval = _mb->get_number_entities_by_dimension(currentSet, 3, dim[2], true);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//
//      logC()
//				<< logMe::dtFormat("[ %i ] handle %i") % meshSetCounter % currentSet
//        << std::endl        
//				<< logMe::dtFormat(
//          "[ %i ] number of entities (1D, 2D, 3D) = (%i, %i, %i)"
//        ) % meshSetCounter % dim[0] % dim[1] % dim[2] 
//        << std::endl;
//      
//      dt__forAllRefAuto(typeString, aTypeString) {
//        aRange.clear();
//        rval = _mb->get_entities_by_type(currentSet, aTypeString.first, aRange);
//        moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//        logC() 
//          << logMe::dtFormat(
//            "[ %i ] %i number of entities of type %s "
//          ) % meshSetCounter % aRange.size() % aTypeString.second
//          << std::endl;
//      }      
//      meshSetCounter++;
//		} 		
		
//		std::vector< moab::Tag > tag_handles;
//		_mb->tag_get_tags(tag_handles);
//
//    logC() 
//      << logMe::dtFormat("[ x ] --- MeshTags ( %3i ) --- ") % tag_handles.size()
//      << std::endl;    
//            
//		dt__forAllIter(std::vector< moab::Tag >, tag_handles, it) {
//			moab::Tag currentTag = *it;
//			std::string tagName;
//			rval = _mb->tag_get_name(currentTag, tagName);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());		
//
//      moab::DataType tagDataType;
//			rval = _mb->tag_get_data_type(currentTag, tagDataType);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());		      
//      
//      int tagLength;
//			rval = _mb->tag_get_length(currentTag, tagLength);
//			moab__throwIf(rval != moab::MB_SUCCESS, getRender());		
//      
//      logC()
//				<< logMe::dtFormat(
//          "[ %s ] length %i, type %s"
//        ) % tagName % tagLength % moab::DataTypeStr[ tagDataType ]
//        << std::endl;
//      
//      dt__forAllRefAuto(typeString, aTypeString) {
//        aRange.clear();
//        rval
//        =
//        _mb->get_entities_by_type_and_tag(
//          0, aTypeString.first, &currentTag, NULL, 1, aRange, 
//          moab::Interface::UNION, true
//        );
//        moab__throwIf(rval != moab::MB_SUCCESS, getRender());
//        logC() 
//          << logMe::dtFormat(
//            "[ %s ] %i number of entities of type %s "
//          ) % tagName % aRange.size() % aTypeString.second
//          << std::endl;    
//      }
//		}
		
//		return vectorHandling< renderInterface * > rV(0);
//		rV[0] = um;
//    
//		return rV;
	}	
  
  std::map< int, ::MVertex * > readMOABMesh::createVertices(
    moab::Interface const & mb
  ) {
    //
    // create error status
    //		
		moab::ErrorCode rval;
    
    //
    // get vertices
    //
    moab::Range aRange;
		rval = mb.get_entities_by_type(0, moab::MBVERTEX, aRange);
		moab__throwIf(rval != moab::MB_SUCCESS, createVertices());
    
    //
    // store coordinates in vector
    //
		std::vector<double> xx(aRange.size());
		std::vector<double> yy(aRange.size());
		std::vector<double> zz(aRange.size());
		rval = mb.get_coords(aRange, &xx[0], &yy[0], &zz[0]);
		moab__throwIf(rval != moab::MB_SUCCESS, createVertices());    

    //
    // global id
    //    
    // get tag
		moab::Tag globalId;
		int zero = 0;
		rval 
		= 
		mb.tag_get_handle(
			"GLOBAL_ID", 1, moab::MB_TYPE_INTEGER,
			globalId, 
			moab::MB_TAG_DENSE | moab::MB_TAG_CREAT, &zero
		);
    moab__throwIf(rval != moab::MB_SUCCESS, createVertices());
    // get data
    std::vector< int > id(aRange.size());
		rval = mb.tag_get_data(globalId, aRange, &id[0]);
    
    //
    // create map
    //
    std::map< int, ::MVertex * > mv_MOAB;
    dt__forAllIndex(id, ii) {
      
//      dt__debug(
//        createVertices(),
//        << logMe::dtFormat(
//          "Create Vertex %i, id = %i,  xyz = (%12.4e, %12.4e, %12.4e"
//        ) % ii % id[ii] % xx[ii] % yy[ii] % zz[ii]
//      );
      mv_MOAB[ id[ii] ] 
      = 
      new ::MVertex(xx[ii], yy[ii], zz[ii], NULL);
    }
    
    return mv_MOAB;
  }
  
  std::map< std::string, dtGmshFace * > readMOABMesh::createFaces( 
    moab::Interface const & mb, std::map< int, ::MVertex * > & mv_MOAB
  ) {
    //
    // create error status
    //		
		moab::ErrorCode rval;    
    
    //
    // logContainer
    //
    logContainer< readMOABMesh > logC(logINFO, "createFaces()");
    
//    //
//    // get all tags
//    //
//		std::vector< moab::Tag > tagHandles;
//		mb.tag_get_tags(tagHandles);
//
//    logC() 
//      << logMe::dtFormat("[ x ] --- MeshTags ( %3i ) --- ") % tagHandles.size()
//      << std::endl;    
//            
//    //
//    // create map between tag and string
//    //
//    std::map< moab::Tag, std::string > string_tag;
//		dt__forAllIter(std::vector< moab::Tag >, tagHandles, it) {
//			moab::Tag currentTag = *it;
//			std::string tagName;
//			rval = mb.tag_get_name(currentTag, tagName);
//			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());	  
//
//      string_tag[ currentTag ] = tagName;
//      
//      moab::DataType tagDataType;
//			rval = mb.tag_get_data_type(currentTag, tagDataType);
//			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());		      
//      
//      int tagLength;
//			rval = mb.tag_get_length(currentTag, tagLength);
//			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());
//      
//      logC()
//				<< logMe::dtFormat(
//          "[ %s ] length %i, type %s"
//        ) % tagName % tagLength % moab::DataTypeStr[ tagDataType ]
//        << std::endl;
//    }

		int nMeshSets;
		rval = mb.num_contained_meshsets(0, &nMeshSets, 0);
		moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

    logC() 
      << logMe::dtFormat("[ x ] --- MeshSets ( %3i ) --- ") % nMeshSets 
      << std::endl;  
    
		moab::Range allSets;
    rval = mb.get_entities_by_type(0, moab::MBENTITYSET, allSets, true);
		moab__throwIf(rval != moab::MB_SUCCESS, createFaces());
		
    
    int meshSetCounter = 0;
    std::map< std::string, dtGmshFace * > ff_string;
		for (moab::Range::iterator it=allSets.begin(); it!=allSets.end(); it++) {
		  moab::EntityHandle currentSet = *it;
      
      //
      // get dimension of entities
      //
			std::vector< int > dim(3,0);
			rval = mb.get_number_entities_by_dimension(currentSet, 1, dim[0], true);
			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());		
			rval = mb.get_number_entities_by_dimension(currentSet, 2, dim[1], true);
			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());		
			rval = mb.get_number_entities_by_dimension(currentSet, 3, dim[2], true);
			moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

      //
      // only 2D elements
      //
      if (dim[1] == 0) continue;
      
      logC()
				<< logMe::dtFormat("[ %i ] handle %i") % meshSetCounter % currentSet
        << std::endl        
				<< logMe::dtFormat(
          "[ %i ] number of entities (1D, 2D, 3D) = (%i, %i, %i)"
        ) % meshSetCounter % dim[0] % dim[1] % dim[2] 
        << std::endl;
      
      dtGmshFace * thisFace = new dtGmshFace(NULL, meshSetCounter+1);
      ff_string[ "meshSet_"+stringPrimitive::intToString(meshSetCounter) ] 
      =
      thisFace;

      moab::Range aRange;
      rval = mb.get_entities_by_type(currentSet, moab::MBQUAD, aRange);
      moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

      logC()
				<< logMe::dtFormat("[ %i ] number of quadrangles %i") 
          % meshSetCounter % aRange.size()
        << std::endl;
      
      for (
        moab::Range::iterator it = aRange.begin(); it != aRange.end(); it++
      ) {   
        //
        // connectivities
        //
        moab::EntityHandle const * conn;
        int nNodes;
        rval = mb.get_connectivity(*it, conn, nNodes);
        moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

        thisFace->addQuadrangle(
          new ::MQuadrangle(
            mv_MOAB[ conn[0] ], 
            mv_MOAB[ conn[1] ], 
            mv_MOAB[ conn[2] ], 
            mv_MOAB[ conn[3] ]
          )
        );
      } 
      meshSetCounter++;
		} 		
    
    return ff_string;
  }
    
  
	std::vector< std::string > readMOABMesh::getMeshTags( void ) const {
		std::vector< std::string > tags;
    dt__forAllRefAuto(_ff_string, aPair) tags.push_back( aPair.first );
    
    return tags;
	}
	
	dtGmshRegion * readMOABMesh::getRegion( std::string const & tag ) const {
		dt__throwUnexpected(getRegion());
	}
	
	dtGmshFace * readMOABMesh::getFace( std::string const & tag ) const {
		//dt__throwUnexpected(getFace());
    return _ff_string.at( tag );
	}

	dtGmshModel * readMOABMesh::getModel( void ) const {
		dt__throwUnexpected(getModel());
	}  
}
