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
  class dtPlugin;
  class dtXmlParser;
  class renderInterface;
  
  class designTool : public covise::coModule {
    public:
      dt__CLASSNAME(designTool);
      designTool(int argc, char *argv[]);
      virtual void postInst();
      virtual void param(const char* paramName, bool inMapLoading);
      virtual int compute(const char *port);     
      virtual ~designTool();
    private:
      void saveCVState(void);
      void loadCVStateLabels(void);
      void loadCVState(void);    
    private:
      covise::coStringParam * _logName;
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

		  covise::coChoiceParam * _pLChoice;
      
      covise::coOutputPort * _aFOut;  
      covise::coOutputPort * _aGOut;  
      covise::coOutputPort * _bVOut;  
      
		  std::vector< std::string > _moduleChoices;    
      twoDArrayHandling< covise::coUifPara * > _uifPara;
     
      ptrHandling< dtXmlParser > _parser;
      vectorHandling< constValue * > _cV;
      vectorHandling< analyticGeometry * > _aG;
      vectorHandling< analyticFunction * > _aF;
      vectorHandling< boundedVolume * > _bV;
      vectorHandling< dtPlugin * > _pL;
      ptrHandling< baseContainer > _bC;
      vectorHandling< boundedVolume * > _bVMakeGrid;
      coDoSetHandling _aGToRender;
      coDoSetHandling _aFToRender;
      coDoSetHandling _bVToRender;
      
      bool _recreate;
  };
}
#endif