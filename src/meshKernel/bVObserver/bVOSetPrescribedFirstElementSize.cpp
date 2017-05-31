#include "bVOSetPrescribedFirstElementSize.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneDPolyInterface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshEdge.h>

#include <dtAnalysis.h>
#include <Math/Functor.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {  
  bVOSetPrescribedFirstElementSize::bVOSetPrescribedFirstElementSize() {
  }

  bVOSetPrescribedFirstElementSize::~bVOSetPrescribedFirstElementSize() {
    
  }
  
  void bVOSetPrescribedFirstElementSize::bVOSetPrescribedFirstElementSize::init( 
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
    
    _type = qtXmlBase::getAttributeInt("type", element);
    _firstElementSize 
    = 
    qtXmlBase::getAttributeFloatMuParse("firstElementSize", element, cV, aF);
    
    scaOneDPolyInterface const * const s1dPoly
    =
    scaOneDPolyInterface::MustDownCast( 
      aF->get( qtXmlBase::getAttributeStr("gradingLabel", element) ) 
    );
    dt__throwIf( !s1dPoly->isCompound(), init() );
    _grading
    =
    analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface >(
      s1dPoly, s1dPoly->vecRef(), s1dPoly->mapRef()
    );
    
    _polyI.reset( scaOneDPolyInterface::MustDownCast( s1dPoly->weakClone() ) );
//			<bVObserver 
//				name="bVOSetPrescribedFirstElementSize"
//				type="7"
//				gradingLabel="aF_dt_gridGradingSpline_7"
//				firstElementSize="0.001"
//			/>					

  }
  
  void bVOSetPrescribedFirstElementSize::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    logContainer< bVOSetPrescribedFirstElementSize > logC(logINFO, "preUpdate()");
    logC() << "firstElementSize = " << _firstElementSize << std::endl;
    dt__forAllRefAuto( gm->dtEdges(), aEdge) {
      if ( aEdge->meshAttributes.typeTransfinite == _type ) {

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
          staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
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
            % ( 100. * _polyI->YFloat( _checkX ) * _ll / _firstElementSize ) 
            % ( 100. * (*theRoot) ) 
          << std::endl;
        _grading.addComponent(
          _polyI.get(), aEdge->tag()
        );          
      }
    }
  }
  
	double bVOSetPrescribedFirstElementSize::F( double const * xx ) {
    _polyI->setDOF( std::vector<float>(_polyI->nDOF(), *xx) );
    return fabs(_polyI->YFloat(_checkX) * _ll - _firstElementSize);
	}    
}
