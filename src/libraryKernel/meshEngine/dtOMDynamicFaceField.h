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

#ifndef dtOMDynamicFaceField_H
#define	dtOMDynamicFaceField_H

#include <dtOOTypeDef.h>

#include "dtOMFaceField.h"

namespace dtOO {
  template < typename T >  
  class dtOMDynamicFaceField : public dtOMFaceField< T > {
  public:
    dt__class(dtOMDynamicFaceField, dtOMFaceField< T >);
    dtOMDynamicFaceField(
      std::string const & label, dtOMMesh & om, T const & init
    );
    virtual ~dtOMDynamicFaceField();
  private:
    dtOMMesh & _om;
  };

  template < typename T >
  dtOMDynamicFaceField< T >::dtOMDynamicFaceField( 
    std::string const & label, dtOMMesh & om, T const & init 
  ) : dtOMFaceField< T >(label, om, init), _om(om) {
    _om.enqueueField(this);
  }
  
  template < typename T >
  dtOMDynamicFaceField< T >::~dtOMDynamicFaceField() {
    _om.dequeueField(this);    
  }    
}
#endif	/* dtOMDynamicFaceField_H */
