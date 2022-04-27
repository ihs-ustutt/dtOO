#include "designTool.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <constValueHeaven/constValue.h>
#include <constValueHeaven/intParam.h>
#include <constValueHeaven/sliderFloatParam.h>
#include <constValueHeaven/constrainedFloatParam.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <dtPlugin.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <resultValueHeaven/resultValue.h>
#include <xmlHeaven/dtXmlParser.h>
#include <exceptionHeaven/eGeneral.h>
#include <baseContainerHeaven/baseContainer.h>
#include <do/coDoSet.h>
#include <abstractModule.h>

namespace dtOO {
  designTool::designTool(
    int argc, char *argv[]) : covise::coModule(argc, argv, "designTool"
  ) {
    _logFileName = "dtOODesignTool";
    
    _logName = addStringParam("_logName", "_logNameDescription");
    _infoStr = addStringParam("_infoStr", "_infoStrDescription");    
    _stateName = addStringParam("_stateName", "_stateNameDescription");
    _clearLog = addBooleanParam("_clearLog", "_clearLogDescription");
    _clearLog->setValue(false);    
		_moduleChoice = addChoiceParam("_moduleChoice", "_moduleChoiceDescription");		
    _xmlBrowser = addFileBrowserParam("_xmlBrowser", "_xmlBrowserDescrition");
    _xmlBrowser->setValue("./machine.xml", "*.xml/*");
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
    _cVStateBrowser 
    = 
    addFileBrowserParam("_cVStateBrowser", "_cVStateBrowserDescription");
    _cVStateBrowser->setValue("./machineSave.xml", "*.xml/*");
    _cVStateBrowser->disable();
    _cVStateLabel = addStringParam("_cVStateLabel", "_cVStateLabelDescription");
    _cVStateLabel->setValue("");
    _cVStateSave = addBooleanParam("_cVStateSave", "_cVStateSaveDescription");
    _cVStateSave->setValue(false);    
    _cVStateLoad = addBooleanParam("_cVStateLoad", "_cVStateLoadDescription");
    _cVStateLoad->setValue(false);
    _cVStateChoice 
    = 
    addChoiceParam("_cVStateChoice", "_cVStateChoiceDescription");
    _cVStateChoice->disable();

    _aFChoice = addChoiceParam("_aFChoice", "_aFChoiceDescription");
    _aFChoice->disable();
    _aFRenderChoice 
    = 
    addChoiceParam("_aFRenderChoice", "_aFRenderChoiceDescription");
    _aFRenderChoice->disable();
    _aFRenderCurrentToggle 
    = 
    addBooleanParam(
      "_aFRenderCurrentToggle", "_aFRenderCurrentToggleDescription"
    );
    _aFRenderCurrentToggle->disable();
    _aFRenderCurrentToggle->setValue(false);	
		
		_aGChoice = addChoiceParam("_aGChoice", "_aGChoiceDescription");
    _aGChoice->disable();
    _aGRenderChoice 
    = 
    addChoiceParam("_aGRenderChoice", "_aGRenderChoiceDescription");
    _aGRenderChoice->disable();
    _aGRenderVector 
    = 
    addInt32VectorParam("_aGRenderVector", "_aGRenderVectorDescription");
    _aGRenderVector->disable();
    _aGRenderCurrentToggle 
    = 
    addBooleanParam(
      "_aGRenderCurrentToggle", "_aGRenderCurrentToggleDescription"
    );
    _aGRenderCurrentToggle->disable();
    _aGRenderCurrentToggle->setValue(false);
    _aGRenderFilterString
    = 
    addStringParam("_aGRenderFilterString", "_aGRenderFilterStringDescription");
    _aGRenderFilterString->setValue("");
    _aGRenderFilterString->disable();
    _aGRenderFilterChoice 
    = 
    addChoiceParam(
      "_aGRenderFilterChoice", "_aGRenderFilterChoiceDescription"
    );
    _aGRenderFilterChoice->disable();

		_bVChoice = addChoiceParam("_bVChoice", "_bVChoiceDescription");
    _bVChoice->disable();
    _bVRenderChoice 
    = 
    addChoiceParam("_bVRenderChoice", "_bVRenderChoiceDescription");
    _bVRenderChoice->disable();
    _bVRenderTags = addChoiceParam("_bVRenderTags", "_bVRenderTagsDescription");
    _bVRenderTags->disable();
    _bVRenderCurrentToggle 
    = 
    addBooleanParam(
      "_bVRenderCurrentToggle", "_bVRenderCurrentToggleDescription"
    );
    _bVRenderCurrentToggle->disable();
    _bVRenderCurrentToggle->setValue(false);	
		_bVRenderString 
    = 
    addStringParam("_bVRenderString", "_bVRenderStringDescription");
    _bVRenderString->disable();
    _bVRenderString->setValue("");
		_dCChoice = addChoiceParam("_dCChoice", "_dCChoiceDescription");
    _dCChoice->disable();
    _dCRunCurrentState 
    = 
    addBooleanParam("_dCRunCurrentState", "_dCRunCurrentStateDescription");
    _dCRunCurrentState->setValue(false);
		_dCStateChoice 
    = 
    addChoiceParam("_dCStateChoice", "_dCStateChoiceDescription");
    _dCStateChoice->disable();
    _dCStateString 
    = 
    addStringParam("_dCStateString", "_dCStateStringDescription");
    _dCStateString->disable();
    _dCStateResValueChoice 
    = 
    addChoiceParam(
      "_dCStateResValueChoice", "_dCStateResValueChoiceDescription"
    );
    _dCStateResValueChoice->disable();
    _dCStateResValue 
    = 
    addFloatParam("_dCStateResValue", "_dCStateResValueDescription");
    _dCStateResValue->disable();
    
		_dPChoice = addChoiceParam("_dPChoice", "_dPChoiceDescription");
    _dPChoice->disable();
		
		_aFOut = addOutputPort("_aFOut", "Vec2", "_aFOutDescription");
    _aGOut 
    = 
    addOutputPort(
      "_aGOut", "Points|Lines|Polygons|StructuredGrid", "_aGOutDescription"
    );				
		_bVOut 
    = 
    addOutputPort(
      "_bVOut", 
      "UnstructuredGrid|Points|Lines|Polygons|StructuredGrid", 
      "_bVOutDescription"
    );				
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
    tmp.push_back( _aGRenderFilterChoice );	
    tmp.push_back( _aGRenderFilterString );
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
    tmp.push_back( _bVRenderString );
		_uifPara.push_back( tmp );		
		//
		// case
		//
		tmp.clear();
		_moduleChoices.push_back("dCGen");
		tmp.push_back( _dCChoice );
    tmp.push_back( _dCRunCurrentState );
    tmp.push_back( _dCStateChoice );
    tmp.push_back( _dCStateString );
    tmp.push_back( _dCStateResValueChoice );
    tmp.push_back( _dCStateResValue );
		_uifPara.push_back( tmp );    
    
		//
		// plugin
		//
		tmp.clear();
		_moduleChoices.push_back("dPGen");
		tmp.push_back( _dPChoice );
		_uifPara.push_back( tmp );
		
		_recreate = true;
    
    //
    // create baseContainer
    //
    _bC.reset( new baseContainer() );   
  }

	void designTool::postInst() {
		abstractModule::updateChoiceParam(_moduleChoice, &_moduleChoices);
    _moduleChoice->show();
		_xmlBrowser->show();
    _logName->show();
		_logName->disable();
    _logName->setValue("");
    _infoStr->show();
		_infoStr->disable();        
    _infoStr->setValue("");
    _stateName->show();
		_stateName->disable();    
    _stateName->setValue("");
    _clearLog->show();
    _dCStateString->disable();
    //
    // init log file
    //
    std::string logFileName 
    = 
    abstractModule::initializeLogFile( "./"+_logFileName+".log" );
    _logName->setValue(logFileName.c_str()); 
    
    _moduleChoice->setValue(0);
    param("_moduleChoice", false);
	}
	
  void designTool::param(const char* paramName, bool inMapLoading) {
    _infoStr->setValue("");
		try {
      if ( strcmp(paramName, "_clearLog") == 0 ) {
        if ( _clearLog->getValue() ) {
          std::string logFileName 
          = 
          abstractModule::initializeLogFile( "./"+_logFileName+".log" );
          _logName->setValue(logFileName.c_str());  
          
          _clearLog->setValue(false);          
        }
        else {
          _clearLog->setValue(false);
        }        
      }
			else if (strcmp(paramName, "_parseXml") == 0) {
					if ( _parseXml->getValue() ) {
            _parser.reset();
						_parseXml->setValue(false);
            _xmlBrowser->enable();
            _cVStateBrowser->enable();
					}		
				}      
      
			if (strcmp(paramName, "_moduleChoice") == 0) {
        dt__forAllIndex(_uifPara, jj) {
          dt__forAllIndex( _uifPara[jj], ii) _uifPara[jj][ii]->hide();
        }
        int const mChoice = _moduleChoice->getValue();
			  dt__forAllIndex(_uifPara[mChoice], ii) _uifPara[mChoice][ii]->show();
			}
			//------------------------------------------------------------------------
			//
			// constValue param
			//
			if ( _cV.size() != 0 ) {
				//
				// update slider parameter in GUI
				//
				if (strcmp(paramName, "_cVChoice") == 0) {
					std::string label = _cVChoice->getActLabel();
					constValue const * cVptr = _cV.get(label);
					sliderFloatParam const * sfp = sliderFloatParam::ConstDownCast(cVptr);
					intParam const * ip = intParam::ConstDownCast(cVptr);
          constrainedFloatParam const * cfp 
          = 
          constrainedFloatParam::ConstDownCast(cVptr);
					
					if (ip) {
            _infoStr->setValue(ip->className().c_str());
						_cVSlider->disable();
						_cVInt->enable();
						_cVInt->setValue(ip->getValue());
					}
					else if (sfp) {
            _infoStr->setValue(sfp->className().c_str());
						_cVSlider->enable();
						_cVInt->disable();
						_cVSlider->setValue(sfp->getMin(), sfp->getMax(), sfp->getValue());				
					}
					else if (cfp) {
            _infoStr->setValue(cfp->className().c_str());
            _cVSlider->enable();
						_cVSlider->setValue(cfp->getMin(), cfp->getMax(), cfp->getValue());				
            _cVSlider->disable();
            _cVInt->disable();
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
					setExecGracePeriod(0.1);
				  selfExec();               
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
			//------------------------------------------------------------------------
			//
			// analyticFunction param
			//
			if ( _aF.size() != 0 ) {		
				if(strcmp(paramName, "_aFChoice") == 0) {    
				  int pos = _aFChoice->getValue();
					if ( !_aFToRender.has( _aF[pos]->getLabel() ) ) {
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
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}				
				else if ( strcmp(paramName, "_aFRenderCurrentToggle") == 0 ) {
					_aFToRender.erase( _aFRenderChoice->getValue() );
					_aFRenderCurrentToggle->setValue(false);				
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();				
				}  					
			}
			//------------------------------------------------------------------------
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
					int pos 
          = 
          _aG.getPosition( std::string(_aGRenderChoice->getActLabel()) );
					map1dTo3d * m1d = map1dTo3d::DownCast(_aG[pos]);
					map2dTo3d * m2d = map2dTo3d::DownCast(_aG[pos]);
					map3dTo3d * m3d = map3dTo3d::DownCast(_aG[pos]);
					if (m1d) {
						m1d->setRenderResolution( 
              0, static_cast<int>(_aGRenderVector->getValue(0)) 
            );
					}
					else if (m2d) {
						m2d->setRenderResolution( 
              0, static_cast<int>(_aGRenderVector->getValue(0)) 
            );
						m2d->setRenderResolution( 
              1, static_cast<int>(_aGRenderVector->getValue(1)) 
            );
					}
					else if (m3d) {
						m3d->setRenderResolution( 
              0, static_cast<int>(_aGRenderVector->getValue(0)) 
            );
						m3d->setRenderResolution( 
              1, static_cast<int>(_aGRenderVector->getValue(1))
            );
						m3d->setRenderResolution(
              2, static_cast<int>(_aGRenderVector->getValue(2)) 
            );
					}
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();			
				}
				else if ( strcmp(paramName, "_aGRenderCurrentToggle") == 0 ) {
  				_aGToRender.erase( _aGRenderChoice->getValue() );
					_aGRenderCurrentToggle->setValue(false);				
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();				
				}  		
		    else if ( strcmp(paramName, "_aGRenderFilterString") == 0 ) {
          _aGRenderFilterChoice->disable();
          std::string pattern(
            abstractModule::blankReConvert(
              _aGRenderFilterString->getValString()
            )
          );
          std::vector<std::string> labels;
          _AGFilterPos.clear();
		      dt__forAllIndex(_aG, ii) {
            if ( _parser->matchWildcard( pattern, _aG[ii]->getLabel() ) ) {
              labels.push_back(_aG[ii]->getLabel());
              _AGFilterPos.push_back(ii);
            }
          }
          _aGRenderFilterChoice->setValue(labels.size(), labels, 0);  
          _infoStr->setValue(
            std::string(
              _parser->intToString(labels.size())
              +
              std::string(" analyticGeometries in filter")
            ).c_str()
          );          
          if ( !labels.empty() ) {            
            _aGRenderFilterChoice->enable();
          }			
		    }  				
				else if ( strcmp(paramName, "_aGRenderFilterChoice") == 0 ) {
				  int pos = _AGFilterPos[ _aGRenderFilterChoice->getValue() ];
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
			}
			//------------------------------------------------------------------------
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
					
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}
				else if ( strcmp(paramName, "_bVRenderTags") == 0 ) {
					std::string str(_bVRenderChoice->getActLabel());
					
					boundedVolume const * bV = _bV.get(str);
					bV->extRender(
            true, abstractModule::blankReConvert(_bVRenderTags->getActLabel()) 
          );
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}
        else if ( strcmp(paramName, "_bVRenderString") == 0 ) {          
          if ( _bVToRender.size() != 0 ) {
            std::string str(_bVRenderChoice->getActLabel());
            boundedVolume const * bV = _bV.get(str);
            bV->extRender(
              true, 
              abstractModule::blankReConvert( 
                _bVRenderString->getValString() 
              ) 
            );      
          }
          _recreate = false;
          setExecGracePeriod(0.1);
          selfExec();		              
        }
			}
			//------------------------------------------------------------------------
			//
			// case param
			//		
			if ( _dC.size() != 0 ) {
				if ( 
          (strcmp(paramName, "_dCChoice") == 0)
          ||
          (strcmp(paramName, "_dCRunCurrentState") == 0)
        ) {
				  int pos = _dCChoice->getValue();
					_dCApply = _dC[pos];
					_recreate = false;
					setExecGracePeriod(0.1);
					selfExec();		
				}		
				else if ( strcmp(paramName, "_dCStateChoice") == 0 ) {
          _dCStateString->setValue(
            _dC[ _dCChoice->getValue() ]->statusStr( 
              std::string(_dCStateChoice->getActLabel()) 
            ).c_str()
          );
          vectorHandling< resultValue * > result
          =
          _dC[ _dCChoice->getValue() ]->result(
            std::string(_dCStateChoice->getActLabel()) 
          );
          abstractModule::updateChoiceParam(
            _dCStateResValueChoice, &result
          );  
          result.destroy();
				}		        
				else if ( strcmp(paramName, "_dCStateResValueChoice") == 0 ) {
          labeledVectorHandling< resultValue * > result
          =
          _dC[ _dCChoice->getValue() ]->result(
            std::string(_dCStateChoice->getActLabel()) 
          );
          _dCStateResValue->setValue( 
            result.get(
              std::string(_dCStateResValueChoice->getActLabel())
            )->operator()()
          );
          result.destroy();
				}		        
			}	      
			//------------------------------------------------------------------------
			//
			// plugin param
			//		
			if ( _dP.size() != 0 ) {
				if ( strcmp(paramName, "_dPChoice") == 0 ) {
				  int pos = _dPChoice->getValue();
					_dP[pos]->apply();
					_recreate = true;
					setExecGracePeriod(0.1);
					selfExec();		
				}		
			}			      
		}		
		catch (std::exception & eGenRef) {
      _infoStr->setValue( "catchException" );           
			dt__catch(param(), eGenRef.what());
		}
    catch(std::string & caught) {
      _infoStr->setValue("catchString");
      dt__catch(param(), caught.c_str());
    }        
    catch(...) {
      _infoStr->setValue("catchUnknown");
      dt__catch(param(), "Unknown exception type");
    }        
    //
    // update state label
    //
    if (!_parser.isNull()) {
      _stateName->setValue(_parser->currentState().c_str());
    }
    return;
  }

  
  int designTool::compute(const char *port) {
    _infoStr->setValue("");
    try {    			
			if ( _recreate ) {
				//
				// try to store visualization
				//
				tryToStore();
				
				_aGToRender.clear();
				_aFToRender.clear();
				_bVToRender.clear();
				
        if (_parser.isNull()) {
          _parser.reset( 
            dtXmlParser::init(
              std::string(_xmlBrowser->getValue()), 
              std::string(_cVStateBrowser->getValue())
            ) 
          );
				  _parser->parse();          
          _xmlBrowser->disable();
          _cVStateBrowser->disable();
          _cV.clear();
        }
				_parser->destroyAndCreate(*_bC, _cV, _aF, _aG, _bV, _dC, _dP);

				//
				// try to remake visualization
				//				
				tryToRemake();
				
				abstractModule::updateChoiceParam(_cVChoice, &_cV);

				abstractModule::updateChoiceParam(_aFChoice, &_aF);

				abstractModule::updateChoiceParam(_aGChoice, &_aG);
				
				abstractModule::updateChoiceParam(_bVChoice, &_bV);
				
        abstractModule::updateChoiceParam(_dCChoice, &_dC);
        
				abstractModule::updateChoiceParam(_dPChoice, &_dP);
        // update filter for analyticGeometries
        param(_aGRenderFilterString->getName(), true);
			}
			else _recreate = true;
      
      //
      // update state label
      //
      _stateName->setValue(_parser->currentState().c_str());
      
      //
      // update case states
      //
      if (_dCApply) {
        _dCApply->update();
        abstractModule::updateChoiceParam(
          _dCStateChoice, _dCApply->allStates()
        );
        if (_dCRunCurrentState->getValue()) {
          _dCRunCurrentState->disable();
          _dCApply->runCurrentState();
          _dCRunCurrentState->setValue(false);
          _dCRunCurrentState->enable();
        }
        _dCApply = NULL;
      }
      
			//
			// rendering
			//
			if ( _aGToRender.size() == 0 ) {
				_aGRenderChoice->disable();
				_aGRenderCurrentToggle->disable();
				_aGRenderVector->disable();
			}
			else {
				_aGRenderChoice->enable();
				_aGRenderCurrentToggle->enable();
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
						boundedVolume * bV 
            = 
            static_cast< boundedVolume * >(_bVToRender[ii]);
						if ( !bV->isMeshed() ) {
							bV->makePreGrid();
							bV->makeGrid();
						}
					}
          set
					= 
					_bVToRender.render3d( 
					  _bVToRender.begin(), _bVToRender.end(),
						_bVOut->getObjName() 
					);
				  _bVOut->setCurrentObject( set );					
        }
      }   			

			return CONTINUE_PIPELINE;
		}
		catch (std::exception & eGenRef) {
      _infoStr->setValue( "catchException" );      
      
			dt__catch(compute(), eGenRef.what());
			
			return STOP_PIPELINE;
		}
    catch(std::string & caught) {
      _infoStr->setValue("catchString");
      
      dt__catch(compute(), caught.c_str());
      
      return STOP_PIPELINE;
    }        
    catch(...) {
      _infoStr->setValue("catchUnknown");
      
      dt__catch(compute(), "Unknown exception type");
      
      return STOP_PIPELINE;
    }    
  }
	
  void designTool::saveCVState(void) {
    if (_parser.get() != NULL ) {
      std::string sLabel = std::string(_cVStateLabel->getValue());
      if (sLabel == "") sLabel = NowDateAndTime();
      _cVStateLabel->setValue("");
      _parser->write( sLabel, &_cV );  
      _cVStateSave->setValue(false);
      _stateName->setValue(_parser->currentState().c_str());
    }
  }	
	
	void designTool::loadCVStateLabels(void) {
    if (_parser.get() != NULL ) {		
			_parser->load();
			std::vector< std::string > choices = _parser->getStates();
			_cVStateChoice->setValue(choices.size(), choices, 0);
			_cVStateChoice->enable();
		}
  }

  void designTool::loadCVState(void) {
    if (_parser.get() != NULL ) {			
			std::string stateName 
      = 
      _cVStateChoice->getLabel( _cVStateChoice->getValue() );
			dt__info(
        loadCVState,
        << "Loading state:" << std::endl
        << dt__eval(_cVStateChoice->getValue()) << std::endl
        << dt__eval(stateName) << std::endl
        << " to constValue."
      );

			_parser->loadStateToConst(stateName, _cV);

			_cVStateChoice->disable();
			_cVStateLoad->setValue(false);
      _stateName->setValue(_parser->currentState().c_str());
		}
  }
	
  designTool::~designTool() {
	  abstractModule::closeLogFile();    
  }
	
	void designTool::tryToStore( void ) {
    _AGmemento.clear();
    _AFmemento.clear();
		if ( !_aGToRender.empty() ) {
      dt__forAllConstIter(coDoSetHandling, _aGToRender, it) {
        labelHandling const * const labelIt 
        = 
        dynamic_cast< labelHandling const * const >(*it);
        if (labelIt) _AGmemento.push_back(labelIt->getLabel());
      }
    }
		if ( !_aFToRender.empty() ) {
      dt__forAllConstIter(coDoSetHandling, _aFToRender, it) {
        labelHandling const * const labelIt 
        = 
        dynamic_cast< labelHandling const * const >(*it);
        if (labelIt) _AFmemento.push_back(labelIt->getLabel());
      }
    }    
	} 

	void designTool::tryToRemake( void ) {
		dt__forAllConstIter(std::vector< std::string >, _AGmemento, it) {
		  if (_aG.has(*it) ) _aGToRender.push_back( _aG.get(*it) );
		}
		dt__forAllIndex(_aGToRender,ii) _aGToRender[ii]->extRender(false);
		dt__forAllConstIter(std::vector< std::string >, _AFmemento, it) {
		  if (_aF.has(*it) ) _aFToRender.push_back( _aF.get(*it) );
		}
		dt__forAllIndex(_aFToRender,ii) _aFToRender[ii]->extRender(false);
	}
}

using namespace covise;
MODULE_MAIN(designTool, dtOO::designTool)
