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
#define dt__forFromToIndex(from, to, index) \
  for (int index = 0; index<to;index++)
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
#define dt__mustCast( object, type, result ) \
  result = dynamic_cast< type * >( object ); \
  if (result == NULL ) \
    dt__throw(dt__mustCast, << "dynamic_cast of "#object" to "#type" fails" );

#define dt__ptrAss( toAss, ptr ) \
  if (ptr == NULL ) \
    dt__throw( dt__ptrAss, << "object "#ptr" cannot assigned to "#toAss ); \
  toAss = ptr

#define dt__toFloat(toAss, value ) toAss = static_cast<float>(value)

namespace dtOO {
  class progHelper {
    public:
      dt__classOnlyName(progHelper);
      template < class T >
      static std::vector< T > list2Vector(std::list< T > & eeList) {
        std::vector< T > ee { 
          std::begin(eeList), 
          std::end(eeList) 
        };		
        return ee;
      }
      template < class T >
      static void removeBastardTwins( std::vector< T > & toMod) {
		    progHelper::sort(toMod);
        toMod.erase( 
				  std::unique(toMod.begin(), toMod.end() ), 
					toMod.end()
				);        
      }     

      /** Moves duplicates to front, returning end of duplicates range.
       *  Use a sorted range as input. */
      template < class T >
      static void removeOnlyChildren( std::vector< T > & toMod ) {
        //
        // sort vector
        //
        progHelper::sort(toMod);
        //
        // remove duplicates
        //
        typename std::vector< T >::iterator dup = toMod.begin();
        for (
          typename std::vector< T >::iterator it = toMod.begin();
          it != toMod.end();
          ++it
        ) {
          typename std::vector< T >::iterator next = it;
          ++next;
          typename std::vector< T >::iterator const miss 
          = 
          std::mismatch(next, toMod.end(), it).second;
          if (miss != it) {
            *dup++ = *miss;
            it = miss;
          }
        }
        toMod.erase(dup, toMod.end());
      }

      template < class T >
      static void sort( std::vector< T > & toMod ) {
        std::sort(toMod.begin(), toMod.end());        
      }
      
      template < class T >      
      static T mostFrequentChild( std::vector< T > const & vec ) {
        typename std::map< T , int > frequencyMap;
        int maxFrequency = 0;
        T mostFrequentElement = vec.front();
        for (T x : vec) {
          int f = ++frequencyMap[x];
          if (f > maxFrequency) {
            maxFrequency = f;
            mostFrequentElement = x;
          }
        }
        return mostFrequentElement;
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

