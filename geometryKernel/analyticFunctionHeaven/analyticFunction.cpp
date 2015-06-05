#include "analyticFunction.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <RConfigure.h>
#include <Rtypes.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLRootFinder.h>
#include <Math/RootFinderAlgorithms.h>
#include <Math/Functor.h>
#include <GSLError.h>

namespace dtOO {
	analyticFunction::analyticFunction() : optionHandling(), labelHandling() {
	}

	analyticFunction::analyticFunction(const analyticFunction& orig)  : optionHandling(orig), labelHandling(orig) {
	}

	analyticFunction::~analyticFunction() {
	}
	
  void analyticFunction::dump(void) const {
    dt__info(
			dump(), 
			<< dt__eval(getLabel()) << std::endl
		  << dt__eval(virtualClassName()) << std::endl
			<< "dumpToString()" << std::endl
			<< dumpToString()
		);		
	}
	
	std::string analyticFunction::dumpToString(void) const {
		return std::string("");
	}	
  
	aFX analyticFunction::x_percent( aFX const & xx ) const {
    aFX ret(xDim(), 0.);
    
    for (int ii=0; ii<xDim(); ii++) {
      ret[ii]= xMin(ii) +  (xMax(ii) - xMin(ii)) * xx[ii];    
    }
    return ret;    
  }
  
  aFX analyticFunction::percent_x( aFX const & xx ) const {
    aFX ret(xDim(), 0.);
    
    for (int ii=0; ii<xDim(); ii++) {
      ret[ii]= (xx[ii] - xMin(ii)) / (xMax(ii) - xMin(ii));    
    }
    return ret;
  }
    
  aFX analyticFunction::invY(aFY const & yy) const {
    dt__info(
      invY(), 
      << "yy = " << yy << std::endl
      << dt__eval(xDim()) << std::endl
      << dt__eval(yDim()) << std::endl
      << dt__eval( Y(x_percent(analyticFunction::aFXTwoD(0.,0.))) ) << std::endl
      << dt__eval( Y(x_percent(analyticFunction::aFXTwoD(1.,0.))) ) << std::endl
      << dt__eval( Y(x_percent(analyticFunction::aFXTwoD(0.,1.))) ) << std::endl
      << dt__eval( Y(x_percent(analyticFunction::aFXTwoD(1.,1.))) ) << std::endl
    );
    _invY = yy;
		// 
		// multidimensional minimization
		//
		ROOT::Math::GSLMinimizer min( ROOT::Math::kVectorBFGS );
		ROOT::Math::Functor toMin(
			this, &analyticFunction::F, xDim() 
		);			
		min.SetFunction(toMin);

		//
		// set bounds
		//
    for (int ii=0; ii<xDim(); ii++) {
      std::string xStr = "x"+stringPrimitive::intToString(ii);
		  min.SetVariable( ii, xStr, .5, 1.1 );			
      min.SetVariableLimits(ii, 0., 1.);	
    }
    
		//
		// minimizer options
		//
		min.SetMaxFunctionCalls(1000000);
		min.SetMaxIterations(100000);
		min.SetTolerance(0.0000001);			
		min.SetPrintLevel(3);

		//
		// minimize
		//
		{
			dt__pipeCout(sp, cs)
			min.Minimize();
		}
		double const * const theRoot = min.X( );

    aFX retX(xDim(),0.);
    for (int ii=0; ii<xDim(); ii++) retX[ii] = theRoot[ii];
    
    dt__info(
      invY(), 
      << "label = " << getLabel() << std::endl
      << "yy = " << yy << std::endl
      << "xx = " << x_percent(retX) << std::endl
      << "f(xx) = " << Y(x_percent(retX)) << std::endl
    );
    
    return x_percent(retX);
  }

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction const * > analyticFunction::compoundInternal( void ) const {
		return vectorHandling< analyticFunction const * >();
	}			
  
	bool analyticFunction::isTransformed( void ) const {
		return false;
	}
	
  aFX analyticFunction::aFXZeroD( void ) {
		return aFX(0);
	}
  
  aFX analyticFunction::aFXOneD( float const & x0 ) {
		aFX ret = aFX(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFX analyticFunction::aFXTwoD( float const & x0, float const & x1 ) {
		aFX ret = aFX(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	} 
	
  aFX analyticFunction::aFXThreeD( float const & x0, float const & x1, float const & x2 ) {
		aFX ret = aFX(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}
  
  aFX analyticFunction::aFXTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFXTwoD(pp.x(), pp.y());
	} 
  
  aFX analyticFunction::aFXThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFXThreeD(pp.x(), pp.y(), pp.z());
	}  
  
  aFY analyticFunction::aFYZeroD( void ) {
		return aFY(0);
	}
  
  aFY analyticFunction::aFYOneD( float const & x0 ) {
		aFY ret = aFY(1, 0.);
		ret[0] = x0;
		
		return ret;
	}
	
  aFY analyticFunction::aFYTwoD( float const & x0, float const & x1 ) {
		aFY ret = aFY(2, 0.);
		ret[0] = x0;
		ret[1] = x1;		
		
		return ret;
	}
	
  aFY analyticFunction::aFYThreeD( float const & x0, float const & x1, float const & x2 ) {
		aFY ret = aFY(3, 0.);
		ret[0] = x0;
		ret[1] = x1;
		ret[2] = x2;	
		
		return ret;
	}  
  
  aFY analyticFunction::aFYTwoD( dtPoint2 const & pp ) {
		return analyticFunction::aFYTwoD(pp.x(), pp.y());
	} 
  
  aFY analyticFunction::aFYThreeD( dtPoint3 const & pp ) {
		return analyticFunction::aFYThreeD(pp.x(), pp.y(), pp.z());
	}    

	double analyticFunction::F(double const * xx) const {	
    aFX xxT(xDim(), 0.);
    for (int ii=0; ii<xDim(); ii++) xxT[ii] = xx[ii];
    xxT = x_percent(xxT);
    aFY yy = Y(xxT);
    
    double objective = 0.;
//    double length = 0.;
    for (int ii=0; ii<yDim(); ii++) {
//      length = length + _invY[ii]*_invY[ii];
      objective = objective + (yy[ii]-_invY[ii])*(yy[ii]-_invY[ii]);
    }
//    objective = objective/sqrt(length);

//    dt__info(
//      F(), 
//      << "xxT = " << xxT << std::endl
////      << "xx[0] = " << xx[0] << std::endl
////      << "xx[1] = " << xx[1] << std::endl
//      << "yy = " << yy << std::endl
//      << "_invY = " << _invY << std::endl            
//      << "objective " << objective << std::endl
//    );
    
    return 100.*objective;
	}	    
}
