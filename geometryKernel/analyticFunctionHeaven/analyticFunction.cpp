#include "analyticFunction.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <Minuit2/Minuit2Minimizer.h>
#include <Math/Functor.h>
#include <omp.h>

namespace dtOO {
	analyticFunction::analyticFunction() : optionHandling(), labelHandling() {

	}

	analyticFunction::analyticFunction(
    const analyticFunction& orig
  ) : optionHandling(orig), labelHandling(orig) {

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
      ret[ii] = xMin(ii) +  (xMax(ii) - xMin(ii)) * xx[ii];    
    }
    return ret;    
  }
  
  aFX analyticFunction::percent_x( aFX const & xx ) const {
    aFX ret(xDim(), 0.);
    
    for (int ii=0; ii<xDim(); ii++) {
      ret[ii] = (xx[ii] - xMin(ii)) / (xMax(ii) - xMin(ii));    
    }
    return ret;
  }
    
  aFX analyticFunction::invY(aFY const & yy) const {
    //
    // save target value in mutable variable
    //
    _invY = yy;

    int const numInitGuess = 11;    
    double initGuess[numInitGuess] 
    = 
    {0.5, 0.6, 0.4, 0.7, 0.3, 0.8, 0.2, 0.9, 0.1, 1.0, 0.0};       
    
    float prec 
    =      
    staticPropertiesHandler::getInstance()->getOptionFloat("invY_precision");    
    int rootPrintLevel
    =
    staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel");
    
    std::vector< int >      ijk(xDim(), 0);
    std::vector< int >   ijkMin(xDim(), 0);
    std::vector< int >   ijkMax(xDim(), numInitGuess);
    std::vector< int > ijkDelta(xDim(), 1);

    float bestDistance = std::numeric_limits<float>::max();
    int depth = 0;
    ijk[0] = ijkMin[depth];
    while (true) {
      if (ijk[depth] < ijkMax[depth]) {
        if (depth == ijk.size()-1) {

          // 
          // multidimensional minimization
          //
          ROOT::Minuit2::Minuit2Minimizer min;
          ROOT::Math::Functor toMin(
            this, &analyticFunction::F, xDim() 
          );			
          min.SetFunction(toMin);

          //
          // minimizer options
          //
          min.SetMaxFunctionCalls(1000000);
          min.SetMaxIterations(100000);
          min.SetTolerance( prec/(10.*xDim()) ); // reduce precision
          min.SetPrintLevel( rootPrintLevel );

          //--------------------------------------------------------------------------    
          //
          // set bounds
          //
          dt__forFromToIndex(0, xDim(), kk) {
            dt__throwIf(ijk[kk]>numInitGuess, invY());
            std::string xStr = "x"+stringPrimitive::intToString(kk);
            min.SetVariable( kk, xStr, initGuess[ ijk[kk] ], 0.01 );
            min.SetVariableLimits(kk, 0., 1.);	
          }

//          dt__info(invY(), << "Trying ijk = " << ijk );

          //
          // minimize
          //
          min.Minimize();
          double const * const theRoot = min.X( );

          //
          // get x (percent)
          //
          aFX retX(xDim(),0.);
          dt__forFromToIndex(0, xDim(), kk) retX[kk] = theRoot[kk];

          //
          // convergence check
          //
          float currentDistance 
          = 
          distance( 
            Y(x_percent(retX)), _invY
          );
          bestDistance = std::min(bestDistance, currentDistance);
          std::cout << "currentDistance = " << currentDistance << std::endl;
          if ( currentDistance <= prec ) {
            dt__info(invY(), << "Converged at iteration ijk = " << ijk);
            return x_percent(retX);  
          }
        
        }
        else {
          depth++;
          ijk[depth] = ijkMin[depth];
          continue;
        }
      }
      else if (--depth == -1) break;

      ijk[depth] += ijkDelta[depth];
    }
    
    aFX xxMin;
    aFX xxMax;
    dt__forFromToIndex(0, xDim(), ii) xxMin.push_back(xMin(ii));
    dt__forFromToIndex(0, xDim(), ii) xxMax.push_back(xMax(ii));
    aFX xxMiddle;
    dt__forFromToIndex(0, xDim(), ii) {
      xxMiddle.push_back(.5 * (xxMax[ii] - xxMin[ii]) );
    }
    dt__throw( 
      invY(),
      << dt__eval(getLabel()) << std::endl
      << dt__eval(_invY) << std::endl
      << dt__eval( Y(xxMin) ) << std::endl
      << dt__eval( Y(xxMiddle) ) << std::endl
      << dt__eval( Y(xxMax) ) << std::endl
      << dt__eval(bestDistance) << std::endl
      << "No convergence reached. Maybe reducing invY solves this problem."
    );    
    
//    return x_percent(retX);
  }

	bool analyticFunction::isCompound( void ) const {
		return false;
	}

	vectorHandling< analyticFunction const * > analyticFunction::compoundInternal( 
    void 
  ) const {
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
	
  aFX analyticFunction::aFXThreeD( 
    float const & x0, float const & x1, float const & x2 
  ) {
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
	
  aFY analyticFunction::aFYThreeD( 
    float const & x0, float const & x1, float const & x2 
  ) {
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

  aFY analyticFunction::aFY_aFX( aFX const & xx ) {
    return aFY(xx);
  }
  
  aFX analyticFunction::aFX_aFY( aFY const & yy ) {
    return aFX(yy);
  }
  
  float analyticFunction::distance( aFX const & x0, aFX const & x1 ) {
    dt__throwIf(x0.size()!=x1.size(), length());
    
    float dist = 0.;
    dt__forAllIndex(x0, ii) dist = dist + (x1[ii] - x0[ii])*(x1[ii] - x0[ii]);
    
    return sqrt(dist);
  }
    
	double analyticFunction::F(double const * xx) const {	
    aFX xxT(xDim(), 0.);
    for (int ii=0; ii<xDim(); ii++) xxT[ii] = xx[ii];
    xxT = x_percent(xxT);
    aFY yy = Y(xxT);
    
    double objective = 0.;
    for (int ii=0; ii<yDim(); ii++) {
      objective 
      = 
      objective 
      + 
      (
        yy[ii] - _invY[ii]
      )
      *
      (
        yy[ii] - _invY[ii]
      );
    }
    return sqrt(objective);
	}	    
  
  dt__C_addCloneForpVH(analyticFunction);  
}
