#include "postBLMeshKit.h"
//#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <moab/Core.hpp>
#include <moab/CN.hpp>
#include <meshkit/MKCore.hpp>
#include <meshkit/MeshOp.hpp>
#include <meshkit/RegisterMeshOp.hpp>
#include <meshkit/ModelEnt.hpp>
#include <meshEngine/dtPostBL.h>
#include <meshkit/SizingFunction.hpp>
#include <meshkit/CopyGeom.hpp>

namespace dtOO {
	postBLMeshKit::postBLMeshKit() {
	}

	postBLMeshKit::~postBLMeshKit() {
	}
	
  void postBLMeshKit::init( 
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
		
		_pBLFile = getOption("postBL_file");
	}
	
  void postBLMeshKit::makeGrid(void) {
		try {
			_mk.reset( new MeshKit::MKCore() );
			_mk->moab_instance()->load_file("dieterherbert.msh");
			
			moab::ErrorCode rval;
			moab::Interface * mb = _mk->moab_instance();
			moab::Tag materialTag;
			rval = mb->tag_get_handle("MATERIAL_SET", materialTag);
			moab__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
//			int materialTagLength;
//			rval = mb->tag_get_length(materialTag, materialTagLength);
			moab__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());			
//				DTINFOWF(
//					makeGrid(), 
//          << DTLOGEVAL(materialTagLength));			
//			for (int ii=0; ii<materialTagLength; ii++) {
//				int const materialTagValue[1];
//				int numberMaterialTagValue;
//				materialTagValue[0] = ii+1;
				moab::Range matEnts;
				rval
				=
				mb->get_entities_by_type_and_tag(
					0, 
					moab::MBENTITYSET, &materialTag, 0, 1,//&numberMaterialTagValue,
					matEnts
				);
//				DTINFOWF(
//					makeGrid(), 
//					<< dtOO::logMe::dtFormat("numberMaterialTagValue %i has %i entries.") 
//						   % 1 % matEnts.size()
//				);
//		  }
			
//			mb->get_entities_by_type_and_tag()
//			moab::Range allSets;
//			rval = mb->get_entities_by_type(0, moab::MBENTITYSET, allSets);
//			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());	

			moab::Range commonRangePos;
      moab::Range commonRangeNeg;
			
			moab::Range::iterator it;
  	  moab::EntityHandle currentSet;
			moab::Range neuEnts;
			
			it=matEnts.begin();
		  neuEnts.clear();			
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangePos.merge(neuEnts);

      it++;
		  neuEnts.clear();			
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangeNeg.merge(neuEnts);
			
      it++;
		  neuEnts.clear();
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangeNeg.merge(neuEnts);	
			
			it++;
			neuEnts.clear();
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangePos.merge(neuEnts);
//
      it++;
		  neuEnts.clear();			
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangeNeg.merge(neuEnts);
//			
      it++;
		  neuEnts.clear();			
			currentSet = *it;
			mb->get_entities_by_dimension(currentSet, 2, neuEnts);
			commonRangePos.merge(neuEnts);	
			
			moab::Tag neuTag;
			moab::EntityHandle neuEntSet;			
			int m_NeumannSet;
			mb->create_meshset(moab::MESHSET_SET, neuEntSet);
			mb->tag_get_handle("NEUMANN_SET", 1, moab::MB_TYPE_INTEGER, neuTag);
			mb->add_entities(neuEntSet, commonRangePos);
			m_NeumannSet = 999999;
			mb->tag_set_data(neuTag, &neuEntSet, 1, (void*) &m_NeumannSet);				
			mb->create_meshset(moab::MESHSET_SET, neuEntSet);
			mb->tag_get_handle("NEUMANN_SET", 1, moab::MB_TYPE_INTEGER, neuTag);
			mb->add_entities(neuEntSet, commonRangeNeg);
			m_NeumannSet = -999999;
			mb->tag_set_data(neuTag, &neuEntSet, 1, (void*) &m_NeumannSet);						
			
//			MeshKit::MeshOpSet::instance()->register_mesh_op(&dtPBL);
			//! create a model entity vector for construting PostBL meshop, note that model entities(mesh) input for PostBL meshop is read from a file.
			MeshKit::MEntVector volso;

			//! construct the meshop and set name
			MeshKit::dtPostBL * pbl = (MeshKit::dtPostBL*) _mk->construct_meshop("dtPostBL", volso);
			pbl->set_name("PostBL");

			//!setup and execute PostBL graph node, point the executable to PostBL input file,
			int argc = 2;
			char ** argv = new char*[2];
			argv[0] = const_cast< char * >("antoineInTheBuilding");
			argv[1] = const_cast< char * >(_pBLFile.c_str());
			pbl->PrepareIO(argc, argv, "");
			pbl->setup_this();
			pbl->execute_this();
			_mk->save_mesh("out_postbl.vtk");
			delete pbl;
		}
		meshkit__CATCH(makeGrid);
		
		boundedVolume::setMeshed();
	}
  
	void postBLMeshKit::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > postBLMeshKit::getRender( void ) const {
		return vectorHandling< renderInterface * >(0);
//		
//		moab::ErrorCode rval;
//		
//		moab::Range verts;
//		rval = _mb->get_entities_by_type(0, moab::MBVERTEX, verts);
//		
//		std::vector<double> xx(verts.size());
//		std::vector<double> yy(verts.size());
//		std::vector<double> zz(verts.size());
//		rval = _mb->get_coords(verts, &xx[0], &yy[0], &zz[0]);
//		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
//		
//		unstructured3dMesh * um = new unstructured3dMesh();
//		rV[0] = um;
//		um->addPoints(
//		  vectorHandling<dtPoint3>(dtLinearAlgebra::toDtPoint3Vector(xx, yy, zz))
//		);
//		
//	  moab::Range hex;
//    rval = _mb->get_entities_by_type(0, moab::MBHEX, hex);
//		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
//		
//		for (moab::Range::iterator it = hex.begin(); it != hex.end(); it++) {
//			moab::EntityHandle const * conn;
//			int nNodes;
//			rval = _mb->get_connectivity(*it, conn, nNodes);
//			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
//
////   		DTINFOWF(
////				makeGrid(), 
////				<< DTLOGEVAL(nNodes) << LOGDEL
////				<< DTLOGEVAL(conn) << LOGDEL
////			  << moab::CN::EntityTypeName(_mb->type_from_handle(*it)) << " " 
////				<< _mb->id_from_handle(*it) << " hex connectivity is: "
////			);
//			vectorHandling< int > tmp(nNodes);
//      for (int i = 0; i < nNodes; i++) tmp[i] = conn[i]-1;
//			um->addElement(tmp);
//    }
//	
//		int nMeshSets;
//		rval = _mb->num_contained_meshsets(0, &nMeshSets, 0);
//		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());
//		DTINFOWF( getRender(), << DTLOGEVAL(nMeshSets) );			
//		
//		moab::Range allSets;
//    rval = _mb->get_entities_by_type(0, moab::MBENTITYSET, allSets);
//		moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
//		for (moab::Range::iterator it=allSets.begin(); it!=allSets.end(); it++) {
//		  moab::EntityHandle currentSet = *it;
//			int dim1; int dim2; int dim3;
//			rval = _mb->get_number_entities_by_dimension(currentSet, 1, dim1, true);
//			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
//			rval = _mb->get_number_entities_by_dimension(currentSet, 2, dim2, true);
//			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
//			rval = _mb->get_number_entities_by_dimension(currentSet, 3, dim3, true);
//			moab__THROW_IF(rval != moab::MB_SUCCESS, getRender());		
//			DTINFOWF(
//				getRender(), 
//				<< "In loop" << LOGDEL
//				<< DTLOGEVAL(dim1) << LOGDEL
//				<< DTLOGEVAL(dim2) << LOGDEL
//				<< DTLOGEVAL(dim3)
//			); 
//		} 		
//		
//		return rV;
	}	
}
