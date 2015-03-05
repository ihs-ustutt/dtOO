#include "geoGen.h"

#include <interfaceHeaven/labelHandling.h>

//------------------------------------------------------------------------------
#include <progHelper.h>
//------------------------------------------------------------------------------
#include <stdlib.h>
//------------------------------------------------------------------------------
#include <do/coDoSet.h>
#include <do/coDoData.h>
//------------------------------------------------------------------------------
#include <logMe/logMe.h>
//------------------------------------------------------------------------------
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
//------------------------------------------------------------------------------
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValue.h>
#include <constValueHeaven/constValueReconstructor.h>
//------------------------------------------------------------------------------
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
//------------------------------------------------------------------------------
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <dtXmlParserDecorator/dtXmlParserDecoratorFactory.h>

//------------------------------------------------------------------------------
#include <exceptionHandling/eGeneral.h>

//predefines
//#define SUCCESS 0;
//#define FAILURE 1;

namespace dtOO {
  // constructor
  geoGen::geoGen(int argc, char *argv[]) : coModule(argc, argv, "geoGen") {
    //ports
    _pIn_constValueP = addInputPort("constValueP", "Vec3", "constValueDescription");
    _pIn_constValueP->setRequired(0);
    _pIn_sFunP = addInputPort("sFunP", "USR_ScaFun", "sFunPDescription");
    _pIn_sFunP->setRequired(0);
    _pIn_anGeoP = addInputPort("pIn_anGeoP", "USR_AnGeo", "pIn_anGeoPDescription");
    _pIn_anGeoP->setRequired(0);
    _pOut_anGeoP = addOutputPort("anGeoP", "USR_AnGeo", "anGeoDescription");
    _pOut_partP = addOutputPort("partP", "Points|Lines|Polygons", "machine parts");
    _pOut_singleDetailsP = addOutputPort("singleDetailsP", "Points|Lines|Polygons", "single part");
    _pOut_pointP = addOutputPort("pointP", "Polygons", "points");
    _pOut_vectorP = addOutputPort("vectorP", "Lines", "vectors");
    
    //initialize
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");
    _p_singleDetailsChoiceParam = addChoiceParam("singleDetailsChoiceParam", "singleDetailChoiceParamDescription");
    _p_singleDetailsChoiceParam->disable();
    _p_passThrough = addBooleanParam("_p_passThrough", "_p_passThroughDescription");
    _p_passThrough->enable();
    _p_passThrough->setValue( false );
    _p_renderOptionsChoiceParam = addChoiceParam("_p_renderOptionsChoiceParam", "_p_renderOptionsChoiceParamDescription");
    _p_renderOptionsChoiceParam->disable();
    _p_renderOptionsVectorParam = addInt32VectorParam("_p_renderOptionsVectorParam", "_p_renderOptionsVectorParamDescription");
    _p_renderOptionsVectorParam->disable();
    _p_hideAll = addBooleanParam("_p_hideAll", "_p_hideAllDescription");
    _p_hideAll->disable();
    _p_hideAll->setValue(false);
           
    //variables
    _pointContainerP = new pointContainer();
    _vectorContainerP = new vectorContainer();
    _singleDetailsP = new std::vector< analyticGeometry *>;
    
    _recreateParts = true;
  }

  void geoGen::param(const char* paramName, bool inMapLoading) {
    //
    //check if cValP is empty, if so go away
    //
    if ( 
			_aGeo.size() == 0			
       ) {
      return;
    }
    else if ( strcmp(paramName, "singleDetailsChoiceParam") == 0 ) {
      //
      //execute me
      //
      _recreateParts = false;
      setExecGracePeriod(0.01);
      selfExec();
    }
    else if ( strcmp(paramName, "_p_passThrough") == 0 ) {
      if ( _p_passThrough->getValue() ) {
        _p_xmlFilebrowser->disable();
        _p_renderOptionsChoiceParam->enable();
        _p_renderOptionsVectorParam->enable();
         _p_hideAll->enable();
      }
      else {
        _p_xmlFilebrowser->enable();
        _p_renderOptionsChoiceParam->disable();
        _p_renderOptionsVectorParam->disable();
        _p_hideAll->disable();
      }
    }    
    else if ( strcmp(paramName, "_p_renderOptionsChoiceParam") == 0 ) {
      int pos = _p_renderOptionsChoiceParam->getValue();
      _p_renderOptionsVectorParam->setValue(
        _renderResolution[pos][0], 
        _renderResolution[pos][1], 
        _renderResolution[pos][2]
      );     
    }       
    else if ( strcmp(paramName, "_p_renderOptionsVectorParam") == 0 ) {
      int pos = _p_renderOptionsChoiceParam->getValue();
      _renderResolution[pos][0] = _p_renderOptionsVectorParam->getValue(0);
      _renderResolution[pos][1] = _p_renderOptionsVectorParam->getValue(1);
      _renderResolution[pos][2] = _p_renderOptionsVectorParam->getValue(2);
    }
    else if ( strcmp(paramName, "_p_hideAll") == 0 ) {
      _p_hideAll->setValue(false);
      dt__FORALL(_renderResolution, pos,
        _renderResolution[pos][0] = 0;
        _renderResolution[pos][1] = 0;
        _renderResolution[pos][2] = 0;
      );
    }    
  }

  
  int geoGen::compute(const char *port) {
    try {
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      coDoSetHandling< analyticGeometry* > aGeoIn;

      //
      // check if there are analyticGeometries on the input port
      //
      if ( _pIn_anGeoP->isConnected() ) {
        //
        // recreate analyticGeometries
        //
        covise::coDistributedObject const * inObj = _pIn_anGeoP->getCurrentObject();
        covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
        abstractModule::recreateAnalyticGeometries(set, aGeoIn);
      }        
      
      if (_recreateParts && !_p_passThrough->getValue()) {
        const covise::coDistributedObject * inObj;
        covise::coDoSet const * set;
        
        //
        // recreate constValues
        //
        if ( _pIn_constValueP->isConnected() ) {
          inObj = _pIn_constValueP->getCurrentObject();
          set = dynamic_cast< covise::coDoSet const * >(inObj);
          _cVal.destroy();
          abstractModule::recreateConstValues(set, _cVal);
        }
        
        //
        // recreate scaFunctions
        //
        if ( _pIn_sFunP->isConnected() ) {
          inObj = _pIn_sFunP->getCurrentObject();
          set = dynamic_cast< covise::coDoSet const * >(inObj);
          _sFun.destroy();
          abstractModule::recreateScaFunctions(set, _sFun);
        }
        
        //
        // parse file
        //
        dtXmlParser parser;
        parser.openFileAndParse( _p_xmlFilebrowser->getValue() );    

        //
        // free space
        //
        delete _pointContainerP;
        delete _vectorContainerP;
        _pointContainerP = new pointContainer();
        _vectorContainerP = new vectorContainer();

        //
        // create parts
        //
        _aGeo.destroy();
        std::vector< std::string > machinePartLabel;
        parser.getNames("part", &machinePartLabel);
        for (int ii = 0;ii<machinePartLabel.size();ii++) {
          int counter = aGeoIn.size();
          parser.createMachinePart(machinePartLabel[ii], 
                                           _pointContainerP, 
                                           _vectorContainerP, 
                                           &_cVal, 
                                           &_sFun, 
                                           &aGeoIn);
          //
          // add new part to local _aGeo vector
          //
          if (counter != aGeoIn.size()) { // sort vector containers out
              for (int ii = counter ; ii<aGeoIn.size() ; ii++) {
               _aGeo.push_back( aGeoIn.at(ii)->clone() );
              }
          }
        }
      }
      else if(_p_passThrough->getValue()) {
        _aGeo.clear();
        dt__FORALL(aGeoIn, ii, 
          _aGeo.push_back( aGeoIn[ii]->clone() );
        );        
        _aGeo.checkForBastardTwinsAndMakeUnique();
      }
      else {
        dt__FORALL(_aGeo, ii, 
          aGeoIn.push_back( _aGeo[ii]->clone() );
        );
        //
        //check for duplicate labels
        //
        aGeoIn.checkForBastardTwins();
      }


      
      //
      // create choiceParam
      //
      updateSingleDetailChoiceParam(&_aGeo);
      
      //
      // resize vector if _aGeo size changes
      //
      if ( _aGeo.size() != _renderResolution.size(0) ) { 
        _renderResolution.resize(_aGeo.size(), 3);
        dt__FORALL(_aGeo, ii,
          map1dTo3d const * m1d;
          map2dTo3d const * m2d;
          map3dTo3d const * m3d;
          dt__CANDOWNCAST(_aGeo[ii], map1dTo3d const, m1d);
          dt__CANDOWNCAST(_aGeo[ii], map2dTo3d const, m2d);
          dt__CANDOWNCAST(_aGeo[ii], map3dTo3d const, m3d);

          int res[3]; res[0] = 0; res[1] = 0; res[2] = 0;
          if (m1d) {
            res[0] = m1d->getRenderResolutionU();
          }
          else if (m2d) {
            res[0] = m2d->getRenderResolutionU();
            res[1] = m2d->getRenderResolutionV();
          }
          else if (m3d) {
            res[0] = m3d->getRenderResolutionU();
            res[1] = m3d->getRenderResolutionV();
            res[2] = m3d->getRenderResolutionW();
          }
          _renderResolution[ii][0] = res[0];
          _renderResolution[ii][1] = res[1];
          _renderResolution[ii][2] = res[2];
        );
        
      }

      //
      // dump
      //
      dt__FORALL(_aGeo, ii,
        _aGeo[ii]->dump();
      );     

//      //
//      // dump
//      //
//      DTINFOWF(compute(), << "AGEOIN ....");
//      dt__FORALL(aGeoIn, ii,
//        aGeoIn[ii]->dump();
//      );        
      
      //
      // reset module
      //
      _recreateParts = true;      
      
      //
      // render machine
      //
      dt__FORALL(_aGeo, pos,
        map1dTo3d * m1d;
        map2dTo3d * m2d;
        map3dTo3d * m3d;
        dt__CANDOWNCAST(_aGeo[pos], map1dTo3d, m1d);
        dt__CANDOWNCAST(_aGeo[pos], map2dTo3d, m2d);
        dt__CANDOWNCAST(_aGeo[pos], map3dTo3d, m3d);

        if (m1d) {
          m1d->setRenderResolution(0, _renderResolution[pos][0]);
        }
        else if (m2d) {
          m2d->setRenderResolution(0, _renderResolution[pos][0]);
					m2d->setRenderResolution(1, _renderResolution[pos][1]);
        }
        else if (m3d) {
          m3d->setRenderResolution(0, _renderResolution[pos][0]);
					m3d->setRenderResolution(1, _renderResolution[pos][1]);
					m3d->setRenderResolution(2, _renderResolution[pos][2]);
        }        
      );      
      renderMe();
      
      //
      // send to next module
      //
      if (_pOut_anGeoP->isConnected()) {
        if (aGeoIn.size() != 0) {
          _pOut_anGeoP->setCurrentObject(
            aGeoIn.packToCoDoSet( 
              aGeoIn, 
              _pOut_anGeoP->getObjName() 
            )
          );  
        }
      }
      
      //
      // free memory
      //
      aGeoIn.destroy();
			
			//
			// close log file
			//
			closeLogFile();
			
			return CONTINUE_PIPELINE;
    }
    catch (eGeneral & eGenRef) {
      DTCATCHERRORWF(compute(), eGenRef.what());
      send_stop_pipeline();
			
			//
			// close log file
			//
			closeLogFile();
			
      return STOP_PIPELINE;
    }
  }

  void geoGen::renderMe(void) {
    coDoSet* aGeoCoDoSetP;
    coDoSet* geoCoDoSetP;
    coDoSet* pointCoDoSetP;
    coDoSet* vectorCoDoSetP;
    coDoSet* singleDetailsCoDoSetP;

    //
    // parts
    //
    if (_pOut_partP->isConnected()) {
      if (_aGeo.size() != 0) {
        aGeoCoDoSetP = _aGeo.toCoDoSet( _aGeo, _pOut_partP->getObjName() );
        _pOut_partP->setCurrentObject(aGeoCoDoSetP);
      }
    }
      
    //
    // point Container
    //
    if (_pOut_pointP->isConnected()) {
      if ( !_pointContainerP->isEmpty() ) {
        pointCoDoSetP = _pointContainerP->toCoDoSet(_pOut_pointP->getObjName());
        _pOut_pointP->setCurrentObject( pointCoDoSetP );
      }
    }
    
    //
    // vector container
    //
    if (_pOut_vectorP->isConnected()) {
      if ( !_vectorContainerP->isEmpty() ) {
        vectorCoDoSetP = _vectorContainerP->toCoDoSet(_pOut_vectorP->getObjName());
        _pOut_vectorP->setCurrentObject( vectorCoDoSetP );
      }    
    }
    
    //
    // singleChoice
    //
    if (_pOut_singleDetailsP->isConnected()) {
      if (_aGeo.size() != 0) {
        singleDetailsCoDoSetP = _aGeo.at(_p_singleDetailsChoiceParam->getValue())->toCoDoSetExt( _pOut_singleDetailsP->getObjName() );
        _pOut_singleDetailsP->setCurrentObject(singleDetailsCoDoSetP);  
      }
    }    
  }

  geoGen::~geoGen() {
    _aGeo.destroy();
    _sFun.destroy();
    _cVal.destroy();
    delete _pointContainerP;
    delete _vectorContainerP;
  }
    
  void geoGen::updateSingleDetailChoiceParam(vectorHandling< analyticGeometry * > * vecHP) {
    _p_singleDetailsChoiceParam->enable();
    abstractModule::updateChoiceParam( _p_singleDetailsChoiceParam, vecHP);
    abstractModule::updateChoiceParam( _p_renderOptionsChoiceParam, vecHP);
  }
}

MODULE_MAIN(designTool, dtOO::geoGen)
