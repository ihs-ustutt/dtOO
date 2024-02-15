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

#ifndef THREADSAFE_H
#define	THREADSAFE_H

#include <dtOOTypeDef.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
  template< typename T >
  class threadSafe {
  public:
    dt__classOnlyName( threadSafe );
    threadSafe() : _t(staticPropertiesHandler::ompGetMaxThreads()) {
    }
    
    virtual ~threadSafe() {
      
    }
    
    T & operator()( void ) {
      return _t[ staticPropertiesHandler::ompGetThreadNum() ];
    };
  private:
    std::vector< T > _t;
  };
}

#endif	/* THREADSAFE_H */
