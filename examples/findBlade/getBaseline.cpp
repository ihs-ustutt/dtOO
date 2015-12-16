#include "getBaseline.h"
#include <analyticFunctionHeaven/scaFunction.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {

  getBaseline::getBaseline() {
  }

  getBaseline::~getBaseline() {
  }

  std::vector< dtPoint2 * > getBaseline::apply( std::vector< dtPoint2 * > const * const toTrans ) const {
    std::vector< dtPoint2 * > transP2;
      if ((toTrans->size()%2) == 1) {
        dt__THROW(apply(),
                << "Point vector has not correct size." << std::endl
                << dt__eval(toTrans->size()) );
    }
	std::ofstream paramstr("Meanline_points.txt", std::ios::trunc);
	 for (int ii=0; ii<=(((toTrans->size())/2)-1);(ii=ii+1)) {

         float xi = toTrans->at(ii)->x();
         float yi = toTrans->at(ii)->y();
         
         float xj = toTrans->at((toTrans->size()) - ii-1)->x();
         float yj = toTrans->at((toTrans->size()) - ii-1)->y();

         float xx = (xi+xj)/2;
         float yy = (yi+yj)/2;
	 
	//paramstr.open ("Solution.txt");
	paramstr << xx << ", "<< yy << std::endl;
      
//      scaFunction * sF;
//      sFunP->get(ii)->copy(&sF);
//      sF->getMeshPoints(_Meshpoints);
	//std::cout << "xx: " << xx << "\tyy: " << yy << std::endl;
      transP2.push_back( new dtPoint2(xx, yy) );
     }
	paramstr.close();

      return transP2;
  
  }

  bool getBaseline::isNecessary( void ) const {
    return true;
  }
}

