#include "jsonPrimitive.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtLinearAlgebra.h>

#include <analyticFunctionHeaven/analyticFunction.h>
#include "analyticGeometryHeaven/analyticGeometry.h"

namespace CGAL {
  //dtPoint2
  void from_json(const ::nlohmann::json& from, ::dtOO::dtPoint2& to) {
    to 
    = 
    ::dtOO::dtPoint2( 
      from.at("dtPoint2").at("x").get<double>(), 
      from.at("dtPoint2").at("y").get<double>() 
    );
  };    
  void to_json(::nlohmann::json& to, const ::dtOO::dtPoint2& from) {
    to["dtPoint2"]["x"] = from.x();
    to["dtPoint2"]["y"] = from.y();
  };  
  //dtPoint3
  void from_json(const ::nlohmann::json& from, ::dtOO::dtPoint3& to) {
    to 
    = 
    ::dtOO::dtPoint3( 
      from.at("dtPoint3").at("x").get<double>(), 
      from.at("dtPoint3").at("y").get<double>(), 
      from.at("dtPoint3").at("z").get<double>() 
    );
  };    
  void to_json(::nlohmann::json& to, const ::dtOO::dtPoint3& from) {
    to["dtPoint3"]["x"] = from.x();
    to["dtPoint3"]["y"] = from.y();
    to["dtPoint3"]["z"] = from.z();
  };    
  //dtVector2
  void from_json(const ::nlohmann::json& from, ::dtOO::dtVector2& to) {
    to 
    = 
    ::dtOO::dtVector2( 
      from.at("dtVector2").at("x").get<double>(), 
      from.at("dtVector2").at("y").get<double>() 
    );
  };    
  void to_json(::nlohmann::json& to, const ::dtOO::dtVector2& from) {
    to["dtVector2"]["x"] = from.x();
    to["dtVector2"]["y"] = from.y();
  };  
  //dtVector3
  void from_json(const ::nlohmann::json& from, ::dtOO::dtVector3& to) {
    to 
    = 
    ::dtOO::dtVector3( 
      from.at("dtVector3").at("x").get<double>(), 
      from.at("dtVector3").at("y").get<double>(), 
      from.at("dtVector3").at("z").get<double>() 
    );
  };    
  void to_json(::nlohmann::json& to, const ::dtOO::dtVector3& from) {
    to["dtVector3"]["x"] = from.x();
    to["dtVector3"]["y"] = from.y();
    to["dtVector3"]["z"] = from.z();
  };  
}

namespace dtOO {
  void from_json(const ::nlohmann::json& from, jsonPrimitive& to) {
    to = jsonPrimitive( from.dump() );
  };  
  void to_json(::nlohmann::json& to, const jsonPrimitive& from) {
    to = ::nlohmann::json( from.json() );
  };
  void to_json(::nlohmann::json& to, analyticFunction const * const & from) {
    to["analyticFunction"]["label"] = from->getLabel();
  };      
  void to_json(::nlohmann::json& to, analyticGeometry const * const & from) {
    to["analyticGeometry"]["label"] = from->getLabel();
  };
}

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
  jsonPrimitive::jsonPrimitive( std::fstream & fstr ) {
    _json.reset( new ::nlohmann::json( ::nlohmann::json::parse(fstr) ) );
  }
  
  jsonPrimitive::~jsonPrimitive() {
  }  

  std::string jsonPrimitive::toStdString( void ) const {
    return _json->dump();           
  }	
  
  bool jsonPrimitive::contains( std::string const & attName ) const {
    if ( stringPrimitive::stringContains("|", attName) ) {
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
    dt__throwIf(
      stringPrimitive::stringContains("|", childName), containsChild()
    );
    
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
  bool jsonPrimitive::lookup< bool >(std::string const &) const;
  template
  jsonPrimitive jsonPrimitive::lookup< jsonPrimitive >( 
    std::string const &
  ) const;    
  template
  dtPoint2 jsonPrimitive::lookup< dtPoint2 >(std::string const &) const;  
  template
  dtVector2 jsonPrimitive::lookup< dtVector2 >(std::string const &) const;  
  template
  dtPoint3 jsonPrimitive::lookup< dtPoint3 >(std::string const &) const;    
  template
  dtVector3 jsonPrimitive::lookup< dtVector3 >(std::string const &) const;  
  template
  std::vector<dtInt> jsonPrimitive::lookup< std::vector<dtInt> >(
    std::string const &
  ) const;  
  template
  std::vector<bool> jsonPrimitive::lookup< std::vector<bool> >(
    std::string const &
  ) const;    
  template
  std::vector<dtReal> jsonPrimitive::lookup< std::vector<dtReal> >(
    std::string const &
  ) const;
  template
  std::vector<std::string> jsonPrimitive::lookup< std::vector<std::string> >(
    std::string const &
  ) const;  
  template
  std::vector<jsonPrimitive> jsonPrimitive::lookup< std::vector<jsonPrimitive> >(
    std::string const &
  ) const;

  // template lookupCast
  template < typename T > 
  dt__pH(T) jsonPrimitive::lookupClone( 
    std::string const & str, labeledVectorHandling< T * > const * const ptrVec
  ) const {
    jsonPrimitive jP 
    = 
    lookup<jsonPrimitive>(str).lookup<jsonPrimitive>( T::className() );
    return dt__pH(T)(ptrVec->get( jP.lookup<std::string>("label") )->clone());
  }
  template
  dt__pH(analyticFunction) jsonPrimitive::lookupClone( 
    std::string const & str, 
    labeledVectorHandling< analyticFunction * > const * const ptrVec
  ) const;
  template
  dt__pH(analyticGeometry) jsonPrimitive::lookupClone( 
    std::string const & str, 
    labeledVectorHandling< analyticGeometry * > const * const ptrVec
  ) const;
  
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
  template
  dtInt jsonPrimitive::lookupDef< dtInt >(
    std::string const &, dtInt const & def
  ) const;
  template
  bool jsonPrimitive::lookupDef< bool >(
    std::string const &, bool const & def
  ) const;
  template
  dtVector3 jsonPrimitive::lookupDef< dtVector3 >(
    std::string const &, dtVector3 const & def
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
  std::vector<std::string> const 
  jsonPrimitive::operator[]< std::vector<std::string> >(
    std::string const &
  ) const;
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
  jsonPrimitive jsonPrimitive::append( 
    std::string const & str, T const & val 
  ) {
    _json->operator[](str) = val;
    return *this;
  }
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, std::string const & val 
  );  
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtReal const & val 
  );  
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtInt const & val 
  );  
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, bool const & val 
  );    
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, jsonPrimitive const & val
  );      
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtPoint2 const & val
  );  
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtVector2 const & val
  );
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtPoint3 const & val
  );    
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, dtVector3 const & val
  );
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, analyticFunction const * const & val
  );
  template jsonPrimitive jsonPrimitive::append( 
    std::string const & str, analyticGeometry const * const & val
  );  
  
  ::nlohmann::json const & jsonPrimitive::json( void ) const {
    return *_json;
  }
}
