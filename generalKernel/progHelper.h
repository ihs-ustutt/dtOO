#ifndef PROGHELPER_H
#define	PROGHELPER_H

#include <logMe/logMe.h>
#include <boost/utility.hpp>

#define dt__FORALL(vector, variable, content) \
    for (int variable = 0; variable<((vector).size());variable++) { \
      content \
    }
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
#define dt__NEXT( object ) boost::next( object )
#define dt__PRIOR( object ) boost::prior( object )
#define dt__TOFLOAT(toAss, value ) toAss = static_cast<float>(value)

#endif	/* PROGHELPER_H */

