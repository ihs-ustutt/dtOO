#ifndef DTXMLPARSER_H
#define	DTXMLPARSER_H

#include <vector>
#include <string>
#include "dtXmlParserBase.h"
#include <QtXml/QDomElement>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class pointContainer;
  class vectorContainer;
  class boundedVolume;
  
  class dtXmlParser : public dtXmlParserBase {
  public:  
    dt__CLASSNAME(dtXmlParser);
    dtXmlParser();
    virtual ~dtXmlParser();
    void openFileAndParse(char const * const fileName);
    void openFileAndWrite(char const * const fileName, 
                          vectorHandling< constValue * > * cValP);
    void openFileAndWrite(char const * const fileName,
                          std::string const stateName,
                          vectorHandling< constValue * > * cValP);
    void openFileAndLoad(char const * const fileName);
    std::vector< std::string > getStates( void );
    void loadStateToConst(std::string const stateName, vectorHandling< constValue * > &cValRef);
    void createConstValue(std::string const constValueLabel, 
                          vectorHandling< constValue * > * cValP) const;
    void createConstValue(vectorHandling< constValue * > * cValP) const;
    void getNames(std::string toLookFor, std::vector< std::string > * machinePartNames ) const;
    std::vector< std::string > getNames( std::string lookType ) const;
    void getName( std::string lookType, std::string * name ) const;
    void getChildLabels(std::string childName, 
                       std::vector< std::string > * labelValue,
                       QDomElement const & parentElement) const;
    QDomElement getElement( std::string const lookType, std::string const lookName ) const ;
    QDomElement getElement( std::string const lookType ) const ;
    bool hasChildElement( std::string const elementTag, 
                          std::string const labelAttributeVal,
                          QDomElement const & parentElement
    ) const;    
    QDomElement getChildElement( std::string const elementTag, 
                                 std::string const labelAttributeVal,
                                 QDomElement const & parentElement) const ;
    void createAnalyticFunction(
	    std::string const functionName, 
      vectorHandling< constValue * > const * const cVP, 
      vectorHandling< analyticFunction * > * sFP) const;
    void createAnalyticFunction(
    vectorHandling< constValue * > const * const cVP, 
    vectorHandling< analyticFunction * > * sFP
	  ) const;
    void createAnalyticGeometry(
      std::string const label,
      pointContainer * const pCP,
      vectorContainer * const vCP,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > * aGP
    ) const;    
    void createAnalyticGeometry(
      pointContainer * const pCP,
      vectorContainer * const vCP,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > * aGP
    ) const;    
    void createBoundedVolume(
      std::string const label,
      pointContainer * const pCP,
      vectorContainer * const vCP,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP
    ) const;    
    void createBoundedVolume(
      pointContainer * const pCP,
      vectorContainer * const vCP,
      vectorHandling< constValue * > const * const cVP,        
      vectorHandling< analyticFunction * > const * const sFP,        
      vectorHandling< analyticGeometry * > const * const aGP,
      vectorHandling< boundedVolume * > * bVP
    ) const;    
    void destroyAndCreate(
      vectorHandling< constValue * > & cV,
      vectorHandling< analyticFunction* > & aF,
      ptrHandling< pointContainer > & pC,
      ptrHandling< vectorContainer > & vC,        
      vectorHandling< analyticGeometry * > & aG,
		  vectorHandling< boundedVolume * > & bV      
    ) const;    
  private:
    void checkFile(char const * const fileName, QDomDocument & xmlDocument);
    static void writeFile(char const * const fileName, QDomDocument & xmlDocument);
    static void writeFile( std::string const & fileName, QDomNode const & node);
    void setStaticProperties( void );
  private:
    std::vector< QDomElement > _rootRead;
    QDomElement _rootLoad;
  };
}
#endif	/* DTXMLPARSER_H */

