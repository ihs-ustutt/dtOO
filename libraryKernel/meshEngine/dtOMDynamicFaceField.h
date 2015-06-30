#ifndef dtOMDynamicFaceField_H
#define	dtOMDynamicFaceField_H

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

