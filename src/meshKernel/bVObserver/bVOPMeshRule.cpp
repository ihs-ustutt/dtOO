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
#include "bVOInterfaceFactory.h"

namespace dtOO {
  bool bVOPMeshRule::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOPMeshRule, new bVOPMeshRule())
  );
  
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
    dt__throwIf( _only.size()!=0, init()  );
    _edgeWeight 
    = 
    dtXmlParserBase::getAttributeFloatVectorMuParse(
      "edgeWeight", element, cV, aF
    );
    _faceWeight 
    = 
    dtXmlParserBase::getAttributeFloatVectorMuParse(
      "faceWeight", element, cV, aF
    );    
    _regionWeight 
    = 
    dtXmlParserBase::getAttributeFloatVectorMuParse(
      "regionWeight", element, cV, aF
    );
    dt__info(
      preUpdate(),
      << "rule = " << rule << std::endl
      << "_rule1D = " << _rule1D << std::endl
      << "_rule2D = " << _rule2D << std::endl
      << "_rule3D = " << _rule3D << std::endl
      << "_only = " << _only << std::endl
      << "_edgeWeight = " << _edgeWeight << std::endl
      << "_faceWeight = " << _faceWeight << std::endl
      << "_regionWeight = " << _regionWeight << std::endl
      << "debug = " << optionHandling::debugTrue()      
    );
  }
  
  bool bVOPMeshRule::isOwnMaster(::GEntity const * const gent) {
    if ( gent->getMeshMaster() == gent ) {
      return true;
    }
    else {
      return false;
    }
  }

  bool bVOPMeshRule::isMeshed(dtGmshVertex const * const gent) {
    if (
      gent->_status == ::GEntity::MeshGenerationStatus::DONE 
    ) {
      return true;
    }
    else {
      return false;
    }        
  }
  
  template < typename T >
  bool bVOPMeshRule::isMeshed(T const * const gent) {
    if (
      gent->meshStatistics.status == ::GEntity::MeshGenerationStatus::DONE 
    ) {
      return true;
    }
    else {
      return false;
    }        
  }  
  template 
  bool bVOPMeshRule::isMeshed< dtGmshEdge >(dtGmshEdge const * const);
  template 
  bool bVOPMeshRule::isMeshed< dtGmshFace >(dtGmshFace const * const);
  
  bool bVOPMeshRule::isMeshed(dtGmshRegion const * const gent) {
    if (
      gent->_status == ::GEntity::MeshGenerationStatus::DONE 
    ) {
      return true;
    }
    else {
      return false;
    }        
  }
  
  template < typename T >
  std::map< ::GEntity *, dtInt > bVOPMeshRule::prepareDistribution(
    std::list< T > list, std::vector< dtReal > weight
  ) {
    std::vector< dtReal > rankBalance(
      staticPropertiesHandler::getInstance()->nRanks(), 0
    );
    std::map< ::GEntity *, dtInt > onRankMap;
    dtInt currentRank = 0;
    dt__forAllRefAuto(list, anEnt) {
      if ( !isMeshed(anEnt) && isOwnMaster(anEnt) ) {
        currentRank = progHelper::minPos(rankBalance);
        rankBalance[ currentRank ] 
        = 
        rankBalance[ currentRank ] 
        +
        weight[ anEnt->tag() - 1 ];        
        onRankMap[ anEnt ] = currentRank;
      }
    }
    
    logContainer< bVOPMeshRule > logC(
      TLogLevel::logINFO, "prepareDistribution()"
    );
    logC() 
      << "Distribution" << std::endl
      << logMe::dtFormat("[ %4s ] -> %4s : %6s") 
        % "tag" % "rank" % "master" 
        << std::endl;    
    dt__forAllRefAuto(list, anEnt) {
      if ( !isOwnMaster(anEnt) ) {
        onRankMap[ anEnt ] = onRankMap[ anEnt->getMeshMaster() ];
      }
      logC() 
        << logMe::dtFormat("[ %4d ] -> %3d : %3d") 
          % anEnt->tag() % onRankMap[anEnt] % anEnt->getMeshMaster()->tag() 
          << std::endl;
    }
    logC() 
      << "rankBalance = " << std::endl
      << rankBalance << std::endl;
    logC.log();    
  
    return onRankMap;
  }
  
  void bVOPMeshRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
         
    if ( ptrBoundedVolume()->isMeshed() ) return;
    
    ::GModel::setCurrent(gm);
    
    std::list< dtGmshEdge * > ee;
    std::list< dtGmshFace * > ff;
    std::list< dtGmshRegion * > rr;   
    
    ee = dtGmshModel::cast2DtGmshEdge( gm->edges() );
    ff = dtGmshModel::cast2DtGmshFace( gm->faces() );        
    rr = dtGmshModel::cast2DtGmshRegion( gm->regions() );   
    
    if ( _edgeWeight.size() != ee.size() ) {
      _edgeWeight = std::vector< dtReal >(ee.size(), 1.0);
      dt__info(preUpdate(), << "Assume uniform _edgeWeight.");
    }    
    if ( _faceWeight.size() != ff.size() ) {
      _faceWeight = std::vector< dtReal >(ff.size(), 1.0);
      dt__info(preUpdate(), << "Assume uniform _faceWeight.");
    }    
    if ( _regionWeight.size() != rr.size() ) {
      _regionWeight = std::vector< dtReal >(rr.size(), 1.0);
      dt__info(preUpdate(), << "Assume uniform _regionWeight.");
    }
    gm->prepareToMesh();
  
    //
    // 0D
    //
    std::list< dtGmshVertex * > vv 
    = 
    dtGmshModel::cast2DtGmshVertex( gm->vertices() );
    
    dt__forAllRefAuto(vv, aVertex) {
      if ( !isMeshed(aVertex) && isOwnMaster(aVertex) ) {
        dtMeshGVertex()( aVertex );
      }
    }

    //
    // corresponding vertices
    //
	  dt__forAllRefAuto(vv, aVertex) {    
      if ( !isOwnMaster(aVertex) ) {
        dtMesh0DOperator::correspondingVertex( 
          dtGmshModel::cast2DtGmshVertex( aVertex->getMeshMaster()), aVertex 
        );
      }
    } 

    std::map< ::GEntity *, dtInt > onRankMap;
    dtInt const thisRank = staticPropertiesHandler::getInstance()->thisRank();
    
    gm->prepareToMesh();   
    onRankMap = prepareDistribution(ee, _edgeWeight);
    
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
          !isMeshed(aEdge) 
          && 
          gm->matchWildCardPhysical(currentGEntityStr, aEdge)
          &&
          (
            onRankMap[aEdge] == thisRank
          )
          &&
          isOwnMaster(aEdge)
        ) (*current1D)(aEdge); 
      }
    }      
          
    meshWhisperer mW(gm);
	  dt__forAllRefAuto(ee, aEdge) {    
      if ( isMeshed(aEdge) ) {
        mW.add(aEdge);
      }
      else {
        aEdge->meshStatistics.status 
        =
        ::GEntity::MeshGenerationStatus::DONE;
      }
    }
    mW.distribute();

    //
    // copy slave edges
    //
	  dt__forAllRefAuto(ee, aEdge) {
      if ( 
        !isOwnMaster(aEdge) 
        && 
        isMeshed( dtGmshEdge::ConstDownCast(aEdge->getMeshMaster()) )
      ) {
        dtMesh1DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshEdge( aEdge->getMeshMaster()), aEdge 
        );
        aEdge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
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
          "_1d_debug.msh", 
          2.2, false, true
        );
//      }
    }
    
    gm->prepareToMesh();
    onRankMap = prepareDistribution(ff, _faceWeight);    

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
      // mesh entities
      //
      dt__forAllRefAuto(ff, aFace) {
        if ( 
          !isMeshed(aFace)
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aFace)
          &&
          (
            onRankMap[aFace] == thisRank
          )
          &&                
          isOwnMaster(aFace)
        ) {
          timeHandling tt(
            "Surface( " + stringPrimitive::intToString(aFace->tag()) + " )"
          );
          (*current2D)(aFace);
          tt.output();
        }
      }      
    }

	  dt__forAllRefAuto(ff, aFace) {    
      if ( isMeshed(aFace) ) {
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
        !isOwnMaster(aFace)
        &&
        isMeshed( dtGmshFace::ConstDownCast(aFace->getMeshMaster()) )
      ) {
        dtMesh2DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshFace( aFace->getMeshMaster()), aFace 
        );
        aFace->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
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
          "_2d_debug.msh", 
          2.2, false, true
        );
//      }
    }

    gm->prepareToMesh();
    onRankMap = prepareDistribution(rr, _regionWeight);
    
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
          !isMeshed(aReg)
          &&
          gm->matchWildCardPhysical(currentGEntityStr, aReg)
//          &&
//          isOwnMaster(aReg)
          &&
          (
            onRankMap[aReg] == thisRank
          )                
        ) (*current3D)(aReg);
      }      
    }

	  dt__forAllRefAuto(rr, aReg) {    
      if ( isMeshed(aReg) ) {
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
          "_3d_debug.msh", 
          2.2, false, true
        );
//      }
    } 
    
    ptrBoundedVolume()->setMeshed();
  }
}
