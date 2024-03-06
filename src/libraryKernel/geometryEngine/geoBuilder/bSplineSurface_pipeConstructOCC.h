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

#ifndef BSPLINESURFACE_PIPECONSTRUCTOCC_H
#define	BSPLINESURFACE_PIPECONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_pipeConstructOCC {
  public:
    dt__classOnlyName(bSplineSurface_pipeConstructOCC);
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, 
      dtCurve const * const aSecOne, 
      dtCurve const * const aSecTwo,
      bool const & polynomial = false,
      dtInt const & absShape = 2,
      dtInt const & maxDegree = 11,
      dtInt const & nbSections = 30      
    );
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, 
      dtCurve const * const aSecOne,
      bool const & polynomial = false,
      dtInt const & absShape = 2,
      dtInt const & maxDegree = 11,
      dtInt const & nbSections = 30
    );    
    bSplineSurface_pipeConstructOCC(
      dtCurve const * const aPath, 
      dtReal const & rr,
      bool const & polynomial,
      dtInt const & absShape,
      dtInt const & maxDegree,
      dtInt const & nbSections      
    );
    virtual ~bSplineSurface_pipeConstructOCC();
    dtSurface * result( void );
  private:
    dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_PIPECONSTRUCTOCC_H */
