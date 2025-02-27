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

#ifndef geometryGeometryDist_H
#define	geometryGeometryDist_H

#include <dtOOTypeDef.h>

//#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include "floatAtt.h"

namespace dtOO { 
  class analyticGeometry;

  class geometryGeometryDist : public floatAtt {
    public:  
      dt__classOnlyName(geometryGeometryDist);
      geometryGeometryDist(
        analyticGeometry const * const aG0,
        analyticGeometry const * const aG1
      );
      virtual ~geometryGeometryDist();
      geometryGeometryDist( geometryGeometryDist const & orig );
      virtual geometryGeometryDist * clone( void ) const;
      virtual dtReal operator()( std::vector< dtReal > const & xx ) const;
      //
      // get properties
      //
      virtual dtInt const & dimension() const;
    private:
      dtInt const _dimension;
      analyticGeometry const * const _aG0;
      analyticGeometry const * const _aG1;
  };
  dt__H_addCloneForpVH(geometryGeometryDist);
}
#endif	/* geometryGeometryDist_H */
