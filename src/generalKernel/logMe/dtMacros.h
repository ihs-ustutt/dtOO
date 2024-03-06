/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef DTMACROS_H
#define	DTMACROS_H

#include <dtOOTypeDef.h>

#include <string>
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
  template< class name > \
  bool Is(name obj) const { \
    return (dynamic_cast<name const *>(this) == NULL ? false : true); \
  }

#define dt__classSelfCreate( name ) \
  virtual name * create( void ) const { return new name(); }

#define dt__class( name, castFrom ) \
  static std::string className( void ) { \
    return std::string(#name); \
  } \
  virtual std::string virtualClassName( void ) const { \
    return std::string(#name); \
  } \
  static bool Is(castFrom const * const obj) { \
    return (dynamic_cast<name const * const >(obj) == NULL ? false : true); \
  } \
  static bool PointerIs(dt__pH(castFrom) const obj) { \
    return (!boost::dynamic_pointer_cast<name, castFrom >(obj) ? false : true); \
  } \
  static name * DownCast(castFrom * obj) { \
    return dynamic_cast<name *>(obj); \
  } \
  static dt__pH(name) PointerDownCast(dt__pH(castFrom) obj) { \
    return boost::dynamic_pointer_cast<name, castFrom>(obj); \
  } \
  static dt__pVH(name) PointerVectorDownCast(dt__pVH(castFrom) obj) { \
    dt__pVH(name) ret; \
    while ( !obj.empty() ) { \
      ret.push_back( \
        dynamic_cast< name * >( obj.pop_back().release() ) \
      ); \
    } \
    std::reverse(ret.base().begin(), ret.base().end()); \
    return ret; \
  } \
  static name * SecureCast(castFrom * obj) { \
    return dynamic_cast<name *>(obj); \
  } \
  static name const * ConstDownCast(castFrom const * obj) { \
    return dynamic_cast<name const *>(obj); \
  } \
  static name const * ConstSecureCast(castFrom const * obj) { \
    return dynamic_cast<name const *>(obj); \
  } \
  static name * MustDownCast(castFrom * obj) { \
    name * ptr = dynamic_cast<name *>(obj); \
    dt__throwIfNoClass( !ptr, MustDownCast() ); \
    return ptr; \
  } \
  static name const * MustConstDownCast(castFrom const * obj) { \
    name const * ptr = dynamic_cast<name const *>(obj); \
    dt__throwIfNoClass( !ptr, MustConstDownCast() ); \
    return ptr; \
  } \
  static name const * ConstCast(name * obj) { \
    return const_cast< name const * >(obj); \
  } \
  static name * CastConst(name const * obj) { \
    return const_cast< name * >(obj); \
  } \
  static std::vector< name * > VectorCastConst(std::vector< name const * > obj) { \
    std::vector< name * > ret; \
    dt__forAllIndex( obj, ii ) { \
      ret.push_back( \
        const_cast< name * >( obj[ii] ) \
      ); \
    } \
    return ret; \
  } \
  static std::vector< name * > VectorMustDownCast(std::vector< castFrom * > obj) { \
    std::vector< name * > ret; \
    dt__forAllIndex( obj, ii ) { \
      name * ptr = dynamic_cast<name *>(obj[ii]); \
      dt__throwIfNoClass( !ptr, VectorMustDownCast() ); \
      ret.push_back( ptr ); \
    } \
    return ret; \
  } \
  static name & RefCast(castFrom & obj) { \
    return static_cast<name &>(obj); \
  } \
  static const name & ConstRefCast(const castFrom & obj) { \
    return static_cast<const name &>(obj); \
  }

  #define dt__handling( name ) \
    typedef labeledVectorHandling< name * > ptrVec
  #define dt__H_addCloneForpVH( type ) \
    type * new_clone( type const & aF );
  #define dt__C_addCloneForpVH( type ) \
    type * new_clone( type const & aF ) { \
      return aF.clone(); \
    }
  #define dt__I_addCloneForpVHNotImpl( type ) \
    inline type * new_clone( type const & aF ) { \
      dt__throwNoClass(new_clone(), << "Not implemented."); \
    }


  #define dt__forAllIndex(vector, index) \
    for (int index = 0; index<((vector).size());index++)
  #define dt__forInnerIndex(vector, index) \
      for (int index = 1; index<((vector).size()-1);index++)
  #define dt__forFromToIndex(from, to, index) \
    for (int index = from; index<to;index++)
  #define dt__forAllIter(type, vector, iter) \
    for ( \
      type::iterator iter = (vector).begin(); \
      iter!=(vector).end(); \
      ++iter \
    )
  #define dt__forAllConstIter(type, vector, iter) \
    for ( \
      type::const_iterator iter = (vector).begin(); \
      iter!=(vector).end(); \
      ++iter \
    )
  #define dt__forAllRefAuto(vector, iterator) \
    for ( auto & iterator : vector )
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

  #define dt__toFloat(toAss, value ) toAss = static_cast<dtReal>(value)
#endif	/* DTMACROS_H */
