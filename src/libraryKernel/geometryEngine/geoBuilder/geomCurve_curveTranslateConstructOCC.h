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

#ifndef geomCurve_curveTranslateConstructOCC_H
#define	geomCurve_curveTranslateConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  
  class geomCurve_curveTranslateConstructOCC {
  public:
    dt__classOnlyName(geomCurve_curveTranslateConstructOCC);    
    geomCurve_curveTranslateConstructOCC();
    geomCurve_curveTranslateConstructOCC(
      dtCurve const * const dtC, dtVector3 const & vv
    );
    virtual ~geomCurve_curveTranslateConstructOCC();
    dtCurve * result( void );    
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_curveTranslateConstructOCC_H */
