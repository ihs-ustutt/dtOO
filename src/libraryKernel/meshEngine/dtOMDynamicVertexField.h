#ifndef dtOMDynamicVertexField_H
#define	dtOMDynamicVertexField_H

#include <dtOOTypeDef.h>

#include "dtOMVertexField.h"

namespace dtOO {
  template < typename T >  
  class dtOMDynamicVertexField : public dtOMVertexField< T > {
    public:
      dt__class(dtOMDynamicVertexField, dtOMVertexField< T >);    
      dtOMDynamicVertexField( 
        std::string const & label, dtOMMesh & om, T const & init 
      );
      virtual ~dtOMDynamicVertexField();
    private:
      dtOMMesh & _om;
  };

  template < typename T >
  dtOMDynamicVertexField< T >::dtOMDynamicVertexField( 
    std::string const & label, dtOMMesh & om, T const & init 
  ) : dtOMVertexField< T >(label, om, init), _om(om) {
    _om.enqueueField(this);
  }
  
  template < typename T >
  dtOMDynamicVertexField< T >::~dtOMDynamicVertexField() {
    _om.dequeueField(this);
  }
}
#endif	/* dtOMDynamicVertexField_H */

