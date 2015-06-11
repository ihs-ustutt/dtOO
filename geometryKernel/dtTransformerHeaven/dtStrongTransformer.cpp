#include "dtStrongTransformer.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <QtXml/QDomElement>

namespace dtOO {
  dtStrongTransformer::dtStrongTransformer() : dtTransformer() {
  }

  dtStrongTransformer::~dtStrongTransformer() {
  }
	
	dtStrongTransformer::dtStrongTransformer(
    dtStrongTransformer const & orig
  ) : dtTransformer(orig) {
		
	}

	dtVector3 dtStrongTransformer::apply(dtVector3 const & pp) const {
		std::vector< dtVector3 > vec;
		vec.push_back( dtVector3(pp) );
		
		std::vector< dtVector3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}

	dtVector3 dtStrongTransformer::retract(dtVector3 const & pp) const {
		std::vector< dtVector3 > vec;
		vec.push_back( dtVector3(pp) );
		
		std::vector< dtVector3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}
}

