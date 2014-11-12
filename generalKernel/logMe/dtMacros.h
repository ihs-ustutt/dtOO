#ifndef DTMACROS_H
#define	DTMACROS_H

#include <string>
#include <sstream>
#include <interfaceHeaven/ptrHandling.h>

#define dt__CLASSNAME( name ) \
    static std::string ClassName( void ) { \
      return std::string(#name); \
    } \
    template< class dt__CLASSNAME_T > \
    bool Is(dt__CLASSNAME_T obj) const { \
      return (dynamic_cast<dt__CLASSNAME_T const *>(this) == NULL ? false : true); \
    }
#define dt__CLASSSTD( name, castFrom ) \
    static std::string ClassName( void ) { \
      return std::string(#name); \
    } \
    template< class dt__CLASSNAME_T > \
    bool Is(dt__CLASSNAME_T obj) const { \
      return (dynamic_cast<dt__CLASSNAME_T const *>(this) == NULL ? false : true); \
    } \
    static name * DownCast(castFrom * obj) { \
      return dynamic_cast<name *>(obj); \
    } \
    static name * SecureCast(castFrom * obj) { \
      return static_cast<name *>(obj); \
    } \
    static name const * ConstDownCast(castFrom const * obj) { \
      return dynamic_cast<name const *>(obj); \
    } \
    static name const * ConstSecureCast(castFrom const * obj) { \
      return static_cast<name const *>(obj); \
    }

  #define dt__pH( name ) \
    ptrHandling< name >

#endif	/* DTMACROS_H */

