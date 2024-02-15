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

#ifndef dtOCCGeomFillSimpleBound_H
#define	dtOCCGeomFillSimpleBound_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCGeomFillBoundary.h"

class GeomFill_SimpleBound;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCGeomFillSimpleBound : public dtOCCGeomFillBoundary {
    public:
      dt__class(dtOCCGeomFillSimpleBound, dtCurve);
      dtOCCGeomFillSimpleBound();
      dtOCCGeomFillSimpleBound( 
        dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
      );
      virtual ~dtOCCGeomFillSimpleBound();
      virtual dtOCCGeomFillSimpleBound * clone( void ) const;
      virtual dtReal minPara( dtInt const & dir ) const;
      virtual dtReal maxPara( dtInt const & dir ) const;    
      virtual dtPoint3 point( dtReal const uu ) const;
    private:
      GeomFill_SimpleBound const * _ptr;
  };
}
#endif	/* dtOCCGeomFillSimpleBound_H */
