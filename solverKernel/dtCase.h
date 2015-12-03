#ifndef dtCase_H
#define	dtCase_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class resultValue;
  
  class dtCase : public labelHandling, public optionHandling {
    public:
      dt__classOnlyName(dtCase);    
      dtCase();
      virtual ~dtCase();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        vectorHandling< dtCase * > const * const dC
      );    
      virtual void runCurrentState( void ) = 0;
      void update( void );
      bool inPipeline( std::string const & state) const;
      std::vector< std::string > allStates( void ) const;
      std::string createDirectory( std::string const & state ) const;
      virtual void createStatus( std::string const & directory ) const = 0;
      std::string getDirectory( std::string const & state ) const;
      int status( std::string const & state ) const;
      std::string statusStr( std::string const & state ) const;
      vectorHandling< resultValue * > result( std::string const & state ) const;
    public:
      static const int SUCCESS;
      static const int RUNNING;
      static const int ERROR;
      static const int UNEXPECTED;
      static const std::string STATUSSTRING[];
    private:
      std::vector< std::string > _directory;
      std::vector< std::string > _state;
      std::vector< int > _status;
  };
}
#endif	/* dtCase_H */

