#include "bVOPMeshRule.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <xmlHeaven/dtXmlParser.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtMeshOperator.h>
#include <meshEngine/dtMeshOperatorFactory.h>
#include <meshEngine/dtMesh0DOperator.h>
#include <meshEngine/dtMeshGVertex.h>
#include <meshEngine/dtMesh1DOperator.h>
#include <meshEngine/dtMesh2DOperator.h>
#include <meshEngine/dtMesh3DOperator.h>
#include <boundedVolume.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/meshWhisperer.h>
#include <interfaceHeaven/timeHandling.h>
#include <progHelper.h>

namespace dtOO {  
  bVOPMeshRule::bVOPMeshRule() {
  }

  bVOPMeshRule::~bVOPMeshRule() {
    _meshOperator.destroy();
  }
  
  void bVOPMeshRule::bVOPMeshRule::init( 
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
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
    _only = qtXmlBase::getAttributeStrVector("only", element);
    
    _faceWeight 
    = 
    dtXmlParserBase::getAttributeFloatVectorMuParse(
      "faceWeight", element, cV, aF
    );
    dt__info(
      preUpdate(),
      << "rule = " << rule << std::endl
      << "_rule1D = " << _rule1D << std::endl
      << "_rule2D = " << _rule2D << std::endl
      << "_rule3D = " << _rule3D << std::endl
      << "_only = " << _only << std::endl
      << "_faceWeight = " << _faceWeight << std::endl
      << "debug = " << optionHandling::debugTrue()      
    );
  }
  
  void bVOPMeshRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
         
    if ( ptrBoundedVolume()->isMeshed() ) return;
    
    ::GModel::setCurrent(gm);
    
    std::list< dtGmshEdge * > ee;
    std::list< dtGmshFace * > ff;
    std::list< dtGmshRegion * > rr;   
    
//    if (_only.empty()) {
    ee = dtGmshModel::cast2DtGmshEdge( gm->edges() );
    ff = dtGmshModel::cast2DtGmshFace( gm->faces() );        
    rr = dtGmshModel::cast2DtGmshRegion( gm->regions() );   
//    }
//    else {
//      dt__forAllRefAuto(_only, anOnly) {
//        ::GEntity * ge = gm->getGEntityByPhysical(anOnly);
//        
//        //
//        // dtGmshFace
//        //
//        if (dtGmshFace::DownCast(ge)) {
//          dtGmshFace * gf = dtGmshFace::SecureCast(ge);
//          dt__forAllRefAuto(
//            dtGmshModel::cast2DtGmshEdge(gf->edges()), anEdge
//          ) ee.push_back(anEdge);
//          ff.push_back(gf);
//        }
//        //
//        // dtGmshRegion
//        //
//        else if (dtGmshRegion::DownCast(ge)) {
//          dtGmshRegion * gr = dtGmshRegion::SecureCast(ge);
//          dt__forAllRefAuto(
//            dtGmshModel::cast2DtGmshFace(gr->faces()), aFace
//          ) {
//            ff.push_back(aFace);
//            dt__forAllRefAuto(
//              dtGmshModel::cast2DtGmshEdge(aFace->edges()), anEdge
//            ) ee.push_back(anEdge);
//          }
//          rr.push_back(gr);
//        }
//        else dt__throwUnexpected(preUpdate());
//      }
//      
//      //
//      // make unique
//      //
//      ee.unique();
//      ff.unique();
//      rr.unique();
//    }    
    
    gm->prepareToMesh();
  
    //
    // 0D
    //
//    gm->mesh(0);
    std::list< dtGmshVertex * > vv 
    = 
    dtGmshModel::cast2DtGmshVertex( gm->vertices() );
    
    dt__forAllRefAuto(vv, aVertex) {  
      if ( 
        (
          aVertex->_status != ::GEntity::MeshGenerationStatus::DONE 
        )
        &&
        (
          aVertex->getMeshMaster() == aVertex
        )
      )
      dtMeshGVertex()( aVertex );
    }

    //
    // corresponding vertices
    //
	  dt__forAllRefAuto(vv, aVertex) {    
      if ( aVertex->getMeshMaster() != aVertex ) {
        dtMesh0DOperator::correspondingVertex( 
          dtGmshModel::cast2DtGmshVertex( aVertex->getMeshMaster()), aVertex 
        );
      }
    } 
    
    gm->prepareToMesh();
    
    int currentRank = 0;
    std::map< ::GEntity *, int > onRankMap;
    std::vector< float > rankBalance(
      staticPropertiesHandler::getInstance()->nRanks(), 0
    );
    
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
      
      dt__forAllRefAuto(ee, aEdge) {
        if ( 
          (
            (
              aEdge->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            gm->matchWildCardPhysical(currentGEntityStr, aEdge)
            &&
            (aEdge->getMeshMaster() == aEdge)
          )
        ) {
//          tag.push_back(aEdge->tag());
//          onRank.push_back(currentRank);
          onRankMap[ aEdge ] = currentRank;
        }
//        dt__debug(
//          preUpdate(),
//          << "Meshing edge " << aEdge->tag() << " on rank " << onRankMap[aEdge]
//        );
        currentRank++;
        if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
          currentRank = 0;
        }        
      }
      logContainer< bVOPMeshRule > dt__infoContainer(logC);
      dt__forAllRefAuto(ee, aEdge) {      
        if ( aEdge != aEdge->getMeshMaster() ) {
          onRankMap[ aEdge ] = 0;
          onRankMap[ aEdge->getMeshMaster() ] = 0;
        }
        logC() 
          << logMe::dtFormat("[ %4d ] -> %3d : %3d") 
            % aEdge->tag() % onRankMap[aEdge] % aEdge->getMeshMaster()->tag() 
            << std::endl;
      }
      logC.log();
    
//      int ii = 0;
      dt__forAllRefAuto(ee, aEdge) {
        if ( 
          (
            (
              aEdge->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            gm->matchWildCardPhysical(currentGEntityStr, aEdge)
            &&
            (
              onRankMap[aEdge] == staticPropertiesHandler::getInstance()->thisRank()
            )
          )
          &&
          (aEdge->getMeshMaster() == aEdge)
        ) (*current1D)(aEdge); 
//        ii = ii + 1;
      }
    }      
      
    //
    // copy slave edges
    //
	  dt__forAllRefAuto(ee, aEdge) {
      if ( 
        (aEdge->getMeshMaster() != aEdge)
        &&
        (
          dtGmshEdge::ConstDownCast(aEdge->getMeshMaster())->meshStatistics.status
          ==
          ::GEntity::MeshGenerationStatus::DONE 
        )
      ) {
        dtMesh1DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshEdge( aEdge->getMeshMaster()), aEdge 
        );
      }
    }
    
    meshWhisperer mW(gm);
	  dt__forAllRefAuto(ee, aEdge) {    
      if (
        aEdge->meshStatistics.status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__debug(
          preUpdate(),
          << "Setting edge " << aEdge->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );        
        mW.add(aEdge);
      }
      else {
        aEdge->meshStatistics.status 
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();
    
    if (optionHandling::debugTrue()) {
//      dt__onlyMaster {
        gm->writeMSH(
          dtXmlParser::reference().currentState()
          +
          "_"
          +
          ptrBoundedVolume()->getLabel()
          +
          "_"
          +
          stringPrimitive::intToString(
            staticPropertiesHandler::getInstance()->thisRank()
          )
          +
          "_debug.msh", 
          2.2, false, true
        );
//      }
    }
    
    gm->prepareToMesh();
    
//    tag.clear();
//    onRank.clear();
    onRankMap.clear();
    currentRank = 0;
    if ( _faceWeight.size() != ff.size() ) {
      _faceWeight = std::vector< float >(ff.size(), 1.0);
      dt__info(preUpdate(), << "Vector _faceWeight has wrong size.");
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
      
      //
      // determine entities
      //
      dt__forAllRefAuto(ff, aFace) {
        if ( 
          (
            aFace->meshStatistics.status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          )
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aFace)
          &&
          (
            aFace->getMeshMaster() == aFace
          )
        ) {
          currentRank = progHelper::minPos(rankBalance);
          onRankMap[ aFace ] = currentRank;
          rankBalance[ currentRank ] 
          = 
          rankBalance[ currentRank ] 
          +
          _faceWeight[ aFace->tag() - 1 ];
        }
        dt__debug(
          preUpdate(),
          << "Meshing face " << aFace->tag() << " on rank " << onRankMap[aFace]
        );
//        currentRank++;
//        if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
//          currentRank = 0;
//        }           
      }
      dt__info( preUpdate(), << "rankBalance = " << rankBalance );      
//      int ii = 0;
      dt__forAllRefAuto(ff, aFace) {
        if ( 
          (
            aFace->meshStatistics.status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          )
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aFace)
          &&
          (
            onRankMap[aFace] == staticPropertiesHandler::getInstance()->thisRank()
          )
          &&                
          (
            aFace->getMeshMaster() == aFace
          )
        ) {
          timeHandling tt(
            "Surface( " + stringPrimitive::intToString(aFace->tag()) + " )"
          );
          (*current2D)(aFace);
          tt.output();
        }
//        ii = ii + 1;
      }      
    }

	  dt__forAllRefAuto(ff, aFace) {    
      if (
        aFace->meshStatistics.status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__debug(
          preUpdate(),
          << "Setting face " << aFace->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );      
        mW.add(aFace);
      }
      else {
        aFace->meshStatistics.status 
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();
    
    //
    // copy slave faces
    //
	  dt__forAllRefAuto(ff, aFace) {    
      if ( 
        aFace->getMeshMaster() != aFace
      ) {
        dtMesh2DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshFace( aFace->getMeshMaster()), aFace 
        );
      }
    }       
    
    if (optionHandling::debugTrue()) {
//      dt__onlyMaster {
        gm->writeMSH(
          dtXmlParser::reference().currentState()
          +
          "_"
          +
          ptrBoundedVolume()->getLabel()
          +
          "_"
          +
          stringPrimitive::intToString(
            staticPropertiesHandler::getInstance()->thisRank()
          )
          +
          "_debug.msh", 
          2.2, false, true
        );
//      }
    }

    gm->prepareToMesh();
    
//    tag.clear();
//    onRank.clear();
    onRankMap.clear();
    currentRank = 0;
    
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
      dt__forAllRefAuto(rr, aReg) {
        if ( 
          (
            aReg->_status
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          )
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aReg)
          &&
          (
            aReg->getMeshMaster() == aReg
          )
        ) {
          onRankMap[aReg] = currentRank;
        }
        dt__debug(
          preUpdate(),
          << "Meshing region " << aReg->tag() << " on rank " << onRankMap[aReg]
        );
        currentRank++;
        if (currentRank == staticPropertiesHandler::getInstance()->nRanks()) {
          currentRank = 0;
        }           
      }
      
//      int ii = 0;
      dt__forAllRefAuto(rr, aReg) {
        if ( 
          (
            aReg->_status
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          )
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aReg)
          &&
          (
            aReg->getMeshMaster() == aReg
          )
        ) (*current3D)(aReg);          
//        ii = ii + 1;           
      }      
    }
  
	  dt__forAllRefAuto(rr, aReg) {    
      if (
        aReg->_status 
        ==
        ::GEntity::MeshGenerationStatus::DONE       
       ) {
        dt__debug(
          preUpdate(),
          << "Setting region " << aReg->tag() 
          << " on rank " << staticPropertiesHandler::getInstance()->thisRank()
          << " to meshWhisperer "
        );      
        mW.add(aReg);
      }
      else {
        aReg->_status
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();  

    if (optionHandling::debugTrue()) {
//      dt__onlyMaster {
        gm->writeMSH(
          dtXmlParser::reference().currentState()
          +
          "_"
          +
          ptrBoundedVolume()->getLabel()
          +
          "_"
          +
          stringPrimitive::intToString(
            staticPropertiesHandler::getInstance()->thisRank()
          )
          +
          "_debug.msh", 
          2.2, false, true
        );
//      }
    } 
    
    ptrBoundedVolume()->setMeshed();
  }
}
