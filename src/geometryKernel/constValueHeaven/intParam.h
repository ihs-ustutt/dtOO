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

#ifndef INTPARAM_H
#define	INTPARAM_H

#include <dtOOTypeDef.h>

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class intParam : public constValue {
    public:
      dt__class(intParam, constValue);        
      intParam(intParam const & orig);
      intParam( std::string const & label, dtInt const & val );    
      virtual ~intParam();
      virtual intParam * clone( void ) const;    
      virtual void dump(void) const;
      virtual void writeToElement(
        ::QDomDocument & doc, ::QDomElement & element
      );
    private:
  };
}
#endif	/* INTPARAM_H */
