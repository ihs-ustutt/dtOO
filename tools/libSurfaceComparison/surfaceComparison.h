#ifndef surfaceComparison_H
#define	surfaceComparison_H

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
  
  class surfaceComparison : public dtPlugin {
  public:
    dt__class(surfaceComparison, dtPlugin);    
    double UAbl_R ( double const * xx ) const ;
    double VAbl_R ( double const * xx ) const ;
    double Euclidean_Distance (double const *Wert) const;
    int Grinding ( int Teiler);
    int RootFinding();
    surfaceComparison();
    virtual ~surfaceComparison();
    virtual dtPlugin * create( void ) const;    
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      dCPtrVec const * const dC,
      dPPtrVec const * const pL 
    );
    virtual void apply(void);
  private:
    analyticGeometry * _surface;
    analyticGeometry * _refSurface;
    dtPoint3  _point;
    double _interval;
    double _transition;
    double _tolerance;
    double _result[2];
    double _extension[2];
    int _printlevel;
    float _span;
    double _auxiliary;
    int _precision;
    
    
    
  };
}

class surfaceComparisonDriver : public dtOO::dtPluginDriver {
public:
    surfaceComparisonDriver();
    dtOO::dtPlugin * create();
};

#endif	/* surfaceComparison_H */