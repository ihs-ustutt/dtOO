#include "analyticGeometryDirector.h"

namespace dtOO {
  DTCLASSLOGMETHODI(analyticGeometryDirector,
          << DTLOGEVAL( &(thisRef._builder) ) );

  analyticGeometryDirector::analyticGeometryDirector() {
  }

  analyticGeometryDirector::~analyticGeometryDirector() {
  }

  void analyticGeometryDirector::setBuilder(analyticGeometryBuilder* _builderToSet) {
    _builder = _builderToSet;
  }

  int analyticGeometryDirector::construct(vectorHandling< analyticGeometry* >* aGeo) {
    int errFlag;

    errFlag = _builder->buildPart();
    _builder->getResult(aGeo);
    _builder->final();
  }
}

