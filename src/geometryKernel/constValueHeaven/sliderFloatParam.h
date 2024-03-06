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

#ifndef SLIDERFLOATPARAM_H
#define	SLIDERFLOATPARAM_H

#include <dtOOTypeDef.h>

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class sliderFloatParam : public constValue {
    public:
      dt__class(sliderFloatParam, constValue);    
      sliderFloatParam(sliderFloatParam const & orig);
      sliderFloatParam(
        std::string const & label, dtReal const & val
      );
      sliderFloatParam(
        std::string const & label, 
        dtReal const & val, dtReal const & min, dtReal const & max
      );
      virtual ~sliderFloatParam();
      virtual sliderFloatParam * clone( void ) const;
      virtual void dump(void) const;
      virtual void writeToElement(
        ::QDomDocument & doc, ::QDomElement & element
      );
  };
}
#endif	/* SLIDERFLOATPARAM_H */
