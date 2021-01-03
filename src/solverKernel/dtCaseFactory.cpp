#include "dtCaseFactory.h"

#include <logMe/logMe.h>
#include "dtCase.h"

namespace dtOO {
  
  dt__pVH(dtCase) dtCaseFactory::_builder;  

  dtCaseFactory::dtCaseFactory() {
  }

  dtCaseFactory::~dtCaseFactory() {
  }

  bool dtCaseFactory::registrate( dtCase const * const dtC ) {
    dt__forAllRefAuto( _builder, aBuilder ) {
      if ( aBuilder.virtualClassName() == dtC->virtualClassName() ) {
        return false;
      }
    }  
    _builder.push_back( dtC->create() );
    
    return true;
  }

  dtCase * dtCaseFactory::create( std::string const str ) {
    dt__info( create(), << "str = " << str);
    dt__forAllRefAuto( _builder, aBuilder ) {
      //
      // check virtual class name
      //
      if ( aBuilder.virtualClassName() == str ) {
        return aBuilder.create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias) {
        if ( anAlias == str ) return aBuilder.create();
      }
    }
    
    std::vector< std::string > av;
    dt__forAllRefAuto( _builder, aBuilder ) {
      av.push_back( aBuilder.virtualClassName() );
      dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias);
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented builder:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );   
  }
}