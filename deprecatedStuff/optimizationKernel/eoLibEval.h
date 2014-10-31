#ifndef EOLIBEVAL_H
#define	EOLIBEVAL_H

#include "optimizableInterface.h"
#include <interfaceHeaven/vectorHandling.h>
#include <constValueHeaven/constValue.h>

#include <eo>
#include <es.h>

namespace dtOO {
  template < class EOT >
  class eoLibEval : public eoEvalFunc< EOT > {
    private:
      optimizableInterface * _optIntP;
    public:
      eoLibEval() {};
      eoLibEval( optimizableInterface * const optIntP ) {
        _optIntP = optIntP;
      }
    void operator()(EOT & _eo) {
      for (int ii = 0; ii < _eo.size(); ii++) {
        float min = _optIntP->getRefToParameters()[ii]->getMin();
        float max = _optIntP->getRefToParameters()[ii]->getMax();
        _optIntP->getRefToParameters()[ii]->setValue( min + (max - min) * _eo[ii] );
      }
      double sum = static_cast< double >( _optIntP->characterizeMe() );
      _eo.fitness(sum);
    }
  };
}

#endif	/* EOLIBEVAL_H */

