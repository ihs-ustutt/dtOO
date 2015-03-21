#ifndef PROGHELPER_H
#define	PROGHELPER_H

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <vector>
#include <list>

#define dt__forAllIndex(vector, index) \
  for (int index = 0; index<((vector).size());index++)
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
#define dt__mustDownCast( object, type, result ) \
  result = dynamic_cast< type * >( object ); \
  if (result == NULL ) { \
    dt__THROW(MACRO(MUSTDOWNCAST), \
              << "dynamic_cast of "#object" to "#type" fails" ); \
    }
#define dt__ptrAss( toAss, ptr ) \
  if (ptr == NULL ) { \
    dt__THROW( dt__ptrAss, << "object "#ptr" cannot assigned to "#toAss ); \
  } \
  toAss = ptr

#define dt__toFloat(toAss, value ) toAss = static_cast<float>(value)

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
      template <class T> 
      static T prior(T x) { 
        return --x; 
      }
      template <class T> 
      static T next(T x) { 
        return ++x; 
      }        
    private:
      progHelper();
  };
}
#endif	/* PROGHELPER_H */

