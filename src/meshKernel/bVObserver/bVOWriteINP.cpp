#include "bVOWriteINP.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParser.h>
#include <meshEngine/dtGmshModel.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {
  bool bVOWriteINP::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOWriteINP, new bVOWriteINP())
  );
  
  bVOWriteINP::bVOWriteINP() {
  }

  bVOWriteINP::~bVOWriteINP() {
    
  }
  
  void bVOWriteINP::bVOWriteINP::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
		boundedVolume * attachTo
  ) {		
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver name="bVOWriteINP" 
//		  filename="mesh.inp"
//		  saveAll="false"
//		/>									
		_filename 
		= 
		qtXmlBase::getAttributeStr("filename", element);		
    
    _saveAll = qtXmlBase::getAttributeBool("saveAll", element, false);
    _saveGroupOfNodes = qtXmlBase::getAttributeBool("saveGroupOfNodes", element, false);
  }
  
  void bVOWriteINP::postUpdate( void ) {
    dt__onlyMaster {
      if ( !ptrBoundedVolume()->isMeshed() ) {
        dt__info(postUpdate(), << "Not yet meshed.");        
        
        return;      
      }

		  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());      

      //
      // set current model
      //
      ::GModel::setCurrent( gm );
      
      //
      // create filename string if empty
      //
      std::string cFileName = _filename;
      if ( cFileName == "" ) {
        cFileName 
        = 
        dtXmlParser::constReference().currentState()
        +
        "_"
        +
        ptrBoundedVolume()->getLabel()
        +
        ".inp";
      }
//      else dt__throwUnexpected(postUpdate);
      
      dt__info(
        postUpdate(),
        << "Write >" << cFileName << "<." << std::endl
        << dt__eval(_saveAll) << std::endl
        << dt__eval(_saveGroupOfNodes)
      );
      
		  ptrBoundedVolume()->getModel()->writeINP(
        cFileName, _saveAll, _saveGroupOfNodes, 1.0
      );
    }
  }
}
