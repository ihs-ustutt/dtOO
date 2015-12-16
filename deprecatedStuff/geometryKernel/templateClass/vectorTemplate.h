#ifndef VECTORTEMPLATE_H
#define	VECTORTEMPLATE_H

#include <vector>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  template <typename T> class vectorTemplate {
  public:
    dt__classOnlyName(vectorTemplate);
    vectorTemplate();

    ~vectorTemplate();
    void clear( void );
    void clearDeletePointer( void );
    void setAttribute( T const * const attribute);
    void setAttribute( T const & attribute);

    T * getAttribute(int indexOfAttribute);
    T & getAttributeRef(int indexOfAttribute);

    int getNAttributes(void) const;
  private:
    std::vector< T > _vector;
  };

  template <typename T> 
  vectorTemplate<T>::vectorTemplate() {

  }

  template <typename T> vectorTemplate<T>::~vectorTemplate() {
      _vector.clear();
  }

  template <typename T> void vectorTemplate<T>::clear( void ) {
    _vector.clear();
  }

  template <typename T> void vectorTemplate<T>::clearDeletePointer( void ) {
    dt__FORALL(_vector, ii,
      delete _vector[ii];
    );
    _vector.clear();
          
  }  
  
  template <typename T> void vectorTemplate<T>::setAttribute(T const * const attribute) {
    _vector.push_back(*attribute);
  }

  template <typename T> void vectorTemplate<T>::setAttribute(T const & attribute) {
    _vector.push_back(attribute);
  }
  
  template <typename T> T * vectorTemplate<T>::getAttribute(int indexOfAttribute) {
    if (indexOfAttribute > _vector.size() ) {
      dt__THROW(getAttribute(),
              << dt__eval(indexOfAttribute) << std::endl
              << "Exceeds vector size.");
    }
    return &(_vector[indexOfAttribute-1]);
  }
  
  template <typename T> T & vectorTemplate<T>::getAttributeRef(int indexOfAttribute) {
    if (indexOfAttribute >= _vector.size() ) {
      dt__THROW(getAttributeRef(),
              << dt__eval(indexOfAttribute)
              << " exceeds vector size.");
    }
    return _vector[indexOfAttribute];
  }  

  template <typename T> int vectorTemplate<T>::getNAttributes(void) const {
    return _vector.size();
  }
}
#endif	/* VECTORTEMPLATE_H */

