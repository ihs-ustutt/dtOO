#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H

#include <dtOOTypeDef.h>

#include "dtMacros.h"
#include "logBase.h"
#include "logMe.h"

#define dt__debugContainer(name) name(logDEBUG, dt__quote(__FUNCTION__))
#define dt__infoContainer(name) name(logINFO, dt__quote(__FUNCTION__))
#define dt__warningContainer(name) name(logWARNING, dt__quote(__FUNCTION__))

namespace dtOO {
  template< class T >
  class logContainer {
    public:
      dt__classOnlyName(logContainer);     
      logContainer( TLogLevel const mode, std::string const & function ) {
        _function = function;
        _mode = mode;
      }
      
      virtual ~logContainer() {
        log();
      }
      
      std::stringstream & operator()( void ) {
        return _ss;
      }
      
      void log( void ) {
        if (_mode > FILELog::ReportingLevel() ) return;
        if ( _function.empty() ) return;
        
        FILELog().Get(_mode)
          << "[ " << T::className() << "::" << _function << " ]" << std::endl
          << _ss.str();      
        
        _ss.str("");
      }
    private:
      TLogLevel _mode;
      std::string _function;
      std::stringstream _ss;
  };
}
#endif /* LOGCONTAINER_H */

