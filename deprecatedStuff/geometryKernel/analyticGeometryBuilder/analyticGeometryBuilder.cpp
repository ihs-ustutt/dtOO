#include "analyticGeometryBuilder.h"

namespace dtOO {
  DTCLASSLOGMETHODI(analyticGeometryBuilder,
          << dt__eval( &(thisRef._point) ) << std::endl
          << dt__eval( &(thisRef._pointTwo) ) << std::endl
          << dt__eval( &(thisRef._vector) ) << std::endl
          << dt__eval( &(thisRef._integer) ) << std::endl
          << dt__eval( &(thisRef._float) ) << std::endl
          << dt__eval( &(thisRef._analyticGeometryP) ) << std::endl
          << dt__eval( &(thisRef._scaFunctionP) ) << std::endl
          << dt__eval( &(thisRef._option) ) );

  analyticGeometryBuilder::analyticGeometryBuilder() {

  }

  analyticGeometryBuilder::~analyticGeometryBuilder() {
    _point.clear();  
    _vector.clear();
    _integer.clear();
    _float.clear();
    _analyticGeometryP.clearDeletePointer();
    _scaFunctionP.clearDeletePointer();
    _option.clear();    

  }

//  void analyticGeometryBuilder::final(void) {
//    _point.~vectorTemplate();  
//    _vector.~vectorTemplate();
//    _integer.~vectorTemplate();
//    _float.~vectorTemplate();
//    _analyticGeometryP.~vectorTemplate();
//    _geometryP.~vectorTemplate();
//    _scaFunctionP.~vectorTemplate();
//    _option.~vectorTemplate();
//  }
  
  void analyticGeometryBuilder::final(void) {
    for (int ii=1;ii<=_point.getNAttributes();ii++) {
      _point.getAttribute(ii)->~dtPoint3();
    }
    _point.clear();
    for (int ii=0;ii<_vector.getNAttributes();ii++) {
      _vector.getAttribute(ii+1)->~dtVector3();
    }
    _vector.clear();
    _integer.clear();
    _float.clear();
    for (int ii=0;ii<_analyticGeometryP.getNAttributes();ii++) {
      delete *(_analyticGeometryP.getAttribute(ii+1));
    }
    _analyticGeometryP.clear();
    for (int ii=0;ii<_scaFunctionP.getNAttributes();ii++) {
      delete *(_scaFunctionP.getAttribute(ii+1));
    }
    _scaFunctionP.clear();
    _option.clear();
  }

  void analyticGeometryBuilder::setAttribute(dtPoint3 const * const _attributeToSet) {
    _point.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(dtPoint2 const * const _attributeToSet) {
    _pointTwo.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(dtVector3 const * const _attributeToSet) {
    _vector.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(int const * const _attributeToSet) {
    _integer.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(float const * const _attributeToSet) {
    _float.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(analyticGeometry* const * const _attributeToSet) {
    _analyticGeometryP.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(scaFunction* const * const _attributeToSet) {
    _scaFunctionP.setAttribute(_attributeToSet);
  }

  void analyticGeometryBuilder::setAttribute(string const * const _attributeToSet) {
    _option.setAttribute(_attributeToSet);
  }
}