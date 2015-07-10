#include "dtXmlParser.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <boundedVolumeFactory.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValueFactory.h>
#include <xmlHeaven/aGXmlBuilder.h>
#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <xmlHeaven/aFXmlBuilder.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <dtPlugin.h>
#include <dtPluginFactory.h>
#include <dtCase.h>
#include <dtCaseFactory.h>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QTextStream>

namespace dtOO {
  dtXmlParser::dtXmlParser() {
  }

  dtXmlParser::~dtXmlParser() {
		dt__forAllIndex(_rootRead, ii) _rootRead[ii].clear();
		_rootRead.clear();
		dt__forAllIndex(_rootReadDoc, ii) _rootReadDoc[ii].clear();
		_rootReadDoc.clear();		
		_rootLoad.clear();
		_rootLoadDoc.clear();
  }

  void dtXmlParser::openFileAndParse(char const * const fileName) {
    QFile xmlFile(fileName);
    QDomDocument xmlDocument;

		//
    // open file
		//
    if( !xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      dt__throw(openFileAndParse, << "Failed to open " << dt__eval(fileName));
    }

    //
    // read file
    //
    QString qString;
    int line;
    int column;
    if( !xmlDocument.setContent( &xmlFile, &qString, &line, &column ) ) {
      dt__throw( openFileAndParse(),
              << dt__eval(qPrintable(qString) ) << std::endl
              << dt__eval(line) << std::endl
              << dt__eval(column) << std::endl    
              << "Failed to parse file " << fileName);
    }

    //
    // close file
    //
    xmlFile.close();

		_rootRead.push_back( 
		  xmlDocument.documentElement().cloneNode(true).toElement()
		);
		//
		// save QDomDocument because owner after cloning remains the same
		//
		_rootReadDoc.push_back( xmlDocument );

		if (_rootRead.size() == 0) {
			dt__throw(
			  openFileAndParse(), 
				<< "Error parsing file " << fileName << std::endl
				<< dt__eval(_rootRead.size()) );
		}
		
	
		//
		// handle includes
		//
		// get labels and store filename in label
		// and remove include nodes
		std::vector< std::string > label;
		getNames("include", &label);
		for (int ii=0; ii<label.size(); ii++) {
		  ::QDomElement wElement = getElement("include", label[ii]);
			dt__info( 
			  openFileAndParse(), 
				<< "Replace include " << dt__eval(label[ii]) << " with "
				<< dt__eval(getAttributeStr("filename", wElement)) 
			);
			label[ii] = getAttributeStr("filename", wElement);
			::QDomNode check 
			= 
			_rootRead.back().removeChild( static_cast<::QDomNode>(wElement) );
			if (check.isNull()) {
				dt__throw(openFileAndParse(), << "Error removing element.");
			}			
		}
    // parse included files
		for (int ii=0; ii<label.size(); ii++) {
			this->openFileAndParse( label[ii].c_str() );
		}
    
		//
		// handle replace-elements
		//
		::QDomElement forElement = getUnlabeledElement("replace");
		while ( !forElement.isNull() ) {
			//
			// parse variable and values to replace
			//
			std::string var = getAttributeStr("variable", forElement);
			std::string valueStr = getAttributeStr("values", forElement);
			std::vector< std::string > values 
      = 
      convertToStringVector("{", "}", valueStr);
			dt__info( 
			  openFileAndParse(), 
				<< "Replace element" << std::endl
				<< " > " << dt__eval(var) << std::endl
				<< " > " << dt__eval(valueStr)
			);			
			
			//
			// replace
			//
			std::vector<::QDomElement> children = getChildVector(forElement);
			for (int kk=0; kk<values.size(); kk++) {
  			for (int jj=0; jj<children.size(); jj++) {
				  ::QDomElement childClone = children[jj].cloneNode(true).toElement();
					replaceRecursiveInAllAttributes(
            "{"+var+"}", values[kk], &(childClone)
            );
					::QDomNode checkOne 
					=							
					_rootRead.back().insertBefore( 
						static_cast<::QDomNode>(childClone),
						static_cast<::QDomNode>(forElement) 
					);
					if (checkOne.isNull()) {
						dt__throw(openFileAndParse(), << "Error inserting element.");
					}
				}
			}
			
			//
			// delete dummy node
			//
			::QDomNode check 
			= 
			_rootRead.back().removeChild( static_cast<::QDomNode>(forElement) );
			if (check.isNull()) {
				dt__throw(openFileAndParse(), << "Error removing element.");
			}	

			//
			// get new replace element
			//
			forElement = getUnlabeledElement("replace");
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

		//
		// save ::QDomNode and QDomDocument because owner after cloning 
		// remains the same
		//		
    _rootLoad = xmlDocument.documentElement().cloneNode(true).toElement(); 
		_rootLoadDoc = xmlDocument;
  }
  
  void dtXmlParser::loadStateToConst(
    std::string const stateName, 
    vectorHandling< constValue * > & cValRef
  ) {  
    ::QDomElement stateElement = getChildElement( "state", stateName, _rootLoad );
    for (int ii=0;ii<cValRef.size();ii++) {
      if ( 
        hasChildElement("constValue", cValRef[ii]->getLabel(), stateElement) 
      ) {    
        ::QDomElement constValueElement = getChildElement("constValue",
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
        dt__warning(
          loadStateToConst(),
          << cValRef[ii]->getLabel() 
          << "-Element not in state file." << std::endl
          << "Leave value as it was before load state."
        );  
      }
    }
  }
  
  std::vector< std::string > dtXmlParser::getStates(void) {
    if (_rootLoad.isNull()) {
      dt__throw(getStates, << dt__eval(_rootLoad.isNull()) );
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
    ::QDomElement newState = createElement(xmlDocument, "state");
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
    dt__info(openFileAndWrite,
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
    ::QDomElement newState = createElement(xmlDocument, "state");
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
    dt__info(openFileAndWrite,
            << "Save state = " << stateName << " to file = " << fileName );
  }
  
  void dtXmlParser::writeFile( 
    char const * const fileName, QDomDocument & xmlDocument
  ) {
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
  
  void dtXmlParser::writeFile( 
    std::string const & fileName, ::QDomNode const & node
  ) {
    //
    // open file and write rootWriteElement to file
    //
		QDomDocument xmlDocument;
    QDomProcessingInstruction myHeader 
		= 
		xmlDocument.createProcessingInstruction( 
		  "xml","version=\"1.0\" encoding=\"ISO-8859-1\"" 
		);
    xmlDocument.appendChild( myHeader );		
		xmlDocument.appendChild(node);
    QFile xmlFile(fileName.c_str());
    xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&xmlFile);
    stream << xmlDocument.toString();

    //
    // close file
    //
    xmlFile.close();    
  }
	
  void dtXmlParser::checkFile(
    char const * const fileName, QDomDocument & xmlDocument
  ) {
    //
    // open file for reading
    //
    QFile xmlFile(fileName);
    
    //
    // check if file exists, if not create
    //
    if ( !xmlFile.exists() ) {
      dt__warning(checkFile,
              << "Failed to open " << dt__eval(fileName) << std::endl
              << "File does not exist. Try to create file.");
      xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
      xmlFile.close();
    }
    
    //
    //open file
    //
    if( !xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      dt__throw(checkFile,
              << "Failed to open " << dt__eval(fileName));
    }

    // try to read file and check if it is correct
    QString qString;
    int line;
    int column;
    //
    // parsing error
    //
    if( !xmlDocument.setContent( &xmlFile, &qString, &line, &column ) ) {
      dt__warning( checkFile(),
              << dt__eval(qPrintable(qString) ) << std::endl
              << dt__eval(line) << std::endl
              << dt__eval(column) << std::endl    
              << "Failed to parse file " << fileName << std::endl
              << "Recreate file.");
      //
      // initialize new file
      //
      xmlDocument = QDomDocument();
      // encoding
      QDomProcessingInstruction myHeader 
      = 
      xmlDocument.createProcessingInstruction( 
        "xml","version=\"1.0\" encoding=\"ISO-8859-1\"" 
      );
      xmlDocument.appendChild( myHeader );
      // machine name of _rootReadElement
      ::QDomElement _rootWriteElement = xmlDocument.createElement(
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
        dt__info(
          checkFile, 
          << "Correct root Element name. Write to this file."
        );
      }
      //
      // rootReadElement and rootWriteElement have not same root name
      //
      else dt__throw(checkFile, << "Incorrect root Element name.");        
    }
    //
    // close file
    //
    xmlFile.close();    
  }

  void dtXmlParser::createConstValue(
    std::string const constValueLabel, 
    vectorHandling< constValue * > * cValP
  ) const {
    ::QDomElement const cVElement = getElement( "constValue", constValueLabel );


    /* ------------------------------------------------------------------------ */
    /* go to last builder */
    /* ------------------------------------------------------------------------ */
    ::QDomElement const wElement = goToLastBuilder(cVElement);

    constValue * aCValP;
    constValueFactory cValFac;

    aCValP = cValFac.create( getAttributeStr("name", wElement) );
    aCValP->setLabel( constValueLabel );
    //
    //check first child
    //should be a float
    //
    ::QDomElement* firstChild = new ::QDomElement( wElement.firstChildElement() );
    if (firstChild->tagName().toStdString() == "float" ) {
      if ( firstChild->hasAttribute("min") 
            && firstChild->hasAttribute("max")
            && firstChild->hasAttribute("value") ) {
        //
        // set value, min and max
        //
        aCValP->setValue( 
          muParseString( getAttributeStr( "value", *firstChild ) ) 
        );
        aCValP->setRange( 
          muParseString( getAttributeStr( "min", *firstChild ) ), 
          muParseString( getAttributeStr( "max", *firstChild ) ) 
        );
      }
      else {
        dt__throw(
          createConstValue(),
          << "Missing attributes min, max or float"
        );
      }
    }
    else if (firstChild->tagName().toStdString() == "int" ) {
      if ( firstChild->hasAttribute("value") ) {
        //
        // set value, min and max
        //
        aCValP->setValue( 
          muParseString( getAttributeStr( "value", *firstChild ) ) 
        );
      }
      else {
        dt__throw(
          createConstValue(),
          << "Missing attributes min, max or float"
        );
      }
    }    
    else {
      dt__throw( 
        createConstValue(),
        << dt__eval( qPrintable(firstChild->tagName()) ) 
      );
    }

    cValP->push_back( aCValP );
  }
	
  void dtXmlParser::createConstValue(
    vectorHandling< constValue * > * cValP
  ) const {
    std::vector< std::string > label = getNames("constValue");

		dt__forAllIndex( label, ii) createConstValue(label[ii], cValP);
  }
	
  void dtXmlParser::getNames( 
    std::string lookType, std::vector< std::string > * names 
  ) const {
		if (_rootRead.size() != 0) {
			for (int ii=0; ii<_rootRead.size(); ii++) {
				if (_rootRead[ii].isNull()) {
					dt__throw(getNames(),
									<< dt__eval(_rootRead[ii].isNull()) << std::endl
									<< "Parsed file results in a NULL Pointer.");
				}
				std::vector< std::string > locNames;
				getChildLabels(lookType, &locNames, _rootRead[ii]);
				dt__forAllIndex(locNames, jj) names->push_back(locNames[jj]);
			}
		}
		else dt__throw(getNames(), << "Please parse a XML file.");
  }
	
  std::vector< std::string > dtXmlParser::getNames( 
    std::string lookType 
  ) const {
		std::vector< std::string > names;
		getNames(lookType, &names);
		return names;
  }	

  void dtXmlParser::getName( std::string lookType, std::string * name ) const {
    std::vector< std::string > names;
    
    getNames(lookType, &names);
    
    if (names.size() != 1) {
      dt__warning(
        getName(),
        << "More than one " << dt__eval(lookType) << " in file." << std::endl
        << "Taking the first one."
      );
    }
    
    *name = names[0];
  }  

  ::QDomElement dtXmlParser::getElement( 
    std::string const lookType, std::string const lookName 
  ) const {
		for (int ii=0; ii<_rootRead.size(); ii++) {
			if ( hasChildElement(lookType, lookName, _rootRead[ii]) ) {
				return getChildElement(lookType, lookName, _rootRead[ii]);
			}
		}
    dt__throw(
		  getElement(),
      << "No " << lookType << "-Element with " << dt__eval(lookName) 
		);
  }
	
  ::QDomElement dtXmlParser::getElement( std::string const lookType ) const {
    std::vector< std::string > label;

    getNames(lookType, &label);

    if (label.size() != 1) {
      dt__throw(getElement(),
              << dt__eval(label.size()) << std::endl
              << "Should be 1.");      
    }
    
    return ::QDomElement( getElement(lookType, label[0]) );
  }
  
	::QDomElement dtXmlParser::getUnlabeledElement( 
    std::string const lookType 
  ) const {
		for (int ii=0; ii<_rootRead.size(); ii++) {
			if ( hasChild(lookType, _rootRead[ii]) ) {
				return getChild(lookType, _rootRead[ii]);
			}
		}
		return ::QDomElement();
  }
	
  void dtXmlParser::createAnalyticFunction(
	  std::string const label, 
		baseContainer * const bC,
    vectorHandling< constValue * > const * const cVP, 
    vectorHandling< analyticFunction * > * sFP
	) const {
    ::QDomElement aFElement = getElement("function", label);
    dt__makeChapter(creating function);
    dt__info(createFunction(), << convertToString(aFElement) );
    

		::QDomElement tE = getChild("builder", aFElement);
		
		//
    // create builder
		//
		std::string builderName = getAttributeStr("name", tE);
		bool buildCompound = false;
		if ( stringPrimitive::stringContains("[]", builderName) ) {
			buildCompound = true;
			stringPrimitive::getStringBetweenAndRemove("[", "]", &builderName);
		}
		ptrHandling< aFXmlBuilder > builder(
  		aFXmlBuilderFactory::create(builderName)
		);

		//
		// call builder
		//
    vectorHandling< analyticFunction * > tmpAF;   
		if (!buildCompound) builder->buildPart(tE, bC, cVP, sFP, &tmpAF);
		else builder->buildPartCompound(tE, bC, cVP, sFP, &tmpAF);

    //
    // copy analyticFunctions from tmpSFun to sFP
    //
    for (int ii=0;ii<tmpAF.size();ii++) {			
      if (label != "*") {
        if ( tmpAF.size() > 1  ) {
          tmpAF[ii]->setLabel(label+"_"+intToString(ii) );
        }
        else {
          tmpAF[ii]->setLabel(label);
        }
        sFP->push_back( tmpAF[ii] );				
      }
      else {
				//
				// replace in vector
				//
        int pos = sFP->getPosition( tmpAF[ii]->getLabel() );
        delete sFP->at(pos);
        sFP->at(pos) = tmpAF[ii];
      }
    }
    tmpAF.clear();    
  }	
	
  void dtXmlParser::createAnalyticFunction(
	  baseContainer * const bC,
    vectorHandling< constValue * > const * const cVP, 
    vectorHandling< analyticFunction * > * sFP
	) const {
		std::vector< std::string > label = getNames("function");
		
		dt__forAllIndex(label, ii) createAnalyticFunction(label[ii], bC, cVP, sFP);
		
		sFP->dump();
  }		

	/**
	 * 
   * @todo: Attribute setting?
   */
  void dtXmlParser::createAnalyticGeometry(
	  std::string const label,
    baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > * aGP
	) const {
    ::QDomElement partElement = getElement("part", label);
    dt__makeChapter(creating part);
    dt__info(createMachinePart(), << convertToString(partElement) );

    //
    // construct via builder
    //
    vectorHandling< analyticGeometry * > tmpAGeo;
    ::QDomElement tE = getChild("builder", partElement);

    //
    // create builder
    //
		std::string builderName = getAttributeStr("name", tE);
		bool buildCompound = false;
		if ( stringPrimitive::stringContains("[]", builderName) ) {
			buildCompound = true;
			stringPrimitive::getStringBetweenAndRemove("[", "]", &builderName);
		}
		ptrHandling< aGXmlBuilder > builder (
			aGXmlBuilderFactory::create( builderName )
		);
		
		if (!buildCompound) builder->buildPart(tE, bC, cVP, sFP, aGP, &tmpAGeo);
		else builder->buildPartCompound(tE, bC, cVP, sFP, aGP, &tmpAGeo);

    for (int ii=0;ii<tmpAGeo.size();ii++) {			
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
    baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > * aGP
	) const {
		std::vector< std::string > label = getNames("part");
		
		dt__forAllIndex(label, ii) {
      createAnalyticGeometry(label[ii], bC, cVP, sFP, aGP);
    }
		
		dt__forAllIndex(*aGP, ii) aGP->at(ii)->dump();
    
    aGP->dump();
  }
	
  void dtXmlParser::createBoundedVolume(
		std::string const label,
    baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
	  vectorHandling< boundedVolume * > * bVP
	) const {
		//
		// get configuration element
		//
		::QDomElement wEl = getElement("boundedVolume", label);

		//
		// create new boundedVolume with factory
		//
		bVP->push_back( 
			boundedVolumeFactory::create( getAttributeStr("name", wEl) ) 
		);
		//
		// initialize boundedVolume
		//
		bVP->back()->init( wEl, bC, cVP, sFP, aGP, bVP );
  }
	
  void dtXmlParser::createBoundedVolume(
    baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP
	) const {
		std::vector< std::string > label = getNames("boundedVolume");
		
		dt__forAllIndex(label, ii) {
      createBoundedVolume(label[ii], bC, cVP, sFP, aGP, bVP);
    }
  }

	void dtXmlParser::createPlugin(
		std::string const label,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP,
		vectorHandling< dtPlugin * > * pLP
	) const {
		//
		// get configuration element
		//
		::QDomElement wEl = getElement("plugin", label);

		//
		// create new plugin with factory
		//
		pLP->push_back( 
			dtPluginFactory::create( getAttributeStr("name", wEl) ) 
		);
		//
		// initialize dtPlugin
		//
		pLP->back()->init( wEl, bC, cVP, sFP, aGP, bVP, pLP );		
	} 
	
	void dtXmlParser::createPlugin(
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP,
		vectorHandling< dtPlugin * > * pLP
	) const {
		std::vector< std::string > label = getNames("plugin");
		
		dt__forAllIndex(label, ii) {
      createPlugin(label[ii], bC, cVP, sFP, aGP, bVP, pLP);
    }
	}
		
	void dtXmlParser::createCase(
		std::string const label,
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP,
		vectorHandling< dtCase * > * dCP
	) const {
		//
		// get configuration element
		//
		::QDomElement wEl = getElement("case", label);

		//
		// create new plugin with factory
		//
		dCP->push_back( 
			dtCaseFactory::create( getAttributeStr("name", wEl) ) 
		);
		//
		// initialize dtPlugin
		//
		dCP->back()->init( wEl, bC, cVP, sFP, aGP, bVP, dCP );		
	}
	
	void dtXmlParser::createCase(
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cVP,        
		vectorHandling< analyticFunction * > const * const sFP,        
		vectorHandling< analyticGeometry * > const * const aGP,
		vectorHandling< boundedVolume * > * bVP,
		vectorHandling< dtCase * > * dCP
	) const {
		std::vector< std::string > label = getNames("case");
		
		dt__forAllIndex(label, ii) {
      createCase(label[ii], bC, cVP, sFP, aGP, bVP, dCP);
    }
	}
		
	void dtXmlParser::destroyAndCreate(
		vectorHandling< constValue * > & cV,
		vectorHandling< analyticFunction* > & aF,
		ptrHandling< baseContainer > & bC,
		vectorHandling< analyticGeometry * > & aG,
		vectorHandling< boundedVolume * > & bV,
		vectorHandling< dtCase * > & dC,
	  vectorHandling< dtPlugin * > & pL
	) const {
		if ( cV.size() == 0 ) {
			createConstValue(&cV);
		}
		
		//
		// destroy
		//
		bC.reset( new baseContainer() );
		aF.destroy();
		aG.destroy();
		bV.destroy();
		dC.destroy();
    pL.destroy();
		
		//
		// create
		//
		createAnalyticFunction(bC.get(), &cV, &aF);
		createAnalyticGeometry(bC.get(), &cV, &aF, &aG);
		createBoundedVolume(bC.get(), &cV, &aF, &aG, &bV);
		createCase(bC.get(), &cV, &aF, &aG, &bV, &dC);
		createPlugin(bC.get(), &cV, &aF, &aG, &bV, &pL);
	}
  
  void dtXmlParser::setStaticProperties( void ) {
    for (int ii=0; ii<_rootRead.size(); ii++) {
			if ( hasChild("staticProperties", _rootRead[ii]) ) {
		    ::QDomElement el = getElement("staticProperties");
				staticPropertiesHandler::getInstance()->init( &el );
				return;
			}
    }
  }
}
