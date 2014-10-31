#include "optimizationCyclingDecorator.h"

#include <logMe/logMe.h>

namespace dtOO {
  optimizationCyclingDecorator::optimizationCyclingDecorator() {
  }

  optimizationCyclingDecorator::optimizationCyclingDecorator(const optimizationCyclingDecorator& orig) {
  }
  
//  optimizationCyclingDecorator::optimizationCyclingDecorator(optimizationCycling * inner) {
//    m_wrappee = inner;
//  }

  optimizationCyclingDecorator::~optimizationCyclingDecorator() {
    
  }
  
  void optimizationCyclingDecorator::doCycle( void ) {
    m_wrappee->doCycle();
  }
  
  void optimizationCyclingDecorator::enqueDecorator(optimizationCycling * inner) {
    m_wrappee = inner;
  }  
}