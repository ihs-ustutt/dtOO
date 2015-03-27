#include "designTool.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <constValueHeaven/constValue.h>
#include <constValueHeaven/intParam.h>
#include <constValueHeaven/sliderFloatParam.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <dtPlugin.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <exceptionHandling/eGeneral.h>
#include <baseContainerHeaven/baseContainer.h>
#include <do/coDoSet.h>
#include <abstractModule.h>

namespace dtOO {
  designTool::designTool(int argc, char *argv[]) : covise::coModule(argc, argv, "designTool") {
    _logName = addStringParam("_logName", "_logNameDescription");
		_moduleChoice = addChoiceParam("_moduleChoice", "_moduleChoiceDescription");		
    _xmlBrowser = addFileBrowserParam("_xmlBrowser", "_xmlBrowserDescrition");
    _xmlBrowser->setValue(".", "*.xml/*");
    _xmlBrowser->disable();

    _parseXml = addBooleanParam("_parseXml", "_parseXmlDescription");
    _parseXml->setValue(false);
    _cVChoice = addChoiceParam("_cVChoice", "_cVChoiceDescription");
    _cVChoice->disable();
    _cVSlider = addFloatSliderParam("_cVSlider", "_cVSliderDescription");
    _cVSlider->setValue(0., 2., 1.);
    _cVSlider->disable();
    _cVInt = addInt32Param("_cVInt", "_cVIntDescription");
    _cVInt->setValue(0.);
    _cVInt->disable();
    _cVStateBrowser = addFileBrowserParam("_cVStateBrowser", "_cVStateBrowserDescription");
    _cVStateBrowser->setValue(".", "*.xml/*");
    _cVStateBrowser->disable();
    _cVStateLabel = addStringParam("_cVStateLabel", "_cVStateLabelDescription");
    _cVStateLabel->setValue("");
    _cVStateSave = addBooleanParam("_cVStateSave", "_cVStateSaveDescription");
    _cVStateSave->setValue(false);    
    _cVStateLoad = addBooleanParam("_cVStateLoad", "_cVStateLoadDescription");
    _cVStateLoad->setValue(false);
    _cVStateChoice = addChoiceParam("_cVStateChoice", "_cVStateChoiceDescription");
    _cVStateChoice->disable();

    _aFChoice = addChoiceParam("_aFChoice", "_aFChoiceDescription");
    _aFChoice->disable();
    _aFRenderChoice = addChoiceParam("_aFRenderChoice", "_aFRenderChoiceDescription");
    _aFRenderChoice->disable();
    //_aFRenderVector = addInt32VectorParam("_aGRenderVector", "_aGRenderVectorDescription");
    //_aFRenderVector->disable();
    _aFRenderCurrentToggle = addBooleanParam("_aFRenderCurrentToggle", "_aFRenderCurrentToggleDescription");
    _aFRenderCurrentToggle->disable();
    _aFRenderCurrentToggle->setValue(false);	
		
		_aGChoice = addChoiceParam("_aGChoice", "_aGChoiceDescription");
    _aGChoice->disable();
    _aGRenderChoice = addChoiceParam("_aGRenderChoice", "_aGRenderChoiceDescription");
    _aGRenderChoice->disable();
    _aGRenderVector = addInt32VectorParam("_aGRenderVector", "_aGRenderVectorDescription");
    _aGRenderVector->disable();
    _aGRenderCurrentToggle = addBooleanParam("_aGRenderCurrentToggle", "_aGRenderCurrentToggleDescription");
    _aGRenderCurrentToggle->disable();
    _aGRenderCurrentToggle->setValue(false);	

		_bVChoice = addChoiceParam("_bVChoice", "_bVChoiceDescription");
    _bVChoice->disable();
    _bVRenderChoice = addChoiceParam("_bVRenderChoice", "_bVRenderChoiceDescription");
    _bVRenderChoice->disable();
    _bVRenderTags = addChoiceParam("_bVRenderTags", "_bVRenderTagsDescription");
    _bVRenderTags->disable();
    _bVRenderCurrentToggle = addBooleanParam("_bVRenderCurrentToggle", "_bVRenderCurrentToggleDescription");
    _bVRenderCurrentToggle->disable();
    _bVRenderCurrentToggle->setValue(false);	
		
		_pLChoice = addChoiceParam("_pLChoice", "_pLChoiceDescription");
    _pLChoice->disable();
		
		_aFOut = addOutputPort("_aFOut", "Vec2", "_aFOutDescription");
    _aGOut = addOutputPort("_aGOut", "Points|Lines|Polygons|StructuredGrid", "_aGOutDescription");				
		_bVOut = addOutputPort("_bVOut", "UnstructuredGrid|Points|Lines|Polygons|StructuredGrid", "_bVOutDescription");				
		std::vector< covise::coUifPara * > tmp;
		//
		// constValues
		//
		_moduleChoices.push_back("cVGen");
    tmp.push_back( _parseXml );
    tmp.push_back( _cVChoice );
    tmp.push_back( _cVSlider );
    tmp.push_back( _cVInt );
    tmp.push_back( _cVStateBrowser );
    tmp.push_back( _cVStateLabel );
    tmp.push_back( _cVStateSave );
    tmp.push_back( _cVStateLoad );
    tmp.push_back( _cVStateChoice );
		_uifPara.push_back( tmp );
    //
		// analyticFunctions
		//
		tmp.clear();
		_moduleChoices.push_back("aFGen");
		tmp.push_back( _aFChoice );
    tmp.push_back(_aFRenderChoice );
    tmp.push_back( _aFRenderCurrentToggle );		
		_uifPara.push_back( tmp );
		//
		// analyticGeometries
		//
		tmp.clear();
		_moduleChoices.push_back("aGGen");
		tmp.push_back( _aGChoice );
    tmp.push_back( _aGRenderChoice );
    tmp.push_back( _aGRenderVector );
    tmp.push_back( _aGRenderCurrentToggle );		
		_uifPara.push_back( tmp );
		//
		// boundedVolumes
		//
		tmp.clear();
		_moduleChoices.push_back("bVGen");		
		tmp.push_back( _bVChoice );
    tmp.push_back( _bVRenderChoice );
    tmp.push_back( _bVRenderTags );
    tmp.push_back( _bVRenderCurrentToggle );
		_uifPara.push_back( tmp );		
		//
		// plugin
		//
		tmp.clear();
		_moduleChoices.push_back("pL");
		tmp.push_back( _pLChoice );
		_uifPara.push_back( tmp );
		
		_recreate = true;
  }

	void designTool::postInst() {
		abstractModule::updateChoiceParam(_moduleChoice, &_moduleChoices);
    _moduleChoice->show();
		_xmlBrowser->show();
	}
	
  void designTool::param(const char* paramName, bool inMapLoading) {
		try {
			_logName->setValue("");
			_logName->show();
			_logName->disable();			
			if (strcmp(paramName, "_moduleChoice") == 0) {
				if ( _moduleChoice->getValue() == 0 ) {
					dt__forAllIndex( _uifPara[0], ii) _uifPara[0][ii]->show();
					dt__forAllIndex( _uifPara[1], ii) _uifPara[1][ii]->hide();
					dt__forAllIndex( _uifPara[2], ii) _uifPara[2][ii]->hide();
					dt__forAllIndex( _uifPara[3], ii) _uifPara[3][ii]->hide();
					dt__forAllIndex( _uifPara[4], ii) _uifPara[4][ii]->hide();
				}
				else if (_moduleChoice->getValue() == 1) {
					dt__forAllIndex( _uifPara[0], ii) _uifPara[0][ii]->hide();
					dt__forAllIndex( _uifPara[1], ii) _uifPara[1][ii]->show();
					dt__forAllIndex( _uifPara[2], ii) _uifPara[2][ii]->hide();				
					dt__forAllIndex( _uifPara[3], ii) _uifPara[3][ii]->hide();
					dt__forAllIndex( _uifPara[4], ii) _uifPara[4][ii]->hide();
				}
				else if (_moduleChoice->getValue() == 2) {
					dt__forAllIndex( _uifPara[0], ii) _uifPara[0][ii]->hide();
					dt__forAllIndex( _uifPara[1], ii) _uifPara[1][ii]->hide();
					dt__forAllIndex( _uifPara[2], ii) _uifPara[2][ii]->show();
					dt__forAllIndex( _uifPara[3], ii) _uifPara[3][ii]->hide();
					dt__forAllIndex( _uifPara[4], ii) _uifPara[4][ii]->hide();
				}			
				else if (_moduleChoice->getValue() == 3) {
					dt__forAllIndex( _uifPara[0], ii) _uifPara[0][ii]->hide();
					dt__forAllIndex( _uifPara[1], ii) _uifPara[1][ii]->hide();
					dt__forAllIndex( _uifPara[2], ii) _uifPara[2][ii]->hide();
					dt__forAllIndex( _uifPara[3], ii) _uifPara[3][ii]->show();
					dt__forAllIndex( _uifPara[4], ii) _uifPara[4][ii]->hide();
				}				
				else if (_moduleChoice->getValue() == 4) {
					dt__forAllIndex( _uifPara[0], ii) _uifPara[0][ii]->hide();
					dt__forAllIndex( _uifPara[1], ii) _uifPara[1][ii]->hide();
					dt__forAllIndex( _uifPara[2], ii) _uifPara[2][ii]->hide();
					dt__forAllIndex( _uifPara[3], ii) _uifPara[3][ii]->hide();
					dt__forAllIndex( _uifPara[4], ii) _uifPara[4][ii]->show();
				}										
			}
			//--------------------------------------------------------------------------
			//
			// constValue param
			//
			if ( _cV.size() != 0 ) {
				if (strcmp(paramName, "_parseXml") == 0) {
					if ( _parseXml->getValue() ) {
						_cV.destroy();
						_parseXml->setValue(false);
						setExecGracePeriod(0.1);
						selfExec();
					}		
				}
				//
				// update slider parameter in GUI
				//
				else if (strcmp(paramName, "_cVChoice") == 0) {
					std::string label = _cVChoice->getActLabel();
					constValue const * cVptr = _cV.get(label);
					sliderFloatParam const * sfp = sliderFloatParam::ConstDownCast(cVptr);
					intParam const * ip = intParam::ConstDownCast(cVptr);
					
					if (ip) {
						_cVSlider->disable();
						_cVInt->enable();
						_cVInt->setValue(ip->getValue());
					}
					else if (sfp) {
						_cVSlider->enable();
						_cVInt->disable();
						_cVSlider->setValue(sfp->getMin(), sfp->getMax(), sfp->getValue());				
					}
				}
				//
				// update constValue from GUI
				//
				else if ( strcmp(paramName, "_cVSlider") == 0 ) {
					std::string label = _cVChoice->getActLabel();
					constValue * cVptr = _cV.get(label);
					dt__ptrAss(
						sliderFloatParam * sfp, 
						sliderFloatParam::DownCast(cVptr)
					);
					sfp->setValue(_cVSlider->getValue());
				}
				else if ( strcmp(paramName, "_cVInt") == 0 ) {
					std::string label = _cVChoice->getActLabel();
					constValue * cVptr = _cV.get(label);
					dt__ptrAss(
						intParam * ip, 
						intParam::DownCast(cVptr)
					);
					ip->setValue( static_cast< int >(_cVInt->getValue()) );
				}
				//
				// save state
				//
				else if(strcmp(paramName, "_cVStateSave") == 0) {
					if ( _cVStateSave->getValue() ) {
						saveCVState();
					}
					else {
						_cVStateSave->setValue(false);
					}
				}
				//
				// load state
				//
				else if(strcmp(paramName, "_cVStateLoad") == 0) {
					if ( _cVStateLoad->getValue() ) {
						loadCVStateLabels();
					}
					else {
						_cVStateChoice->disable();
					}
				}
				//
				// state choice param
				//
				else if(strcmp(paramName, "_cVStateChoice") == 0) {
					if ( _cVStateLoad->getValue() ) {
						loadCVState();
						setExecGracePeriod(0.1);
						selfExec();        
					}
					else {
						_cVStateLoad->setValue(false);
						_cVStateChoice->disable();
					}
				}
			}
			//--------------------------------------------------------------------------
			//
			// analyticFunction param
			//
			if ( _aF.size() != 0 ) {		
				if(strcmp(paramName, "_aFChoice") == 0) {    
//					_aFToRender.clear();
//					std::string label( _aFChoice->getLabel( _aFChoice->getValue() ) );
//					_aFToRender.push_back( _aF.get(label) );
//					_recreate = false;
//					setExecGracePeriod(0.1);
//					selfExec();
				  int pos = _aFChoice->getValue();
					if ( !_aFToRender.has( _aF[pos]->getLabel() ) ) {
						// set ext rendering for chosen geometry
//						_aF[pos]->extRender(true);
						//reset other geometries
						//dt__forAllIndex(_aFToRender,ii, _aFToRender[ii]->extRender(false); );
						_aFToRender.push_back( _aF[pos] );
					}
					else {
						int toDel = _aFToRender.getPosition( _aF[pos]->getLabel() );
						_aFToRender.erase(toDel);
					}
		      _recreate = false;
		      setExecGracePeriod(0.1);
		      selfExec();					
				}		
				else if ( strcmp(paramName, "_aFRenderChoice") == 0 ) {
					std::string str(_aFRenderChoice->getActLabel());
//					map1dTo3d const * m1d = map1dTo3d::ConstDownCast( _aG.get(str) );
//					map2dTo3d const * m2d = map2dTo3d::ConstDownCast( _aG.get(str) );
//					map3dTo3d const * m3d = map3dTo3d::ConstDownCast( _aG.get(str) );
//					int res[3]; res[0] = 0; res[1] = 0; res[2] = 0;
//					if (m1d) {
//						res[0] = m1d->getRenderResolutionU();
//					}
//					else if (m2d) {
//						res[0] = m2d->getRenderResolutionU();
//						res[1] = m2d->getRenderResolutionV();
//					}
//					else if (m3d) {
//						res[0] = m3d->getRenderResolutionU();
//						res[1] = m3d->getRenderResolutionV();
//						res[2] = m3d->getRenderResolutionW();
//					}
					
//					_aGRenderVector->setValue( res[0], res[1], res[2] );
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}				
				else if ( strcmp(paramName, "_aFRenderCurrentToggle") == 0 ) {
//					int pos = _aGRenderChoice->getValue();
//					if ( !_aGToRender.has( _aG[pos]->getLabel() ) ) {
//						_aGToRender.push_back( _aG[pos] );
//					}
//					else {
						_aFToRender.erase( _aFRenderChoice->getValue() );
//					}
					_aFRenderCurrentToggle->setValue(false);				

					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();				
				}  					
			}
			//--------------------------------------------------------------------------
			//
			// analyticGeometry param
			//
			if ( _aG.size() != 0 ) {
				if ( strcmp(paramName, "_aGChoice") == 0 ) {
				  int pos = _aGChoice->getValue();
					if ( !_aGToRender.has( _aG[pos]->getLabel() ) ) {
						// set ext rendering for chosen geometry
						_aG[pos]->extRender(true);
						//reset other geometries
						dt__forAllIndex(_aGToRender,ii) _aGToRender[ii]->extRender(false);
						_aGToRender.push_back( _aG[pos] );
					}
					else {
						int toDel = _aGToRender.getPosition( _aG[pos]->getLabel() );
						_aGToRender.erase(toDel);
					}
		      _recreate = false;
		      setExecGracePeriod(0.1);
		      selfExec();
				}		
//				else if ( strcmp(paramName, "_aGRenderClear") == 0 ) {
//					if ( _aGRenderClear->getValue() ) {
//						_aGToRender.clear();
//						_aGRenderClear->setValue(false);
//					  _recreate = false;
//						setExecGracePeriod(0.1);
//						selfExec();			
//					}
//				}
				else if ( strcmp(paramName, "_aGRenderChoice") == 0 ) {
					std::string str(_aGRenderChoice->getActLabel());
					map1dTo3d const * m1d = map1dTo3d::ConstDownCast( _aG.get(str) );
					map2dTo3d const * m2d = map2dTo3d::ConstDownCast( _aG.get(str) );
					map3dTo3d const * m3d = map3dTo3d::ConstDownCast( _aG.get(str) );
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
					
					_aGRenderVector->setValue( res[0], res[1], res[2] );
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}
				else if ( strcmp(paramName, "_aGRenderVector") == 0 ) {
					int pos = _aG.getPosition( std::string(_aGRenderChoice->getActLabel()) );
					map1dTo3d * m1d = map1dTo3d::DownCast(_aG[pos]);
					map2dTo3d * m2d = map2dTo3d::DownCast(_aG[pos]);
					map3dTo3d * m3d = map3dTo3d::DownCast(_aG[pos]);
					if (m1d) {
						m1d->setRenderResolution( 0, static_cast<int>(_aGRenderVector->getValue(0)) );
					}
					else if (m2d) {
						m2d->setRenderResolution( 0, static_cast<int>(_aGRenderVector->getValue(0)) );
						m2d->setRenderResolution( 1, static_cast<int>(_aGRenderVector->getValue(1)) );
					}
					else if (m3d) {
						m3d->setRenderResolution( 0, static_cast<int>(_aGRenderVector->getValue(0)) );
						m3d->setRenderResolution( 1, static_cast<int>(_aGRenderVector->getValue(1)) );
						m3d->setRenderResolution( 2, static_cast<int>(_aGRenderVector->getValue(2)) );
					}
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();			
				}
				else if ( strcmp(paramName, "_aGRenderCurrentToggle") == 0 ) {
//					int pos = _aGRenderChoice->getValue();
//					if ( !_aGToRender.has( _aG[pos]->getLabel() ) ) {
//						_aGToRender.push_back( _aG[pos] );
//					}
//					else {
						_aGToRender.erase( _aGRenderChoice->getValue() );
//					}
					_aGRenderCurrentToggle->setValue(false);				

					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();				
				}  		
		//    else if ( strcmp(paramName, "_aGRenderShowAll") == 0 ) {
		//      _aGRenderShowAll->setValue(false);
		//      dt__forAllIndex(_aG, ii,
		//        _aG[ii]->setRenderResolution(0, 10);
		//			  _aG[ii]->setRenderResolution(1, 10);
		//  			_aG[ii]->setRenderResolution(2, 10);
		//      );
		//			_recreate = false;
		//      setExecGracePeriod(0.1);
		//      selfExec();			
		//    }  				
			}
			//--------------------------------------------------------------------------
			//
			// boundedVolumes param
			//		
			if ( _bV.size() != 0 ) {
				if ( strcmp(paramName, "_bVChoice") == 0 ) {
				  int pos = _bVChoice->getValue();
					if ( !_bVToRender.has( _bV[pos]->getLabel() ) ) {
						_bVToRender.push_back( _bV[pos] );
					}
					else {
						int toDel = _bVToRender.getPosition( _bV[pos]->getLabel() );
						_bVToRender.erase(toDel);
					}
					
					_bVRenderTags->disable();
					
		      _recreate = false;
		      setExecGracePeriod(0.1);
		      selfExec();					
				}
				else if ( strcmp(paramName, "_bVRenderChoice") == 0 ) {
					std::string str(_bVRenderChoice->getActLabel());
					
					boundedVolume const * bV = _bV.get(str);
          std::vector< std::string > tags = bV->getMeshTags();
					abstractModule::updateChoiceParam(_bVRenderTags, &tags);
					bV->extRender(true, abstractModule::blankReConvert(_bVRenderTags->getActLabel()) );
				
					_bVRenderTags->enable();
					
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}
				else if ( strcmp(paramName, "_bVRenderTags") == 0 ) {
					std::string str(_bVRenderChoice->getActLabel());
					
					boundedVolume const * bV = _bV.get(str);
					bV->extRender(true, abstractModule::blankReConvert(_bVRenderTags->getActLabel()) );
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}
//				else if ( strcmp(paramName, "_bVMakeGridChoice") == 0 ) {
//					std::string str(_bVMakeGridChoice->getActLabel());
//					_bVToRender.get(str)->writeGrid();
//					_recreate = false;
//					setExecGracePeriod(0.1);
//					selfExec();		
//				}
//				else if ( strcmp(paramName, "_bVRenderCurrentToggle") == 0 ) {
//					_bVToRender.erase( _bVRenderChoice->getValue() );
//					_bVRenderCurrentToggle->setValue(false);				
//
//					_recreate = false;
//					setExecGracePeriod(0.1);
//					selfExec();				
//				}  		
			}
			//--------------------------------------------------------------------------
			//
			// plugin param
			//		
			if ( _pL.size() != 0 ) {
				if ( strcmp(paramName, "_pLChoice") == 0 ) {
				  int pos = _pLChoice->getValue();
					_pLApply.push_back( _pL[pos] );
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}		
			}			
		}		
		catch (eGeneral & eGenRef) {
			DTCATCHERRORWF(compute(), eGenRef.what());
			return;
		}
  }

  
  int designTool::compute(const char *port) {
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( covise::coModule::getTitle() );
      logFileName = abstractModule::initializeLogFile( "./"+logFileName+".log" );
	    _logName->setValue(logFileName.c_str());
			
			if ( _recreate ) {
				_aGToRender.clear();
				_aFToRender.clear();
				_bVToRender.clear();
				
				_parser.reset( new dtXmlParser() );
				_parser->openFileAndParse( _xmlBrowser->getValue() );
				_parser->destroyAndCreate(_cV, _aF, _bC, _aG, _bV, _dC, _pL);

				abstractModule::updateChoiceParam(_cVChoice, &_cV);

				abstractModule::updateChoiceParam(_aFChoice, &_aF);

				abstractModule::updateChoiceParam(_aGChoice, &_aG);
				
				abstractModule::updateChoiceParam(_bVChoice, &_bV);
				
				abstractModule::updateChoiceParam(_pLChoice, &_pL);
			}
			else {
			  _recreate = true;
			}
			
			//
			// rendering
			//
			if ( _aGToRender.size() == 0 ) {
				_aGRenderChoice->disable();
				_aGRenderCurrentToggle->disable();
//				_aGRenderInfo->disable();
				_aGRenderVector->disable();
			}
			else {
				_aGRenderChoice->enable();
				_aGRenderCurrentToggle->enable();
//				_aGRenderInfo->enable();
				_aGRenderVector->enable();
			  abstractModule::updateChoiceParam(_aGRenderChoice, &_aGToRender);				
			}
			if ( _aFToRender.size() == 0 ) {
				_aFRenderChoice->disable();
				_aFRenderCurrentToggle->disable();
			}
			else {
				_aFRenderChoice->enable();
				_aFRenderCurrentToggle->enable();
			  abstractModule::updateChoiceParam(_aFRenderChoice, &_aFToRender);				
			}			
			if ( _bVToRender.size() == 0 ) {
				_bVRenderChoice->disable();
				_bVRenderCurrentToggle->disable();
			}
			else {
				_bVRenderChoice->enable();
				_bVRenderCurrentToggle->enable();
			  abstractModule::updateChoiceParam(_bVRenderChoice, &_bVToRender);				
			}						
			if (_aFOut->isConnected()) {
				covise::coDoSet * set;
        if (_aFToRender.size() != 0) {
          set  = _aFToRender.render2d( _aFToRender, _aFOut->getObjName() );
        }
				else {
					set = NULL;
				}
				_aFOut->setCurrentObject( set );
      }   
			if (_aGOut->isConnected()) {
				covise::coDoSet * set;
        if (_aGToRender.size() != 0) {
          set  = _aGToRender.render3d( _aGToRender, _aGOut->getObjName() );
        }
				else {
					set = NULL;
				}
				_aGOut->setCurrentObject( set );
      }   			
			if (_bVOut->isConnected()) {
				covise::coDoSet * set = NULL;
        if (_bVToRender.size() != 0) {
					dt__forAllIndex(_bVToRender, ii) {
						boundedVolume * bV = static_cast< boundedVolume * >(_bVToRender[ii]);
						if ( !bV->isMeshed() ) {
							bV->makePreGrid();
							bV->makeGrid();
						}
					}
          set
					= 
					_bVToRender.render3d( 
					  _bVToRender.begin(), _bVToRender.end(),//dt__PRIOR(_bVToRender.end()), 
						_bVOut->getObjName() 
					);
				  _bVOut->setCurrentObject( set );					
        }
      }   			

			dt__forAllIndex( _pLApply, ii) _pLApply[ii]->apply();
			_pLApply.clear();					
			
			abstractModule::closeLogFile();
			
			return CONTINUE_PIPELINE;
		}
		catch (eGeneral & eGenRef) {
			DTCATCHERRORWF(compute(), eGenRef.what());
			
			abstractModule::closeLogFile();
			return STOP_PIPELINE;
		}    		
  }
	
  void designTool::saveCVState(void) {
    if (_parser.get() != NULL ) {
      std::string sLabel = std::string(_cVStateLabel->getValue());
      if (sLabel == "") {
        _parser->openFileAndWrite( _cVStateBrowser->getValue(), &_cV );
      }
      else {
        _cVStateLabel->setValue("");
        _parser->openFileAndWrite( _cVStateBrowser->getValue(), sLabel, &_cV );  
      }
      _cVStateSave->setValue(false);
    }
  }	
	
	void designTool::loadCVStateLabels(void) {
    if (_parser.get() != NULL ) {		
			_parser->openFileAndLoad( _cVStateBrowser->getValue() );
			std::vector< std::string > choices = _parser->getStates();
			_cVStateChoice->setValue(choices.size(), choices, 0);
			_cVStateChoice->enable();
		}
  }

  void designTool::loadCVState(void) {
    if (_parser.get() != NULL ) {			
			std::string stateName = _cVStateChoice->getLabel( _cVStateChoice->getValue() );
			DTINFOWF(loadStateToConst,
							<< "Loading state:" << LOGDEL
							<< DTLOGEVAL(_cVStateChoice->getValue()) << LOGDEL
							<< DTLOGEVAL(stateName) << LOGDEL
							<< " to constValue.");

			_parser->loadStateToConst(stateName, _cV);

			_cVChoice->disable();
			_cVStateLoad->setValue(false);
		}
  }
	
  designTool::~designTool() {
  }
    
}

using namespace covise;
MODULE_MAIN(designTool, dtOO::designTool)
//}
