#ifndef DTOMEDGEFIELD_H
#define	DTOMEDGEFIELD_H

#include <logMe/dtMacros.h>
#include <progHelper.h>
#include <interfaceHeaven/labelHandling.h>
#include "dtOMMesh.h"

class MVertex;

namespace dtOO {
  template < typename T >  
  class dtOMEdgeField : public labelHandling {
  public:
    dt__CLASSNAME(dtOMEdgeField);    
    dtOMEdgeField( std::string const & label, dtOMMesh const & om, T const & init );
    virtual ~dtOMEdgeField();
    T & operator[](omEdgeH const & eH);    
    T const & operator[](omEdgeH const & eH) const;    
    void execute( T (*executeMe)(T const & member) );
    long unsigned int size( void ) const;
    dtOMMesh const & refMesh( void ) const;
  private:
    std::map< omEdgeH, T > _field;
    dtOMMesh const & _om;
  };

  template < typename T >
  dtOMEdgeField< T >::dtOMEdgeField( 
    std::string const & label, dtOMMesh const & om, T const & init 
  ) : labelHandling(label), _om(om) {
		dt__forFromToIter(omConstEdgeI, _om.edges_begin(), _om.edges_end(), eIt) {
      _field[*eIt] = init;
    }
  }
  
  template < typename T >
  dtOMEdgeField< T >::~dtOMEdgeField() {
    
  }    
  
  template < typename T >
  T & dtOMEdgeField< T >::operator[](omEdgeH const & eH) {
    return _field[eH];
  }
  
  template < typename T >
  T const & dtOMEdgeField< T >::operator[](omEdgeH const & eH) const {
    return _field.at(eH);
  }
  
  template < typename T >
  void dtOMEdgeField< T >::execute( T (*executeMe)(T const & member) ) {
    dt__forFromToIter(
      omConstEdgeI, _om.edges_begin(), _om.edges_end(), eIt
    ) _field[*eIt] = (*executeMe)(_field[*eIt]);
  }

  template < typename T >
  long unsigned int dtOMEdgeField< T >::size( void ) const {
    return _field.size();
  }
  
  template < typename T >
  dtOMMesh const & dtOMEdgeField< T >::refMesh( void ) const {
    return _om;
  }
}
#endif	/* DTOMEDGEFIELD_H */

