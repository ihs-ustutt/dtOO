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
    dtInt Grinding ( dtInt Teiler);
    dtInt RootFinding();
    surfaceComparison();
    virtual ~surfaceComparison();
    virtual dtPlugin * create( void ) const;    
    virtual void init( 
      ::QDomElement const & element,
      baseContainer * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticGeometry const * const aG,
      lvH_boundedVolume const * const bV,
      lvH_dtCase const * const dC,
      lvH_dtPlugin const * const pL 
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
    dtInt _printlevel;
    dtReal _span;
    double _auxiliary;
    dtInt _precision;
    
    
    
  };
}

class surfaceComparisonDriver : public dtOO::dtPluginDriver {
public:
    surfaceComparisonDriver();
    dtOO::dtPlugin * create();
};

#endif	/* surfaceComparison_H */
