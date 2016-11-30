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
    virtual ~dtXmlParser();        
    static dtXmlParser * init(
      std::string const & inFile, std::string const & outFile
    );
    static dtXmlParser * ptr( void );
    static dtXmlParser const & constReference( void );
    static dtXmlParser & reference( void );
    void parse(void);
    void write( cVPtrVec const * const cValP ) const;
    void write(
      std::string const stateName, 
      cVPtrVec const * const cValP
    ) const;
    void write(
      std::vector< std::string > const stateName, 
      std::vector< cVPtrVec > const & cValP
    ) const;
    void write( cVPtrVec const & cValP ) const;
    void write(
      std::string const stateName, 
      cVPtrVec const & cValP
    ) const;       
    void writeUpdate(
      std::string const stateName, 
      cVPtrVec const & cValP
    ) const;
    void extract(
      std::string const stateName, 
      cVPtrVec const & cValP,
      std::string const fileName
    ) const;
    void remove( std::string const stateName ) const;
    void load( void ) const;
    std::vector< std::string > getStates( void );
    bool hasState( std::string const stateName ) const;
    void loadStateToConst(
      std::string const stateName, cVPtrVec &cValRef
    );
    std::string currentState( void ) const;
    void setState( std::string const & newState) const;
    void freeState( void ) const;
    bool stateLoaded( void ) const;
    void createConstValue(
      std::string const constValueLabel, cVPtrVec * cValP
    ) const;
    void createConstValue(cVPtrVec * cValP) const;
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
      cVPtrVec const * const cVP, 
      aFPtrVec * aFP
    ) const;
    void createAnalyticFunction(
      baseContainer * const bC,
      cVPtrVec const * const cVP, 
      aFPtrVec * aFP
	  ) const;
    void createAnalyticGeometry(
      std::string const label,
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec * aGP
    ) const;    
    void createAnalyticGeometry(
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec * aGP
    ) const;    
    void createBoundedVolume(
      std::string const label,
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP
    ) const;    
    void createBoundedVolume(
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP
    ) const;
    void createPlugin(
      std::string const label,
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP,
      dCPtrVec const * const dCP,
      dPPtrVec * pLP
    ) const;      
    void createPlugin(
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP,
      dCPtrVec const * const dCP,
      dPPtrVec * pLP
    ) const;        
    void createCase(
      std::string const label,
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP,
      dCPtrVec * dCP
    ) const;      
    void createCase(
      baseContainer * const bC,
      cVPtrVec const * const cVP,        
      aFPtrVec const * const sFP,        
      aGPtrVec const * const aGP,
      bVPtrVec * bVP,
      dCPtrVec * dCP
    ) const;       
    void destroyAndCreate(
      baseContainer & bC,
      cVPtrVec & cV,
      aFPtrVec & aF,
      aGPtrVec & aG,
      bVPtrVec & bV,
      dCPtrVec & dC,
      dPPtrVec & pL
    ) const;  
  private:
    dtXmlParser(std::string const & inFile, std::string const & outFile);
    void parse(char const * const fileName);
    void checkFile(
      char const * const fileName, QDomDocument & xmlDocument
    ) const;
    static void writeFile(
      std::string const & fileName, QDomDocument & xmlDocument
    );
    void setStaticProperties( void );
  private:
    std::vector< ::QDomElement > _rootRead;
    std::vector< ::QDomDocument > _rootReadDoc;
    mutable ::QDomElement _rootLoad;
    mutable ::QDomDocument _rootLoadDoc;
    mutable std::string _currentState;
    std::string const _inFile;
    std::string const _outFile;
    static dtXmlParser * _pH;
  };
}
#endif	/* DTXMLPARSER_H */

