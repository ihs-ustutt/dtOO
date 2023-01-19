#ifndef PROGHELPER_H
#define	PROGHELPER_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <vector>
#include <list>

namespace dtOO {
  class progHelper {
    public:
      dt__classOnlyName(progHelper);
      template < class T >
      static std::vector< T > list2Vector(std::list< T > const & eeList) {
        std::vector< T > ee { 
          std::begin(eeList), 
          std::end(eeList) 
        };		
        return ee;
      }
      template < class T >
      static std::list< T > vector2List(std::vector< T > const & eeVec) {
        std::list< T > ee { 
          std::begin(eeVec), 
          std::end(eeVec) 
        };		
        return ee;
      }      
      template < class T >
      static void removeChildren( std::vector< T > & toMod, T const & toDel) {
        toMod.erase( 
          std::remove(
            toMod.begin(), toMod.end(), toDel
          ),
          toMod.end()
        );  
      }
      
      template < class T >
      static void removeBastardTwins( std::vector< T > & toMod) {
        progHelper::sort(toMod);
        toMod.erase( 
          std::unique(toMod.begin(), toMod.end() ), 
                toMod.end()
        );        
      }     

      template < class T >
      static void removeBastardTwins( std::list< T > & toMod) {
        toMod.sort();
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
        typename std::map< T , dtInt > frequencyMap;
        dtInt maxFrequency = 0;
        T mostFrequentElement = vec.front();
        for (T x : vec) {
          dtInt f = ++frequencyMap[x];
          if (f > maxFrequency) {
            maxFrequency = f;
            mostFrequentElement = x;
          }
        }
        return mostFrequentElement;
      }

      template < class T >      
      static void reverse( std::vector< T > & vec ) {
        std::reverse(vec.begin(), vec.end());
      }      
      
      template <class T> 
      static T prior(T x) { 
        return --x; 
      }
      template <class T> 
      static T next(T x) { 
        return ++x; 
      }        
      template < class T > 
      static T min( std::vector< T > const & x) { 
        typename std::vector< T >::const_iterator result 
        = 
        std::min_element(
          std::begin(x), std::end(x)
        );
        return *result;        
      }
      template < class T > 
      static dtInt minPos( std::vector< T > const & x) { 
        typename std::vector< T >::const_iterator result 
        = 
        std::min_element(
          std::begin(x), std::end(x)
        );
        return std::distance(std::begin(x), result);
      }      
      template < class T > 
      static dtInt maxPos( std::vector< T > const & x) { 
        typename std::vector< T >::const_iterator result 
        = 
        std::max_element(
          std::begin(x), std::end(x)
        );
        return std::distance(std::begin(x), result);
      }            
      template < class T > 
      static T max( std::vector< T > const & x) { 
        typename std::vector< T >::const_iterator result 
        = 
        std::max_element(
          std::begin(x), std::end(x)
        );
        return *result;        
      }      
      template < class T > 
      static bool contains( std::list< T > const & x, T const & y) { 
        if ( std::find(x.begin(), x.end(), y) == x.end() ) return false;
        return true;
      }
      template< class T >
      static void erase( std::vector< T > & vec, dtInt const & pos ) {
        vec.erase( vec.begin() + pos );
      }
    private:
      progHelper() {
        
      }
  };
}
#endif	/* PROGHELPER_H */

