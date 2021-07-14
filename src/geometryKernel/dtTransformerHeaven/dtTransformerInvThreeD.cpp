#include "dtTransformerInvThreeD.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <QtXml/QDomElement>

namespace dtOO { 
  dtTransformerInvThreeD::dtTransformerInvThreeD() : dtTransformer() {
  }

  dtTransformerInvThreeD::~dtTransformerInvThreeD() {
  }
	
	dtTransformerInvThreeD::dtTransformerInvThreeD(
    dtTransformerInvThreeD const & orig
  ) : dtTransformer(orig) {
		
	}

	dtVector3 dtTransformerInvThreeD::apply(dtVector3 const & pp) const {
		std::vector< dtVector3 > vec;
		vec.push_back( dtVector3(pp) );
		
		std::vector< dtVector3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}

	dtVector3 dtTransformerInvThreeD::retract(dtVector3 const & pp) const {
		std::vector< dtVector3 > vec;
		vec.push_back( dtVector3(pp) );
		
		std::vector< dtVector3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}
  
	dtPoint3 dtTransformerInvThreeD::apply(dtPoint3 const & pp) const {
		std::vector< dtPoint3 > vec;
		vec.push_back( dtPoint3(pp) );
		
		std::vector< dtPoint3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}

	dtPoint3 dtTransformerInvThreeD::retract(dtPoint3 const & pp) const {
		std::vector< dtPoint3 > vec;
		vec.push_back( dtPoint3(pp) );
		
		std::vector< dtPoint3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}
  
	std::vector< dtVector3 > dtTransformerInvThreeD::apply(
    std::vector< dtVector3 > const & pp
  ) const {
    return apply(&pp);
	}

	std::vector< dtVector3 > dtTransformerInvThreeD::retract(
    std::vector< dtVector3 > const & pp
  ) const {
		return retract(&pp);
	}

	std::vector< dtPoint3 > dtTransformerInvThreeD::apply(
    std::vector< dtPoint3 > const & pp
  ) const {
    return apply(&pp);
	}

	std::vector< dtPoint3 > dtTransformerInvThreeD::retract(
    std::vector< dtPoint3 > const & pp
  ) const {
		return retract(&pp);
	}  
}

