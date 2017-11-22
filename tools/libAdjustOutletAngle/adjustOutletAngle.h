#ifndef adjustOutletAngle_H
#define	adjustOutletAngle_H

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <pluginEngine/pugg/Driver.h>
#include <dtPlugin.h>
#include <dtPluginDriver.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <dtCase.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class baseContainer;
  class scaOneD;
  
  class adjustOutletAngle : public dtPlugin {
  public:
    dt__class(adjustOutletAngle, dtPlugin);    
//    double UAbl_R ( double const * xx ) const ;
//    double VAbl_R ( double const * xx ) const ;

//    int Grinding ( int Teiler);
//    int RootFinding();
    adjustOutletAngle();
    virtual ~adjustOutletAngle();
    virtual dtPlugin * create( void ) const;    
    virtual void init( 
      ::QDomElement const & element,
      baseContainer * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      dCPtrVec const * const dC,
      dPPtrVec const * const pL 
    );
    virtual void apply(void);
  private:
    double F( double xx );
  private:
    std::string _betaS;
    scaOneD const * _beta;
    std::string _betaPrimeS;
    std::string _betaModS;
    scaOneD const * _betaMod;
//    baseContainer const * _bC;
    std::string _adjustConstValueS;
		cVPtrVec const * _cV;    
//    aFPtrVec const * _aF;    
//    cVPtrVec 
//    dtPoint3  _point;
//    double _interval;
//    double _transition;
//    double _tolerance;
//    double _result[2];
//    double _extension[2];
//    int _printlevel;
//    float _span;
//    double _auxiliary;
//    int _precision;
    
    
    
  };
}

class adjustOutletAngleDriver : public dtOO::dtPluginDriver {
public:
    adjustOutletAngleDriver();
    dtOO::dtPlugin * create();
};

#endif	/* adjustOutletAngle_H */