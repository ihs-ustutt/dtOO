#ifndef _GEOGEN_H
#define _GEOGEN_H

#define _USE_MATH_DEFINES 
#include <string>
#include <math.h>
#include <vector>
#include <api/coModule.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <abstractModule.h>

namespace dtOO {
  class scaFunction;
  class constValue;
  class analyticGeometry;
  class pointContainer;
  class vectorContainer;
  class dtXmlParser;

  class geoGen : public covise::coModule,
                 public abstractModule {
    public:
      dt__CLASSNAME(geoGen);
    private:
      //
      //covise attributes
      //
      /* -------------------------------------------------------------------- */
      /* parsing */
      /* -------------------------------------------------------------------- */    
      covise::coFileBrowserParam* _p_xmlFilebrowser;

      covise::coChoiceParam * _p_singleDetailsChoiceParam;
      covise::coBooleanParam * _p_passThrough;
      covise::coChoiceParam * _p_renderOptionsChoiceParam;
      covise::coIntVectorParam * _p_renderOptionsVectorParam;
      covise::coBooleanParam * _p_hideAll;
      /* -------------------------------------------------------------------- */
      /* input ports */
      /* -------------------------------------------------------------------- */
      covise::coInputPort * _pIn_constValueP;
      covise::coInputPort * _pIn_sFunP;
      covise::coInputPort * _pIn_anGeoP;
      /* -------------------------------------------------------------------- */
      /* output ports */
      /* -------------------------------------------------------------------- */
      covise::coOutputPort* _pOut_partP;
      covise::coOutputPort* _pOut_pointP;   
      covise::coOutputPort* _pOut_vectorP;
      covise::coOutputPort* _pOut_singleDetailsP;
      covise::coOutputPort* _pOut_anGeoP;
      
      /* -------------------------------------------------------------------- */
      /* parts, geometries,functions, ... */
      /* -------------------------------------------------------------------- */
      coDoSetHandling< analyticGeometry* > _aGeo;
      coDoSetHandling< scaFunction* > _sFun;
      coDoSetHandling< constValue* > _cVal;
      pointContainer * _pointContainerP;
      vectorContainer * _vectorContainerP;
      std::vector<analyticGeometry*> * _singleDetailsP;
      
      bool _recreateParts;
      twoDArrayHandling< int > _renderResolution;
  public:
       /**
        * overload compute of coModule
        */
       virtual int compute(const char *port);     
       /**
        * constructor
        */
       geoGen(int argc, char *argv[]);
       /**
        * param
        */
       virtual void param(const char* paramName, bool inMapLoading);
       /**
        *destructor 
        */
       virtual ~geoGen();
  private:
      void renderMe(void);     
      void updateSingleDetailChoiceParam(vectorHandling< analyticGeometry * > * vecHP);
  };
}
#endif
