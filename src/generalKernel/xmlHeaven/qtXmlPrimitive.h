#ifndef QTXMLPRIMITIVE_H
#define	QTXMLPRIMITIVE_H


#include <string>
#include <vector>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/stringPrimitive.h>

namespace dtOO {
  class qtXmlPrimitive : public stringPrimitive {
  public:
    dt__classOnlyName(qtXmlPrimitive);  
    virtual ~qtXmlPrimitive();  
    static ::QDomElement getSibling(
      std::string const sibName, ::QDomElement const element 
    );
    static std::vector< ::QDomElement > getSiblingVector(
      std::string const sibName, ::QDomElement const element 
    );
    static std::vector< ::QDomElement > getSiblingVector(    
      ::QDomElement const element 
    );
    static std::vector< ::QDomElement > getChildVector(
      std::string const sibName, ::QDomElement const element 
    );
    static std::vector< ::QDomElement > getDescendantVector(
      std::string const sibName, ::QDomElement const element 
    );
    static std::vector< ::QDomElement > getChildVector( 
      ::QDomElement const element 
    );
    static ::QDomElement getChild(
      std::string const sibName, ::QDomElement const element 
    );
    static ::QDomElement getChildByName(
      std::string const sibName, 
      std::string const name, 
      ::QDomElement const element 
    );
    static ::QDomElement getChildByAttributeMatch(
      std::string const sibName,  
      std::string const attributeName,  
      std::string const attributeValue, 
      ::QDomElement const element 
    );
    static bool hasChildElement( 
      std::string const elementTag, 
      std::string const labelAttributeVal,
      ::QDomElement const & parentElement
    );    
    static ::QDomElement getChildElement( 
      std::string const elementTag, 
      std::string const labelAttributeVal,
      ::QDomElement const & parentElement
    );    
    static void getChildLabels(
      std::string childName, 
      std::vector< std::string > * labelValue,
      ::QDomElement const & parentElement
    );    
    static int getNChildren(
      std::string const sibName, ::QDomElement const element 
    );
    static bool hasSibling(
      std::string const sibName, ::QDomElement const element 
    );
    static bool hasChild(
      std::string const sibName, ::QDomElement const element 
    );
    static ::QDomElement getNextSibling(
      std::string const sibName, ::QDomElement const element 
    );
    static bool is( std::string const tagName, ::QDomElement const element );  
    static bool isBuilder( ::QDomElement const element );  
    static bool hasAttribute(
      std::string const attName, ::QDomElement const element 
    );
    static int getAttributeInt( 
      std::string const attName, ::QDomElement const element 
    );
    static float getAttributeFloat( 
      std::string const attName, ::QDomElement const element 
    );
    static std::string getAttributeStr( 
      std::string const attName, ::QDomElement const element 
    ); 
    static std::string getAttributeStr(
      std::string const attName, const ::QDomElement element, 
      std::string const def
    );    
    static std::string getAttributeRareStr( 
      std::string const attName, ::QDomElement const element 
    );     
    static bool getAttributeBool(
      std::string const attName, const ::QDomElement element
    );
    static std::vector< std::string > getAttributeStrVector(
      std::string const attName, ::QDomElement const element
    );    
    static std::vector< std::string > getAttributeRareStrVector(
      std::string const attName, ::QDomElement const element
    );
    static std::vector< int > getAttributeIntVector( 
      std::string const attName, ::QDomElement const element
    );
    static std::vector< float > getAttributeFloatVector( 
      std::string const attName, ::QDomElement const element
    );    
    static std::string convertToString( ::QDomNode const node );
    static std::string getTagName(::QDomElement const element);
    static ::QDomElement createElement(
      QDomDocument & doc, std::string const name
    );
    static void appendChildElement(QDomDocument & doc, ::QDomElement & element);
    static void appendChildElementInElement(
      QDomDocument &doc, 
      std::string const elName,
      std::string const elAttribute,
      std::string const elAttributeValue,
      ::QDomElement & element
    );
    static void appendChildElementInElement(
      ::QDomElement & documentElement, 
      std::string const elName,
      std::string const elAttribute,
      std::string const elAttributeValue,
      ::QDomElement & element
    );        
    static void replaceInAllAttributes( 
      std::string const replace, 
      std::string const with, 
      ::QDomElement * const element 
    );
    static void replaceRecursiveInAllAttributes( 
      std::string const replace, 
      std::string const with, 
      ::QDomElement * const element 
    );    
  protected:
    qtXmlPrimitive();        
  private:
    static std::vector< QDomAttr > getAttributeVector( 
      ::QDomElement const element 
    );
  };
}

#endif	/* QTXMLPRIMITIVE_H */

