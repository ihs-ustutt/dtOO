#ifndef jsonPrimitive_H
#define	jsonPrimitive_H

#include <dtOOTypeDef.h>
#include <string>
#include <vector>
#include <logMe/dtMacros.h>
#include <nlohmann/json_fwd.hpp>
#include <fstream>

namespace dtOO {  
  template< typename T > class labeledVectorHandling;

  class jsonPrimitive {
    public:
      dt__classOnlyName(jsonPrimitive);  
      jsonPrimitive();         
      jsonPrimitive( std::string const & str );
      jsonPrimitive( jsonPrimitive const & orig );
      jsonPrimitive( std::fstream & fstr );
      virtual ~jsonPrimitive();
      void write( std::string const & fname ) const;
      bool containsChild( std::string const & childName ) const;    
      bool contains( std::string const & attName ) const;
      template < typename T > T lookup( std::string const & str ) const;
      template < typename T > T lookupDef( 
        std::string const & str, T const & def 
      ) const;
      template < typename T > 
      dt__pH(T) lookupClone( 
        std::string const & str, 
        labeledVectorHandling< T * > const * const ptrVec
      ) const;
      template < typename T > 
      dt__pH(T) lookupClone( 
        std::string const & str, 
        const labeledVectorHandling< T * > & refVec
      ) const;
      template < typename T > 
      T const * const lookupRaw( 
        std::string const & str, 
        labeledVectorHandling< T * > const * const ptrVec
      ) const;
      template < typename T > 
      const T & lookupRef( 
        std::string const & str, 
        labeledVectorHandling< T * > const * const ptrVec
      ) const;
      template < typename T > 
      dt__pVH(T) lookupVecClone( 
        std::string const & str, 
        labeledVectorHandling< T * > const * const ptrVec
      ) const;
      template < typename T > 
      std::vector< T const * > lookupVecRaw( 
        std::string const & str, 
        labeledVectorHandling< T * > const * const ptrVec
      ) const;
//      template < typename T > 
//      std::vector< T const * > lookupVecRaw( 
//        std::string const & str, 
//        labeledVectorHandling< T * > const * const ptrVec
//      ) const;
    
      template < typename T > 
      T const operator[]( std::string const & str ) const;
      template < typename T > 
      jsonPrimitive append( std::string const & str, T const & val );
      std::string toStdString( void ) const;
      ::nlohmann::json const & json( void ) const;
    private:
      dt__pH(::nlohmann::json) _json;
  };
}

#endif	/* jsonPrimitive_H */