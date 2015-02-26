#ifndef PROGHELPER_H
#define	PROGHELPER_H

#include <logMe/logMe.h>
//#include <boost/utility.hpp>

#define dt__FORALL(vector, variable, content) \
    for (int variable = 0; variable<((vector).size());variable++) { \
      content \
    }
#define dt__FORALLINDEX(vector, index) \
    for (int index = 0; index<((vector).size());index++)
#define dt__FORALLITER(type, vector, iter) \
  for ( \
    type::iterator iter = vector.begin(); \
    iter!=vector.end(); \
    ++iter \
  )
#define dt__FORALLCONSTITER(type, vector, iter) \
  for ( \
    type::const_iterator iter = vector.begin(); \
    iter!=vector.end(); \
    ++iter \
  )
#define dt__MUSTDOWNCAST( object, type, result ) \
  result = dynamic_cast< type * >( object ); \
  if (result == NULL ) { \
    dt__THROW(MACRO(MUSTDOWNCAST), \
              << "dynamic_cast of "#object" to "#type" fails" ); \
    }
#define dt__PTRASS( toAss, ptr ) \
  if (ptr == NULL ) { \
    dt__THROW( dt__PTRASS, << "object "#ptr" cannot assigned to "#toAss ); \
  } \
  toAss = ptr
namespace dtOO {
  template <class T> inline T prior(T x) { return --x; }
  template <class T> inline T next(T x) { return ++x; }  
}
#define dt__NEXT( object ) dtOO::next( object )
#define dt__PRIOR( object ) dtOO::prior( object )
#define dt__TOFLOAT(toAss, value ) toAss = static_cast<float>(value)

#endif	/* PROGHELPER_H */

