#include "constValueGenerator.h"

#include <logMe/logMe.h>

#include <logMe/dtMacros.h>
#include <constValueHeaven/constValue.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <exceptionHandling/eGeneral.h>

#include <api/coFeedback.h>

#define SUCCESS 0;
#define FAILURE 1;

namespace dtOO { 
  constValueGenerator::constValueGenerator(
    int argc, char *argv[] ) : coModule(argc, argv, "constValueGenerator") {

    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");
    _p_xmlFilebrowser->disable();
    _p_parseXml = addBooleanParam("parseXml", "parseXmlDescription");
    _p_parseXml->setValue(false);
    _p_constValueChoiceParam = addChoiceParam("constValueChoiceParam", "constValueChoiceParamDescription");
    _p_constValueChoiceParam->disable();
    _p_constValueFloatSliderParam = addFloatSliderParam("constValueFloatSliderParam", "constValuefloatSliderParamDescription");
    _p_constValueFloatSliderParam->setValue(0., 2., 1.);
    _p_constValueFloatSliderParam->disable();
    _p_constValueIntParam = addInt32Param("constValueIntParam", "constValueIntParamDescription");
    _p_constValueIntParam->setValue(0.);
    _p_constValueIntParam->disable();
    _p_constValueSetChoiceParam = addChoiceParam("_p_constValueSetChoiceParam", "_p_constValueSetChoiceParamDescription");
    _p_constValueSetChoiceParam->disable();
    _p_constValueSetFloatSliderParam = addFloatSliderParam("_p_constValueSetFloatSliderParam", "_p_constValueSetFloatSliderParamDescription");
    _p_constValueSetFloatSliderParam->setValue(0., 2., 1.);
    _p_xmlStateFilebrowser = addFileBrowserParam("xmlSaveFilebrowser", "xmlSaveFilebrowserDescription");
    _p_xmlStateFilebrowser->setValue(".", "*.xml/*");
    _p_xmlStateFilebrowser->disable();
    _p_stateName = addStringParam("stateName", "stateNameDescription");
    _p_stateName->setValue("");
    _p_saveState = addBooleanParam("saveState", "saveStateDescription");
    _p_saveState->setValue(false);    
    _p_loadState = addBooleanParam("loadState", "loadStateDescription");
    _p_loadState->setValue(false);
    _p_stateChoiceParam = addChoiceParam("stateChoiceParam", "stateChoiceParamDescription");
    _p_stateChoiceParam->disable();

    _pIn_constValueP = addInputPort("_pIn_constValueP", "Vec3", "_pIn_constValuePDescription");
    _pIn_constValueP->setRequired(0);    
    _pOut_constValueP = addOutputPort("_pOut_constValueP", "Vec3", "_pOut_constValuePDescription");
    _dtXmlParserP = NULL;
  }

  void constValueGenerator::param(const char* paramName, bool inMapLoading) {
    try {
      //
      //check if cValP is empty, if so go away
      //
      if (_cVal.size() == 0) return;

      //
      // constValueChoiceParam
      //
      if (strcmp(paramName, "constValueChoiceParam") == 0) {
        dt__info(param,
                << "constValueChoiceParam changed");
        //
        //update slider parameter
        //
        for (int ii = 0; ii<_cVal.size();ii++) {
          (_cVal[ii])->pushValueToGui();
        }
      }
      //
      // constValueFloatSliderParam or constValueIntParam
      //
      else if ( (strcmp(paramName, "constValueFloatSliderParam") == 0) 
                  || (strcmp(paramName, "constValueIntParam") == 0) ){
        dt__info(param(),
                << paramName << " changed");
        //
        //update constValue
        //
        for (int ii = 0; ii<_cVal.size();ii++) {
          (_cVal[ii])->getValueFromGui();
        }
      } 
      //
      // constValueChoiceParam
      //
      else if (strcmp(paramName, "_p_constValueSetChoiceParam") == 0) {
        dt__info(param,
                << dt__eval(paramName) );
        //
        // update slider parameter
        //
        int actChoice = _p_constValueSetChoiceParam->getValue();
        constValue * cVRef = _cValSet[actChoice].at(0);
        _p_constValueSetFloatSliderParam->setValue(
          cVRef->getMin(), 
          cVRef->getMax(), 
          cVRef->getValue() 
        );

      }
      //
      // constValueFloatSliderParam or constValueIntParam
      //
      else if ( (strcmp(paramName, "_p_constValueSetFloatSliderParam") == 0) ){
        dt__info(param(),
                << paramName << " changed");
        //
        //update constValue
        //
        int actChoice = _p_constValueSetChoiceParam->getValue();
        float valToSet = _p_constValueSetFloatSliderParam->getValue();
        for (int ii=0;ii<_cValSet[actChoice].size();ii++) {
          _cValSet[actChoice].at(ii)->setValue(valToSet);
        }
      } 
      //
      // parseXml
      //
      else if(strcmp(paramName, "parseXml") == 0) {
        dt__info(param,
                << "parseXml changed");   
        if( _p_parseXml->getValue() ) {
          _p_xmlFilebrowser->enable();
        }
        else {
          _p_xmlFilebrowser->disable();
        }
      }
      //
      // save state
      //
      else if(strcmp(paramName, "saveState") == 0) {
        if ( _p_saveState->getValue() ) {
          saveState();
        }
        else {
          _p_saveState->setValue(false);
        }
      }   
      //
      // load state
      //
      else if(strcmp(paramName, "loadState") == 0) {
        if ( _p_loadState->getValue() ) {
          loadState();
        }
        else {
          _p_stateChoiceParam->disable();
        }
      }
      //
      // state choice param
      //
      else if(strcmp(paramName, "stateChoiceParam") == 0) {
        if ( _p_loadState->getValue() ) {
          loadStateToConst();
          //
          //execute me
          //
          setExecGracePeriod(0.01);
          selfExec();        
        }
        else {
          _p_loadState->setValue(false);
          _p_stateChoiceParam->disable();
        }
      }     
    }
    catch (eGeneral & eGenRef) {
      dt__catch(param(), eGenRef.what());
    }
  }

  int constValueGenerator::compute(char const * str) {          
      try {        
        //
        // init log file
        //
        std::string logFileName = std::string( coModule::getTitle() );
        abstractModule::initializeLogFile( "./"+logFileName+".log" );
        
        //
        //check if parsing is necessary
        //
        if ( _p_parseXml->getValue() ) {
          parseXmlFile();
          //set boolean to false
          _p_parseXml->setValue(false);
          _p_xmlFilebrowser->disable();
        }
          
        /**
         * 
         * @todo Is this check necessary?
         */
        if ( _cVal.size() == 0 ) {
          dt__THROW(compute(),
                  << dt__eval(_cVal.size()) );
        }
        
        if ( _pOut_constValueP->isConnected() ) {
          //
          // recreate constValues and append to cValTmp
          //
          coDoSetHandling< constValue * > cValTmp;
          for ( int ii=0; ii<_cVal.size(); ii++) {
            cValTmp.push_back( _cVal[ii]->clone() );
          }
          if ( _pIn_constValueP->isConnected() ) {
            covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(
                                            _pIn_constValueP->getCurrentObject()
                                          );
            
            abstractModule::recreateConstValues(set, cValTmp);
          }
        
          //
          // check if there are duplicate labels
          //
          cValTmp.checkForBastardTwins();
          
          //
          // send cVals to next module
          //
          covise::coDoSet * set = cValTmp.packToCoDoSet( 
                                    cValTmp,
                                    _pOut_constValueP->getObjName()
                                  );
          
          //
          // free memeory
          //
          cValTmp.destroy();
          
          //
          // feedback
          //
          covise::coFeedback fb("dtOOCVGen");
          fb.addPara(_p_constValueChoiceParam);
          fb.addPara(_p_constValueFloatSliderParam);
          fb.apply(set);
          _pOut_constValueP->setCurrentObject(
                               set 
                             );
        }
//        abstractModule::closeLogFile();
        return SUCCESS;
      }
      catch (eGeneral & eGenRef) {
        dt__catch(compute(), eGenRef.what());
        send_stop_pipeline();
//        abstractModule::closeLogFile();
        return FAIL;
      }
  }

  void constValueGenerator::parseXmlFile(void) {
    //
    // free memory
    //
    _cVal.destroy();
    //parser
    if (_dtXmlParserP) {
      delete _dtXmlParserP;
    }
    _dtXmlParserP = new dtXmlParser();
    
    //
    // parse file
    //
    _dtXmlParserP->openFileAndParse( _p_xmlFilebrowser->getValue() );

    std::vector< std::string > label;    
    //
    // creating constValues
    //
    _dtXmlParserP->getNames("constValue", &label);
    //clear choiceParam
    std::vector<string> choices(label.size(), "empty");
    _p_constValueChoiceParam->setValue(label.size(), choices, 0);
    for (int ii = 0;ii<label.size();ii++) {
      _dtXmlParserP->createConstValue(label[ii], &_cVal);
      _cVal[ii]->save(_p_constValueFloatSliderParam);
      _cVal[ii]->save(_p_constValueIntParam);
      _cVal[ii]->registrate(ii, _p_constValueChoiceParam);
    }
    
    //
    // creating constValueSets
    //
    _cValSet.clear();
    label.clear();
    _dtXmlParserP->getNames("constValueSet", &label);
    for (int ii = 0;ii<label.size();ii++) {
      _cValSet.push_back( vectorHandling< constValue * >() );      
      _dtXmlParserP->createConstValueSet(label[ii], _cVal, _cValSet[ii]);
    }
    abstractModule::updateChoiceParam(_p_constValueSetChoiceParam, &label);
    
    
  }

  void constValueGenerator::saveState(void) {
    if (_dtXmlParserP != NULL ) {
      std::string stateName = std::string(_p_stateName->getValue());
      if (stateName == "") {
        _dtXmlParserP->openFileAndWrite( _p_xmlStateFilebrowser->getValue(), &_cVal );
      }
      else {
        _p_stateName->setValue("");
        _dtXmlParserP->openFileAndWrite( _p_xmlStateFilebrowser->getValue(), stateName, &_cVal );  
      }
      _p_saveState->setValue(false);
      
    }
  }

  void constValueGenerator::loadState(void) {
    _dtXmlParserP->openFileAndLoad( _p_xmlStateFilebrowser->getValue() );
    std::vector< std::string > choices = _dtXmlParserP->getStates();
    _p_stateChoiceParam->setValue(choices.size(), choices, 0);
    _p_stateChoiceParam->enable();
  }
  
  void constValueGenerator::loadStateToConst(void) {
    std::string stateName = _p_stateChoiceParam->getLabel(_p_stateChoiceParam->getValue());
    dt__info(loadStateToConst,
            << "Loading state:" << std::endl
            << dt__eval(_p_stateChoiceParam->getValue()) << std::endl
            << dt__eval(stateName) << std::endl
            << " to constValue.");
    
    _dtXmlParserP->loadStateToConst(stateName, _cVal);
    
    _p_stateChoiceParam->disable();
    _p_loadState->setValue(false);
  }  
  
}
MODULE_MAIN(designTool, dtOO::constValueGenerator)
