#include "baseContainer.h"

#include "pointContainer.h"
#include "vectorContainer.h"
#include "transformerContainer.h"

namespace dtOO {
	baseContainer::baseContainer() {
		_pC.reset(new pointContainer());
		_vC.reset(new vectorContainer());
		_tC.reset(new transformerContainer());
	}

	baseContainer::~baseContainer() {
	}
  
  void baseContainer::clear( void ) {
		_pC.reset(new pointContainer());
		_vC.reset(new vectorContainer());
		_tC.reset(new transformerContainer());   
  }
  
//  dt__pH(pointContainer) & baseContainer::refPtrHPointContainer( void ) {
//		return _pC;
//	}
//  
//	dt__pH(vectorContainer) & baseContainer::refPtrHVectorContainer( void ) {
//		return _vC;
//	}
//	
//  dt__pH(transformerContainer) & baseContainer::refPtrHTransformerContainer( void ) {
//		return _tC;
//	}
	
	pointContainer * const baseContainer::ptrPointContainer( void ) {
		dt__throwIf(_pC.get() == NULL, ptrPointContainer);
		
		return _pC.get();
	}

	pointContainer const * const baseContainer::constPtrPointContainer( 
    void 
  ) const {
	  dt__throwIf(_pC.get() == NULL, constPtrPointContainer());
		
		return _pC.get();
	}	
  
	vectorContainer * const baseContainer::ptrVectorContainer( void ) {
	  dt__throwIf(_vC.get() == NULL, ptrVectorContainer());
		
		return _vC.get();
	}

	vectorContainer const * const baseContainer::constPtrVectorContainer( 
    void 
  ) const {
	  dt__throwIf(_vC.get() == NULL, constPtrVectorContainer());
		
		return _vC.get();
	}	
  
	transformerContainer * const baseContainer::ptrTransformerContainer( void ) {
	  dt__throwIf(_tC.get() == NULL, ptrTransformerContainer);
		
		return _tC.get();
  }

	transformerContainer const * const 
  baseContainer::constPtrTransformerContainer( void ) const {
	  dt__throwIf(_tC.get() == NULL, constPtrTransformerContainer);
		
		return _tC.get();
  }  

  pointContainer const & baseContainer::dtPoint3( void ) {
    return *(_pC);
  }

  vectorContainer const & baseContainer::dtVector3( void ) {
    return *(_vC);
  }
  
  transformerContainer const & baseContainer::dtTransformer( void ) {
    return *(_tC);
  }

  std::vector< std::string > baseContainer::labels( void ) const {
    std::vector< std::string > pLab = this->constPtrPointContainer()->labels();
    std::vector< std::string > vLab = this->constPtrVectorContainer()->labels();
    std::vector< std::string > tLab = this->constPtrTransformerContainer()->labels();
    
    std::vector< std::string > ret;
    ret.insert( ret.end(), pLab.begin(), pLab.end() );
    ret.insert( ret.end(), vLab.begin(), vLab.end() );
    ret.insert( ret.end(), tLab.begin(), tLab.end() );
    
    return ret;
  }
}
