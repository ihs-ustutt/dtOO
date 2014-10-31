#include "qtXmlPrimitive.h"

#include <QtXml/QDomNode>
#include <logMe/logMe.h>
#include <iomanip>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  qtXmlPrimitive::qtXmlPrimitive() {
  }

  qtXmlPrimitive::~qtXmlPrimitive() {
  }  

  std::string qtXmlPrimitive::convertToString(QDomNode const node) const {
		std::string retStr;
		if (node.isNull()) {
			retStr = "Trying to convert a Null-Node to a string.";
		}
    else {
			QDomElement element = node.toElement();

			std::ostringstream head;

			head << "nodeName = " << node.nodeName().toStdString() << LOGDEL;
			head << "nodeType = " << node.nodeType() << LOGDEL;
			//
			// if element add tagName to output
			//
			if ( !element.isNull() ) {
				head << "tagName = " << element.tagName().toStdString() << LOGDEL;
			}

			//
			// write attributes
			//
			QDomNamedNodeMap attrs = node.attributes();
			for(int ii = 0; ii<attrs.size(); ii++) {
					QDomAttr attr = attrs.item(ii).toAttr();
					if ( !attr.isNull() ) {
						head << attr.name().toStdString() 
								 << "=" 
								 << attr.value().toStdString() << LOGDEL;
					}
			}
			retStr = head.str();
		}		
		return retStr;
  }	

  std::string qtXmlPrimitive::getTagName(QDomElement const element) const {
    return element.tagName().toStdString();
  }
      
  QDomElement qtXmlPrimitive::getBuilderSibling( QDomElement const element ) const {
    return ( getSibling("builder", element) );
  }    

  bool qtXmlPrimitive::hasSibling(std::string const sibName, QDomElement const element ) const {
    QDomElement tmpElement;

    tmpElement = getSibling(sibName, element);
    if ( tmpElement.isNull() ) {
      return false;
    }
    else {
      return true;
    }
  }

    bool qtXmlPrimitive::hasChild(std::string const sibName, QDomElement const element ) const {
    QDomElement tmpElement;

    tmpElement = getChild(sibName, element);
    if ( tmpElement.isNull() ) {
      return false;
    }
    else {
      return true;
    }
  }

  QDomElement qtXmlPrimitive::getSibling(std::string const sibName, QDomElement const element ) const {
//    QDomElement* tmpElementP;
    bool goBack;

    ptrHandling< QDomElement > tmpElementP(new QDomElement( element ) );

    goBack = false;
    while( !tmpElementP->isNull() ) {
      //
      // found an element, check if it has correct name
      // if so break
      //
      if ( tmpElementP->tagName().toStdString() == sibName ) {
        break;
      }
      else {
        if (goBack) {
          tmpElementP.reset( new QDomElement( tmpElementP->previousSiblingElement() ) );
        }
        else {
          tmpElementP.reset( new QDomElement( tmpElementP->nextSiblingElement() ) );
        }
        if ( tmpElementP->isNull() ) {
          //
          //return null element
          //          
          if (goBack) {
            tmpElementP.reset( new QDomElement() );
            break;
          }
          goBack = true;
          tmpElementP.reset( new QDomElement( element.previousSiblingElement() ) );
        }
      }
    }
    return ( QDomElement (*tmpElementP) );
  }    

  std::vector< QDomElement > qtXmlPrimitive::getSiblingVector(std::string const sibName, QDomElement const element ) const {
    std::vector< QDomElement > sibVec;
    bool goBack = false;
    
    QDomElement tmpElementP( element.nextSiblingElement() );
    while ( !is(sibName, tmpElementP) && !tmpElementP.isNull() ) {
      tmpElementP = tmpElementP.nextSiblingElement();
    }
    if ( tmpElementP.isNull() ) {
      goBack = true;
      tmpElementP = element.previousSiblingElement();
      while ( !is(sibName, tmpElementP) && !tmpElementP.isNull() ) {
        tmpElementP = tmpElementP.previousSiblingElement();
      }      
    }
    
    if ( tmpElementP.isNull() ) {
      return sibVec;
    }
    
    while( !tmpElementP.isNull() ) {
      if ( is(sibName, tmpElementP) ) sibVec.push_back( tmpElementP );
      //
      // found an element, check if it has correct name
      // if so break
      //
      if ( !goBack ) {
        tmpElementP = tmpElementP.nextSiblingElement();
      }
      else if (goBack) {
        tmpElementP = tmpElementP.previousSiblingElement();
      }
    }
    return sibVec;
  }
  
  std::vector< QDomElement > qtXmlPrimitive::getChildVector(std::string const sibName, QDomElement const element ) const {
    QDomElement first = getChild(sibName, element);
    std::vector< QDomElement > retVec = getSiblingVector(sibName, first);

    retVec.insert(retVec.begin(), first);

    return retVec;
  }  
	
	std::vector< QDomElement > qtXmlPrimitive::getChildVector( QDomElement const element ) const {
			QDomNodeList list = element.childNodes();
			std::vector< QDomElement > elVec;
			for (int jj=0; jj<list.length(); jj++) {
				QDomElement tmpEl = list.at(jj).toElement();
				if ( tmpEl.isNull() ) {
//					DTWARNINGWF(
//					  getChildVector(), 
//						<< "Ignore following Node, because it is not an element." << LOGDEL
//						<< DTLOGEVAL(tmpEl.isNull()) << LOGDEL
//						<< "element:" << LOGDEL
//						<< convertToString(element) << LOGDEL
//						<< "list.at(" << jj << "):" << LOGDEL
//						<< convertToString(list.at(jj)) 
//					);
				}
				else {
  		    elVec.push_back( tmpEl );
				}
			}			
			
			return elVec;
	}
  
  QDomElement qtXmlPrimitive::getChild(std::string const sibName, QDomElement const element ) const {
    bool goBack;

    ptrHandling< QDomElement > tmpElementP(new QDomElement( element.firstChildElement() ) );

    goBack = false;
    while( !tmpElementP->isNull() ) {
      //
      // found an element, check if it has correct name
      // if so break
      //
      if ( is(sibName, *tmpElementP) ) {//)->tagName().toStdString() == sibName ) {
        break;
      }
      else {
        if (goBack) {
          tmpElementP.reset( new QDomElement( tmpElementP->previousSiblingElement() ) );
        }
        else {
          tmpElementP.reset( new QDomElement( tmpElementP->nextSiblingElement() ) );
        }
        if ( tmpElementP->isNull() ) {
          //
          //return null element
          //          
          if (goBack) {
            tmpElementP.reset( new QDomElement() );
            break;
          }
          goBack = true;
          tmpElementP.reset( new QDomElement( element.previousSiblingElement() ) );
        }
      }
    }
    return ( QDomElement (*tmpElementP) );
  }
  
  QDomElement qtXmlPrimitive::getChildByName(
    std::string const sibName, 
    std::string const name, 
    QDomElement const element ) const {
    
    QDomElement wElement = getChild(sibName, element);
    
    while ( !wElement.isNull() ) {
      if ( getAttributeStr("name", wElement) == name ) {
        return wElement;
      }
      wElement = getNextSibling(sibName, wElement);
    }
    dt__THROW(getChildByName(),
            << "There is no child " << sibName << " with name " << name
    );
  }

  QDomElement qtXmlPrimitive::getChildByAttributeMatch(
    std::string const sibName, 
    std::string const attributeName, 
    std::string const attributeValue,
    QDomElement const element ) const {
    
    QDomElement wElement = getChild(sibName, element);
    
    while ( !wElement.isNull() ) {
      if ( getAttributeStr(attributeName, wElement) == attributeValue ) {
        return wElement;
      }
      wElement = getNextSibling(sibName, wElement);
    }
    dt__THROW(getChildByName(),
            << "There is no child " << sibName << " with " << attributeName << " " << attributeValue
    );
  }  

  int qtXmlPrimitive::getNChildren(std::string const sibName, QDomElement const element) const {
    int counter = 0;
    QDomElement wElement = getChild(sibName, element);
    while ( !wElement.isNull() ) {
      counter++;
      wElement = getNextSibling( sibName, wElement);
    }
    return counter;
  }

  QDomElement qtXmlPrimitive::getNextSibling(std::string const sibName, QDomElement const element ) const {
    QDomElement tmpElement = QDomElement( element.nextSiblingElement() );
    while ( (tmpElement.tagName().toStdString() != sibName) && (!tmpElement.isNull()) ) {
      tmpElement = QDomElement( tmpElement.nextSiblingElement() );
    }             
    return getSibling(sibName, tmpElement);
  } 

  bool qtXmlPrimitive::isBuilder( QDomElement const element ) const {
    if ( element.tagName().toStdString() == "builder" ) {
      return true;
    }
    else {
      return false;
    }
  }  

  bool qtXmlPrimitive::hasAttribute(std::string const attName, const QDomElement element) const {
    return element.hasAttribute(attName.c_str());
  }
    
  int qtXmlPrimitive::getAttributeInt(std::string const attName, const QDomElement element) const {
    bool flag;
    int rInt = 0;
    rInt = element.attribute( attName.c_str() ).toInt(&flag);
    if (!flag) {
      dt__THROW( getAttributeInt(),
        << convertToString(element) << LOGDEL
        << DTLOGEVAL(attName) << LOGDEL
        << "Attribute is not an Integer");
    }
    return rInt;
  }

  float qtXmlPrimitive::getAttributeFloat(std::string const attName, const QDomElement element) const {
    bool flag;
    float rInt = 0;
    rInt = element.attribute( attName.c_str() ).toFloat(&flag);
    if (!flag) {
      dt__THROW( getAttributeInt(),
              << convertToString(element) << LOGDEL
              << DTLOGEVAL(attName) << LOGDEL
              << "Attribute is not a Float");
    }
    return rInt;
  }  

  std::string qtXmlPrimitive::getAttributeStr(std::string const attName, const QDomElement element) const {
    std::string rInt;
    rInt = element.attribute( attName.c_str(), "" ).toStdString();
    return rInt;
  }

  bool qtXmlPrimitive::getAttributeBool(std::string const attName, const QDomElement element) const {
    std::string attValue;
    attValue = element.attribute( attName.c_str() ).toStdString();
    if (attValue == "true") {
      return true;
    }
    else if (attValue == "false") {
      return false;
    }
    return false;
    DTWARNINGWF(getAttributeBool(),
            << "Value of attribute = " << attValue << LOGDEL
            << "Should be true or false.");
  }    

  bool qtXmlPrimitive::is(std::string const tagName, QDomElement const element ) const {
    if ( element.tagName().toStdString() == tagName ) {
      return true;
    }
    else {
      return false;
    }
  }  

  QDomElement qtXmlPrimitive::goToLastBuilder( QDomElement const element ) const {
    QDomElement tmpElementP = QDomElement( getBuilderSibling( element.firstChildElement() ) );  
//		if ( tmpElementP.isNull() ) {
//			return element;
//		}
    while( !tmpElementP.isNull() ) {
      //
      // found an element, check if it is a builder
      // if so go to its child, if not go away
      //
      if ( isBuilder(tmpElementP) ) {
				QDomElement tmpChild = tmpElementP.firstChildElement();
				if ( tmpChild.isNull() ) return tmpElementP;
        tmpElementP = QDomElement( tmpElementP.firstChildElement() );        
      }
      else {
        if ( getBuilderSibling(tmpElementP).isNull() ) {
          break;
        }
        else {
          tmpElementP = QDomElement( getBuilderSibling(tmpElementP) );          
        }
      }
    }
    //
    // workingNodeAsElement is a null element
    // go to the parent and that's it
    //		
    QDomNode tmpNodeP = QDomNode( tmpElementP.parentNode() );
    return ( QDomElement ( tmpNodeP.toElement() ) );//*tmpElementP ) );      
  }

  QDomElement qtXmlPrimitive::createElement(QDomDocument & doc, std::string const name) const {
    return doc.createElement( name.c_str() );
  }

  void qtXmlPrimitive::appendChildElement(QDomDocument & doc, QDomElement & element) const {
    doc.documentElement().appendChild(element);
  }
    
  void qtXmlPrimitive::appendChildElementInElement(QDomDocument & doc, 
                                              std::string const elName,
                                              std::string const elAttribute,
                                              std::string const elAttributeValue,
                                              QDomElement & element) const {
    QDomElement documentElement = doc.documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( elName.c_str() ); 
    for (int ii=0;ii<elements.size();ii++) {
      QDomElement foundEl = elements.at(ii).toElement();
      if ( foundEl.hasAttribute( elAttribute.c_str() ) ) {
        if ( getAttributeStr(elAttribute, foundEl)
             ==
             elAttributeValue ) {
        elements.at(ii).toElement().appendChild(element);            
        } 
      }

    }
  }

  void qtXmlPrimitive::appendChildElementInElement(QDomElement & documentElement, 
                                              std::string const elName,
                                              std::string const elAttribute,
                                              std::string const elAttributeValue,
                                              QDomElement & element) const {
    QDomNodeList elements = documentElement.elementsByTagName( elName.c_str() ); 
    for (int ii=0;ii<elements.size();ii++) {
      QDomElement foundEl = elements.at(ii).toElement();
      if ( foundEl.hasAttribute( elAttribute.c_str() ) ) {
        if ( getAttributeStr(elAttribute, foundEl)
             ==
             elAttributeValue ) {
        elements.at(ii).toElement().appendChild(element);            
        } 
      }

    }
}    
}

