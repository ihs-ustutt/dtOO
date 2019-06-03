#include "dtXmlParser.h"
#include <interfaceHeaven/systemHandling.h>

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/muParserFunctions.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <boundedVolumeFactory.h>
#include <baseContainerHeaven/baseContainer.h>
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
  dtXmlParser * dtXmlParser::_pH = NULL;
    
  dtXmlParser * dtXmlParser::init(
    std::string const & inFile, std::string const & outFile
  ) {
    if (_pH) delete _pH;
    
    _pH = new dtXmlParser(inFile, outFile);
    
    return _pH;
  }
  
  dtXmlParser * dtXmlParser::reinit( void ) {
    dt__throwIf(_pH==NULL, reinit() );
    
    return init( _pH->_inFile, _pH->_outFile );
  }
  
  dtXmlParser * dtXmlParser::ptr( void ) {
    dt__throwIf(!_pH, ptr());
    return _pH;
  }
  
  dtXmlParser const & dtXmlParser::constReference( void ) {
    dt__throwIf( !_pH, constReference() );
    return *_pH;
  }

  dtXmlParser & dtXmlParser::reference( void ) {
    dt__throwIf( !_pH, reference() );
    return *_pH;  
  }
  
  dtXmlParser::dtXmlParser(
    std::string const & inFile, std::string const & outFile
  ) : _inFile(inFile), _outFile(outFile) {
    _currentState = "";
  }

  dtXmlParser::~dtXmlParser() {
		dt__forAllIndex( _pH->_rootRead, ii) _pH->_rootRead[ii].clear();
		_pH->_rootRead.clear();
		dt__forAllIndex(_pH->_rootReadDoc, ii) _pH->_rootReadDoc[ii].clear();
		_pH->_rootReadDoc.clear();		
		_pH->_rootLoad.clear();
		_pH->_rootLoadDoc.clear();
    _pH->_currentState = "";
    _pH = NULL;
  }
  
  void dtXmlParser::parse( void ) {
    parseFile(_inFile.c_str());
  }
  
  void dtXmlParser::parseFile(char const * const fileName) {
    QFile xmlFile(fileName);
    QDomDocument xmlDocument;

		//
    // open file
		//
    if( !xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      dt__throw(parse, << "Failed to open " << dt__eval(fileName));
    }

    //
    // read file
    //
    QString qString;
    int line;
    int column;
    dt__throwIfWithMessage(
      !xmlDocument.setContent( &xmlFile, &qString, &line, &column ),
      parse(),
      << dt__eval(qPrintable(qString) ) << std::endl
      << dt__eval(line) << std::endl
      << dt__eval(column) << std::endl    
      << "Failed to parse file " << fileName
    );

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

    dt__throwIfWithMessage(
      _rootRead.size()==0,
      parse(), 
      << "Error parsing file " << fileName << std::endl
      << dt__eval(_rootRead.size()) 
    );
		
	  //
    // handle constant expressions
    //
    dt__forAllRefAuto( 
      getChildVector("constant", _rootRead.back()), aConstant 
    ) {
      dt__forAllRefAuto(getSiblingVector(aConstant), aSib) {
        replaceRecursiveInAllAttributes(
          getAttributeStr("label", aConstant), 
          getAttributeStr("value", aConstant), 
          &aSib
        );
      }
    }
    
    //
    // get labels of includes
    //
		std::vector< std::string > label = getLabels("include");
    
		//
		// handle includes
		//
		// get labels and store filename in label
		// and remove include nodes    
    if ( !label.empty() ) {
      logContainer< dtXmlParser > logC(logINFO, "parse()");
      logC() 
        << "include files:" 
        << std::endl
        << logMe::dtFormat("[ %23s ] -> %s") % "<label>" % "<filename> "
        << std::endl;
      for (int ii=0; ii<label.size(); ii++) {
        ::QDomElement wElement = getElement("include", label[ii]);
        logC() 
          << logMe::dtFormat("[ %23s ] -> %s") 
            % label[ii] % getAttributeStr("filename", wElement) 
          << std::endl;
        label[ii] = getAttributeStr("filename", wElement);
        ::QDomNode check 
        = 
        _rootRead.back().removeChild( static_cast<::QDomNode>(wElement) );
        dt__throwIf(check.isNull(), parse());
      }
    }

    //
    // parse included files
    //
    for (int ii=0; ii<label.size(); ii++) this->parseFile( label[ii].c_str() );
    
    
		//
		// handle replace-elements
		//
    dt__forAllRefAuto( getElementRecursive("replace"), forElement ) {
			//
			// parse variable and values to replace
			//
			std::string var = getAttributeStr("variable", forElement);
			  std::string valueStr = getAttributeStr("values", forElement);
			
			//
			// replace
			//
      dt__forAllRefAuto(convertToStringVector("{", "}", valueStr), aValue) {
        dt__forAllRefAuto(getChildVector(forElement), aChild) {
				  ::QDomElement childClone = aChild.cloneNode(true).toElement();
					replaceRecursiveInAllAttributes( "{"+var+"}", aValue, &(childClone) );
					::QDomNode checkOne 
					=							
          forElement.parentNode().insertBefore( 
						static_cast<::QDomNode>(childClone),
						static_cast<::QDomNode>(forElement) 
					);
				  dt__throwIf(checkOne.isNull(), parse());
				}
			}
			
			//
			// delete dummy node
			//
			::QDomNode check 
			= 
			forElement.parentNode().removeChild( static_cast<::QDomNode>(forElement) );
		  dt__throwIf(check.isNull(), parse());
	  }
    
		//
		// handle vectorReplace-elements
		//
    dt__forAllRefAuto( getElementRecursive("vectorReplace"), forElement ) {
			//
			// parse variable and values to replace
			//
			std::vector< std::string > varStr
      = 
      getAttributeStrVector("variables", forElement);
			std::vector< std::string > valueStr 
      = 
      getAttributeStrVector("values", forElement);
			
			//
			// replace
			//
      int iiInd = 0;
      dt__forAllRefAuto(valueStr, aValueVec) {
        dt__debug(
          parse(),
          << "vectorReplace[ " << iiInd << " ] = " << aValueVec
        );
        dt__forAllRefAuto(getChildVector(forElement), aChild) {
          ::QDomElement childClone = aChild.cloneNode(true).toElement();
          
          int jjInd = 0;          
          dt__forAllRefAuto(convertToStringVector("{", "}", aValueVec), aValue) {            
            std::string var = varStr[jjInd];

            dt__debug(
              parse(),
              << "vectorReplace[  " << iiInd << " ] " << var << " < with > " 
              << aValue << " <."
            );            
            
            replaceRecursiveInAllAttributes( "{"+var+"}", aValue, &(childClone) );
            
            jjInd = jjInd + 1;
          }
          ::QDomNode checkOne 
          =							          
          forElement.parentNode().insertBefore( 
            static_cast<::QDomNode>(childClone),
            static_cast<::QDomNode>(forElement) 
          );
          dt__throwIf(checkOne.isNull(), parse());
                    
        }
        iiInd = iiInd + 1;
      }
			
			//
			// delete dummy node
			//
			::QDomNode check 
			= 
			forElement.parentNode().removeChild( static_cast<::QDomNode>(forElement) );
		  dt__throwIf(check.isNull(), parse());
	  }    
		
		//
    // set static properties on classes
    //
    setStaticProperties();
  }

  void dtXmlParser::load( void ) const {
    _rootLoad.clear();
    _rootLoadDoc.clear();
    //
    // check and initialize file
    //
    checkFile(_outFile.c_str(), _rootLoadDoc);
    
		//
		// save ::QDomNode and QDomDocument because owner after cloning 
		// remains the same
		//		
    _rootLoad = _rootLoadDoc.documentElement().cloneNode(true).toElement(); 
  }

  bool dtXmlParser::hasState( std::string const stateName ) const {    
    return hasChildElement("state", stateName, _rootLoad);      
  }
  
  
  void dtXmlParser::loadStateToConst(
    std::string const stateName, cVPtrVec & cValRef
  ) {
    dt__throwIf(cValRef.empty(), loadStateToConst());
    
    ::QDomElement stateElement 
    = 
    getChildElement("state", stateName, _rootLoad);      
    
    dt__forAllIndex(cValRef, ii) {
      if ( 
        hasChildElement("constValue", cValRef[ii]->getLabel(), stateElement) 
      ) {
        if ( cValRef[ii]->loadable() ) {
          cValRef[ii]->setValue(
            getAttributeFloat(
              "value",
              getChildElement(
                "constValue", cValRef[ii]->getLabel(), stateElement
              )
            )
          );
        }
        else {
          dt__warning(
            loadStateToConst(),
            << "Defer not loadable constValue > " 
            << cValRef[ii]->getLabel() << " <."
          );
        }
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
    
    //
    // resolve constraints
    //
    dt__forAllRefAuto(cValRef, aCV) aCV->resolveConstraint( &cValRef );    
    
    //
    // store this state name in parser
    //
    _currentState = stateName;
    dt__info(loadStateToConst(), << "Current working state: " << _currentState);
  }
   
  std::vector< std::string > dtXmlParser::getStates(void) const {
    dt__throwIf( _rootLoad.isNull(), getStates() );
      
    std::vector< std::string > labels;
    getChildLabels("state", &labels, _rootLoad);
    return labels;
  }
    
  void dtXmlParser::write( 
    cVPtrVec const * const cValP 
  ) const {
    write(NowDateAndTime(), cValP);
  }

  void dtXmlParser::write(
    std::string const stateName, 
    cVPtrVec const * const cValP
  ) const {
    //
    // filename
    //
    char const * const fileName = _outFile.c_str();
       
    ::QDomElement newState = createElement(_rootLoadDoc, "state");
    newState.setAttribute("label", stateName.c_str());
    appendChildElement(_rootLoadDoc, newState);
    
    dt__forAllRefAuto(*cValP, aCV) aCV->writeToElement(_rootLoadDoc, newState);
    
    //
    // write file
    //
    writeFile(fileName, _rootLoadDoc);
    
    //
    // update rootLoad and rootLoadDoc
    //
    load();
    
    //
    // set state label
    //
    setState(stateName);
    
    dt__info(
      write(),
      << "Save state = " << stateName << " to file = " << fileName << std::endl
      << dt__eval(currentState())
    );
  }

  void dtXmlParser::write(
    std::vector< std::string > const stateName, 
    std::vector< cVPtrVec > const & cValP
  ) const {
    //
    // filename
    //
    char const * const fileName = _outFile.c_str();
       
    dt__forAllIndex(stateName, ii) {
      ::QDomElement newState = createElement(_rootLoadDoc, "state");
      newState.setAttribute("label", stateName[ii].c_str());
      appendChildElement(_rootLoadDoc, newState);    
      dt__forAllRefAuto(cValP[ii], aCV) aCV->writeToElement(_rootLoadDoc, newState);
    }
    
    //
    // write file
    //
    writeFile(fileName, _rootLoadDoc);
    
    //
    // update rootLoad and rootLoadDoc
    //
    load();
    
    //
    // set state label
    //
    setState(stateName.back());
    
    dt__info(
      write(),
      << "Save " << stateName.size() << " states  to file = " 
      << fileName << std::endl
      << dt__eval(currentState())
    );
  }

  void dtXmlParser::write( 
    cVPtrVec const & cValP 
  ) const {
    write(&cValP);
  }
  
  void dtXmlParser::write(
    std::string const stateName, 
    cVPtrVec const & cValP
  ) const {
    write(stateName, &cValP);
  }

  void dtXmlParser::writeUpdate(
    std::string const stateName, 
    cVPtrVec const & cValP
  ) const {
    //
    // filename
    //
    char const * const fileName = _outFile.c_str();
    
    //
    // remove old state element
    //
    ::QDomElement toRemove
    =
    getChildElement("state", stateName, _rootLoadDoc.documentElement());
    ::QDomNode removedNode = toRemove.parentNode().removeChild( toRemove );
    dt__throwIf( removedNode.isNull(), writeUpdate() );
    removedNode.clear();
    
    //
    // update state element by create a new one
    //
    ::QDomElement newState = createElement(_rootLoadDoc, "state");
    newState.setAttribute("label", stateName.c_str());
    appendChildElement(_rootLoadDoc, newState);
    
    //
    // write constValues to QDomDocument
    //
    dt__forAllRefAuto(cValP, aCV) aCV->writeToElement(_rootLoadDoc, newState);
    
    //
    // write file
    //
    writeFile(fileName, _rootLoadDoc);
    
    //
    // update rootLoad and rootLoadDoc
    //
    load();
    
    //
    // set state label
    //
    setState(stateName);
    
    dt__info(
      writeUpdate(),
      << "Update state = " << stateName 
      << " to file = " << fileName << std::endl
      << dt__eval(currentState())
    );
  }  
  
  void dtXmlParser::extract(
    std::string const stateName, 
    cVPtrVec const & cValP,
    std::string const fileName
  ) const {
    ::QDomDocument doc;

    checkFile(fileName.c_str(), doc);
    
    ::QDomElement newState = createElement(doc, "state");
    newState.setAttribute("label", stateName.c_str());
    appendChildElement(doc, newState);
    
    dt__forAllRefAuto(cValP, aCV) aCV->writeToElement(doc, newState);
    
    //
    // write file
    //
    writeFile(fileName, doc);

    dt__info(
      extract(),
      << "Extract state = " << stateName << " to separate file = " << fileName
    );
  }
  
  void dtXmlParser::extractAll( cVPtrVec & cValP ) {
    dt__forAllRefAuto( getStates(), aState) {
      loadStateToConst(aState, cValP);
      extract(aState, cValP, aState+".xml");
    }
  }  

  void dtXmlParser::remove( std::string const stateName ) const {
    //
    // filename
    //
    char const * const fileName = _outFile.c_str();
    
    //
    // remove old state element
    //
    ::QDomElement toRemove
    =
    getChildElement("state", stateName, _rootLoadDoc.documentElement());
    ::QDomNode removedNode = toRemove.parentNode().removeChild( toRemove );
    dt__throwIf( removedNode.isNull(), writeUpdate() );
    removedNode.clear();
    
    //
    // write file
    //
    writeFile(fileName, _rootLoadDoc);
    
    //
    // update rootLoad and rootLoadDoc
    //
    load();

    dt__info(
      writeUpdate(),
      << "Remove state = " << stateName << " in file = " << fileName
    );
  }    
  
  void dtXmlParser::writeFile(
    std::string const & fileName, QDomDocument & xmlDocument
  ) {
    //
    // backup file if already exists
    //
    if ( systemHandling::fileExists(fileName) ) {
      systemHandling::renameFile(fileName, fileName+".backup");
    }
    
    //
    // create and open file
    //
    ::QFile xmlFile(fileName.c_str());
    dt__throwIf( 
      !xmlFile.open(QIODevice::WriteOnly | QIODevice::Text), writeFile() 
    );
    
    ::QTextStream(&xmlFile) << xmlDocument.toString();

    //
    // close file
    //
    xmlFile.close();    
  }
  	
  void dtXmlParser::checkFile(
    char const * const fileName, QDomDocument & xmlDocument
  ) const {
    //
    // open file for reading
    //
    QFile xmlFile(fileName);
    
    //
    // check if file exists, if not create
    //
    if ( !xmlFile.exists() ) {
      dt__warning(
        checkFile,
        << "File " << dt__eval(fileName) 
        << " does not exist. Try to create file."
      );
      xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
      xmlFile.close();
    }
    
    //
    // open file
    //
    dt__throwIfWithMessage( 
      !xmlFile.open(QIODevice::ReadWrite | QIODevice::Text), 
      checkFile(), 
      << "Failed to open " << dt__eval(fileName) 
    );

    //
    // try to read file and check if it is correct
    //
    QString qString;
    int line;
    int column;
    if( !xmlDocument.setContent( &xmlFile, &qString, &line, &column ) ) {
      dt__warning(
        checkFile(),
        << dt__eval(qPrintable(qString) ) << std::endl
        << dt__eval(line) << std::endl
        << dt__eval(column) << std::endl    
        << "Failed to parse file " << fileName << std::endl
        << "Recreate file."
      );
           
      //
      // initialize new file
      //
      xmlDocument = ::QDomDocument();
      // encoding
      xmlDocument.appendChild(
        xmlDocument.createProcessingInstruction( 
          "xml","version=\"1.0\" encoding=\"ISO-8859-1\"" 
        )
      );
      // machine name
      xmlDocument.appendChild(
        xmlDocument.createElement( _rootRead[0].tagName() )
      );
      // write to file
      ::QTextStream( &xmlFile ) << xmlDocument.toString();
    }
    else {
      // check if root element has correct name
      if ( 
        xmlDocument.documentElement().tagName() 
        == 
        _rootRead[0].tagName() 
      ) {
        dt__info( checkFile, << "Correct root Element name. Write to file." );
      }
      //
      // rootReadElement and rootWriteElement have not same root name
      //
      else {
        //
        // close file
        //
        xmlFile.close();
        
        dt__throw(
          checkFile(), 
          << "Incorrect root element name." << std::endl
          << "Root element name of file           : " 
          << getTagName( xmlDocument.documentElement() ) << std::endl
          << "Root element name of current parser : " 
          << getTagName(_rootRead[0])
        );
      }
    }
    
    //
    // close file
    //
    xmlFile.close();    
  }
  
  std::string dtXmlParser::currentState( void ) const {
    return _currentState;
  }
  
  void dtXmlParser::setState( std::string const & newState) const {
    _currentState = newState;
  }
  
  void dtXmlParser::freeState( void ) const {
    _currentState = "";
  }

  bool dtXmlParser::stateLoaded( void ) const {
    if (_currentState == "") return false;
    else return true;
  }  
  
  void dtXmlParser::createConstValue(
    cVPtrVec * cValP
  ) const {
    //
    // create const values
    //
		dt__forAllRefAuto( getLabels("constValue"), label) {
     // createConstValue(label[ii], cValP);
      ::QDomElement const wElement = getElement("constValue", label);

      constValue * aCV 
      = 
      constValue::create( 
        getAttributeStr("name", wElement), 
        label, 
        getAttributeStr("value", wElement),
        getIfAttributeBool("loadable", wElement, true)
      );

      if ( hasAttribute("min", wElement) && hasAttribute("max", wElement) ) {
        aCV->setRange( 
          muParseString( getAttributeStr( "min", wElement ) ), 
          muParseString( getAttributeStr( "max", wElement ) ) 
        );      
      }

      cValP->push_back( aCV );       
    }
       
    //
    // resolve constraints
    //
    dt__forAllRefAuto(*cValP, aCV) aCV->resolveConstraint( cValP );
    
    //
    // init values
    //
    dt__forAllRefAuto( *cValP, aCV) {
      aCV->setValue(
        getAttributeFloatMuParse( 
          "value", 
          getElement("constValue", aCV->getLabel()), cValP 
        )
      );         
    }
    
    //
    // load from state
    //
    ::QDomElement cVInit = getUnlabeledElement("constValueInit");
    if ( !cVInit.isNull() ) {
      dt__info(
        createConstValue(), 
        << "Load init state " << getAttributeStr("state", cVInit)
      );
      dtXmlParser::reference().load();
      
      if ( 
        dtXmlParser::reference().hasState( getAttributeStr("state", cVInit) )
      ) {
        dtXmlParser::reference().loadStateToConst( 
          getAttributeStr("state", cVInit), *cValP
        );
      }
      else {
        dt__warning(
          createConstValue(), 
          << "Parser does not contain state = " 
          << getAttributeStr("state", cVInit)
        );        
      }
    }    
  }
	
  void dtXmlParser::createConstValue(
    cVPtrVec & cValRef
  ) const {
    createConstValue(&cValRef);
  }
  void dtXmlParser::getLabels( 
    std::string lookType, std::vector< std::string > * names 
  ) const {
		dt__throwIfWithMessage(
      _rootRead.size()==0, getLabels(), << "No XML file parsed."
    );

    dt__forAllIndex(_rootRead, ii) {
      dt__throwIf(_rootRead[ii].isNull(), getLabel());
      
      std::vector< std::string > locNames;
      getChildLabels(lookType, &locNames, _rootRead[ii]);
      dt__forAllIndex(locNames, jj) names->push_back(locNames[jj]);
    }
  }
	
  std::vector< std::string > dtXmlParser::getLabels( 
    std::string lookType 
  ) const {
		std::vector< std::string > names;
		getLabels(lookType, &names);
		return names;
  }	

  void dtXmlParser::getLabel( std::string lookType, std::string * name ) const {
    std::vector< std::string > names;
    
    getLabels(lookType, &names);
    
    if (names.size() != 1) {
      dt__warning(
        getLabel(),
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

    getLabels(lookType, &label);

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

	std::vector< ::QDomElement > dtXmlParser::getElementRecursive( 
    std::string const lookType 
  ) const {
    std::vector< ::QDomElement > retVec(0);
		dt__forAllRefAuto(_rootRead, aRoot) {
      dt__forAllRefAuto(getDescendantVector(lookType, aRoot), anEl) {
        retVec.push_back( anEl );          
      }
		}
		return retVec;
  }  
	
  void dtXmlParser::createAnalyticFunction(
	  std::string const label, 
		baseContainer * const bC,
    cVPtrVec const * const cVP, 
    aFPtrVec * aFP
	) const {
    ::QDomElement aFElement = getElement("function", label);
    
    //
    // log
    //
    dt__makeChapter(creating function);
    dt__info(createAnalyticFunction(), << convertToString(aFElement) );
    

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
    aFPtrVec tmpAF;   
		if (!buildCompound) builder->buildPart(tE, bC, cVP, aFP, &tmpAF);
		else builder->buildPartCompound(tE, bC, cVP, aFP, &tmpAF);

    logContainer< dtXmlParser > logC(logINFO, "createAnalyticFunction()");
    
    //
    // no labeling necessary
    //      
    if ( tmpAF.empty() ) return;
    
    //
    // vector of labels
    //
    if ( stringPrimitive::stringContains("{", label) ) {
      std::vector< std::string > labelVector
      =
      stringPrimitive::convertToStringVector("{", "}", label);
      std::string baseName = labelVector[0];
      
      logC() 
        << "label = " << label << std::endl
        << "baseName = " << baseName << std::endl;
      
      int counterLabel = 0;
      dt__forAllIndex(tmpAF, ii) {
        if ( !labelVector.empty() ) {
          tmpAF[ii]->setLabel( labelVector.front() );
          logC() << "setLabel( " << labelVector.front() << " )" << std::endl;
          labelVector.erase( labelVector.begin() );
        }
        else {
          tmpAF[ii]->setLabel( baseName+"_"+intToString(counterLabel) );            
          logC() 
            << "setLabel( " 
            << baseName+"_"+intToString(counterLabel) 
            << " )" 
            << std::endl;          
          counterLabel++;
        }
        aFP->push_back( tmpAF[ii] );				
      }
    }
    //
    // single label
    //
    else {
      if ( tmpAF.size() > 1  ) {
        dt__forAllIndex(tmpAF, ii) {
          logC() 
            << "setLabel( " 
            << label+"_"+intToString(ii) 
            << " )" 
            << std::endl;           
          tmpAF[ii]->setLabel( label+"_"+intToString(ii) );
          aFP->push_back( tmpAF[ii] );            
        }
      }
      else {
        tmpAF[0]->setLabel(label);
        aFP->push_back( tmpAF[0] );
      }
    }   
  }	
	
  void dtXmlParser::createAnalyticFunction(
	  baseContainer * const bC,
    cVPtrVec const * const cVP, 
    aFPtrVec * aFP
	) const {
    //
    // registrate analytic functions on muparser
    //
    muParserFunctions::instance()->registrateAnalyticFunctions( aFP );
    
		std::vector< std::string > label = getLabels("function");
		
		dt__forAllIndex(label, ii) createAnalyticFunction(label[ii], bC, cVP, aFP);
		
		aFP->dump();
  }		

	/**
	 * 
   * @todo: Attribute setting?
   */
  void dtXmlParser::createAnalyticGeometry(
	  std::string const label,
    baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec * aGP
	) const {
    ::QDomElement partElement = getElement("part", label);
    dt__makeChapter(creating part);
    dt__info(createMachinePart(), << convertToString(partElement) );

    //
    // construct via builder
    //
    aGPtrVec tmpAGeo;
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
      if ( !isWildcard( label ) ) {
        if ( tmpAGeo.size() > 1  ) {
          tmpAGeo[ii]->setLabel(label+"_"+intToString(ii) );
        }
        else {
          tmpAGeo[ii]->setLabel(label);
        }
        aGP->push_back( tmpAGeo[ii] );				
      }
      else {
        if ( !isFullWildcard(label) ) {
          //
          // modify label and push back to vector
          //          
          tmpAGeo[ii]->setLabel(
            stringPrimitive::replaceStringInString(
              "*", tmpAGeo[ii]->getLabel(), label
            )
          );
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
    }
    tmpAGeo.clear();
  }

  void dtXmlParser::createAnalyticGeometry(
    baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec * aGP
	) const {
		std::vector< std::string > label = getLabels("part");
		
		dt__forAllIndex(label, ii) {
      createAnalyticGeometry(label[ii], bC, cVP, sFP, aGP);
    }
		
    if ( staticPropertiesHandler::getInstance()->optionTrue("geometry_sort") ) {
      aGP->sort();
    }
    
		dt__forAllIndex(*aGP, ii) aGP->at(ii)->dump();
    
    aGP->dump();
  }
	
  void dtXmlParser::createBoundedVolume(
		std::string const label,
    baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
	  bVPtrVec * bVP
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
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
		bVPtrVec * bVP
	) const {
		std::vector< std::string > label = getLabels("boundedVolume");
		
		dt__forAllIndex(label, ii) {
      createBoundedVolume(label[ii], bC, cVP, sFP, aGP, bVP);
    }
  }

	void dtXmlParser::createPlugin(
		std::string const label,
		baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
		bVPtrVec * bVP,
    dCPtrVec const * const dCP,
		dPPtrVec * pLP
	) const {
		//
		// get configuration element
		//
		::QDomElement wEl = getElement("plugin", label);

		//
		// create new plugin with factory
		//
    if ( hasAttribute("pluginLibrary", wEl) ) {
      pLP->push_back( 
        dtPluginFactory::createFromPlugin( 
          getAttributeStr("name", wEl) ,
          getAttributeStr("pluginLibrary", wEl),
          getAttributeStr("pluginDriver", wEl)
        ) 
      ); 
    }
    else {
      pLP->push_back( 
        dtPluginFactory::create( getAttributeStr("name", wEl) ) 
      );
    }
    
		//
		// initialize dtPlugin
		//
		pLP->back()->init( wEl, bC, cVP, sFP, aGP, bVP, dCP, pLP );		
	} 
	
	void dtXmlParser::createPlugin(
		baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
		bVPtrVec * bVP,
    dCPtrVec const * const dCP,
		dPPtrVec * pLP
	) const {
		std::vector< std::string > label = getLabels("plugin");
		
		dt__forAllIndex(label, ii) {
      createPlugin(label[ii], bC, cVP, sFP, aGP, bVP, dCP, pLP);
    }
	}
		
	void dtXmlParser::createCase(
		std::string const label,
		baseContainer * const bC,
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
		bVPtrVec * bVP,
		dCPtrVec * dCP
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
		cVPtrVec const * const cVP,        
		aFPtrVec const * const sFP,        
		aGPtrVec const * const aGP,
		bVPtrVec * bVP,
		dCPtrVec * dCP
	) const {
		std::vector< std::string > label = getLabels("case");
		
		dt__forAllIndex(label, ii) {
      createCase(label[ii], bC, cVP, sFP, aGP, bVP, dCP);
    }
	}

	void dtXmlParser::destroyAndCreate(
    baseContainer & bC,
		cVPtrVec & cV,
		aFPtrVec & aF,
		aGPtrVec & aG,
		bVPtrVec & bV,
		dCPtrVec & dC,
	  dPPtrVec & pL
	) const {
		if ( cV.size() == 0 ) createConstValue(&cV);
		
		//
		// destroy
		//
		bC.clear();
		aF.destroy();
		aG.destroy();
		bV.destroy();
		dC.destroy();
    pL.destroy();
		
		//
		// create
		//
		createAnalyticFunction(&bC, &cV, &aF);
		createAnalyticGeometry(&bC, &cV, &aF, &aG);
		createBoundedVolume(&bC, &cV, &aF, &aG, &bV);
		createCase(&bC, &cV, &aF, &aG, &bV, &dC);
		createPlugin(&bC, &cV, &aF, &aG, &bV, &dC, &pL);
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
  
  void dtXmlParser::dump( void ) const {
    dt__info(
			dump(), 
			<< dt__eval(_inFile) << std::endl
		  << dt__eval(_outFile) << std::endl
			<< "_pH = " << _pH
		);
  }  
}
