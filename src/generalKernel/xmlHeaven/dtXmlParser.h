#ifndef DTXMLPARSER_H
#define	DTXMLPARSER_H

#include <vector>
#include <string>
#include "dtXmlParserBase.h"
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class boundedVolume;
  class dtPlugin;
  class dtCase;
  
  class dtXmlParser : public dtXmlParserBase {
  public:  
    dt__classOnlyName(dtXmlParser);
    dtXmlParser(std::string const & inFile, std::string const & outFile);
    virtual ~dtXmlParser();
    void parse(void);
    void write( vectorHandling< constValue * > const * const cValP ) const;
    void write(
      std::string const stateName, 
      vectorHandling< constValue * > const * const cValP
    ) const;
    void write(
      std::vector< std::string > const stateName, 
      std::vector< vectorHandling< constValue * > > const & cValP
    ) const;
    void write( vectorHandling< constValue * > const & cValP ) const;
    void write(
      std::string const stateName, 
      vectorHandling< constValue * > const & cValP
    ) const;       
    void writeUpdate(
      std::string const stateName, 
      vectorHandling< constValue * > const & cValP
    ) const;
    void extract(
      std::string const stateName, 
      vectorHandling< constValue * > const & cValP,
      std::string const fileName
    ) const;
    void remove( std::string const stateName ) const;
    void load( void ) const;
    std::vector< std::string > getStates( void );
    void loadStateToConst(
      std::string const stateName, vectorHandling< constValue * > &cValRef
    );
    std::string currentState( void ) const;
    void setState( std::string const & newState) const;
    void freeState( void ) const;
    bool stateLoaded( void ) const;
    void createConstValue(
      std::string const constValueLabel, vectorHandling< constValue * > * cValP
    ) const;
    void createConstValue(vectorHandling< constValue * > * cValP) const;
    void getLabels(
      std::string toLookFor, std::vector< std::string > * machinePartNames 
    ) const;
    std::vector< std::string > getLabels( std::string lookType ) const;
    void getLabel( std::string lookType, std::string * name ) const;
    ::QDomElement getElement( 
      std::string const lookType, std::string const lookName 
    ) const ;
    ::QDomElement getElement( std::string const lookType ) const ;
    ::QDomElement getUnlabeledElement( std::string const lookType ) const;
    std::vector< ::QDomElement > getElementRecursive( 
      std::string const lookType 
    ) const;
    void createAnalyticFunction(
	    std::string const functionName,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP, 
      vectorHandling< analyticFunction * > * aFP
    ) const;
    void createAnalyticFunction(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP, 
      vectorHandling< analyticFunction * > * aFP
	  ) const;
    void createAnalyticGeometry(
      std::string const label,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > * aGP
    ) const;    
    void createAnalyticGeometry(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > * aGP
    ) const;    
    void createBoundedVolume(
      std::string const label,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP
    ) const;    
    void createBoundedVolume(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP
    ) const;
    void createPlugin(
      std::string const label,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP,
      vectorHandling< dtCase * > const * const dCP,
      vectorHandling< dtPlugin * > * pLP
    ) const;      
    void createPlugin(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP,
      vectorHandling< dtCase * > const * const dCP,
      vectorHandling< dtPlugin * > * pLP
    ) const;        
    void createCase(
      std::string const label,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP,
      vectorHandling< dtCase * > * dCP
    ) const;      
    void createCase(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP,
      vectorHandling< dtCase * > * dCP
    ) const;       
    void destroyAndCreate(
      vectorHandling< constValue * > & cV,
      vectorHandling< analyticFunction* > & aF,
      ptrHandling< baseContainer > & bC,
      vectorHandling< analyticGeometry * > & aG,
		  vectorHandling< boundedVolume * > & bV,
      vectorHandling< dtCase * > & dC,
      vectorHandling< dtPlugin * > & pL
    ) const;    
    void destroyAndCreate(
      baseContainer & bC,
      vectorHandling< constValue * > & cV,
      vectorHandling< analyticFunction* > & aF,
      vectorHandling< analyticGeometry * > & aG,
      vectorHandling< boundedVolume * > & bV,
      vectorHandling< dtCase * > & dC,
      vectorHandling< dtPlugin * > & pL
    ) const;  
  private:
    void parse(char const * const fileName);
    void checkFile(
      char const * const fileName, QDomDocument & xmlDocument
    ) const;
    static void writeFile(
      char const * const fileName, QDomDocument & xmlDocument
    );
    static void writeFile( 
      std::string const & fileName, ::QDomNode const & node
    );
    void setStaticProperties( void );
    void createRetroConstValue(
      std::string const constValueLabel, vectorHandling< constValue * > * cValP
    ) const;     
    void loadRetroStateToConst(
      std::string const stateName, vectorHandling< constValue * > &cValRef
    );    
  private:
    std::vector< ::QDomElement > _rootRead;
    std::vector< ::QDomDocument > _rootReadDoc;
    mutable ::QDomElement _rootLoad;
    mutable ::QDomDocument _rootLoadDoc;
    mutable std::string _currentState;
    std::string const _inFile;
    std::string const _outFile;
  };
}
#endif	/* DTXMLPARSER_H */

