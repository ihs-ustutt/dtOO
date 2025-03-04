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

#ifndef scaOneDPolyIElSize_H
#define	scaOneDPolyIElSize_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include "floatAtt.h"

namespace dtOO { 
  class scaOneDPolyInterface;

  class scaOneDPolyIElSize : public floatAtt {
    public:  
      dt__classOnlyName(scaOneDPolyIElSize);
      scaOneDPolyIElSize(
        scaOneDPolyInterface * const polyI,
        dtReal const & x0,
        dtReal const & x1,
        dtReal const & lByS0,
        dtReal const & lByS1
      );
      virtual ~scaOneDPolyIElSize();
      scaOneDPolyIElSize( scaOneDPolyIElSize const & orig );
      virtual scaOneDPolyIElSize * clone( void ) const;
      virtual dtReal operator()( std::vector< dtReal > const & xx ) const;
      //
      // get properties
      //
      virtual dtInt const & dimension() const;
    private:
      scaOneDPolyInterface * const _polyI;
      dtReal const _lByS0;
      dtReal const _lByS1;
      dtReal const _x0;
      dtReal const _x1;
      dtInt const _dimension;
  };
  dt__H_addCloneForpVH(scaOneDPolyIElSize);
}
#endif	/* scaOneDPolyIElSize_H */
