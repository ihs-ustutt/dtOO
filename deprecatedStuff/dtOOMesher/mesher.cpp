#include "mesher.h"

#include <debugHeaven/extInfoContainer.h>

//---------------------------------
#include <do/coDoSet.h>
//------------------------------------------------------------------------------
#include <logMe/logMe.h>
//------------------------------------------------------------------------------
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticGeometryReconstructor.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <constValueHeaven/constValueReconstructor.h>
#include <boundedVolume.h>
#include <boundedVolumeFactory.h>
//#include <api/coFeedback.h>
//#include <optimizableInterface.h>
#include <debugHeaven/extInfoContainer.h>

//#define SUCCESS 1;
//#define FAILURE 0;

namespace dtOO {
  mesher::mesher(int argc, char *argv[]) : coModule(argc, argv, "mesher") {
    //
    // port
    //
    _pIn_constValueP = addInputPort("constValueP", "Vec3", "constValueDescription");
    _pIn_constValueP->setRequired(0);
    _pIn_sFunP = addInputPort("sFunP", "USR_ScaFun", "sFunPDescription");
    _pIn_sFunP->setRequired(0);
    _pIn_anGeometryP = addInputPort("anGeometryP", "USR_AnGeo", "_p_anGeometryDescription");
    _pOut_boundedVolumeP = addOutputPort("boundedVolumeP", "Lines", "boundedVolumeDescription");
    _pOut_unGridP = addOutputPort("unGridP", "UnstructuredGrid", "unGridDescription");
    _pOut_extInfoContainerP = addOutputPort("_pOut_extInfoContainerP", "USR_ExtInfo", "_pOut_extInfoContainerPDescription");
    _pOut_surfGridP = addOutputPort("surfGridP", "UnstructuredGrid", "unSurfGridDescription");
//    _pOut_bocoP = addOutputPort("boco", "USR_FenflossBoco", "BoundaryConditions");
    
//    _pOut_optiControlP = addOutputPort("_pOut_optiControlP", "USR_ExtInfo", "_pOut_optiControlPDescription");
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");
    
    _p_boundFaceNum = addInt32Param("boundFaceNum", "displayBoundingFaceNumber");
    _p_boundFaceNum->setValue(0);
    _p_createBVol = addBooleanParam("_p_createBVol", "createBoundedVolumes");
    _p_createBVol->setValue(TRUE);
    
    _parserP = NULL;
//    _p_newResult = addBooleanParam("_p_newResult", "_p_newResultDescription");
//    _p_newResult->disable();
//    _p_newResult->setValue(false);
//    _p_resultValue = addFloatParam("_p_resultValue", "_p_resultValueDescription");
//    _p_resultValue->disable();
	}
  
//  void mesher::param(const char* paramName, bool inMapLoading) {
////      if ( paramName==_p_boundFaceNum->getName() ) {
////       if (_pOut_surfGridP->isConnected()) {
//        if (_bVol.size() != 0) {
//             _pOut_surfGridP->setCurrentObject(
//               _bVol[0]->toCoDoSurfUnsGrid( &_bVol, _pOut_surfGridP->getObjName(), _p_boundFaceNum->getValue() )
//             );
//        }    
////      }
////      }
//  }


  int mesher::compute(const char * port) {    
    try {
        
          
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      const covise::coDistributedObject * inObj;
      covise::coDoSet const * set;
      
      if (_p_createBVol->getValue()) {
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
      // recreate analyticGeometries
      //
      inObj = _pIn_anGeometryP->getCurrentObject();
      set = dynamic_cast< covise::coDoSet const * >(inObj);
      _aGeo.destroy();
      abstractModule::recreateAnalyticGeometries(set, _aGeo);

      //
      // parse xml file
      //
      if (_parserP) {
        delete _parserP;
      }
      _parserP = new dtXmlParser();
      _parserP->openFileAndParse( _p_xmlFilebrowser->getValue() );
    
      //
      // create boundedVolumes
      //
    
      createBoundedVolumes();
      
      }
      //
      // output
      //
      if (_pOut_boundedVolumeP->isConnected()) {
        if (_bVol.size() != 0) {
          _pOut_boundedVolumeP->setCurrentObject(
            _bVol[0]->toCoDoSet( &_bVol, _pOut_boundedVolumeP->getObjName() )
          );
        }    
      }
      if (_pOut_unGridP->isConnected()) {
        if (_bVol.size() != 0) {
          _pOut_unGridP->setCurrentObject(
            _bVol[0]->toCoDoUnstructuredGrid( &_bVol, _pOut_unGridP->getObjName() )
          );
          
        }    
      }
      if ( _pOut_extInfoContainerP->isConnected() ) {
        extInfoContainer eIC;
        dt__FORALL(_bVol, ii,
          _bVol[ii]->packToExtInfoContainer( &eIC );
        );
        _pOut_extInfoContainerP->setCurrentObject(
          eIC.packToCoDoSet( _pOut_extInfoContainerP->getObjName() )
        );        
      }
      
     
      
//      if (_pOut_bocoP->isConnected()) {  
      if (_pOut_surfGridP->isConnected()) {
        if (_bVol.size() != 0) {
             _pOut_surfGridP->setCurrentObject(
               _bVol[0]->toCoDoSurfUnsGrid( &_bVol, _pOut_surfGridP->getObjName(), _p_boundFaceNum->getValue() )
             );
        }    
      }
   
      
      
//      if ( _pOut_optiControlP->isConnected() ) {
//        //
//        // check for optimization
//        //
//        optimizableInterface * optiIntP;
//        dt__CANDOWNCAST(_bVol[0], optimizableInterface, optiIntP);  
////        if (optiIntP) {
//  //        //
//  //        // create result value
//  //        //
//  //        ptrHandling< constValue > resValue( constValueFactory().create("sliderFloatParam") );
//  //        resValue->setName("resValue");
////          resValue->setValue( optiIntP->characterizeMe() );
//  //        //
//  //        // pack to coDoSet
//  //        //
//  //        covise::coDoSet * set = resValue->packToCoDoSet( _pOut_resValueP->getObjName() );
//
//          //
//          // write to file
//          //
//          std::ofstream optiResFile;
//          std::string optiResFileStr = "resValue.txt";
//          optiResFile.open( optiResFileStr.c_str(), std::ofstream::out | std::ofstream::trunc );
//          optiResFile << 5.;//optiIntP->characterizeMe();
//          optiResFile.close();
//                 
//          
//    //        //
//    //        // set to output port
//    //        //
//    //        _pOut_resValueP->setCurrentObject(set);
//          return STOP_PIPELINE;
////        }
//      }
    }
    catch (eGeneral & eGenRef) {
      DTCATCHERRORWF(compute(), eGenRef.what());
      
      //
      // close log file
      //
      abstractModule::closeLogFile();

      return FAIL;
    }
    //
    // close log file
    //
    abstractModule::closeLogFile();
      
    return SUCCESS;
  }

  void mesher::createBoundedVolumes(void) {
    _bVol.destroy();

    //
    //get boundedVolume names
    //
    std::vector< std::string > boundedVolumeLabel;
    _parserP->getNames("boundedVolume", &boundedVolumeLabel);
    
    //
    // create blocks
    //
    boundedVolumeFactory bVFac;
//    dt__FORALL( boundedVolumeLabel,ii,
    for (int ii=0; ii<boundedVolumeLabel.size(); ii++) {
      //
      // get configuration element
      //
      QDomElement element = _parserP->getElement(
                                        "boundedVolume", 
                                        boundedVolumeLabel[ii]
                                      );
      
      //
      // create new boundedVolume with factory
      //
      _bVol.push_back( 
        bVFac.create( _parserP->getAttributeStr("name", element) ) 
      );
      //
      // initialize boundedVolume
      //
      _bVol.back()->init(
        _parserP->getElement("boundedVolume", boundedVolumeLabel[ii]),
        &_cVal,
        &_sFun,
        &_aGeo,
        &_bVol
      );
      
      if ( _pOut_unGridP->isConnected() && _pOut_boundedVolumeP->isConnected() ) {
        _bVol[ii]->writeGrid();
      }
      else {
        _bVol[ii]->writeCheapGrid();
      }
  }
//    );
  }
  
  vectorHandling< boundedVolume * > & mesher::getRefToBoundedVolume( void ) {
    return _bVol;
  }
}

#ifndef DTOO_NOMAIN
MODULE_MAIN(designTool, dtOO::mesher)
#endif
        


