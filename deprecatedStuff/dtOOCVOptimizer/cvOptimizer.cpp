#include "cvOptimizer.h"
#include <api/coCallFeedback.h>

#include <constValueHeaven/constValue.h>

#include <dtXmlParserDecorator/dtXmlParser.h>
#include <eoLib.h>
#include <interfaceHeaven/systemHandling.h>
#include <limits>

#define SUCCESS 0;
#define FAILURE 1;

namespace dtOO {
  cvOptimizer::cvOptimizer(int argc, char *argv[]) : coModule(argc, argv, "cvOptimizer") {   
    _pIn_constValueP = addInputPort("inConstValueP", "Vec3", "inConstValueDescription");
    _pIn_optiControlP = addInputPort("_pIn_optiControlP", "USR_Optim", "_pIn_optiControlPDescription");   
    _pIn_optiControlP->setRequired(0);
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");  
    _resValueChoiceParam = addChoiceParam("_resValueChoiceParam", "_resValueChoiceParamDescription");
    _p_startCycle = addBooleanParam("_p_startCycle", "_p_startCycleDescription");
    _initialized = false;
    _eoLib = new eoLib();
    _cVcCF = NULL;
  }

  void cvOptimizer::param(const char* paramName, bool inMapLoading) {   
    if ( strcmp(paramName, "_p_startCycle") == 0 ) {
      if ( _p_startCycle->getValue() ) {
        if (!_initialized) {
          DTWARNINGWF(param(),
                  << "Please initialize me first!");
          return;
        }
        _resValueChoiceParam->disable();
        _p_startCycle->disable();
        try {
          _eoLib->optimizeInterface( this );
          
          dt__FORALL(_cVal, ii,
            _cVcCF->setChoiceParam("constValueChoiceParam", _cVal[ii]->getLabel());
            _cVcCF->setFloatSliderParam( "constValueFloatSliderParam", _cVal[ii]->getValue() );
          );
        }
        catch (eGeneral & eGenRef) {
        _resValueChoiceParam->enable();
        _p_startCycle->enable();
        
          return;
        } 
        
        _p_startCycle->setValue(false); 
        _resValueChoiceParam->enable();
        _p_startCycle->enable();
        
        _cVcCF->executeModule();
      }
    }    
  }
  
  cvOptimizer::~cvOptimizer() {
    delete _eoLib;
  }
  
  int cvOptimizer::compute(const char * port) {
          
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
        

      const covise::coDistributedObject * inObj;
      covise::coDoSet const * set;
      //
      // recreate constValues
      //
      coDoSetHandling< constValue * > cValTmp;
      if ( _pIn_constValueP->isConnected() ) {
        inObj = _pIn_constValueP->getCurrentObject();
        set = dynamic_cast< covise::coDoSet const * >(inObj);
        abstractModule::recreateConstValues(set, cValTmp);
      }

      if (_cVcCF) {
        delete _cVcCF;
      }
      _cVcCF = new coCallFeedback();      
      _cVcCF->init(
            "INTERACTOR", 
            _pIn_constValueP->getCurrentObject()->getAttribute("INTERACTOR")
      );
        
      //
      // parse
      //
      dtXmlParser parser;
      parser.openFileAndParse( _p_xmlFilebrowser->getValue() );

      //
      //get cvOptimizer labels
      //
      std::string cvOptimizerLabel;
      parser.getName("cvOptimizer", &cvOptimizerLabel);

      QDomElement element = parser.getElement("cvOptimizer", cvOptimizerLabel);

      _cVal.destroy();
      QDomElement wElement = parser.getChild("constValue", element);
      while ( !wElement.isNull() ) {
        std::string label = parser.getAttributeStr("label", wElement);          
        _cVal.push_back( cValTmp.get(label)->clone() );

        //
        // go to next element
        //
        wElement = parser.getNextSibling("constValue", wElement);      
      }
      
//      optimizableInterface::removeFiles();

      
      if (_pIn_optiControlP->isConnected() ) {

        resValueMap rM = optimizableInterface::getResValuesFromFile();
        _fM = optimizableInterface::getFailedValuesFromFile();

        std::vector< std::string > labels;
        for ( resValueMap::const_iterator cIt = rM.begin(); cIt != rM.end(); ++cIt ) {
          labels.push_back( cIt->first );
        }
        abstractModule::updateChoiceParam(_resValueChoiceParam, &labels);
        
        _initialized = true;
      }
      else {
        _initialized = false;
      }
    }
    catch (eGeneral & eGenRef) {
      DTCATCHERRORWF(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }    

    return SUCCESS;
  }

  vectorHandling< constValue * > & cvOptimizer::getRefToParameters( void ) {
    return _cVal;
  }
  
  float cvOptimizer::characterizeMe( void ) const {
    if ( systemHandling().fileExists("killOptimizer") ) {
      systemHandling().deleteFile("killOptimizer");
      dt__THROW(characterizeMe(),
              << "User Abort.");
    }
    
    optimizableInterface::removeFiles();
        
    dt__FORALL(_cVal, ii,
      _cVcCF->setChoiceParam("constValueChoiceParam", _cVal[ii]->getLabel());
      _cVcCF->setFloatSliderParam( "constValueFloatSliderParam", _cVal[ii]->getValue() );
    );
    
    _cVcCF->executeModule();

    while ( !optimizableInterface::canRead() ) {

    }
    std::string aLabel( _resValueChoiceParam->getActLabel() );

    resValueMap rM = optimizableInterface::getResValuesFromFile();

    float resValue;
    try {
      resValue = rM.at(aLabel);
    }
    catch ( const std::out_of_range & oor ) {
      resValue = _fM.at(aLabel);
    }
    
    return resValue;

  }
  
  int cvOptimizer::getNParameters( void ) const {
    return _cVal.size();
  }
}
MODULE_MAIN(designTool, dtOO::cvOptimizer)