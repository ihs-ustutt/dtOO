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

#ifndef pointGeometryOneDDist_H
#define	pointGeometryOneDDist_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include "floatAtt.h"

namespace dtOO { 
  class analyticGeometry;

  class pointGeometryOneDDist : public floatAtt {
    public:  
      dt__classOnlyName(pointGeometryOneDDist);
      pointGeometryOneDDist(
        dtReal const & value,
        analyticGeometry const * const aG,
        dtInt const & dir
      );
      virtual ~pointGeometryOneDDist();
      pointGeometryOneDDist( pointGeometryOneDDist const & orig );
      virtual pointGeometryOneDDist * clone( void ) const;
      virtual dtReal operator()( std::vector< dtReal > const & xx ) const;
      //
      // get properties
      //
      virtual dtInt const & dimension() const;
    private:
      dtReal const _value;
      dtInt const _dir;
      dtInt const _dimension;
      analyticGeometry const * const _aG;
  };
  dt__H_addCloneForpVH(pointGeometryOneDDist);
}
#endif	/* pointGeometryOneDDist_H */
