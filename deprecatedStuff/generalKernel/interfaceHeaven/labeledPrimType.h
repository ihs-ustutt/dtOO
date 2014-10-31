#ifndef LABELEDPRIMTYPE_H
#define	LABELEDPRIMTYPE_H

#include "labelHandling.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
  //----------------------------------------------------------------------------
  // h
  //----------------------------------------------------------------------------  
  template < typename T >  
  class labeledPrimType : public labelHandling {
  public:
    dt__CLASSNAME(labeledPrimType);  
    labeledPrimType();
    labeledPrimType(const labeledPrimType& orig);
    labeledPrimType( std::string const label);
    virtual ~labeledPrimType();
    T & getRefToVal( void );
  private:
    T _val;
  };
  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------  
  template< typename T >  
  labeledPrimType< T >::labeledPrimType() {
  }

  template< typename T >
  labeledPrimType< T >::labeledPrimType(const labeledPrimType& orig) {
  }
  
  template< typename T >
  labeledPrimType< T >::labeledPrimType( std::string const label) {
    labelHandling::setLabel( label );
  }
  
  template< typename T >
  labeledPrimType< T >::~labeledPrimType() {
  }
  
  template< typename T >  
  T & labeledPrimType< T >::getRefToVal( void ) {
    return _val;
  }  
}
#endif	/* LABELEDPRIMTYPE_H */

