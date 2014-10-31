#include "dtXmlParser.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <boundedVolumeFactory.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <constValueHeaven/constValueFactory.h>
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <dtXmlParserDecorator/dtXmlParserDecoratorFactory.h>
#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <dtXmlParserDecorator/dtXmlParserFunctionDecoratorFactory.h>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QTextStream>

namespace dtOO {
  dtXmlParser::dtXmlParser() {
  }

  dtXmlParser::~dtXmlParser() {
  }

  void dtXmlParser::openFileAndParse(char const * const fileName) {
    QFile xmlFile(fileName);
    QDomDocument xmlDocument;

		//
    // open file
		//
    if( !xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      dt__THROW(openFileAndParse, << "Failed to open " << DTLOGEVAL(fileName));
    }

    //
    // read file
    //
    QString qString;
    int line;
    int column;
    if( !xmlDocument.setContent( &xmlFile, &qString, &line, &column ) ) {
      dt__THROW( openFileAndParse(),
              << DTLOGEVAL(qPrintable(qString) ) << LOGDEL
              << DTLOGEVAL(line) << LOGDEL
              << DTLOGEVAL(column) << LOGDEL    
              << "Failed to parse file " << fileName);
    }

    //
    // close file
    //
    xmlFile.close();

		_rootRead.push_back( 
		  xmlDocument.documentElement().cloneNode(true).toElement()
		);

		if (_rootRead.size() == 0) {
			dt__THROW(
			  openFileAndParse(), 
				<< "Error parsing file " << fileName << LOGDEL
				<< DTLOGEVAL(_rootRead.size()) );
		}
		
	
		//
		// handle includes
		//
		// get labels and store filename in label
		// and remove include nodes
		std::vector< std::string > label;
		getNames("include", &label);
		for (int ii=0; ii<label.size(); ii++) {
		  QDomElement wElement = getElement("include", label[ii]);
			DTINFOWF( 
			  openFileAndParse(), 
				<< "Replace include " << DTLOGEVAL(label[ii]) << " with "
				<< DTLOGEVAL(getAttributeStr("filename", wElement)) 
			);
			label[ii] = getAttributeStr("filename", wElement);
			QDomNode check 
			= 
			_rootRead.back().removeChild( static_cast<QDomNode>(wElement) );
			if (check.isNull()) {
				dt__THROW(
					openFileAndParse(),
					<< "Error removing element:" << LOGDEL
					<< convertToString(check)
				);
			}			
		}
    // parse included files
		for (int ii=0; ii<label.size(); ii++) {
			this->openFileAndParse( label[ii].c_str() );
		}
    
		//
    // set static properties on classes
    //
    setStaticProperties();
  }

  void dtXmlParser::openFileAndLoad(char const * const fileName) {

    QDomDocument xmlDocument;
    //
    // check and initialize file
    //
    checkFile(fileName, xmlDocument);

    _rootLoad = xmlDocument.documentElement().cloneNode(true).toElement(); 
  }
  
  void dtXmlParser::loadStateToConst(
    std::string const stateName, 
    vectorHandling< constValue * > & cValRef
  ) {  
    QDomElement stateElement = getChildElement( "state", stateName, _rootLoad );
    for (int ii=0;ii<cValRef.size();ii++) {
      if ( hasChildElement("constValue", cValRef[ii]->getLabel(), stateElement) ) {
          
        QDomElement constValueElement = getChildElement("constValue",
                                          cValRef[ii]->getLabel(),
                                          stateElement
                                        );
        cValRef[ii]->setValue( 
          getAttributeFloat( 
            "value",
            constValueElement.firstChildElement().firstChildElement() 
          ) 
        );
      }
      else {
        DTWARNINGWF(loadStateToConst(),
                << cValRef[ii]->getLabel() << "-Element not in state file." << LOGDEL
                << "Leave value as it was before load state.");  
      }
    }
  }
  
  std::vector< std::string > dtXmlParser::getStates(void) {
    if (_rootLoad.isNull()) {
      dt__THROW(getStates, << DTLOGEVAL(_rootLoad.isNull()) );
    }
    std::vector< std::string > labels;
    getChildLabels("state", &labels, _rootLoad);
    return labels;
  }
    
  void dtXmlParser::openFileAndWrite(char const * const fileName,
                                     vectorHandling< constValue * > * cValP) {

    QDomDocument xmlDocument;
    //
    // check and initialize file
    //
    checkFile(fileName, xmlDocument);
    
    //*_rootWriteElement = xmlDocument.toElement();
    QDomElement newState = createElement(xmlDocument, "state");
    std::string timeStr = NowDateAndTime();
    newState.setAttribute("label", timeStr.c_str());
    appendChildElement(xmlDocument, newState);
    
    for (int ii=0;ii<cValP->size();ii++) {
      cValP->at(ii)->writeToElement(xmlDocument, newState);
    }
    
    //
    // write file
    //
    writeFile(fileName, xmlDocument);
    DTINFOWF(openFileAndWrite,
            << "Save state = " << timeStr << " to file = " << fileName );
  }

  void dtXmlParser::openFileAndWrite(char const * const fileName,
                                     std::string const stateName,
                                     vectorHandling< constValue * > * cValP) {

    QDomDocument xmlDocument;
    //
    // check and initialize file
    //
    checkFile(fileName, xmlDocument);
    
    //*_rootWriteElement = xmlDocument.toElement();
    QDomElement newState = createElement(xmlDocument, "state");
//    std::string timeStr = std::string(stateName);//NowDateAndTime();
    newState.setAttribute("label", stateName.c_str());
    appendChildElement(xmlDocument, newState);
    
    for (int ii=0;ii<cValP->size();ii++) {
      cValP->at(ii)->writeToElement(xmlDocument, newState);
    }
    
    //
    // write file
    //
    writeFile(fileName, xmlDocument);
    DTINFOWF(openFileAndWrite,
            << "Save state = " << stateName << " to file = " << fileName );
  }
  
  void dtXmlParser::writeFile( char const * const fileName, QDomDocument & xmlDocument) {
    //
    // open file and write rootWriteElement to file
    //
    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&xmlFile);
    stream << xmlDocument.toString();

    //
    // close file
    //
    xmlFile.close();    
  }
  
  void dtXmlParser::checkFile(char const * const fileName, QDomDocument & xmlDocument) {
    //
    // open file for reading
    //
    QFile xmlFile(fileName);
    
    //
    // check if file exists, if not create
    //
    if ( !xmlFile.exists() ) {
      DTWARNINGWF(checkFile,
              << "Failed to open " << DTLOGEVAL(fileName) << LOGDEL
              << "File does not exist. Try to create file.");
      xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
      xmlFile.close();
    }
    
    //
    //open file
    //
    if( !xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      dt__THROW(checkFile,
              << "Failed to open " << DTLOGEVAL(fileName));
    }

    // try to read file and check if it is correct
    QString qString;
    int line;
    int column;
    //
    // parsing error
    //
    if( !xmlDocument.setContent( &xmlFile, &qString, &line, &column ) ) {
      DTWARNINGWF( checkFile(),
              << DTLOGEVAL(qPrintable(qString) ) << LOGDEL
              << DTLOGEVAL(line) << LOGDEL
              << DTLOGEVAL(column) << LOGDEL    
              << "Failed to parse file " << fileName << LOGDEL
              << "Recreate file.");
      //
      // initialize new file
      //
      xmlDocument = QDomDocument();
      // encoding
      QDomProcessingInstruction myHeader = xmlDocument.createProcessingInstruction( "xml","version=\"1.0\" encoding=\"ISO-8859-1\"" );
      xmlDocument.appendChild( myHeader );
      // machine name of _rootReadElement
      QDomElement _rootWriteElement = xmlDocument.createElement(
        _rootRead[0].tagName()
      );
      xmlDocument.appendChild(_rootWriteElement);
    }
    //
    // successful parsing
    //
    else {
      // check if root element has correct name
      if ( xmlDocument.documentElement().tagName() 
           == 
           _rootRead[0].tagName() ) {
        DTINFOWF(checkFile, << "Correct root Element name. Write to this file.");
      }
      //
      // rootReadElement and rootWriteElement have not same root name
      //
      else {
        dt__THROW(checkFile, << "Incorrect root Element name.");        
      }
    }
    //
    // close file
    //
    xmlFile.close();    
  }

  void dtXmlParser::createConstValue(string const constValueLabel, 
                                    vectorHandling< constValue * > * cValP) const {

    QDomElement const cVElement = getElement( "constValue", constValueLabel );


    /* ------------------------------------------------------------------------ */
    /* go to last builder */
    /* ------------------------------------------------------------------------ */
    QDomElement const wElement = goToLastBuilder(cVElement);

    constValue * aCValP;
    constValueFactory cValFac;

    aCValP = cValFac.create( getAttributeStr("name", wElement) );
    aCValP->setLabel( constValueLabel );
    //
    //check first child
    //should be a float
    //
    QDomElement* firstChild = new QDomElement( wElement.firstChildElement() );
    if (firstChild->tagName().toStdString() == "float" ) {
      if ( firstChild->hasAttribute("min") 
            && firstChild->hasAttribute("max")
            && firstChild->hasAttribute("value") ) {
        //
        // set value, min and max
        //
        aCValP->setValue( muParseString( getAttributeStr( "value", *firstChild ) ) );
        aCValP->setRange( muParseString( getAttributeStr( "min", *firstChild ) ), 
                          muParseString( getAttributeStr( "max", *firstChild ) ) );        
      }
      else {
        dt__THROW(createConstValue(),
                << "Missing attributes min, max or float");
      }
    }
    else if (firstChild->tagName().toStdString() == "int" ) {
      if ( firstChild->hasAttribute("value") ) {
        //
        // set value, min and max
        //
        aCValP->setValue( muParseString( getAttributeStr( "value", *firstChild ) ) );        
      }
      else {
        dt__THROW(createConstValue(),
                << "Missing attributes min, max or float");
      }
    }    
    else {
      dt__THROW( createConstValue(),
              << DTLOGEVAL( qPrintable(firstChild->tagName()) ) );
    }

    cValP->push_back( aCValP );
  }
	
  void dtXmlParser::createConstValue(vectorHandling< constValue * > * cValP) const {
    std::vector< std::string > label = getNames("constValue");

		dt__FORALL( label, ii, createConstValue(label[ii], cValP); );
  }
	
  void dtXmlParser::getNames( string lookType, std::vector< string > * names ) const {
		if (_rootRead.size() != 0) {
			for (int ii=0; ii<_rootRead.size(); ii++) {
				if (_rootRead[ii].isNull()) {
					dt__THROW(getNames(),
									<< DTLOGEVAL(_rootRead[ii].isNull()) << LOGDEL
									<< "Parsed file resulsts in a NULL Pointer.");
				}
				std::vector< std::string > locNames;
				getChildLabels(lookType, &locNames, _rootRead[ii]);
				dt__FORALL(locNames, jj, names->push_back(locNames[jj]); );
			}
		}
		else {
				dt__THROW(getNames(), << "Please parse a XML file.");
		}
  }
	
  std::vector< std::string > dtXmlParser::getNames( string lookType ) const {
		std::vector< std::string > names;
		getNames(lookType, &names);
		return names;
  }	

  void dtXmlParser::getName( string lookType, std::string * name ) const {
    std::vector< std::string > names;
    
    getNames(lookType, &names);
    
    if (names.size() != 1) {
      DTWARNINGWF(getName(),
              << "More than one " << DTLOGEVAL(lookType) << " in file." << LOGDEL
              << "Taking the first one.");
    }
    
    *name = names[0];
  }  

  void dtXmlParser::getChildLabels( std::string childName, 
                                   std::vector< string > * labelValue, 
                                   QDomElement const & parentElement) const {
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

  QDomElement dtXmlParser::getElement( string const lookType, string const lookName ) const {
		for (int ii=0; ii<_rootRead.size(); ii++) {
			if ( hasChildElement(lookType, lookName, _rootRead[ii]) ) {
				return getChildElement(lookType, lookName, _rootRead[ii]);
			}
		}
    dt__THROW(
		  getElement(),
      << "No " << lookType << "-Element with " << DTLOGEVAL(lookName) 
		);
  }
  
  QDomElement dtXmlParser::getElement( string const lookType ) const {
    std::vector< std::string > label;

    getNames(lookType, &label);

    if (label.size() != 1) {
      dt__THROW(getElement(),
              << DTLOGEVAL(label.size()) << LOGDEL
              << "Should be 1.");      
    }
    
    return QDomElement( getElement(lookType, label[0]) );
  }

  bool dtXmlParser::hasChildElement( 
    std::string const elementTag, 
    std::string const labelAttributeVal,
    QDomElement const & parentElement 
  ) const {   
    /* ------------------------------------------------------------------------ */
    /* check for a part element in each child of root (first generation) */
    /* ------------------------------------------------------------------------ */  
    QDomElement element = QDomElement(parentElement.firstChildElement());
    while( !element.isNull() ) {
      //
      // found a part, now check for a name
      // if so store me
      //
      if (element.tagName().toStdString() == elementTag) {
        if ( element.hasAttribute("label") ) {
          if ( element.attribute("label").toStdString() == labelAttributeVal ) {
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
      element = QDomElement( element.nextSiblingElement() );
    }

    return false;
  }
  
  QDomElement dtXmlParser::getChildElement( 
    std::string const elementTag, 
    std::string const labelAttributeVal,
    QDomElement const & parentElement ) const {
    
    /* ------------------------------------------------------------------------ */
    /* check for a part element in each child of root (first generation) */
    /* ------------------------------------------------------------------------ */  
    QDomElement element = QDomElement(parentElement.firstChildElement());
    while( !element.isNull() ) {
      //
      // found a part, now check for a name
      // if so store me
      //
      if (element.tagName().toStdString() == elementTag) {
        if ( element.hasAttribute("label") ) {
          if ( element.attribute("label").toStdString() == labelAttributeVal ) {
            return QDomElement( element );
          }
        }
        else {
          dt__THROW(getChildElement, << "No name attribute.");
        }      
      }
      //
      //go to next sibling
      //
      element = QDomElement( element.nextSiblingElement() );
    }

    //
    //error case, no such element
    //
    dt__THROW(getChildElement(),
            << "No " << elementTag << "-Element with " << DTLOGEVAL(labelAttributeVal) );
  }

  void dtXmlParser::createAnalyticFunction(
	  string const functionName, 
    vectorHandling< constValue * > const * const cVP, 
    vectorHandling< analyticFunction * > * sFP
	) const {
    //
	  // get function and builder element
	  //
    QDomElement aFElement = getElement("function", functionName);
		QDomElement builderElement = getChild("builder", aFElement);
		
		//
		// logging
		//
    DTLOGCHAP(creating function);
    DTINFOWF(createFunction(), << convertToString(aFElement) );
		//
    // create builder
		//
		std::string builderName = getAttributeStr("name", builderElement);
		bool buildCompound = false;
		if ( stringPrimitive::stringContains("[]", builderName) ) {
			buildCompound = true;
			stringPrimitive::getStringBetweenAndRemove("[", "]", &builderName);
		}
		ptrHandling<dtXmlParserFunctionDecorator> funDecoP(
  		dtXmlParserFunctionDecoratorFactory::create(builderName)
		);

		//
		// call builder
		//
    vectorHandling< analyticFunction * > tmpSFun;
		if (!buildCompound) {
      funDecoP->buildPart(builderElement, cVP, sFP, &tmpSFun);
		}
		else {
			funDecoP->buildPartCompound(builderElement, cVP, sFP, &tmpSFun);
		}

    //
    // copy analyticFunctions from tmpSFun to sFP
    //
    for (int ii=0;ii<tmpSFun.size();ii++) {
      sFP->push_back( tmpSFun[ii] );
      sFP->back()->setLabel(functionName);
    }
    tmpSFun.clear();
  }	
	
  void dtXmlParser::createAnalyticFunction(
    vectorHandling< constValue * > const * const cVP, 
    vectorHandling< analyticFunction * > * sFP
	) const {
		std::vector< std::string > label = getNames("function");
		
		dt__FORALL( label, ii, createAnalyticFunction(label[ii], cVP, sFP); );
  }		

	/**
	 * 
   * @todo: Attribute setting?
   */
  void dtXmlParser::createAnalyticGeometry(
	  string const label,
    pointContainer * const pCP,
		vectorContainer * const vCP,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > * aGP
	) const {
    ptrHandling< QDomNode > tmpNodeP;

    QDomElement partElement = getElement("part", label);
    DTLOGCHAP(creating part);
    DTINFOWF(createMachinePart(), << convertToString(partElement) );

    //
    // construct via builder
    //
    vectorHandling< analyticGeometry * > tmpAGeo;
    QDomElement tE = getChild("builder", partElement);
		QDomElement * tEP = &tE;

    //
    // create builder
    //
		std::string builderName = getAttributeStr("name", tE);
		bool buildCompound = false;
		if ( stringPrimitive::stringContains("[]", builderName) ) {
			buildCompound = true;
			stringPrimitive::getStringBetweenAndRemove("[", "]", &builderName);
		}
		ptrHandling< dtXmlParserDecorator > decoP (
			dtXmlParserDecoratorFactory::create( builderName )
		);
		
		if (!buildCompound) {
			decoP->buildPart(&tEP, pCP, vCP, cVP, sFP, aGP, &tmpAGeo);
		}
		else {
			decoP->buildPartCompound(&tEP, pCP, vCP, cVP, sFP, aGP, &tmpAGeo);
		}

    for (int ii=0;ii<tmpAGeo.size();ii++) {
//      //
//      // set render attributes
//      //
//      if ( partElement.hasAttribute("render_color") ) {
//        tmpAGeo[ii]->setAttribute( "COLOR", getAttributeStr("render_color", partElement));
//      }    
//      if ( partElement.hasAttribute("render_transparency") ) {
//        tmpAGeo[ii]->setAttribute( "TRANSPARENCY", getAttributeStr("render_transparency", partElement));
//      }      
//      if ( partElement.hasAttribute("render_material") ) {
//        tmpAGeo[ii]->setAttribute( "MATERIAL", getAttributeStr("render_material", partElement));
//      }  
			
      if (label != "*") {
        if ( tmpAGeo.size() > 1  ) {
          tmpAGeo[ii]->setLabel(label+"_"+intToString(ii) );
        }
        else {
          tmpAGeo[ii]->setLabel(label);
        }
        aGP->push_back( tmpAGeo[ii] );				
      }
      else {
				//
				// replace in vector
				//
        int pos = aGP->getPosition( tmpAGeo[ii]->getLabel() );
        delete aGP->at(pos);
        aGP->at(pos) = tmpAGeo[ii];
      }
    }
    tmpAGeo.clear();
  }

  void dtXmlParser::createAnalyticGeometry(
    pointContainer * const pCP,
		vectorContainer * const vCP,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > * aGP
	) const {
		std::vector< std::string > label = getNames("part");
		
		dt__FORALL( label, ii, createAnalyticGeometry(label[ii], pCP, vCP, cVP, sFP, aGP); );
  }
	
  void dtXmlParser::createBoundedVolume(
		std::string const label,
	  pointContainer * const pCP,
		vectorContainer * const vCP,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
	  vectorHandling< boundedVolume * > * bVP
	) const {
		//
		// get configuration element
		//
		QDomElement wEl = getElement("boundedVolume", label);

		//
		// create new boundedVolume with factory
		//
		bVP->push_back( 
			boundedVolumeFactory::create( getAttributeStr("name", wEl) ) 
		);
		//
		// initialize boundedVolume
		//
		bVP->back()->init( wEl, cVP, sFP, aGP, bVP );
  }
	
  void dtXmlParser::createBoundedVolume(
    pointContainer * const pCP,
		vectorContainer * const vCP,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP
	) const {
		std::vector< std::string > label = getNames("boundedVolume");
		
		dt__FORALL( label, ii, createBoundedVolume(label[ii], pCP, vCP, cVP, sFP, aGP, bVP); );
  }
	
	void dtXmlParser::destroyAndCreate(
		vectorHandling< constValue * > & cV,
		vectorHandling< analyticFunction* > & aF,
		ptrHandling< pointContainer > & pC,
		ptrHandling< vectorContainer > & vC,        
		vectorHandling< analyticGeometry * > & aG,
		vectorHandling< boundedVolume * > & bV
	) const {
		if ( cV.size() == 0 ) {
			createConstValue(&cV);
		}
		
		//
		// destroy
		//
		pC.reset( new pointContainer() );
		vC.reset( new vectorContainer() );
		aF.destroy();
		aG.destroy();
		bV.destroy();

		//
		// create
		//
		createAnalyticFunction(&cV, &aF);
		createAnalyticGeometry(pC.get(), vC.get(), &cV, &aF, &aG);
		createBoundedVolume(pC.get(), vC.get(), &cV, &aF, &aG, &bV);
	}
  
  void dtXmlParser::setStaticProperties( void ) {
    for (int ii=0; ii<_rootRead.size(); ii++) {
			if ( hasChild("staticProperties", _rootRead[ii]) ) {
		    QDomElement el = getElement("staticProperties");
				staticPropertiesHandler::getInstance()->init( &el );
				return;
			}
    }
  }
}
