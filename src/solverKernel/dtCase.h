#ifndef dtCase_H
#define	dtCase_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class resultValue;
  
  class dtCase : public labelHandling, public optionHandling {
    public:
      dt__class(dtCase, labelHandling);
      dtCase();
      virtual ~dtCase();
      virtual dtCase * create( void ) const = 0;
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtCase const * const dC
      );    
      virtual void runCurrentState( void ) = 0;
      void update( void );
      bool inPipeline( std::string const & state) const;
      std::vector< std::string > allStates( void ) const;
      std::string createDirectory( std::string const & state ) const;
      std::string getDirectory( std::string const & state ) const;
      dtInt status( std::string const & state ) const;
      std::string statusStr( std::string const & state ) const;
      vectorHandling< resultValue * > result( std::string const & state ) const;
    public:
      static const dtInt SUCCESS;
      static const dtInt RUNNING;
      static const dtInt ERROR;
      static const dtInt UNEXPECTED;
      static const std::string STATUSSTRING[];
    private:
      std::vector< std::string > _directory;
      std::vector< std::string > _state;
      std::vector< dtInt > _status;
  };
}
#endif	/* dtCase_H */

