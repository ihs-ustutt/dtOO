#ifndef OPTIMIZER_H
#define	OPTIMIZER_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/optimizationHandling.h>
//#include <interfaceHeaven/optimizationCyclingDecorator.h>
#include <dtXmlParserDecorator/qtXmlPrimitive.h>
#include <dtLinearAlgebra.h>

namespace dtOO {

  extern "C" {
    void f_optim_1_4_(void);
    void f_optim_1_3_(void);
//    void testing_();
    void c_optimizer_callback_(void);  
  }
  class analyticGeometry;
  class constValue;
  class scaFunction;
  class boundedVolume;
  class optimizationCyclingDecorator;
  class optimizationCycling;

  class optimizer : public labelHandling,
                    public optionHandling,
                    public qtXmlPrimitive {
  public:
    DTCLASSNAMEMETHOD(optimizer);
    DTCLASSLOGMETHOD(optimizer);
    optimizer();
    optimizer(const optimizer& orig);
    virtual ~optimizer();
    void optimizeMe(void);
    void cycle( void );
    void init( 
      QDomElement const & element,
      vectorHandling< constValue * > * const cValP,
      vectorHandling< analyticGeometry * > * const aGeoP,
      vectorHandling< boundedVolume * > * const bVolP,
      optimizationCycling * firstOptiCycling
    );    
    void dump(void);
    void updateToBestDesign(void);
    void registrateScaFunctions(vectorHandling< scaFunction * > * sFunP);
    std::vector< dtPoint2 > getDesignsResult(void);
    std::vector< dtPoint2 > getDesignsParameter(int const pIndex);
  private:
    void writeOptiStf(void);
    void readOptiParamAndWriteRes(void);
    void readOptiParamAndWriteResPara(void);
  private:
    static optimizationCyclingDecorator * _optiCyclingDecorator;
    static std::vector< constValue * > * _cValP;
    static optimizationHandling *  _optiHandling;
    std::string _scaFuncName;
  };
}
#endif	/* OPTIMIZER_H */

