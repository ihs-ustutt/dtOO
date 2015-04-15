#ifndef MESHER_H
#define MESHER_H

#include <string.h>
#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class analyticGeometry;
  class constValue;
  class boundedVolume;
  class dtXmlParser;
  class scaFunction;
  
  class mesher : public covise::coModule,
                 public abstractModule {
    public:
      dt__classOnlyName(mesher);
      mesher(int argc, char *argv[]);
      vectorHandling< boundedVolume * > & getRefToBoundedVolume( void );
      virtual int compute(const char *);
//      virtual void param(const char *, bool);
    private:
      void createBoundedVolumes(void);
    private:
      covise::coInputPort * _pIn_constValueP;
      covise::coInputPort * _pIn_sFunP;
      covise::coInputPort * _pIn_anGeometryP;
      covise::coOutputPort * _pOut_boundedVolumeP;
      covise::coOutputPort * _pOut_unGridP;
      covise::coOutputPort * _pOut_extInfoContainerP;
      covise::coOutputPort * _pOut_surfGridP;
//      covise::coOutputPort * _pOut_optiControlP;
//      covise::coOutputPort * _pOut_bocoP;
      
      covise::coFileBrowserParam* _p_xmlFilebrowser;
      
      covise::coIntScalarParam * _p_boundFaceNum;
      covise::coBooleanParam * _p_createBVol;
//      covise::coBooleanParam * _p_newResult;
//      covise::coFloatParam * _p_resultValue;
      
      dtXmlParser * _parserP;
      coDoSetHandling< analyticGeometry * > _aGeo;
      coDoSetHandling< constValue * > _cVal;
      vectorHandling< boundedVolume * > _bVol;
      coDoSetHandling< scaFunction * > _sFun;
  };
}
#endif
