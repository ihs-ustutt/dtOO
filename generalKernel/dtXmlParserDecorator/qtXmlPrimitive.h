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
    dt__CLASSNAME(qtXmlPrimitive);  
    qtXmlPrimitive();
    virtual ~qtXmlPrimitive();  
    /**
     * xml navigation
     */
    QDomElement goToLastBuilder( QDomElement const element ) const;
    /**
     * xml navigation
     */
    QDomElement getBuilderSibling( QDomElement const element ) const;
    /**
     * xml navigation
     */
    QDomElement getSibling(std::string const sibName, QDomElement const element ) const;
    std::vector< QDomElement > getSiblingVector(std::string const sibName, QDomElement const element ) const;
    std::vector< QDomElement > getChildVector(std::string const sibName, QDomElement const element ) const;
    std::vector< QDomElement > getChildVector( QDomElement const element ) const;
    /**
     * xml navigation
     */  
    QDomElement getChild(std::string const sibName, QDomElement const element ) const;
    QDomElement getChildByName(
      std::string const sibName, 
      std::string const name, 
      QDomElement const element 
    ) const;
    QDomElement getChildByAttributeMatch(
      std::string const sibName,  
      std::string const attributeName,  
      std::string const attributeValue, 
      QDomElement const element 
    ) const;
    /**
     * xml navigation
     */
    int getNChildren(std::string const sibName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    bool hasSibling(std::string const sibName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    bool hasChild(std::string const sibName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    QDomElement getNextSibling(std::string const sibName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    bool is( std::string const tagName, QDomElement const element ) const;  
    /**
     * xml navigation
     */
    bool isBuilder( QDomElement const element ) const;  
    /**
     * xml navigation
     */
    bool hasAttribute(std::string const attName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    int getAttributeInt( std::string const attName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    float getAttributeFloat( std::string const attName, QDomElement const element ) const;
    /**
     * xml navigation
     */
    std::string getAttributeStr( std::string const attName, QDomElement const element ) const; 
    /**
     * xml navigation
     */    
    bool getAttributeBool(std::string const attName, const QDomElement element) const;
    /**
     * output a xml element
     */
    std::string convertToString( QDomNode const node ) const;
    std::string getTagName(QDomElement const element) const;
    /**
     * create an element
     */
    QDomElement createElement(QDomDocument & doc, std::string const name) const;
    /**
     * append element in first child
     */
    void appendChildElement(QDomDocument & doc, QDomElement & element) const;
    /**
     * append element in given element as child
     */
    void appendChildElementInElement(QDomDocument &doc, 
                                     std::string const elName,
                                     std::string const elAttribute,
                                     std::string const elAttributeValue,
                                     QDomElement & element) const;
    void appendChildElementInElement(QDomElement & documentElement, 
                                      std::string const elName,
                                      std::string const elAttribute,
                                      std::string const elAttributeValue,
                                      QDomElement & element) const;    
  };
}

#endif	/* QTXMLPRIMITIVE_H */

