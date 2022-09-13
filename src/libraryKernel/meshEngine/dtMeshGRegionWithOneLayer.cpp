#include "dtMeshGRegionWithOneLayer.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include "dtGmshFace.h"
#include "dtGmshVertex.h"
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"

#include <xmlHeaven/qtXmlBase.h>

#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>
#include <gmsh/MHexahedron.h>

namespace dtOO {
  
  dtMeshGRegionWithOneLayer::dtMeshGRegionWithOneLayer() {
    
	}

	dtMeshGRegionWithOneLayer::~dtMeshGRegionWithOneLayer() {
    
	}
	
  void dtMeshGRegionWithOneLayer::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
		_faceMaster
		= 
		qtXmlPrimitive::getAttributeStr("faceMaster", element);
		_faceSlave
		= 
		qtXmlPrimitive::getAttributeStr("faceSlave", element);    
	}
	
  void dtMeshGRegionWithOneLayer::operator() (dtGmshRegion * dtgr) {
    Msg::Info(
      "Meshing volume %d ( dtMeshGRegionWithOneLayer )", dtgr->tag()
    );
    
		//
		// get master and slave face and check if faces belong to this volume
		//
    dtGmshFace * masterFace
    =
    dtgr->refDtGmshModel().getDtGmshFaceByPhysical( _faceMaster );
    dt__throwIf(
      std::find(
        dtgr->dtFaces().begin(), dtgr->dtFaces().end(), masterFace
      ) 
      == 
      dtgr->dtFaces().end()
      , 
      operator()()
    );
    dtGmshFace * slaveFace
    =
    dtgr->refDtGmshModel().getDtGmshFaceByPhysical( _faceSlave );
    dt__throwIf(
      std::find(
        dtgr->dtFaces().begin(), dtgr->dtFaces().end(), slaveFace
      ) 
      == 
      dtgr->dtFaces().end()
      , 
      operator()()
    );    
		
    dt__forFromToIndex( 0, masterFace->getNumMeshElements(), ii ) {
      ::MElement * me = slaveFace->getMeshElement(ii);
      if ( dynamic_cast< ::MTriangle * >(me) ) {
        std::vector< ::MVertex * > vv;
        dt__forFromToIndex(0, 3, jj) vv.push_back( me->getVertex(jj) );
        dt__forFromToIndex(0, 3, jj) {
          dt__ptrAss(
            ::MVertex * mv, 
            slaveFace->correspondingVertices[ me->getVertex(jj) ]
          );
          vv.push_back( mv );
        }
        dtgr->addElement( new ::MPrism( vv ) );
      }
      else if ( dynamic_cast< ::MQuadrangle * >(me) ) {
        std::vector< ::MVertex * > vv;
        dt__forFromToIndex(0, 4, jj) vv.push_back( me->getVertex(jj) );
        dt__forFromToIndex(0, 4, jj) {
          dt__ptrAss(
            ::MVertex * mv, 
            slaveFace->correspondingVertices[ me->getVertex(jj) ]
          );
          vv.push_back( mv );
        }
        dtgr->addElement( new ::MHexahedron( vv ) );        
      }
      else dt__throwUnexpected(operator()());
    }

    //
    // mark as meshed
    //
    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
  }  
}