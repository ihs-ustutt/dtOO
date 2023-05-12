#include "bVOSetPrescribedFirstElementSize.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshEdge.h>

#include <dtAnalysis.h>
#include <Math/Functor.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOSetPrescribedFirstElementSize::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(
      bVOSetPrescribedFirstElementSize, new bVOSetPrescribedFirstElementSize()
    )
  );
  
  bVOSetPrescribedFirstElementSize::bVOSetPrescribedFirstElementSize() {
  }

  bVOSetPrescribedFirstElementSize::~bVOSetPrescribedFirstElementSize() {
    
  }
 
   void bVOSetPrescribedFirstElementSize::jInit( 
    jsonPrimitive const & jE,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    boundedVolume * attachTo
  ) {
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);

    //dt__pH(scaOneDPolyInterface) s1dPoly
    const scaOneDPolyInterface & s1dPoly
    =
    scaOneDPolyInterface::ConstRefCast(
      jE.lookupRef< analyticFunction >( "_grading", aF )
    );
    dt__throwIf( !s1dPoly.isCompound(), init() );
    _grading
    =
    analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface >(
      &s1dPoly, s1dPoly.vecRef(), s1dPoly.mapRef()
    );
    _polyI.reset( scaOneDPolyInterface::MustDownCast( s1dPoly.weakClone() ) );
  }  
 
  void bVOSetPrescribedFirstElementSize::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {
    // <bVObserver 
    // 	name="bVOSetPrescribedFirstElementSize"
    // 	type="7"
    // 	gradingLabel="aF_dt_gridGradingSpline_7"
    // 	firstElementSize="0.001"
    // />					
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
    jsonPrimitive jE;
    jE.append< dtInt >("_type",qtXmlBase::getAttributeInt("type", element));
    jE.append< dtReal >(
      "_firstElementSize",
      dtXmlParserBase::getAttributeFloatMuParse(
        "firstElementSize", element, cV, aF
      )
    );
    
    jE.append< analyticFunction const * >("_grading",
      aF->get( qtXmlBase::getAttributeStr("gradingLabel", element) ) 
    );
    bVOSetPrescribedFirstElementSize::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOSetPrescribedFirstElementSize::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    logContainer< bVOSetPrescribedFirstElementSize > logC(
      logINFO, "preUpdate()"
    );
    logC() 
      << "firstElementSize = " << config().lookup< dtReal >("_firstElementSize") 
      << std::endl;
    dt__forAllRefAuto( gm->dtEdges(), aEdge) {
      if ( 
        aEdge->meshAttributes.typeTransfinite 
        == 
        config().lookup< dtInt >("_type") 
      ) {

        //
        // get edge length
        //
        _ll = aEdge->getMap1dTo3d()->length();
        
        //
        // x value of checkpoint of function
        //
        _checkX = 1./(aEdge->meshAttributes.nbPointsTransfinite-1);

        //
        // reset pointer to new clone
        //
        _polyI.reset( 
          scaOneDPolyInterface::MustDownCast( _polyI->weakClone() )
        );
        
        // 
        // multidimensional minimization
        //
        dt__pH(dtMinimizer) min(
          dtAnalysis::createMinimizer(":Minuit2::kMigrad:")
        );
        ::ROOT::Math::Functor toMin(
          this, &bVOSetPrescribedFirstElementSize::F, 1
        );			
        min->SetFunction(toMin);    

        //
        // set bounds
        //
        min->SetVariable( 0, "U", 0.5, 0.01 );
        min->SetVariableLimits(0, 0., 1.);

        //
        // minimizer options
        //        
        min->SetMaxFunctionCalls(1000000);
        min->SetMaxIterations(1000);
        min->SetTolerance( 1.e-8 );			
        min->SetPrintLevel(
          staticPropertiesHandler::getInstance()->getOptionInt(
            "root_printLevel"
          ) 
        );

        //
        // minimize
        //
        min->Minimize();    

        double const * const theRoot = min->X();
        
        logC() 
          << logMe::dtFormat(
            "Edge %4d : %16.10e -> %6.1f %% - DOF %6.1f %%"
          ) 
            % aEdge->tag()
            % (_polyI->YFloat( _checkX ) * _ll)
            % ( 
                100. * _polyI->YFloat( _checkX ) * _ll 
                / 
                config().lookup< dtReal >("_firstElementSize")
              ) 
            % ( 100. * (*theRoot) ) 
          << std::endl;
        _grading.addComponent(
          _polyI.get(), aEdge->tag()
        );          
      }
    }
  }
  
	double bVOSetPrescribedFirstElementSize::F( double const * xx ) {
    _polyI->setDOF( std::vector<dtReal>(_polyI->nDOF(), *xx) );
    return fabs(
      _polyI->YFloat(_checkX) * _ll 
      - 
      config().lookup< dtReal >("_firstElementSize")
    );
	}    
}
