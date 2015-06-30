#ifndef dtOMFaceField_H
#define	dtOMFaceField_H

#include <logMe/dtMacros.h>
#include <progHelper.h>
#include "dtOMField.h"
#include "dtOMMesh.h"

namespace dtOO {
  template < typename T >  
  class dtOMFaceField : public dtOMField {
  public:
    dt__class(dtOMFaceField, dtOMField);
    dtOMFaceField(
      std::string const & label, dtOMMesh const & om, T const & init
    );
    virtual ~dtOMFaceField();
    T & operator[](omFaceH const & fH);
    T const & at(omFaceH const & fH) const;
    void assign( dtOMMesh const & om, T const & value );
    void execute( T (*executeMe)(T const & member) );
    long unsigned int size( void ) const;
    virtual void update( void );     
  private:
    std::map< omFaceH, T > _field;
    T _init;
  };

  template < typename T >
  dtOMFaceField< T >::dtOMFaceField( 
    std::string const & label, dtOMMesh const & om, T const & init 
  ) : dtOMField(label, om), _init(init) {
		dt__forFromToIter(
      omConstFaceI, refMesh().faces_begin(), refMesh().faces_end(), fIt
    ) _field[*fIt] = init;
  }
  
  template < typename T >
  dtOMFaceField< T >::~dtOMFaceField() {
    
  }    
  
  template < typename T >
  T & dtOMFaceField< T >::operator[](omFaceH const & fH) {
    return _field[fH];
  }
  
  template < typename T >
  T const & dtOMFaceField< T >::at(omFaceH const & fH) const {
    return _field.at(fH);
  }  
  
  template < typename T >  
  void dtOMFaceField< T >::assign( dtOMMesh const & om, T const & value ) {
		dt__forFromToIter(
      omConstFaceI, om.faces_begin(), om.faces_end(), fIt
    ) {
      ::MElement const * const me = om.at(*fIt);
      omFaceH const fH = refMesh().at(me);
      _field[fH] = value;
    }    
  }  
  
  template < typename T >
  void dtOMFaceField< T >::execute( T (*executeMe)(T const & member) ) {
    dt__forFromToIter(
      omConstEdgeI, refMesh().edges_begin(), refMesh().edges_end(), eIt
    ) _field[*eIt] = (*executeMe)(_field[*eIt]);
  }

  template < typename T >
  long unsigned int dtOMFaceField< T >::size( void ) const {
    return _field.size();
  }  

  template < typename T >
  void dtOMFaceField< T >::update( void ) {
    dt__forFromToIter(
      omConstFaceI, refMesh().faces_begin(), refMesh().faces_end(), fIt
    ) {
      typename std::map< omFaceH, T >::iterator it = _field.find(*fIt);
      if (it == _field.end()) {
        _field[*fIt] = _init;
      }
    }
  }
}
#endif	/* dtOMFaceField_H */

