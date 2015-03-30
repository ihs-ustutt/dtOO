#ifndef DTOMVERTEXFIELD_H
#define	DTOMVERTEXFIELD_H

#include <logMe/dtMacros.h>
#include <progHelper.h>
#include <interfaceHeaven/labelHandling.h>
#include "dtOMMesh.h"

class MVertex;

namespace dtOO {
  template < typename T >  
  class dtOMVertexField : public labelHandling {
  public:
    dt__CLASSNAME(dtOMVertexField);    
    dtOMVertexField( std::string const & label, dtOMMesh const & om, T const & init );
    virtual ~dtOMVertexField();
    T & operator[](omVertexH const & vH);    
    T const & operator[](omVertexH const & vH) const;    
    T & operator[](::MVertex const * const mv);
    T const & at(::MVertex const * const mv) const;    
    T const & operator[](::MVertex const * const mv) const;
    void execute( T (*executeMe)(T const & member) );
	  void laplacianSmooth( void );
    long unsigned int size( void ) const;
    dtOMMesh const & refMesh( void ) const;
  private:
    std::map< omVertexH, T > _field;
    dtOMMesh const & _om;
  };

  template < typename T >
  dtOMVertexField< T >::dtOMVertexField( 
    std::string const & label, dtOMMesh const & om, T const & init 
  ) : labelHandling(label), _om(om) {
		dt__forFromToIter(omConstVertexI, _om.vertices_begin(), _om.vertices_end(), vIt) {
      _field[*vIt] = init;
    }
  }
  
  template < typename T >
  dtOMVertexField< T >::~dtOMVertexField() {
    
  }    
  
  template < typename T >
  T & dtOMVertexField< T >::operator[](omVertexH const & vH) {
    return _field[vH];
  }
  
  template < typename T >
  T const & dtOMVertexField< T >::operator[](omVertexH const & vH) const {
    return _field.at(vH);
  }

  template < typename T >
  T & dtOMVertexField< T >::operator[](::MVertex const * const mv) {
    return _field[_om[mv]];
  }
  
  template < typename T >
  T const & dtOMVertexField< T >::at(::MVertex const * const mv) const {
    return _field.at(_om[mv]);
  }  
  
  template < typename T >
  T const & dtOMVertexField< T >::operator[](::MVertex const * const mv) const {
    return _field[_om[mv]];
  }
  
  template < typename T >
  void dtOMVertexField< T >::execute( T (*executeMe)(T const & member) ) {
    dt__forFromToIter(
      omConstVertexI, _om.vertices_begin(), _om.vertices_end(), vIt
    ) _field[*vIt] = (*executeMe)(_field[*vIt]);
  }
    
  template < typename T >
	void dtOMVertexField< T >::laplacianSmooth( void ) {
		std::vector< T > av(_field.size());
		int counter = 0;
		dt__forFromToIter(omConstVertexI, _om.vertices_begin(), _om.vertices_end(), vIt) {
      omConstVertexVertexI vBegin = _om.cvv_begin(*vIt);
			T tmp = _field[*vBegin];
			dt__forFromToIter(omConstVertexVertexI, _om.cvv_begin(*vIt), _om.cvv_end(*vIt), vvIt) {			
        tmp = tmp + _field[*vvIt];
			}
			av[counter] = tmp;
			counter++;
		}
		
		counter = 0;
		dt__forFromToIter(omConstVertexI, _om.vertices_begin(), _om.vertices_end(), vIt) {
			_field[*vIt] = _field[*vIt] + av[counter];
			counter++;
		}		
	}

  template < typename T >
  long unsigned int dtOMVertexField< T >::size( void ) const {
    return _field.size();
  }
  
  template < typename T >
  dtOMMesh const & dtOMVertexField< T >::refMesh( void ) const {
    return _om;
  }
}
#endif	/* DTOMVERTEXFIELD_H */

