/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

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
      static_cast< dtReal >(xx) 
      *
      (
        _cV->get( _adjustConstValueS )->getMax()
        -
        _cV->get( _adjustConstValueS )->getMin()
      )
    );
    lvH_analyticFunction theF;
    //dtXmlParser::reference().createAnalyticFunction(_betaS, NULL, _cV, &theF);
    dtXmlParser::reference().createAnalyticFunction(_betaPrimeS, NULL, _cV, &theF);
    scaOneD const * beta = _beta;
    scaOneD const * betaMod = _betaMod;
    scaOneD const * betaPrime = scaOneD::ConstDownCast( theF[0] );
    
    dtInt nn = 100;
    dtReal const deltaR = 1./nn;
    dtReal sum = 0.;
    dt__forFromToIndex(0, 100, ii) {
      
      dtReal r_i = .5 * (1./nn) + ii * deltaR;
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
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC,          
    lvH_dtPlugin const * const pL
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
    lvH_analyticFunction theF;
    
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
