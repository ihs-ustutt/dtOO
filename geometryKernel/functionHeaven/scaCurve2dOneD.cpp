#include "scaCurve2dOneD.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve2d.h>
#include <progHelper.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/floatHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <solid2dLine.h>
#include <RConfigure.h>
#include <Rtypes.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLRootFinder.h>
#include <Math/RootFinderAlgorithms.h>
#include <Math/Functor.h>
#include <GSLError.h>

namespace dtOO {
  scaCurve2dOneD::scaCurve2dOneD() : scaOneD() {
  }

  scaCurve2dOneD::scaCurve2dOneD(scaCurve2dOneD const & orig) : scaOneD(orig) {
    _dtC2d.reset( orig._dtC2d->clone() );
    scaOneD::setMinMax(orig.xMin(0), orig.xMax(0));
  }

  scaCurve2dOneD::scaCurve2dOneD( dtCurve2d const * const orig ) : scaOneD() {

    _dtC2d.reset( orig->clone() );

    dtPoint2 pp = _dtC2d->pointPercent(0.);
    
    scaOneD::setMinMax(pp.x(), pp.x());
    
    pp = _dtC2d->pointPercent(1.);
    
    if ( pp.x() < scaOneD::xMax(0) ) {
      scaOneD::setMin( pp.x() );
    }
    else {
      scaOneD::setMax( pp.x() );
    }
  }
  
  scaCurve2dOneD * scaCurve2dOneD::clone( void ) const {
    return new scaCurve2dOneD( *this );
  }
  
  scaCurve2dOneD * scaCurve2dOneD::create( void ) const {
    return new scaCurve2dOneD();
  }  
  
  scaCurve2dOneD::~scaCurve2dOneD() {
  }

	double scaCurve2dOneD::funValue(const double xx ) const {	
		dtPoint2 pp = _dtC2d->point( static_cast<float>(xx) );
		return static_cast< double >( _tmpX - pp.x() );
	}

	double scaCurve2dOneD::diffFunValue(const double xx ) const {
		dtVector2 der = _dtC2d->firstDer( static_cast<float>(xx) );
		return static_cast< double >( der.y() / der.x() );
	}	
	
  float scaCurve2dOneD::YFloat(float const & xx) const {
		bool mustIterate = true;
		float theRoot;
		if ( (xx>xMax(0)) || (xx<xMin(0)) ) {
			float const range = fabs(xMax(0) - xMin(0));
			if ( floatHandling::isSmall( (xMin(0)-xx)/range ) ) { 
				mustIterate = false;
				theRoot = _dtC2d->minU();
			}
			else if ( floatHandling::isSmall( (xMax(0)-xx)/range ) ) {
				mustIterate = false;
				theRoot = _dtC2d->maxU();
			}
			else {
				dt__THROW(
					YFloat(),
					<< DTLOGEVAL( getLabel() ) << LOGDEL
					<< DTLOGEVAL(xx) << LOGDEL 
					<< DTLOGEVAL(xMin(0)) << LOGDEL 
					<< DTLOGEVAL(xMax(0)) << LOGDEL 
					<< "xx out of range."
				);
			}
		}
		if (mustIterate) {
			_tmpX = xx;

			// Create the Integrator
			bool check = false;
			ROOT::Math::Roots::Bisection bisectF;
			ROOT::Math::Functor1D f0(this, &scaCurve2dOneD::funValue );

			if (
				(funValue(_dtC2d->minU()) < 0.0 && funValue(_dtC2d->maxU()) < 0.0) 
				|| (funValue(_dtC2d->minU()) > 0.0 && funValue(_dtC2d->maxU()) > 0.0) 
			) {

			}
			else {
				check 
				= 
				bisectF.SetFunction(
					f0,
					static_cast<double>(_dtC2d->minU()), 
					static_cast<double>(_dtC2d->maxU())
				); 
			}

			if ( check ) {
				check = bisectF.Solve();
			}

			if ( !check ) {
				ROOT::Math::GradFunctor1D f1(this, &scaCurve2dOneD::funValue, &scaCurve2dOneD::diffFunValue );
				ROOT::Math::Roots::Newton newtonF;
				check 
				= 
				newtonF.SetFunction(
					f1,
					static_cast<double>(_dtC2d->minU() + .5*(_dtC2d->maxU() - _dtC2d->minU()))
				); 

				if (check) {
					check = newtonF.Solve();
				}
				if ( !check ) {
					dt__THROW(YFloat(), << DTLOGEVAL(check) << LOGDEL
								 << DTLOGEVAL(getLabel()) );			
				}
				theRoot = newtonF.Root();
			}
			else {
				theRoot = bisectF.Root();
			}
		}

		//
		// output
		//
		dtPoint2 pp = _dtC2d->point( static_cast<float>( theRoot ) );
//		DTINFOWF(
//			YFloat(),
//			<< DTLOGEVAL(mustIterate) << LOGDEL
//			<< DTLOGEVAL(xx) << LOGDEL
//			<< DTLOGEVAL(fabs(xx-pp.x())) << LOGDEL
//		  << DTLOGPOI2D(pp) << LOGDEL
//			<< getLabel() << "(" << pp.x() << ") = " << pp.y()
//		);
		
	  return pp.y();
  }
 
  vectorHandling< renderInterface * > scaCurve2dOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		
		vectorHandling< dtPoint2 > p2(nU);
    float interval = (_dtC2d->maxU() - _dtC2d->minU()) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			float iiF = static_cast<float>(ii);
      p2[ii] = _dtC2d->point( _dtC2d->minU() + iiF * interval );
    }

		vectorHandling< dtPoint2 > p2Cp(_dtC2d->nControlPoints());
    for (int ii=0;ii<p2Cp.size();ii++) {
      p2Cp[ii] = _dtC2d->controlPoint( ii );
    }
		
		vectorHandling< renderInterface * > rV(2);
		rV[0] = new solid2dLine(p2);
		rV[1] = new discrete2dPoints(p2Cp);
		
		return rV;
  }
	
  dtCurve2d const * scaCurve2dOneD::ptrDtCurve2d( void ) const {
    return _dtC2d.get();
  }

  void scaCurve2dOneD::translate( dtVector2 const vector ) {
    //
    // perform translation
    //
    _dtC2d->translate(vector);
    
    //
    // adjust range
    //
    scaOneD::setMinMax(
      _dtC2d->controlPoint(0).x(),
      _dtC2d->lastControlPoint().x()
    );
  }
   
  std::vector< dtPoint2 > scaCurve2dOneD::getControlPoints( void ) const {
    std::vector< dtPoint2 > cP;
    
    for (int ii=0; ii<_dtC2d->nControlPoints(); ii++) {
      cP.push_back( _dtC2d->controlPoint(ii) );
    }
    
    return cP;
  }
}