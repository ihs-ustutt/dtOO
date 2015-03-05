#include "perfRotatingSpline.h"

#include <logMe/logMe.h>

#include <do/coDoSet.h>
#include <analyticFunctionHeaven/scaFunctionReconstructor.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <progHelper.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtSislCurve.h>
#include <interfaceHeaven/ptrHandling.h>
#include <debugHeaven/extInfoContainer.h>

namespace dtOO {
  DTCLASSLOGMETHODIN(perfRotatingSpline);
  
  perfRotatingSpline::perfRotatingSpline() : rotatingSpline() {
    _phiU = NULL;
    _uPhi = NULL;
    _phiUNPoints = 2;
    _phiUOrder = 3;
    _meridianV = NULL;
    _vMeridian = NULL;
    _meridianVNPoints = 10;
    _meridianVOrder = 3;
    _meridianR = NULL;
    _rMeridian = NULL;
    _meridianRNPoints = 10;
    _meridianROrder = 3;        
  }

  perfRotatingSpline::perfRotatingSpline(const perfRotatingSpline& orig) : rotatingSpline(orig){
    _phiU = orig._phiU->clone();
    _uPhi = orig._uPhi->clone();
    _phiUNPoints = orig._phiUNPoints;
    _phiUOrder = orig._phiUOrder;
    _meridianVNPoints = orig._meridianVNPoints;
    _meridianV = orig._meridianV->clone();
    _vMeridian = orig._vMeridian->clone();
    _meridianRNPoints = orig._meridianRNPoints;
    _meridianR = orig._meridianR->clone();
    _rMeridian = orig._rMeridian->clone();
  }

  perfRotatingSpline::perfRotatingSpline(const rotatingSpline& orig) : rotatingSpline(orig){
    _phiU = NULL;
    _uPhi = NULL;
    _phiUNPoints = 2;
    _phiUOrder = 3;
    _meridianV = NULL;
    _vMeridian = NULL;
    _meridianVNPoints = 10;
    _meridianVOrder = 3;
    _meridianR = NULL;
    _rMeridian = NULL;
    _meridianRNPoints = 10;
    _meridianROrder = 3;   
  }  

  perfRotatingSpline::~perfRotatingSpline() {
    delete _phiU;
    delete _uPhi;
    delete _meridianV;
    delete _vMeridian;
    delete _meridianR;
    delete _rMeridian;    
  }

  void perfRotatingSpline::initInternalFunctions( void ) {
    //
    // create phi(u)
    //
    createPhiU();
    createMeridianV();
    createMeridianR();
  }  
  
  void perfRotatingSpline::createPhiU(void) {
    if (_phiU) delete _phiU;
    if (_uPhi) delete _uPhi;
    
    dtSurface const * surf = getConstDtSislSurf();
//    ptrHandling< dtCurve > circ( getCircleCurve( surf->getVMin() ) );
    std::vector< dtPoint2 > ppPhiU;// = ->getPointSetEquidistant2d( _phiUNPoints );
    std::vector< dtPoint2 > ppUPhi;
    
    //
    // calc angles
    //
//    float const ll = circ->getLength();
    float const rr = r_v(surf->getVMin());
    float const angleInc = 2.*M_PI/static_cast<float>(_phiUNPoints);
    for (int ii=0; ii<=_phiUNPoints;ii++) {
      float const iiF = static_cast<float>(ii);
      float xx = angleInc*iiF;
      //float yy = getParameterPhiRadiusMeridian(xx*rr, 0.).x();
      float yy = rotatingSpline::uv_phiM(xx, 0.).x();
      ppPhiU.push_back( dtPoint2( xx, yy) );
      ppUPhi.push_back( dtPoint2( yy, xx) );
    } 
    
    ptrHandling< dtCurve > dtC( new dtSislCurve(&ppPhiU, _phiUOrder) );
    _phiU = new scaSISLSplineTwoD( dtC.get() );
    dtC.reset( new dtSislCurve( &ppUPhi, _phiUOrder) );
    _uPhi = new scaSISLSplineTwoD( dtC.get() );
  }

  void perfRotatingSpline::createMeridianV(void) {
    if (_meridianV) delete _meridianV;
    if (_vMeridian) delete _vMeridian;

    dtSurface const * surf = getConstDtSislSurf();
    ptrHandling< dtCurve > merid( getRadiusCurve() );
    std::vector< dtPoint2 > pp;// = ->getPointSetEquidistant2d( _phiUNPoints );
    std::vector< dtPoint2 > ppInv;
    
    float const mmMax = merid->getLength();
    float const mmInc = mmMax/static_cast<float>(_meridianVNPoints);
    for (int ii=0; ii<=_meridianVNPoints; ii++) {
      float const iiF = static_cast<float>(ii);
      float xx = mmInc*iiF;
      float yy = rotatingSpline::uv_phiRadiusM(0., xx).y();
      pp.push_back( dtPoint2( xx, yy) );
      ppInv.push_back( dtPoint2( yy, xx) );
    } 
    
    ptrHandling< dtCurve > dtC( new dtSislCurve(&pp, _meridianVOrder) );
    _meridianV = new scaSISLSplineTwoD( dtC.get() );
    dtC.reset( new dtSislCurve(&ppInv, _meridianVOrder) );
    _vMeridian = new scaSISLSplineTwoD( dtC.get() );
  }

  void perfRotatingSpline::createMeridianR(void) {
    if (_meridianR) delete _meridianR;
    if (_rMeridian) delete _rMeridian;

    dtSurface const * surf = getConstDtSislSurf();
    ptrHandling< dtCurve > merid( getRadiusCurve() );
    std::vector< dtPoint2 > pp;// = ->getPointSetEquidistant2d( _phiUNPoints );
    std::vector< dtPoint2 > ppInv;
    
    float const mmMax = merid->getLength();
    float const mmInc = mmMax/static_cast<float>(_meridianRNPoints);
    for (int ii=0; ii<=_meridianRNPoints; ii++) {
      float const iiF = static_cast<float>(ii);
      float xx = mmInc*iiF;
      dtPoint2 uv = rotatingSpline::uv_phiRadiusM(0., xx);
      float yy = rotatingSpline::r_v(uv.y());
      pp.push_back( dtPoint2( xx, yy) );
      ppInv.push_back( dtPoint2( yy, xx) );
    } 
    
    ptrHandling< dtCurve > dtC( new dtSislCurve(&pp, _meridianROrder) );
    _meridianR = new scaSISLSplineTwoD( dtC.get() );
    dtC.reset( new dtSislCurve(&ppInv, _meridianROrder) );
    _rMeridian = new scaSISLSplineTwoD( dtC.get() );
  }  
  
  dtPoint2 perfRotatingSpline::uv_phiRadiusM(float const & phiRadius, float const & mm) const {    
    float vv = _meridianV->getValue(mm);
    float rr = _meridianR->getValue(mm);

    float uu;
    if (phiRadius < 0.) {
      uu = _phiU->getValue(fabs(phiRadius)/rr);
      uu = -1. * uu;
    }
    else {
      uu = _phiU->getValue(phiRadius/rr);
    }
    dtPoint2 retPoint(uu, vv);
    
    DTDEBUGWF(uv_phiRadiusM(),
            << DTLOGPOI2D(retPoint) << LOGDEL
            << DTLOGPOI2D(rotatingSpline::uv_phiRadiusM(phiRadius, mm)) );
    return retPoint;
  }
  
  dtPoint2 perfRotatingSpline::uv_deltaPhiRadiusDeltaM(
            float const & bU,
            float const & bV,
            float const & deltaPhiRadius, 
            float const & deltaMm) const {
    float phi = _uPhi->getValue(bU);
    float rr = r_v(bV);
    float mm = _vMeridian->getValue(bV);
    

    dtPoint2 retPoint = uv_phiRadiusM(phi*rr+deltaPhiRadius, mm+deltaMm);
    
    DTDEBUGWF(uv_deltaPhiRadiusDeltaM(),
            << DTLOGPOI2D(retPoint) << LOGDEL
            << DTLOGPOI2D(rotatingSpline::uv_deltaPhiRadiusDeltaM(bU, bV, deltaPhiRadius, deltaMm)) );
    return retPoint;
  }
  
  dtPoint2 perfRotatingSpline::uv_phiM(float const & phi, float const & mm) const {
    float vv = _meridianV->getValue(mm);
    float uu = _phiU->getValue(phi);

    dtPoint2 retPoint(uu, vv);
    
    DTDEBUGWF(uv_phiM(),
            << DTLOGPOI2D(retPoint) << LOGDEL
            << DTLOGPOI2D(rotatingSpline::uv_phiM(phi, mm)) );
    
    return retPoint;
  }
  
  float perfRotatingSpline::r_m(const float & meridLength) const {
    float retValue = _meridianR->getValue(meridLength);
    
    DTDEBUGWF(r_m(),
            << DTLOGEVAL(retValue) << LOGDEL
            << DTLOGEVAL(rotatingSpline::r_m(meridLength)) );
        
    
    return retValue;
  }
      
  void perfRotatingSpline::packToExtInfoContainer( extInfoContainer * const eIC ) const {
    if ( eIC != NULL ) {
      scaFunction * sF;
      sF = _phiU->clone();
      sF->setLabel( sF->getLabel()+"_phiU::"+getLabel() );
      eIC->addNoClone(sF);
      sF = _uPhi->clone();
      sF->setLabel( sF->getLabel()+"_UPhi::"+getLabel() );
      eIC->addNoClone(sF);      
      sF = _meridianV->clone();
      sF->setLabel( sF->getLabel()+"_meridianV::"+getLabel() );
      eIC->addNoClone(sF);      
      sF = _vMeridian->clone();
      sF->setLabel( sF->getLabel()+"_vMeridian::"+getLabel() );
      eIC->addNoClone(sF);      
      sF = _meridianR->clone();
      sF->setLabel( sF->getLabel()+"_meridianR::"+getLabel() );
      eIC->addNoClone(sF);      
      sF = _rMeridian->clone();
      sF->setLabel( sF->getLabel()+"_rMeridian::"+getLabel() );
      eIC->addNoClone(sF);            
    }
  }  
  
  covise::coDoSet * perfRotatingSpline::packToCoDoSet( char const * const str) const {       
    
    covise::coDistributedObject ** obj = new covise::coDistributedObject*[8];
    //
    // pack origin and rotation vector
    //   

    obj[0] = _phiU->packToCoDoSet( ("pRS::coDoSet::phiU::"+getLabel()).c_str() );
    obj[1] = _uPhi->packToCoDoSet( ("pRS::coDoSet::uPhi::"+getLabel()).c_str() );
    obj[2] = _meridianV->packToCoDoSet( ("pRS::coDoSet::meridianV::"+getLabel()).c_str() );
    obj[3] = _vMeridian->packToCoDoSet( ("pRS::coDoSet::vMeridian::"+getLabel()).c_str() );
    obj[4] = _meridianR->packToCoDoSet( ("pRS::coDoSet::meridianR::"+getLabel()).c_str() );
    obj[5] = _rMeridian->packToCoDoSet( ("pRS::coDoSet::rMeridian::"+getLabel()).c_str() );        
    obj[6] = const_cast< coDistributedObject * >(
               rotatingSpline::packToCoDoSet(("pRS::rS::coDoSet::"+getLabel()).c_str())->getElement(0)
             );
    
    obj[7] = NULL;
    
    covise::coDistributedObject ** retElem = new covise::coDistributedObject*[2];    
    retElem[0] = new covise::coDoSet(("rS::coDoSet::"+getLabel()).c_str(), obj);
    delete obj[0];
    delete obj[1];
    delete obj[2];
    delete obj[3];
    delete obj[4];
    delete obj[5];
    delete obj[6];
    delete [] obj;
    
    retElem[0]->addAttribute("factoryType", "perfRotatingSpline");
    retElem[0]->addAttribute("name", getLabel().c_str());
    retElem[1] = NULL;
    covise::coDoSet * retSet = new covise::coDoSet(str, retElem);
    
    delete retElem[0];
    delete [] retElem;
    
    return retSet;
  }

  void perfRotatingSpline::unpackFromCoDoSet( covise::coDoSet const * const set ) {           
    //
    //unpack origin and rotation vector
    //
    //0
    covise::coDoSet const * phiUSet;
    dt__MUSTDOWNCAST(set->getElement(0), covise::coDoSet const, phiUSet);
    covise::coDoSet const * phiUSetSet;
    dt__MUSTDOWNCAST(phiUSet->getElement(0), covise::coDoSet const, phiUSetSet);    
    //1
    covise::coDoSet const * UPhiSet;
    dt__MUSTDOWNCAST(set->getElement(1), covise::coDoSet const, UPhiSet);
    covise::coDoSet const * UPhiSetSet;
    dt__MUSTDOWNCAST(UPhiSet->getElement(0), covise::coDoSet const, UPhiSetSet);    
    //2
    covise::coDoSet const * meridianVSet;
    dt__MUSTDOWNCAST(set->getElement(2), covise::coDoSet const, meridianVSet);
    covise::coDoSet const * meridianVSetSet;
    dt__MUSTDOWNCAST(meridianVSet->getElement(0), covise::coDoSet const, meridianVSetSet);    
    //3
    covise::coDoSet const * vMeridianSet;
    dt__MUSTDOWNCAST(set->getElement(3), covise::coDoSet const, vMeridianSet);
    covise::coDoSet const * vMeridianSetSet;
    dt__MUSTDOWNCAST(vMeridianSet->getElement(0), covise::coDoSet const, vMeridianSetSet);    
    //4
    covise::coDoSet const * meridianRSet;
    dt__MUSTDOWNCAST(set->getElement(4), covise::coDoSet const, meridianRSet);
    covise::coDoSet const * meridianRSetSet;
    dt__MUSTDOWNCAST(meridianRSet->getElement(0), covise::coDoSet const, meridianRSetSet);    
    //5
    covise::coDoSet const * rMeridianSet;
    dt__MUSTDOWNCAST(set->getElement(5), covise::coDoSet const, rMeridianSet);
    covise::coDoSet const * rMeridianSetSet;
    dt__MUSTDOWNCAST(rMeridianSet->getElement(0), covise::coDoSet const, rMeridianSetSet);    
    //6
    covise::coDoSet const * rSSet;
    dt__MUSTDOWNCAST(set->getElement(6), covise::coDoSet const, rSSet);

    //
    // call on base class
    //
    rotatingSpline::unpackFromCoDoSet(rSSet);
    
    //
    // store functions
    //   
    _phiU = scaFunctionReconstructor().reconFromCoDoSet(phiUSetSet);
    _uPhi = scaFunctionReconstructor().reconFromCoDoSet(UPhiSetSet);
    _meridianV = scaFunctionReconstructor().reconFromCoDoSet(meridianVSetSet);
    _vMeridian = scaFunctionReconstructor().reconFromCoDoSet(vMeridianSetSet);
    _meridianR = scaFunctionReconstructor().reconFromCoDoSet(meridianRSetSet);
    _rMeridian = scaFunctionReconstructor().reconFromCoDoSet(rMeridianSetSet);    
    //
    // set name
    //
    setLabel( set->getAttribute("name") ); 
  }  
}
