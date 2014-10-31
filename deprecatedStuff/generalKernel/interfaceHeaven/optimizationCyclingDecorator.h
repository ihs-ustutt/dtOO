#ifndef OPTIMIZATIONCYCLINGDECORATOR_H
#define	OPTIMIZATIONCYCLINGDECORATOR_H

#include <interfaceHeaven/optimizationCycling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class optimizationCyclingDecorator : public optimizationCycling {
  public:
    dt__CLASSNAME(optimizationCyclingDecorator);
    optimizationCyclingDecorator();
    optimizationCyclingDecorator(const optimizationCyclingDecorator& orig);
//    optimizationCyclingDecorator(optimizationCycling * inner);
    virtual ~optimizationCyclingDecorator();
    virtual void doCycle( void );
    virtual void enqueDecorator( optimizationCycling * inner );
  private:
    optimizationCycling * m_wrappee;
  };
}
#endif	/* OPTIMIZATIONCYCLINGDECORATOR_H */

