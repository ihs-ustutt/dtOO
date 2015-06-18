#ifndef DTMACROS_H
#define	DTMACROS_H

#include <string>
#include <sstream>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/ptrVectorHandling.h>

#define dt__vH( name ) vectorHandling< name >
#define dt__pH( name ) ptrHandling< name >
#define dt__tmpPtr( name, init ) ptrHandling< name >(init).get()
#define dt__pVH( name ) ptrVectorHandling< name >

#define dt__classOnlyName( name ) \
  static std::string className( void ) { \
    return std::string(#name); \
  } \
  virtual std::string virtualClassName( void ) const { \
    return std::string(#name); \
  } \
  template< class dt__CLASSNAME_T > \
  bool Is(dt__CLASSNAME_T obj) const { \
    return (dynamic_cast<dt__CLASSNAME_T const *>(this) == NULL ? false : true); \
  }

#define dt__class( name, castFrom ) \
  static std::string className( void ) { \
    return std::string(#name); \
  } \
  virtual std::string virtualClassName( void ) const { \
    return std::string(#name); \
  } \
  template< class dt__CLASSNAME_T > \
  bool Is(dt__CLASSNAME_T obj) const { \
    return (dynamic_cast<dt__CLASSNAME_T const *>(this) == NULL ? false : true); \
  } \
  static bool Is(castFrom * obj) { \
    return (dynamic_cast<name *>(obj) == NULL ? false : true); \
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

  #define dt__H_addCloneForpVH( type ) \
    type * new_clone( type const & aF );
  #define dt__C_addCloneForpVH( type ) \
    type * new_clone( type const & aF ) { \
      return aF.clone(); \
    }
#endif	/* DTMACROS_H */

