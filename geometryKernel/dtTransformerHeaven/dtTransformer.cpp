#include "dtTransformer.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <QtXml/QDomElement>

namespace dtOO {
  dtTransformer::dtTransformer() {
  }

  dtTransformer::~dtTransformer() {
  }

  void dtTransformer::init( QDomElement * transformerElementP, 
                            pointContainer * const pointContainerP,
                            vectorContainer * const vectorContainerP,    
                            vectorHandling< constValue * > const * const cValP,
                            vectorHandling< analyticFunction * > const * const sFunP,
                            vectorHandling< analyticGeometry * > const * const depAGeoP ) {
    DTWARNINGWF(init(), << "Call on abstract class!");
  }
    
  void dtTransformer::init( QDomElement * transformerElementP,
             vectorHandling< constValue * > const * const cValP,
             vectorHandling< analyticFunction * > const * const sFunP) {
    DTWARNINGWF(init(), << "Call on abstract class!");
  }
  
  std::vector< dtPoint2 * > dtTransformer::apply( std::vector< dtPoint2 * > const * const pointVecP ) const {
    DTWARNINGWF(init(), << "Call on abstract class!");
  }

  vectorHandling< dtPoint2 * > dtTransformer::apply( vectorHandling< dtPoint2 * > const * const pointVecP ) const {
    std::vector< dtPoint2 * > const * const pV 
      = dynamic_cast< std::vector< dtPoint2 * > const * const>(pointVecP);

    if (!pV) {
      dt__THROW(apply(), << DTLOGEVAL(pV) );
    }
    std::vector< dtPoint2 * > vecOut = this->apply(pV);
    vectorHandling< dtPoint2 * > vHOut;
    dt__FORALL(vecOut, ii,
      vHOut.push_back(vecOut[ii]);
    );
    vecOut.clear();
    
    return vHOut;
  }
  
  std::vector< dtPoint3 * > dtTransformer::apply( std::vector< dtPoint3 * > const * const pointVecP ) const {
    DTWARNINGWF(init(), << "Call on abstract class!");
  }

  vectorHandling< analyticFunction * > dtTransformer::apply( vectorHandling< analyticFunction * > const * const sFunP ) const {
    DTWARNINGWF(init(), << "Call on abstract class!");    
  }

  vectorHandling< analyticGeometry * > dtTransformer::apply( vectorHandling< analyticGeometry * > const * const sFunP ) const {
    DTWARNINGWF(init(), << "Call on abstract class!");    
  }
  
  analyticFunction * dtTransformer::apply(analyticFunction const * const sF) const {
    vectorHandling< analyticFunction * > vHIn;
    vHIn.push_back(const_cast< analyticFunction *>(sF));
    vectorHandling< analyticFunction * > vHOut = this->apply( &vHIn );
    
    return vHOut[0];
  }

  analyticGeometry * dtTransformer::apply(analyticGeometry const * const aG) const {
    vectorHandling< analyticGeometry * > vHIn;
    vHIn.push_back(const_cast< analyticGeometry *>(aG));
    vectorHandling< analyticGeometry * > vHOut = this->apply( &vHIn );
    
    return vHOut[0];
  }  
  
  void dtTransformer::handleFloat(std::string const name, float const value) {
    dt__THROW(handleFloat(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGEVAL(value) );         
  }
  
  void dtTransformer::handleInt(std::string const name, int const value) {
    dt__THROW(handleInt(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGEVAL(value) );      
  }
  
  void dtTransformer::handleAnalyticGeometry(std::string const name, analyticGeometry const * value) {
    dt__THROW(handleAnalyticGeometry(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGEVAL(value) << LOGDEL
            << DTLOGEVAL(value->getLabel()) );
  }
  
  void dtTransformer::handleAnalyticFunction(std::string const name, analyticFunction const * value) {
    dt__THROW(handleAnalyticFunction(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGEVAL(value) << LOGDEL
            << DTLOGEVAL(value->getLabel()) );    
  }
  
  void dtTransformer::handlePoint2d(std::string const name, dtPoint2 const value) {
    dt__THROW(handlePoint2d(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGPOI2D(value) );  
  }

  void dtTransformer::handleBool(std::string const name, bool const value) {
    dt__THROW(handleBool(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGEVAL(value) );  
  }  
	
	void dtTransformer::handleDtVector3(std::string const name, dtVector3 const value) {
    dt__THROW(handleDtVector3(),
            << "Could not handle:" << LOGDEL
            << DTLOGEVAL(name) << LOGDEL
            << DTLOGPOI3D(value) );  		
	}
}

