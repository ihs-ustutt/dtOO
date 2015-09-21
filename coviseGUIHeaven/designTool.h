#ifndef _DESIGNTOOL_H
#define _DESIGNTOOL_H

#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include "coDoSetHandling.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace covise {
  class coFileBrowserParam;
  class coBooleanParam;
  class coChoiceParam;
  class coFloatSliderParam;
  class coIntScalarParam;
  class coStringParam;
}

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtCase;
  class dtPlugin;
  class dtXmlParser;
  class renderInterface;
  
  class designTool : public covise::coModule {
    public:
      dt__classOnlyName(designTool);
      designTool(int argc, char *argv[]);
      virtual void postInst();
      virtual void param(const char* paramName, bool inMapLoading);
      virtual int compute(const char *port);     
      virtual ~designTool();
    private:
      void saveCVState(void);
      void loadCVStateLabels(void);
      void loadCVState(void);
      void tryToStoreAGRender();
      void tryToRemakeAGRender(void);      
    private:
      covise::coStringParam * _logName;
      covise::coStringParam * _stateName;
      covise::coChoiceParam * _moduleChoice;
      covise::coFileBrowserParam * _xmlBrowser;

      covise::coBooleanParam * _parseXml;
      covise::coChoiceParam * _cVChoice;    
      covise::coFloatSliderParam * _cVSlider;
      covise::coIntScalarParam * _cVInt;
      covise::coFileBrowserParam * _cVStateBrowser;
      covise::coStringParam * _cVStateLabel;
      covise::coBooleanParam* _cVStateSave;
      covise::coBooleanParam* _cVStateLoad;         
      covise::coChoiceParam* _cVStateChoice;

      covise::coChoiceParam* _aFChoice;
      covise::coChoiceParam * _aFRenderChoice;
      covise::coBooleanParam * _aFRenderCurrentToggle;
    
      covise::coChoiceParam * _aGChoice;
      covise::coChoiceParam * _aGRenderChoice;
      covise::coIntVectorParam * _aGRenderVector;
      covise::coBooleanParam * _aGRenderCurrentToggle;

		  covise::coChoiceParam * _bVChoice;
      covise::coChoiceParam * _bVRenderChoice;
      covise::coChoiceParam * _bVRenderTags;
      covise::coBooleanParam * _bVRenderCurrentToggle;

		  covise::coChoiceParam * _dCChoice;
      covise::coBooleanParam * _dCRunCurrentState;
      covise::coChoiceParam * _dCStateChoice;
      covise::coStringParam * _dCStateString;
      
		  covise::coChoiceParam * _dPChoice;
      
      covise::coOutputPort * _aFOut;  
      covise::coOutputPort * _aGOut;  
      covise::coOutputPort * _bVOut;  
      
		  std::vector< std::string > _moduleChoices;    
      twoDArrayHandling< covise::coUifPara * > _uifPara;
     
      ptrHandling< dtXmlParser > _parser;
      ptrHandling< baseContainer > _bC;      
      vectorHandling< constValue * > _cV;
      vectorHandling< analyticGeometry * > _aG;
      vectorHandling< analyticFunction * > _aF;
      vectorHandling< boundedVolume * > _bV;
      vectorHandling< dtCase * > _dC;
      vectorHandling< dtPlugin * > _dP;
      dtCase * _dCApply;
      coDoSetHandling _aGToRender;
      coDoSetHandling _aFToRender;
      coDoSetHandling _bVToRender;
      
      bool _recreate;
      
      std::vector< std::string > _memento;
  };
}
#endif
