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

#ifndef DTOMVERTEXFIELD_H
#define	DTOMVERTEXFIELD_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <logMe/dtMacros.h>
#include "dtOMField.h"
#include "dtOMMesh.h"

class MVertex;

namespace dtOO {
  template < typename T >  
  class dtOMVertexField : public dtOMField {
  public:
    dt__class(dtOMVertexField, dtOMField);    
    dtOMVertexField( 
      std::string const & label, dtOMMesh const & om, T const & init 
    );
    virtual ~dtOMVertexField();
    T & operator[](omVertexH const & vH);    
    T const & at(omVertexH const & vH) const;    
    T & operator[](::MVertex const * const mv);
    T const & at(::MVertex const * const mv) const;    
    void assign( dtOMMesh const & om, T const & value );    
    void execute( T (*executeMe)(T const & member) );
	  T oneRingAverage( omVertexH const & vH ) const;
    dtLongUnsInt size( void ) const;
    virtual void update( void );    
  private:
    std::map< omVertexH, T > _field;
    T _init;
  };

  template < typename T >
  dtOMVertexField< T >::dtOMVertexField( 
    std::string const & label, dtOMMesh const & om, T const & init 
  ) : dtOMField(label, om), _init(init) {
		dt__forFromToIter(
      omConstVertexI, refMesh().vertices_begin(), refMesh().vertices_end(), vIt
    ) _field[*vIt] = init;
  }
  
  template < typename T >
  dtOMVertexField< T >::~dtOMVertexField() {
    
  }    
  
  template < typename T >
  T & dtOMVertexField< T >::operator[](omVertexH const & vH) {
    return _field[vH];
  }
  
  template < typename T >
  T const & dtOMVertexField< T >::at(omVertexH const & vH) const {
    return _field.at(vH);
  }

  template < typename T >
  T & dtOMVertexField< T >::operator[](::MVertex const * const mv) {
    return _field[refMesh().at(mv)];
  }
  
  template < typename T >
  T const & dtOMVertexField< T >::at(::MVertex const * const mv) const {
    return _field.at(refMesh().at(mv));
  }  
  
  template < typename T >  
  void dtOMVertexField< T >::assign( dtOMMesh const & om, T const & value ) {
		dt__forFromToIter(
      omConstVertexI, om.vertices_begin(), om.vertices_end(), vIt
    ) {
      ::MVertex const * const mv = om.at(*vIt);
      omVertexH const vH = refMesh().at(mv);
      _field[vH] = value;
    }    
  }
    
  template < typename T >
  void dtOMVertexField< T >::execute( T (*executeMe)(T const & member) ) {
    dt__forFromToIter(
      omConstVertexI, refMesh().vertices_begin(), refMesh().vertices_end(), vIt
    ) _field[*vIt] = (*executeMe)(_field[*vIt]);
  }
    
  template < typename T >
	T dtOMVertexField< T >::oneRingAverage( omVertexH const & vH ) const {
    T tmp = _field.at(vH);
    dtReal tmpC = 1.;
    dt__forFromToIter(
      omConstVertexVertexI, refMesh().cvv_begin(vH), refMesh().cvv_end(vH), vvIt
    ) {			
      tmp = tmp + _field.at(*vvIt);
      tmpC = tmpC + 1.;
    }
    return (1./tmpC) * tmp;
	}

  template < typename T >
  dtLongUnsInt dtOMVertexField< T >::size( void ) const {
    //return _om.n_vertices();
    return _field.size();
  }
  
  template < typename T >
  void dtOMVertexField< T >::update( void ) {
    dt__forFromToIter(
      omConstVertexI, refMesh().vertices_begin(), refMesh().vertices_end(), vIt
    ) {
      typename std::map< omVertexH, T >::iterator it = _field.find(*vIt);
      if (it == _field.end())  {
        _field[*vIt] = _init;
      }
    }    
  }
}
#endif	/* DTOMVERTEXFIELD_H */
