#include "bVOMeshRule.h"
#include "meshEngine/dtGmshRegion.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
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
    _meshOperator.destroy();
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
    _only = qtXmlBase::getAttributeStrVector("only", element);
    
    dt__info(
      preUpdate(),
      << "rule = " << rule << std::endl
      << "_rule1D = " << _rule1D << std::endl
      << "_rule2D = " << _rule2D << std::endl
      << "_rule3D = " << _rule3D << std::endl
      << "_only = " << _only
    );
  }
  
  void bVOMeshRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
    ::GModel::setCurrent(gm);
    
    //
    // 0D
    //
    gm->mesh(0);
    
    std::list< dtGmshEdge * > ee;
    std::list< dtGmshFace * > ff;
    std::list< dtGmshRegion * > rr;   
    
    if (_only.empty()) {
      ee = dtGmshModel::cast2DtGmshEdge( gm->edges() );
      ff = dtGmshModel::cast2DtGmshFace( gm->faces() );        
      rr = dtGmshModel::cast2DtGmshRegion( gm->regions() );   
    }
    else {
      dt__forAllRefAuto(_only, anOnly) {
        ::GEntity * ge = gm->getGEntityByPhysical(anOnly);
        
        //
        // dtGmshFace
        //
        if (dtGmshFace::DownCast(ge)) {
          dtGmshFace * gf = dtGmshFace::SecureCast(ge);
          dt__forAllRefAuto(
            dtGmshModel::cast2DtGmshEdge(gf->edges()), anEdge
          ) ee.push_back(anEdge);
          ff.push_back(gf);
        }
        //
        // dtGmshRegion
        //
        else if (dtGmshRegion::DownCast(ge)) {
          dtGmshRegion * gr = dtGmshRegion::SecureCast(ge);
          dt__forAllRefAuto(
            dtGmshModel::cast2DtGmshFace(gr->faces()), aFace
          ) {
            ff.push_back(aFace);
            dt__forAllRefAuto(
              dtGmshModel::cast2DtGmshEdge(aFace->edges()), anEdge
            ) ee.push_back(anEdge);
          }
          rr.push_back(gr);
        }
        else dt__throwUnexpected(preUpdate());
      }
      
      //
      // make unique
      //
      ee.unique();
      ff.unique();
      rr.unique();
    }    
    
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
			  dt__forAllRefAuto(ee, aEdge) {
          if ( 
            (
              aEdge->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            (aEdge->meshMaster() == aEdge)
          ) (*current1D)(aEdge); 
          if (optionHandling::optionTrue("debug")) {
            gm->writeMSH(
              ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
            );
          }
        }
      }
      else dt__throw(preUpdate(), << "Only (*)-meshing is currently supported.");
    }
    
    //
    // copy slave edges
    //
	  dt__forAllRefAuto(ee, aEdge) {    
      if ( aEdge->meshMaster() != aEdge ) {
        dtMesh1DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshEdge( aEdge->meshMaster()), aEdge 
        );
      }
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
      // general wild card
      //
      if (currentGEntityStr == "*") {
			  dt__forAllRefAuto(ff, aFace) {
          if ( 
            (
              aFace->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE
            )
            &&
            (
              aFace->meshMaster() == aFace
            )
          ) (*current2D)(aFace);
          if (optionHandling::optionTrue("debug")) {
            gm->writeMSH(
              ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
            );
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
//			  std::list< dtGmshFace * > ff 
//        = 
//        dtGmshModel::cast2DtGmshFace( gm->faces() );        
			  dt__forAllRefAuto(ff, aFace) {
          if ( 
            (
              aFace->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            (
              stringPrimitive::stringContains(
                patternGE, gm->getPhysicalString(aFace)
              )
            )
            &&
            (
              aFace->meshMaster() == aFace
            )
          ) (*current2D)(aFace);
          if (optionHandling::optionTrue("debug")) {
            gm->writeMSH(
              ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
            );
          }          
        }            
      }
      //
      // no wild card
      //
      else if ( !stringPrimitive::stringContains("*", currentGEntityStr) ) {
			  dt__forAllRefAuto(ff, aFace) {
          if ( aFace->getPhysicalString() !=  currentGEntityStr ) continue;
          if ( 
            (
              aFace->meshStatistics.status 
              !=
              ::GEntity::MeshGenerationStatus::DONE 
            )
            &&
            (
              aFace->meshMaster() == aFace
            )
          ) (*current2D)( aFace );
          if (optionHandling::optionTrue("debug")) {        
            gm->writeMSH(
              ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
            );        
          }
        }
      }
      else dt__throw( preUpdate(), << dt__eval(currentGEntityStr) );
    }        
    //
    // copy slave faces
    //
	  dt__forAllRefAuto(ff, aFace) {    
      if ( aFace->meshMaster() != aFace ) {
        dtMesh2DOperator::copyMesh( 
          dtGmshModel::cast2DtGmshFace( aFace->meshMaster()), aFace 
        );
      }
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
			  dt__forAllRefAuto(rr, aRegion) {
          if ( 
            aRegion->_status 
            !=
            ::GEntity::MeshGenerationStatus::DONE 
          ) (*current3D)(aRegion);
          if (optionHandling::optionTrue("debug")) {
            gm->writeMSH(
              ptrBoundedVolume()->getLabel()+"_building.msh", 2.2, false, true
            );
          }          
        }    
      }
      else {
			  dt__forAllRefAuto(rr, aReg) {
          if ( aReg->getPhysicalString() !=  currentGEntityStr ) continue;        
          (*current3D)( gm->getDtGmshRegionByPhysical(currentGEntityStr) );
        }
      }
    }    
  }
}
