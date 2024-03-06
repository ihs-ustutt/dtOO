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

#ifndef scaOneDPolyInterface_H
#define scaOneDPolyInterface_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"

namespace dtOO {
  class dtTransformer;
  
  class scaOneDPolyInterface : public scaOneD {
    public:
      dt__class(scaOneDPolyInterface, analyticFunction);
      scaOneDPolyInterface();
      scaOneDPolyInterface(scaOneDPolyInterface const & orig);    
      virtual scaOneDPolyInterface * clone( void ) const = 0;
      virtual ~scaOneDPolyInterface();
      virtual dtInt nDOF( void ) const = 0;
      virtual void setDOF( std::vector< dtReal > const value ) = 0;
  };
  dt__H_addCloneForpVH(scaOneDPolyInterface);
}
#endif /* scaOneDPolyInterface_H */
