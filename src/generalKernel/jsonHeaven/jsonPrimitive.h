#ifndef jsonPrimitive_H
#define	jsonPrimitive_H

#include <dtOOTypeDef.h>
#include <string>
#include <vector>
#include <logMe/dtMacros.h>
#include <nlohmann/json_fwd.hpp>

namespace dtOO {
  class jsonPrimitive {
    public:
      dt__classOnlyName(jsonPrimitive);  
      jsonPrimitive();         
      jsonPrimitive( std::string const & str );
      jsonPrimitive( jsonPrimitive const & orig );
      virtual ~jsonPrimitive();
      bool containsChild( std::string const & childName ) const;    
      bool contains( std::string const & attName ) const;
      template < typename T > T lookup( std::string const & str ) const;
      template < typename T > T lookupDef( 
        std::string const & str, T const & def 
      ) const;
      template < typename T > T const operator[]( std::string const & str ) const;
      template < typename T > 
      jsonPrimitive append( std::string const & str, T const & val );
      std::string toStdString( void ) const;
      ::nlohmann::json const & json( void ) const;
    private:
      dt__pH(::nlohmann::json) _json;
  };
}

#endif	/* jsonPrimitive_H */