#include "openFOAMSetupRule.h"

#include <logMe/logMe.h>
#include "openFOAMWallRule.h"
#include "openFOAMGgiRule.h"
#include <xmlHeaven/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <volFields.H>

namespace dtOO {
  openFOAMSetupRule::openFOAMSetupRule() {
  }

  openFOAMSetupRule::~openFOAMSetupRule() {
  }
  
  openFOAMSetupRule * openFOAMSetupRule::create( 
    std::string const & name 
  ) {
    if (name == "openFOAMWallRule") return new openFOAMWallRule();
    if (name == "openFOAMGgiRule") return new openFOAMGgiRule();
    
    dt__throw(create(), << "Cannot create " << name);
  }
  
  void openFOAMSetupRule::executeOnVolVectorField(
    std::vector< std::string > const & rule, ::Foam::volVectorField & field
  ) const {
    dt__info(
      executeOnVolVectorField(), 
      << "Nothing to do on ::Foam::volVectorField = " << field.name()
    );  
  }
    
  void openFOAMSetupRule::executeOnVolScalarField(
    std::vector< std::string > const & rule, ::Foam::volScalarField & field
  ) const {
    dt__info(
      executeOnVolScalarField(), 
      << "Nothing to do on ::Foam::volScalarField = " << field.name()
    );      
  }
    
  std::string openFOAMSetupRule::getRuleOfField(
    std::string const & fieldName, std::vector< std::string > const & rule
  ) const {
    //
    // start at position 3
    //
    for(int ii = 3; ii < rule.size(); ii+2) {
      //
      // if rule[ii] is equal field name return
      //
      if ( rule[ii] == fieldName ) {
        dt__throwIf( (ii+1)>(rule.size()-1), getRuleOfField());
        return rule[ii+1];
      }
    }
    return std::string("");    
  }
  
  std::string openFOAMSetupRule::parseOptionStr(
    std::string const & name, std::string const & str
  ) const {
    if ( stringPrimitive::stringContains(name, str) ) {
      return stringPrimitive::getStringBetween("(", ")", str);
    }
    return std::string("");
  }    
  
  bool openFOAMSetupRule::parseOptionBool(
    std::string const & name, std::string const & str
  ) const {
    if ( parseOptionStr(name, str) == "true") return true;
    return false;    
  }
   
  dtVector3 openFOAMSetupRule::parseOptionDtVector3(
    std::string const & name, std::string const & str
  ) const {
    std::vector< float > ff
    =
    qtXmlBase::muParseCSString( parseOptionStr(name, str) );
    
    dt__throwIf(ff.size()!=3, parseOptionDtVector3());
    
    return dtVector3(ff[0], ff[1], ff[2]);
  }
}
