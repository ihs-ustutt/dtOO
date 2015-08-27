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
    dtXmlParser();
    virtual ~dtXmlParser();
    void openFileAndParse(char const * const fileName);
    void openFileAndWrite(
      char const * const fileName, vectorHandling< constValue * > * cValP
    );
    void openFileAndWrite(
      char const * const fileName, 
      std::string const stateName, 
      vectorHandling< constValue * > * cValP
    );
    void openFileAndLoad(char const * const fileName);
    std::vector< std::string > getStates( void );
    void loadStateToConst(
      std::string const stateName, vectorHandling< constValue * > &cValRef
    );
    void createConstValue(
      std::string const constValueLabel, vectorHandling< constValue * > * cValP
    ) const;
    void createConstValue(vectorHandling< constValue * > * cValP) const;
    void getNames(
      std::string toLookFor, std::vector< std::string > * machinePartNames 
    ) const;
    std::vector< std::string > getNames( std::string lookType ) const;
    void getName( std::string lookType, std::string * name ) const;
    ::QDomElement getElement( 
      std::string const lookType, std::string const lookName 
    ) const ;
    ::QDomElement getElement( std::string const lookType ) const ;
	  ::QDomElement getUnlabeledElement( std::string const lookType ) const;
    void createAnalyticFunction(
	    std::string const functionName,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP, 
      vectorHandling< analyticFunction * > * sFP) const;
    void createAnalyticFunction(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP, 
      vectorHandling< analyticFunction * > * sFP
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
      vectorHandling< dtPlugin * > * pLP
    ) const;      
    void createPlugin(
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP,
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
  private:
    void checkFile(char const * const fileName, QDomDocument & xmlDocument);
    static void writeFile(char const * const fileName, QDomDocument & xmlDocument);
    static void writeFile( std::string const & fileName, ::QDomNode const & node);
    void setStaticProperties( void );
    void createRetroConstValue(
      std::string const constValueLabel, vectorHandling< constValue * > * cValP
    ) const;     
    void loadRetroStateToConst(
      std::string const stateName, vectorHandling< constValue * > &cValRef
    );    
  private:
    std::vector< ::QDomElement > _rootRead;
    std::vector< QDomDocument > _rootReadDoc;
    ::QDomElement _rootLoad;
    QDomDocument _rootLoadDoc;
  };
}
#endif	/* DTXMLPARSER_H */

