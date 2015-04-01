#include "postBLMeshKit.h"
#include "meshEngine/dtMoabCore.h"
#include "dtXmlParserDecorator/qtXmlBase.h"
//#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <meshEngine/dtGmshFace.h>
#include <moab/Core.hpp>
#include <moab/CN.hpp>
#include <meshkit/MKCore.hpp>
#include <meshkit/MeshOp.hpp>
#include <meshkit/RegisterMeshOp.hpp>
#include <meshkit/ModelEnt.hpp>
#include <meshEngine/dtPostBL.h>
#include <meshEngine/dtMoabCore.h>
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
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
    _thickness = optionHandling::getOptionFloat("thickness");
    _intervals = optionHandling::getOptionInt("intervals");
    _bias = optionHandling::getOptionFloat("bias");
		_debug = optionHandling::optionTrue("debug");
		
		//
		// boundedVolume
		//		
    QDomElement wElement = qtXmlPrimitive::getChild("boundedVolume", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		_faceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("faceLabel", wElement);
		_faceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("faceOrientation", wElement, cV, aF);
		
		//
		// get boundedVolume
		//
    _meshedBV = bV->get(label);
	}
	
  void postBLMeshKit::makeGrid(void) {
		try {
			//
			// create moab Core
			//
			dtMoabCore * mbCore = new dtMoabCore();

			//
			// create MeshKit core
			//
			_mk.reset( 
			  new MeshKit::MKCore(NULL, mbCore, NULL, NULL, true) 
			);			
			
			dtGmshFace const * gf;
			std::vector< ::MVertex const * > mv;
      twoDArrayHandling< ::MElement const * > me(_faceLabel.size(), 0);			
			dt__forAllIndex(_faceLabel, ii) {
			  gf = _meshedBV->getFace(_faceLabel[ii]);
			  gf->getMeshVerticesAndElements(&mv, &(me[ii]));
			}
			
			//
			// add vertices
			//
			mbCore->addVertices(mv);

      //
			// add elements according to orientation
			//			
			moab::Range commonRangePos;
      moab::Range commonRangeNeg;
			moab::Range commonRangeFix;
			dt__forAllIndex(_faceOrientation, ii) {
				if (_faceOrientation[ii] > 0) {
			    commonRangePos.merge(mbCore->addElements(me[ii]));
				}
				else if (_faceOrientation[ii] < 0) {
			    commonRangeNeg.merge(mbCore->addElements(me[ii]));
				}
				else {
					commonRangeFix.merge(mbCore->addElements(me[ii]));
				}
			}
			
			//! create a model entity vector for construting PostBL meshop, note that model entities(mesh) input for PostBL meshop is read from a file.
			MeshKit::MEntVector volso;

			//! construct the meshop and set name
			MeshKit::dtPostBL * pbl = (MeshKit::dtPostBL*) _mk->construct_meshop("dtPostBL", volso);
			pbl->set_name("PostBL");

			pbl->setup_this();
//			pbl->setLogStream( Output2FILE::Stream() );
			pbl->debug(_debug);
			pbl->init(_thickness, _intervals, _bias);
			pbl->addPosRange(commonRangePos);
			pbl->addNegRange(commonRangeNeg);
			pbl->addFixRange(commonRangeFix);
			
			{
				dt__LOGCOUT(spread, coutswitch);
				pbl->execute_this();
			}
			
			mbCore->write_mesh("dieterherbert_nach.vtk");
			delete pbl;
		}
		meshkit__CATCH(makeGrid);
		
		boundedVolume::postNotify();
		
		boundedVolume::setMeshed();
	}
  
	void postBLMeshKit::makePreGrid(void) {
		boundedVolume::preNotify();
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
