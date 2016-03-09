#include "dtTransformer.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <QtXml/QDomElement>

namespace dtOO {
  dtTransformer::dtTransformer() : labelHandling(), optionHandling() {
  }

  dtTransformer::~dtTransformer() {
  }
	
	dtTransformer::dtTransformer(
    dtTransformer const & orig
  ) : labelHandling(orig) {
		
	}

  void dtTransformer::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG
	) {
		if ( dtXmlParserBase::hasAttribute("label", *tE) ) {
			labelHandling::setLabel(dtXmlParserBase::getAttributeStr("label", *tE));
		}
    optionHandling::init(*tE, bC, cV, aF, aG);
  }
    
  void dtTransformer::init( 
	  ::QDomElement const * tE,
	  baseContainer const * const bC, 
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP
	) {
    init(tE, bC, cValP, sFunP, NULL);
  }
  
  std::vector< dtPoint2 > dtTransformer::apply( 
    std::vector< dtPoint2 > const * const pointVecP 
  ) const {
    dt__warning(init(), << "Call on abstract class!");
  }
 
  std::vector< dtPoint3 > dtTransformer::apply( 
    std::vector< dtPoint3 > const * const pointVecP 
  ) const {
    dt__warning(init(), << "Call on abstract class!");
  }

  vectorHandling< analyticFunction * > dtTransformer::apply( 
    vectorHandling< analyticFunction * > const * const sFunP 
  ) const {
    dt__warning(apply(), << "Call on abstract class!");    
  }

  vectorHandling< analyticGeometry * > dtTransformer::apply( 
    vectorHandling< analyticGeometry * > const * const sFunP 
  ) const {
    dt__warning(apply(), << "Call on abstract class!");    
  }
  
	dtPoint3 dtTransformer::apply(dtPoint3 const & pp) const {
		std::vector< dtPoint3 > vec;
		vec.push_back( dtPoint3(pp) );
		
		std::vector< dtPoint3 > retVec = this->apply(&vec);
	
		return retVec[0];
	}

	dtPoint2 dtTransformer::apply(dtPoint2 const & pp) const {
		std::vector< dtPoint2 > vec;
		vec.push_back( dtPoint2(pp) );
		
		std::vector< dtPoint2 > retVec = this->apply(&vec);
	
		return retVec[0];
	}	
	
  analyticFunction * dtTransformer::apply(
    analyticFunction const * const sF
  ) const {
    vectorHandling< analyticFunction * > vHIn;
    vHIn.push_back(const_cast< analyticFunction *>(sF));
    vectorHandling< analyticFunction * > vHOut = this->apply( &vHIn );
    
    return vHOut[0];
  }

  analyticGeometry * dtTransformer::apply(
    analyticGeometry const * const aG
  ) const {
    vectorHandling< analyticGeometry * > vHIn;
    vHIn.push_back(const_cast< analyticGeometry *>(aG));
    vectorHandling< analyticGeometry * > vHOut = this->apply( &vHIn );
    
    return vHOut[0];
  }  
  
  std::vector< dtPoint3 > dtTransformer::retract(
    std::vector< dtPoint3 > const * const toRetract
  ) const {
    dt__throw(retract(), << "Call on base class.");
  }
  
	dtPoint3 dtTransformer::retract(dtPoint3 const & pp) const {
		std::vector< dtPoint3 > vec;
		vec.push_back( dtPoint3(pp) );
		
		std::vector< dtPoint3 > retVec = this->retract(&vec);
	
		return retVec[0];
	}	
  
  std::vector< dtPoint2 > dtTransformer::retract(
    std::vector< dtPoint2 > const * const toRetract
  ) const {
    dt__throw(retract(), << "Call on base class.");
  }
  
	dtPoint2 dtTransformer::retract(dtPoint2 const & pp) const {
		std::vector< dtPoint2 > vec;
		vec.push_back( dtPoint2(pp) );
		
		std::vector< dtPoint2 > retVec = this->retract(&vec);
	
		return retVec[0];
	}	  
  
  void dtTransformer::handleFloat(std::string const name, float const value) {
    dt__throw(
      handleFloat(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(value) 
    );         
  }
  
  void dtTransformer::handleInt(std::string const name, int const value) {
    dt__throw(
      handleInt(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(value) 
    );      
  }
  
  void dtTransformer::handleAnalyticGeometry(
    std::string const name, analyticGeometry const * value
  ) {
    dt__throw(
      handleAnalyticGeometry(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(value) << std::endl
      << dt__eval(value->getLabel()) 
    );
  }
  
  void dtTransformer::handleAnalyticFunction(
    std::string const name, analyticFunction const * value
  ) {
    dt__throw(
      handleAnalyticFunction(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(value) << std::endl
      << dt__eval(value->getLabel()) 
    );    
  }
  
  void dtTransformer::handleDtPoint2(
    std::string const name, dtPoint2 const value
  ) {
    dt__throw(
      handleDtPoint2(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__point2d(value) 
    );  
  }

  void dtTransformer::handleBool(std::string const name, bool const value) {
    dt__throw(
      handleBool(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__eval(value) 
    );  
  }  
	
	void dtTransformer::handleDtVector3(
    std::string const name, dtVector3 const value
  ) {
    dt__throw(
      handleDtVector3(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__vector3d(value) 
    );  		
	}

	void dtTransformer::handleDtVector2(
    std::string const name, dtVector2 const value
  ) {
    dt__throw(
      handleDtVector2(),
      << "Could not handle:" << std::endl
      << dt__eval(name) << std::endl
      << dt__vector2d(value) 
    );  		
	}	
}

