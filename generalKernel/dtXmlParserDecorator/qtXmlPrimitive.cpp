#include "qtXmlPrimitive.h"

#include <QtXml/QDomNode>
#include <logMe/logMe.h>
#include <iomanip>
#include <interfaceHeaven/ptrHandling.h>
#include <progHelper.h>

namespace dtOO {
  qtXmlPrimitive::qtXmlPrimitive() {
  }

  qtXmlPrimitive::~qtXmlPrimitive() {
  }  

  std::string qtXmlPrimitive::convertToString(QDomNode const node) {
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

  std::string qtXmlPrimitive::getTagName(QDomElement const element) {
    return element.tagName().toStdString();
  }
      
  QDomElement qtXmlPrimitive::getBuilderSibling( QDomElement const element ) {
    return ( getSibling("builder", element) );
  }    

  bool qtXmlPrimitive::hasSibling(std::string const sibName, QDomElement const element ) {
    QDomElement tmpElement;

    tmpElement = getSibling(sibName, element);
    if ( tmpElement.isNull() ) {
      return false;
    }
    else {
      return true;
    }
  }

    bool qtXmlPrimitive::hasChild(std::string const sibName, QDomElement const element ) {
    QDomElement tmpElement;

    tmpElement = getChild(sibName, element);
    if ( tmpElement.isNull() ) {
      return false;
    }
    else {
      return true;
    }
  }

  QDomElement qtXmlPrimitive::getSibling(std::string const sibName, QDomElement const element ) {
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

  std::vector< QDomElement > qtXmlPrimitive::getSiblingVector(std::string const sibName, QDomElement const element ) {
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
  
  std::vector< QDomElement > qtXmlPrimitive::getChildVector(std::string const sibName, QDomElement const element ) {
    QDomElement first = getChild(sibName, element);
    std::vector< QDomElement > retVec = getSiblingVector(sibName, first);

    retVec.insert(retVec.begin(), first);

    return retVec;
  }  
	
	std::vector< QDomElement > qtXmlPrimitive::getChildVector( QDomElement const element ) {
		QDomNodeList list = element.childNodes();
		std::vector< QDomElement > elVec;
		for (int jj=0; jj<list.length(); jj++) {
			QDomElement tmpEl = list.at(jj).toElement();
			if ( !tmpEl.isNull() ) {					
				elVec.push_back( tmpEl );
			}
		}			

		return elVec;
	}
	 
  QDomElement qtXmlPrimitive::getChild(std::string const sibName, QDomElement const element ) {
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
    QDomElement const element ) {
    
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
    QDomElement const element ) {
    
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

  bool qtXmlPrimitive::hasChildElement( 
    std::string const elementTag, 
    std::string const labelAttributeVal,
    QDomElement const & parentElement 
  ) {   
    QDomElement element = parentElement.firstChildElement();
    while( !element.isNull() ) {
      //
      // found a part, now check for a name
      // if so store me
      //
      if (getTagName(element) == elementTag) {
        if ( element.hasAttribute("label") ) {
          if ( getAttributeStr("label", element) == labelAttributeVal ) {
            return true;
          }
        }
        else {
          dt__THROW(getChildElement, << "No name attribute.");
        }      
      }
      //
      //go to next sibling
      //
      element = element.nextSiblingElement();
    }

    return false;
  }
  
  QDomElement qtXmlPrimitive::getChildElement( 
    std::string const elementTag, 
    std::string const labelAttributeVal,
    QDomElement const & parentElement ) {
    
    /* ------------------------------------------------------------------------ */
    /* check for a part element in each child of root (first generation) */
    /* ------------------------------------------------------------------------ */  
    QDomElement element = parentElement.firstChildElement();
    while( !element.isNull() ) {
      //
      // found a part, now check for a name
      // if so store me
      //
      if ( getTagName(element) == elementTag) {
        if ( hasAttribute("label", element) ) {
          if ( getAttributeStr("label", element) == labelAttributeVal ) {
            return element;
          }
        }
        else {
          dt__THROW(
						getChildElement, 
						<< DTLOGEVAL(elementTag) << LOGDEL
						<< DTLOGEVAL(labelAttributeVal) << LOGDEL
						<< "No label attribute."
					);
        }      
      }
      //
      //go to next sibling
      //
      element = element.nextSiblingElement();
    }

    //
    //error case, no such element
    //
    dt__THROW(
			getChildElement(),
      << "No " << elementTag << "-Element with " 
			<< DTLOGEVAL(labelAttributeVal) 
		);
  }	

  void qtXmlPrimitive::getChildLabels( 
	  std::string childName, 
		std::vector< std::string > * labelValue, 
		QDomElement const & parentElement
	) {
		std::vector< QDomElement > children = getChildVector(parentElement);
		for (int ii=0; ii<children.size(); ii++) {
      //
      // found a part, now check for a name
      // if so store me
      //
      if ( getTagName(children[ii]) == childName) {
        if ( hasAttribute("label", children[ii]) ) {
          labelValue->push_back( getAttributeStr("label", children[ii]) );
        }
        else {
          dt__THROW(getChildLabels(), << "No label attribute!");
        }      
      }
    }
  }
	
  int qtXmlPrimitive::getNChildren(std::string const sibName, QDomElement const element) {
    int counter = 0;
    QDomElement wElement = getChild(sibName, element);
    while ( !wElement.isNull() ) {
      counter++;
      wElement = getNextSibling( sibName, wElement);
    }
    return counter;
  }

  QDomElement qtXmlPrimitive::getNextSibling(std::string const sibName, QDomElement const element ) {
    QDomElement tmpElement = QDomElement( element.nextSiblingElement() );
    while ( (tmpElement.tagName().toStdString() != sibName) && (!tmpElement.isNull()) ) {
      tmpElement = QDomElement( tmpElement.nextSiblingElement() );
    }             
    return getSibling(sibName, tmpElement);
  } 

  bool qtXmlPrimitive::isBuilder( QDomElement const element ) {
    if ( element.tagName().toStdString() == "builder" ) {
      return true;
    }
    else {
      return false;
    }
  }  

  bool qtXmlPrimitive::hasAttribute(std::string const attName, const QDomElement element) {
    return element.hasAttribute(attName.c_str());
  }
    
  int qtXmlPrimitive::getAttributeInt(std::string const attName, const QDomElement element) {
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

  float qtXmlPrimitive::getAttributeFloat(std::string const attName, const QDomElement element) {
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

  std::string qtXmlPrimitive::getAttributeStr(std::string const attName, const QDomElement element) {
    std::string str = element.attribute( attName.c_str(), "" ).toStdString();
		
		//
		// remove whitespaces
		//
    str.erase(
		  std::remove_if(
		    str.begin(), 
				str.end(), 
				::isspace
		  ), 
			str.end()
	  );
    return str;
  }

  bool qtXmlPrimitive::getAttributeBool(std::string const attName, const QDomElement element) {
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

	std::vector< std::string > qtXmlPrimitive::getAttributeStringVector(
		std::string const attName, QDomElement const element
	) {
		std::string att = getAttributeStr(attName, element);
		return convertToStringVector("{", "}", att);
	}
	
  bool qtXmlPrimitive::is(std::string const tagName, QDomElement const element ) {
    if ( element.tagName().toStdString() == tagName ) {
      return true;
    }
    else {
      return false;
    }
  }  

  QDomElement qtXmlPrimitive::goToLastBuilder( QDomElement const element ) {
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

  QDomElement qtXmlPrimitive::createElement(QDomDocument & doc, std::string const name) {
    return doc.createElement( name.c_str() );
  }

  void qtXmlPrimitive::appendChildElement(QDomDocument & doc, QDomElement & element) {
    doc.documentElement().appendChild(element);
  }
    
  void qtXmlPrimitive::appendChildElementInElement(QDomDocument & doc, 
                                              std::string const elName,
                                              std::string const elAttribute,
                                              std::string const elAttributeValue,
                                              QDomElement & element) {
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

  void qtXmlPrimitive::appendChildElementInElement(
	  QDomElement & documentElement, 
		std::string const elName,
		std::string const elAttribute,
		std::string const elAttributeValue,
		QDomElement & element
	) {
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
 
	void qtXmlPrimitive::replaceInAllAttributes( 
	  std::string const replace, std::string const with, 
	  QDomElement * const element 
	) {
		std::vector< QDomAttr > attrs = getAttributeVector(*element);
		dt__FORALL(attrs, ii,	
      std::string name = attrs[ii].name().toStdString();						
			std::string value = attrs[ii].value().toStdString();
			std::string replaced = replaceStringInString(replace, with, value);
			attrs[ii].setValue( QString(replaced.c_str()) );
//			DTINFOWF(
//				replaceInAllAttributes(),
//				<< "Replace attribute " << name << " = " << value << "." << LOGDEL
//				<< "-> " << replaced
//			);
		);
	}  
	
	void qtXmlPrimitive::replaceRecursiveInAllAttributes( 
	  std::string const replace, std::string const with, 
	  QDomElement * const element 
	) {
		replaceInAllAttributes(replace, with, element);
    std::vector< QDomElement > children = getChildVector(*element);			
		dt__FORALL(children, ii,	
			replaceRecursiveInAllAttributes(replace, with, &children[ii]);						
		);		
	}  
	
	std::vector< QDomAttr > qtXmlPrimitive::getAttributeVector( QDomElement const element ) {
		std::vector< QDomAttr > attrs;
		if( element.hasAttributes() ) {
			QDomNamedNodeMap map = element.attributes();
			for( int i = 0 ; i < map.length() ; ++i ) {
			  if(!(map.item(i).isNull())) {
					QDomNode debug = map.item(i);
					QDomAttr attr = debug.toAttr();
					if(!attr.isNull()) {
						attrs.push_back(attr);
					}
			  }
		  }
		}
		
		return attrs;
	}		
}

