#include "jsonPrimitive.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>

namespace dtOO {  
  jsonPrimitive::jsonPrimitive() {
    _json.reset( new ::nlohmann::json( ::nlohmann::json::parse("{}") ) );
  }

  jsonPrimitive::jsonPrimitive( std::string const & str) {
    _json.reset( new ::nlohmann::json( ::nlohmann::json::parse(str) ) );
  }
  
  jsonPrimitive::jsonPrimitive( jsonPrimitive const & orig ) {
    _json.reset( new ::nlohmann::json( *(orig._json) ) );
  }
  
  jsonPrimitive::~jsonPrimitive() {
  }  

  std::string jsonPrimitive::toStdString( void ) const {
    return _json->dump();           
  }	
  
  bool jsonPrimitive::contains( std::string const & attName ) const {
    if ( stringContains("|", attName) ) {
      std::vector< std::string > attNameV;
      ::boost::split(
        attNameV, attName, ::boost::is_any_of("|"), ::boost::token_compress_on
      );
      //
      // check if one attName is not included in existing
      //
      dt__forAllRefAuto(attNameV, anAtt) {
        if ( _json->contains(anAtt) == false ) return false;
      }
      return true;
    }
    return _json->contains(attName);
  }
  bool jsonPrimitive::containsChild( std::string const & childName ) const {
    dt__throwIf(stringContains("|", childName), containsChild());
    
    if ( this->contains(childName) ) {
      return _json->at(childName).is_object();
    }
    return false;
  }

  // template lookup
  template < typename T > 
  T jsonPrimitive::lookup( std::string const & str ) const {
    return _json->at(str);
  }
  template 
  std::string jsonPrimitive::lookup< std::string >(std::string const &) const;
  template
  dtReal jsonPrimitive::lookup< dtReal >(std::string const &) const;
  template
  dtInt jsonPrimitive::lookup< dtInt >(std::string const &) const;
  template
  std::vector<dtInt> jsonPrimitive::lookup< std::vector<dtInt> >(
    std::string const &
  ) const;  
  template
  std::vector<dtReal> jsonPrimitive::lookup< std::vector<dtReal> >(
    std::string const &
  ) const;
  
  jsonPrimitive jsonPrimitive::lookup( std::string const & str ) const {
    return jsonPrimitive( _json->at(str).dump() );
  }  
  
  // template lookupDef
  template < typename T > 
  T jsonPrimitive::lookupDef( std::string const & str, T const & def ) const {
    if ( _json->contains(str) ) return _json->at(str);
    return def;
  }  
  template
  dtReal jsonPrimitive::lookupDef< dtReal >(
    std::string const &, dtReal const & def
  ) const;  
  
  // template  operator[]
  template < typename T > 
  T const jsonPrimitive::operator[]( std::string const & str ) const {
    return _json->operator[]( str );
  }
  template
  std::string const jsonPrimitive::operator[]< std::string >(
    std::string const &
  ) const;
  template
  dtReal const jsonPrimitive::operator[]< dtReal >(std::string const &) const;
  template
  dtInt const jsonPrimitive::operator[]< dtInt >(std::string const &) const;
  template
  std::vector<dtInt> const jsonPrimitive::operator[]< std::vector<dtInt> >(
    std::string const &
  ) const;  
  template
  std::vector<dtReal> const jsonPrimitive::operator[]< std::vector<dtReal> >(
    std::string const &
  ) const;  
  
  // template append
  template < typename T > 
  void jsonPrimitive::append( std::string const & str, T const & val ) {
    _json->operator[](str) = val;
  }
  template void jsonPrimitive::append( 
    std::string const & str, dtReal const & val 
  );  
  template void jsonPrimitive::append( 
    std::string const & str, dtInt const & val 
  );  
  template void jsonPrimitive::append( 
    std::string const & str, std::string const & val 
  );
}