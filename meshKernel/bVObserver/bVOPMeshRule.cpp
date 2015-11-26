#include "bVOPMeshRule.h"
#include "meshEngine/dtGmshRegion.h"
#include "interfaceHeaven/staticPropertiesHandler.h"
#include "logMe/dtParMacros.h"

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
#include <interfaceHeaven/meshWhisperer.h>

namespace dtOO {  
  bVOPMeshRule::bVOPMeshRule() {
  }

  bVOPMeshRule::~bVOPMeshRule() {
    _meshOperator.destroy();
  }
  
  void bVOPMeshRule::bVOPMeshRule::init( 
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
    //  name="bVOPMeshRule"
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
  
  void bVOPMeshRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
    ::GModel::setCurrent(gm);
    
    //
    // 0D
    //
    gm->mesh(0);

    std::vector< int > tag;
    std::vector< int > onRank;
    int currentRank = 0;
    
    //
    // 1D
    //
    std::vector< dtMesh1DOperator * > moOne;
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
          ) {
            moOne.push_back(current1D);
            tag.push_back((*it)->tag());
            onRank.push_back(currentRank);
            dt__info(
              preUpdate(),
              << "Meshing edge " << tag.back() << " on rank " << onRank.back()
            );
            currentRank++;
            if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
              currentRank = 0;
            }
          }
        }
      }
      else dt__throw(
        preUpdate(), 
        << "Only (*)-meshing is currently supported."
      );
    }
    
    dt__forAllIndex(tag, ii) {
      if (onRank[ii] == staticPropertiesHandler::getInstance()->thisRank()) {
        moOne[ii]->operator()( gm->getDtGmshEdgeByTag(tag[ii]) );
      }
    }
    
    meshWhisperer mW(gm);
    std::list< dtGmshEdge * > ee 
    = 
    dtGmshModel::cast2DtGmshEdge( gm->edges() );    
	  dt__forAllIter(std::list< dtGmshEdge * >, ee, it) {    
      if (
        (*it)->meshStatistics.status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__info(
          preUpdate(),
          << "Setting edge " << (*it)->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );        
        mW.add(*it);
      }
      else {
        (*it)->meshStatistics.status 
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();

    if (optionHandling::optionTrue("debug")) {
      dt__onlyMaster {
        gm->writeMSH(
          ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
        );
      }
    }
            
    tag.clear();
    onRank.clear();
    currentRank = 0;
    //
    // 2D
    //
    std::vector< dtMesh2DOperator * > moTwo;    
    dt__forAllConstIter(std::vector< std::string >, _rule2D, it) {
      std::string currentOperatorStr = *it;
      std::string currentGEntityStr 
      = 
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

      dt__ptrAss(
        dtMesh2DOperator * current2D,
        dtMesh2DOperator::DownCast(_meshOperator.get(currentOperatorStr) )
      );
      
      //
      // general wild card
      //      
      if (currentGEntityStr == "*") {
			  std::list< dtGmshFace * > ff 
        = 
        dtGmshModel::cast2DtGmshFace( gm->faces() );
			  dt__forAllIter(std::list< dtGmshFace * >, ff, it) {
          if ( 
            (*it)->meshStatistics.status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          ) {
            moTwo.push_back(current2D);
            tag.push_back((*it)->tag());
            onRank.push_back(currentRank);
            (*it)->meshStatistics.status 
            = 
            ::GEntity::MeshGenerationStatus::DONE;
          }
        }    
      }
      //
      // specific wild card
      //
      else if ( stringPrimitive::stringContains("*", currentGEntityStr) ) {
        //
        // clean specific wild card
        //
        std::string patternGE
        = 
        stringPrimitive::replaceStringInString("*", "", currentGEntityStr);
        
        //
        // determine entities
        //
			  std::list< dtGmshFace * > ff 
        = 
        dtGmshModel::cast2DtGmshFace( gm->faces() );        
			  dt__forAllIter(std::list< dtGmshFace * >, ff, it) {
          if ( 
            (
              (*it)->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            (
              stringPrimitive::stringContains(
                patternGE, gm->getPhysicalString(*it)
              )
            )
          ) {
            moTwo.push_back(current2D);
            tag.push_back((*it)->tag());
            onRank.push_back(currentRank);
            (*it)->meshStatistics.status 
            = 
            ::GEntity::MeshGenerationStatus::DONE;   
          }
        }            
      }      
      //
      // no wild card
      //      
      else if ( !stringPrimitive::stringContains("*", currentGEntityStr) ) {
        moTwo.push_back(current2D);
        tag.push_back(gm->getDtGmshFaceByPhysical(currentGEntityStr)->tag());
        onRank.push_back(currentRank);  
        gm->getDtGmshFaceByPhysical(currentGEntityStr)->meshStatistics.status
        = 
        ::GEntity::MeshGenerationStatus::DONE;
      }
      else dt__throw( preUpdate(), << dt__eval(currentGEntityStr) );
      
      dt__info(
        preUpdate(),
        << "Meshing face " << tag.back() << " on rank " << onRank.back()
      );
      currentRank++;
      if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
        currentRank = 0;
      }            
    }           
    dt__forAllIndex(tag, ii) {
      gm->getDtGmshFaceByTag(tag[ii])->meshStatistics.status 
      =
      ::GEntity::MeshGenerationStatus::PENDING;
      if (onRank[ii] == staticPropertiesHandler::getInstance()->thisRank()) {
        moTwo[ii]->operator()( gm->getDtGmshFaceByTag(tag[ii]) );
      }
    }
    
    std::list< dtGmshFace * > ff 
    = 
    dtGmshModel::cast2DtGmshFace( gm->faces() );    
	  dt__forAllIter(std::list< dtGmshFace * >, ff, it) {    
      if (
        (*it)->meshStatistics.status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__info(
          preUpdate(),
          << "Setting face " << (*it)->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );      
        mW.add(*it);
      }
      else {
        (*it)->meshStatistics.status 
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();
    
    if (optionHandling::optionTrue("debug")) {
      dt__onlyMaster {
        gm->writeMSH(
          ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
        );
      }
    }
    
//    //
//    // 3D
//    //
//    dt__onlyMaster {
//      dt__forAllConstIter(std::vector< std::string >, _rule3D, it) {
//        std::string currentOperatorStr = *it;
//        std::string currentGEntityStr 
//        = 
//        qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);
//
//        dt__ptrAss(
//          dtMesh3DOperator * current3D,
//          dtMesh3DOperator::DownCast(_meshOperator.get(currentOperatorStr) )
//        );
//
//        if (currentGEntityStr == "*") {
//          std::list< dtGmshRegion * > rr 
//          = 
//          dtGmshModel::cast2DtGmshRegion( gm->regions() );
//          dt__forAllIter(std::list< dtGmshRegion * >, rr, it) {
//            if ( 
//              (*it)->_status 
//              !=
//              ::GEntity::MeshGenerationStatus::DONE 
//            ) (*current3D)(*it);                  
//          }    
//        }
//        else (*current3D)( gm->getDtGmshRegionByPhysical(currentGEntityStr) );
//      }    
//    }
    tag.clear();
    onRank.clear();
    currentRank = 0;
    //
    // 3D
    //
    std::vector< dtMesh3DOperator * > moThree;    
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
          ) {
            moThree.push_back(current3D);
            tag.push_back((*it)->tag());
            onRank.push_back(currentRank);
            (*it)->_status 
            = 
            ::GEntity::MeshGenerationStatus::DONE;
          }
        }    
      }
      else {
        moThree.push_back(current3D);
        tag.push_back(gm->getDtGmshRegionByPhysical(currentGEntityStr)->tag());
        onRank.push_back(currentRank);
        gm->getDtGmshRegionByPhysical(currentGEntityStr)->_status
        = 
        ::GEntity::MeshGenerationStatus::DONE;
      }
      currentRank++;
      if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
        currentRank = 0;
      }      
      dt__info(
        preUpdate(),
        << "Meshing region " << tag.back() << " on rank " << onRank.back()
      );      
    }           
    dt__forAllIndex(tag, ii) {
      gm->getDtGmshRegionByTag(tag[ii])->_status 
      =
      ::GEntity::MeshGenerationStatus::PENDING;
      if (onRank[ii] == staticPropertiesHandler::getInstance()->thisRank()) {
        moThree[ii]->operator()( gm->getDtGmshRegionByTag(tag[ii]) );
      }
    }    
    
    std::list< dtGmshRegion * > rr 
    = 
    dtGmshModel::cast2DtGmshRegion( gm->regions() );    
	  dt__forAllIter(std::list< dtGmshRegion * >, rr, it) {    
      if (
        (*it)->_status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__info(
          preUpdate(),
          << "Setting region " << (*it)->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );      
        mW.add(*it);
      }
      else {
        (*it)->_status
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();  

    if (optionHandling::optionTrue("debug")) {
      dt__onlyMaster {
        gm->writeMSH(
          ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
        );
      }
    }    
  }
}
