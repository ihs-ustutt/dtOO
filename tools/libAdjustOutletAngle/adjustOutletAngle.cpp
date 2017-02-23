#include "adjustOutletAngle.h"
#include "xmlHeaven/dtXmlParser.h"

#include <iostream>
#include <string>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/scaOneD.h>

#include <Math/Functor.h>
#include <Math/RootFinderAlgorithms.h>
#include <Math/BrentRootFinder.h>

namespace dtOO {
  adjustOutletAngle::adjustOutletAngle() : dtPlugin() {
    dt__info(
      adjustOutletAngle(), 
      << "Create adjustOutletAngle"
    );
  }

  adjustOutletAngle::~adjustOutletAngle() {
    dt__info(
      adjustOutletAngle(), 
      << "Destroy adjustOutletAngle"
    );
  }
  
  dtPlugin * adjustOutletAngle::create( void ) const {
    return new adjustOutletAngle();
  }

  double adjustOutletAngle::F( double xx ) {
    
    _cV->get( _adjustConstValueS )->setValue( 
      _cV->get( _adjustConstValueS )->getMin()
      +
      static_cast< float >(xx) 
      *
      (
        _cV->get( _adjustConstValueS )->getMax()
        -
        _cV->get( _adjustConstValueS )->getMin()
      )
    );
    aFPtrVec theF;
    //dtXmlParser::reference().createAnalyticFunction(_betaS, NULL, _cV, &theF);
    dtXmlParser::reference().createAnalyticFunction(_betaPrimeS, NULL, _cV, &theF);
    scaOneD const * beta = _beta;
    scaOneD const * betaMod = _betaMod;
    scaOneD const * betaPrime = scaOneD::ConstDownCast( theF[0] );
    
    int nn = 100;
    float const deltaR = 1./nn;
    float sum = 0.;
    dt__forFromToIndex(0, 100, ii) {
      
      float r_i = .5 * (1./nn) + ii * deltaR;
      sum 
      = 
      sum 
      + 
      r_i * r_i  * (  
        1./tan(M_PI * (beta->YFloat(r_i)+betaMod->YFloat(r_i))/180. ) 
        - 
        1./tan(M_PI * (beta->YFloat(r_i) + betaMod->YFloat(r_i) + betaPrime->YFloat(r_i))/180.) 
      ) * deltaR;
    }
    
    theF.destroy();
    
    return sum;
  }
   
  void adjustOutletAngle::init( 
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    dCPtrVec const * const dC,          
    dPPtrVec const * const pL
  ) {
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

    _betaS 
    = 
    qtXmlPrimitive::getAttributeStr("outletAngle", element);
    
    _beta = scaOneD::MustConstDownCast( aF->get(_betaS) );
    
    _betaModS 
    = 
    qtXmlPrimitive::getAttributeStr("outletAngleModification", element);
    _betaMod = scaOneD::MustConstDownCast( aF->get(_betaModS) );
    
    _betaPrimeS
    = 
    qtXmlPrimitive::getAttributeStr("outletAnglePrime", element);

    _adjustConstValueS
    = 
    qtXmlPrimitive::getAttributeStr("adjustConstValue", element);
    _cV = cV;    
  }

  void adjustOutletAngle::apply(void) {
    aFPtrVec theF;
    
    ::ROOT::Math::Functor1D fun(this, &adjustOutletAngle::F);
    // Create the Integrator
   ::ROOT::Math::BrentRootFinder brf;
 
   // Set parameters of the method
   brf.SetFunction( fun, 0., 1. );
   brf.SetNpx(100);
   brf.Solve( 10000, 1.e-8, 0. );

   double theR = brf.Root();
    dt__info( apply(), <<  "F( " << theR << " ) = " << F(theR) );
  }
}

//
// pugg driver
//
adjustOutletAngleDriver::adjustOutletAngleDriver() : dtPluginDriver(
  std::string("adjustOutletAngleDriver"), dtOO::dtPlugin::version
) { 

}

//
// pugg plugin creation via driver
//
dtOO::dtPlugin * adjustOutletAngleDriver::create() {
  return new dtOO::adjustOutletAngle();
}

//
// registrate plugin in driver
//
#include <pluginEngine/pugg/Kernel.h>
extern "C" void register_pugg_plugin(pugg::Kernel* kernel) 
  {kernel->add_driver(new adjustOutletAngleDriver());}


