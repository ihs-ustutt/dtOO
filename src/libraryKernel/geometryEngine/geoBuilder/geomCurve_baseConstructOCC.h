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

#ifndef geomCurve_baseConstructOCC_H
#define	geomCurve_baseConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtOCCCurveBase;
  
  class geomCurve_baseConstructOCC {
  public:
    dt__classOnlyName(geomCurve_baseConstructOCC);
    geomCurve_baseConstructOCC( dtOCCCurveBase const & base );
    virtual ~geomCurve_baseConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* geomCurve_baseConstructOCC_H */
