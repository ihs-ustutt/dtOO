#ifndef PROGHELPER_H
#define	PROGHELPER_H

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <vector>
#include <list>

//#include <boost/utility.hpp>

#define dt__FORALL(vector, variable, content) \
    for (int variable = 0; variable<((vector).size());variable++) { \
      content \
    }
#define dt__forInnerIndex(vector, index) \
    for (int index = 1; index<((vector).size()-1);index++)
#define dt__forAllIter(type, vector, iter) \
  for ( \
    type::iterator iter = vector.begin(); \
    iter!=vector.end(); \
    ++iter \
  )
#define dt__forAllConstIter(type, vector, iter) \
  for ( \
    type::const_iterator iter = vector.begin(); \
    iter!=vector.end(); \
    ++iter \
  )
#define dt__forFromToIter(iterator_type, from, to, iter) \
  for ( \
    iterator_type iter = from; \
    iter != to; \
    ++iter \
  )
#define dt__FORALLINDEX(vector, index) \
  dt__forInnerIndex(vector, index)
#define dt__FORINNERINDEX(vector, index) \
  dt__forInnerIndex(vector, index)
#define dt__FORALLITER(type, vector, iter) \
  dt__forAllIter(type, vector, iter)
#define dt__FORALLCONSTITER(type, vector, iter) \
  dt__forAllConstIter(type, vector, iter)
#define dt__FROMTOITER(iterator_type, from, to, iter) \
  dt__forFromToIter(iterator_type, from, to, iter)
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

namespace dtOO {
  class progHelper {
    public:
      dt__CLASSNAME(progHelper);
      template < class T >
      static std::vector< T > list2Vector(std::list< T > & eeList) {
        std::vector< T > ee { 
          std::begin(eeList), 
          std::end(eeList) 
        };		
        return ee;
      }
    private:
      progHelper();
  };
}	

#endif	/* PROGHELPER_H */

