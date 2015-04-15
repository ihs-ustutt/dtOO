#include "sFGen.h"
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <constValueHeaven/constValue.h>
#include <do/coDoSet.h>
#include <logMe/logMe.h>

#define SUCCESS 1;
#define FAILURE 0;

namespace dtOO {
  sFGen::sFGen(int argc, char *argv[] ) : coModule(argc, argv, "sFGen") {
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");
    _p_xmlFilebrowser->disable();    
    _p_functionChoiceParam = addChoiceParam("functionChoiceParam", "functionChoiceParamDescription");
    _p_functionChoiceParam->disable();
    _p_functionSetChoiceParam = addChoiceParam("functionSetChoiceParam", "functionSetChoiceParamDescription");
    _p_functionSetChoiceParam->disable();
    
    _pIn_constValueP = addInputPort("constValueP", "Vec3", "constValueDescription");
    _pIn_constValueP->setRequired(0);
    _pIn_sFunP = addInputPort("_pIn_sFunP", "USR_ScaFun", "_pIn_sFunPDescription");
    _pIn_sFunP->setRequired(0);    
    _pOut_sFunP = addOutputPort("_pOut_sFunP", "USR_ScaFun", "_pOut_sFunPDescription");
    _pOut_functionP = addOutputPort("functionsP", "Vec2", "function vectors");
    _pOut_functionSetP = addOutputPort("functionsSetP", "Vec2", "function set vectors");
    
    _p_passThrough = addBooleanParam("_p_passThrough", "_p_passThroughDescription");
    _p_passThrough->enable();
    _p_passThrough->setValue( false );
    
    _recreate = true;
  }
  
  void sFGen::param(const char* paramName, bool inMapLoading) {  
    //
    //check if cValP is empty, if so go away
    //
    if (_sFun.size() == 0) return;
    
    if(strcmp(paramName, "functionChoiceParam") == 0) {    
      //
      //execute me
      //
      _recreate = false;
      setExecGracePeriod(0.01);
      selfExec();
    }
  }

  int sFGen::compute(char const * str) {
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      //
      // recreate constValues
      //
      covise::coDistributedObject const * inObj = _pIn_constValueP->getCurrentObject();
      covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
      _cVal.destroy();
      abstractModule::recreateConstValues(set, _cVal);
    
      //
      // recreate scaFunctions
      //
      coDoSetHandling< scaFunction * > tmpSFun;
      if ( _pIn_sFunP->isConnected() ) {
        const covise::coDistributedObject * inObj = _pIn_sFunP->getCurrentObject();
        covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
        abstractModule::recreateScaFunctions(set, tmpSFun);
      }
      
      if (_recreate) {
        dtXmlParser parser;
        parser.openFileAndParse( _p_xmlFilebrowser->getValue() );

        //
        // create functions
        //
        std::vector< std::string > label;
        parser.getNames("function", &label);
        
        //
        // create new functions
        //
        _sFun.destroy();
        for (int ii = 0;ii<label.size();ii++) {
          parser.createFunction(label[ii], &_cVal, &tmpSFun);
          _sFun.push_back( 
					  scaFunction::SecureCast(tmpSFun.back()->clone())
					);
        }

        if (_p_passThrough->getValue()) {
          dt__FORALL(tmpSFun, ii,
            _sFun.push_back( scaFunction::SecureCast(tmpSFun[ii]->clone()) );
          );          
        }
        //
        // update choice param
        //
        updateFunctionChoiceParam(&_sFun);

        //
        // create function sets
        //
        label.clear();
        parser.getNames("functionSet", &label);

        for (int ii = 0;ii<_sFunSet.size();ii++) {
          _sFunSet[ii].destroy();
        }
        _sFunSet.clear();

        for (int ii = 0;ii<label.size();ii++) {
          _sFunSet.push_back( coDoSetHandling< scaFunction * >() );
          parser.createFunctionSet( label[ii], _sFun, &(_sFunSet[ii]) );
        }
        //
        // update choice param
        //
        abstractModule::updateChoiceParam(_p_functionSetChoiceParam, &label);
      }
      else {
        if ( !_p_passThrough->getValue() ) {
          dt__FORALL(_sFun, ii,
            tmpSFun.push_back( scaFunction::SecureCast(_sFun[ii]->clone()) );
          );
        }
      }
      
      //
      // reset module
      //
      _recreate = true;
      
      //
      // check for duplicate labels
      //
      tmpSFun.checkForBastardTwins();
      
      //
      // send to next module
      //
      if ( _pOut_sFunP->isConnected() ) {
        _pOut_sFunP->setCurrentObject(
                       tmpSFun.packToCoDoSet( 
                         tmpSFun,
                         _pOut_sFunP->getObjName()
                       )
                     );
      }
      
      //
      // destroy temporary function container
      //
      tmpSFun.destroy();
      

      //
      // functions
      //
      if (_pOut_functionP->isConnected()) {
        if (_sFun.size() != 0) {
          char const * label = _p_functionChoiceParam->getActLabel();
          dt__ptrAss(
						scaOneD const * const sF, scaOneD::ConstDownCast(_sFun.get(label))
					);
          covise::coDoSet * set = sF->toCoDoSet( _pOut_functionP->getObjName() );
          _pOut_functionP->setCurrentObject( set );
        }
      }    
      if (_pOut_functionSetP->isConnected()) {
        if (_sFun.size() != 0) {
          int choice = _p_functionSetChoiceParam->getValue();
          covise::coDoSet * set = _sFunSet[choice].toCoDoSet(
                                    _sFunSet[choice], 
                                    _pOut_functionSetP->getObjName() 
                                  );
          _pOut_functionSetP->setCurrentObject( set );
        }
      }        
    }
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }    
          
  }

  void sFGen::updateFunctionChoiceParam(vectorHandling< scaFunction * > * vecHP) {
    abstractModule::updateChoiceParam( _p_functionChoiceParam, vecHP);
  }
  
  sFGen::~sFGen() {
    
  }
  
  coDoSetHandling< scaFunction * > & sFGen::getRefToScaFunction( void ) {
    return _sFun;
  }
}
#ifndef DTOO_NOMAIN
MODULE_MAIN(designTool, dtOO::sFGen)
#endif