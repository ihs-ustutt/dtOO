#include "analyticFunctionCombination.h"

#include <logMe/logMe.h>

namespace dtOO {
  analyticFunctionCombination::analyticFunctionCombination() : vec3dThreeD() {
    
  }

  analyticFunctionCombination::analyticFunctionCombination(
    analyticFunctionCombination const & orig
  ) : vec3dThreeD(orig) {
    _fun.reset( orig._fun->clone() );
    _invFun.reset( orig._invFun->clone() );
  }

  analyticFunctionCombination::analyticFunctionCombination(
    vec3dThreeD const * const fun, vec3dThreeD const * const invFun
	) : vec3dThreeD() {
    _fun.reset( fun->clone() );
    _invFun.reset( invFun->clone() );
    setMin(0, _fun->xMin(0));
    setMin(1, _fun->xMin(1));
    setMin(2, _fun->xMin(2));
    setMax(0, _fun->xMax(0));
    setMax(1, _fun->xMax(1));
    setMax(2, _fun->xMax(2));    
  }


  analyticFunctionCombination::~analyticFunctionCombination() {
//    delete _parser;
  }

  aFY analyticFunctionCombination::Y( aFX const & xx ) const {
    return _fun->Y(xx);
	}
  
  aFX analyticFunctionCombination::invY( aFY const & yy ) const {
    return _invFun->Y( analyticFunction::aFXThreeD(yy[0], yy[1], yy[2]) );
  }
	
	bool analyticFunctionCombination::closed( int const & dir ) const {
		dt__throwIf( (dir!=0) && (dir!=1) && (dir!=2), closed );
		return false;
	}
	
  analyticFunctionCombination * analyticFunctionCombination::clone( void ) const {
    return new analyticFunctionCombination( *this );
  }
  
  analyticFunctionCombination * analyticFunctionCombination::create( void ) const {
    return new analyticFunctionCombination();
  }
}