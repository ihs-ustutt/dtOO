#include "bVOMeshRule.h"
#include "meshEngine/dtGmshRegion.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtMeshOperator.h>
#include <meshEngine/dtMeshOperatorFactory.h>
#include <meshEngine/dtMesh1DOperator.h>
#include <meshEngine/dtMesh2DOperator.h>
#include <meshEngine/dtMesh3DOperator.h>
#include <boundedVolume.h>

namespace dtOO {  
  bVOMeshRule::bVOMeshRule() {
  }

  bVOMeshRule::~bVOMeshRule() {
    
  }
  
  void bVOMeshRule::bVOMeshRule::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		boundedVolume * attachTo
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
    //<bVObserver 
    //  name="bVOMeshRule"
    //  rule="
    //    :
    //    {dtMeshGEdge(*)}
    //    :
    //    :
    //    {dtMeshGFace(*)}
    //    :
    //    :
    //    {dtMeshGRegionWithBoundaryLayer(REG1)}
    //    {dtMeshGRegion(REGI_1)}
    //    :
    //  "
    //>
    //  <dtMeshOperator
    //    name="dtMeshGEdge"
    //    label="dtMeshGEdge"
    //  />
    //  <dtMeshOperator
    //    name="dtMeshGFace"
    //    label="dtMeshGFace"
    //  />
    //  <dtMeshOperator
    //    name="dtMeshGRegion"
    //    label="dtMeshGRegion"
    //    pyramidHeightScale="0.25"
    //  />		
    //  <dtMeshOperator
    //    name="dtMeshGRegionWithBoundaryLayer"
    //    label="dtMeshGRegionWithBoundaryLayer"
    //    spacing="{.050}{.125}{.225}"
    //    thickness="0.01"
    //    nSmoothingSteps="0"
    //    nShrinkingSteps="0"
    //    maxDihedralAngle="_pi*60./180."
    //    faceLabel="{HUB}"
    //    faceOrientation="{-1}"
    //    fixedFaceLabel="{INLET}"
    //    fixedFaceOrientation="{1}"
    //    slidableFaceLabel="{INTERNAL_3}"
    //    slidableFaceOrientation="{-1}"
    //  />
    //</bVObserver>
								
    std::vector< ::QDomElement > meshOp
    =
    qtXmlPrimitive::getChildVector("dtMeshOperator", element);
    
    dt__forAllConstIter(std::vector< ::QDomElement >, meshOp, it) {
      dtMeshOperator * anOp 
      = 
      dtMeshOperatorFactory::create(
        qtXmlBase::getAttributeStr("name", *it)
      );
      anOp->init(*it, bC, cV, aF, aG, bV, &_meshOperator);
      
      _meshOperator.push_back(anOp);
    }
    std::string rule = qtXmlPrimitive::getAttributeStr("rule", element);
    _rule1D 
    = 
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    );
    _rule2D 
    = 
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    );
    _rule3D 
    = 
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    );
    
    dt__info(
      preUpdate(),
      << "rule = " << rule << std::endl
      << "_rule1D = " << _rule1D << std::endl
      << "_rule2D = " << _rule2D << std::endl
      << "_rule3D = " << _rule3D
    );
  }
  
  void bVOMeshRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
//		dt__throwIf(gm==NULL, preUpdate());
    
    ::GModel::setCurrent(gm);
    
    //
    // 0D
    //
    gm->mesh(0);
    
    //
    // 1D
    //
    dt__forAllConstIter(std::vector< std::string >, _rule1D, it) {
      std::string currentOperatorStr = *it;
      std::string currentGEntityStr 
      = 
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

      dt__ptrAss(
        dtMesh1DOperator * current1D,
        dtMesh1DOperator::DownCast(_meshOperator.get(currentOperatorStr) )
      );
      
      if (currentGEntityStr == "*") {
			  std::list< dtGmshEdge * > ee 
        = 
        dtGmshModel::cast2DtGmshEdge( gm->edges() );
			  dt__forAllIter(std::list< dtGmshEdge * >, ee, it) {
          if ( 
            (*it)->meshStatistics.status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          ) (*current1D)(*it); 
        }
      }
      else dt__throw(preUpdate(), << "Only (*)-meshing is currently supported.");
    }
    
    //
    // 2D
    //
    dt__forAllConstIter(std::vector< std::string >, _rule2D, it) {
      std::string currentOperatorStr = *it;
      std::string currentGEntityStr 
      = 
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

      dt__ptrAss(
        dtMesh2DOperator * current2D,
        dtMesh2DOperator::DownCast(_meshOperator.get(currentOperatorStr) )
      );
      
      if (currentGEntityStr == "*") {
			  std::list< dtGmshFace * > ff 
        = 
        dtGmshModel::cast2DtGmshFace( gm->faces() );
			  dt__forAllIter(std::list< dtGmshFace * >, ff, it) {
          if ( 
            (*it)->meshStatistics.status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          ) (*current2D)(*it);           
        }    
      }
      else (*current2D)( gm->getDtGmshFaceByPhysical(currentGEntityStr) );
    }        

    //
    // 3D
    //
    dt__forAllConstIter(std::vector< std::string >, _rule3D, it) {
      std::string currentOperatorStr = *it;
      std::string currentGEntityStr 
      = 
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

      dt__ptrAss(
        dtMesh3DOperator * current3D,
        dtMesh3DOperator::DownCast(_meshOperator.get(currentOperatorStr) )
      );
      
      if (currentGEntityStr == "*") {
			  std::list< dtGmshRegion * > rr 
        = 
        dtGmshModel::cast2DtGmshRegion( gm->regions() );
			  dt__forAllIter(std::list< dtGmshRegion * >, rr, it) {
          if ( 
            (*it)->_status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          ) (*current3D)(*it);                  
        }    
      }
      else (*current3D)( gm->getDtGmshRegionByPhysical(currentGEntityStr) );
    }    
  }
}