#include "bVOReadMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>
#include <xmlHeaven/dtXmlParser.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOReadMSH::bVOReadMSH() {
  }

  bVOReadMSH::~bVOReadMSH() {
    
  }
  
  void bVOReadMSH::bVOReadMSH::init( 
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
    
//		<bVObserver name="bVOReadMSH" 
//		  filename="mesh.msh"
//		/>									
		_filename = qtXmlBase::getAttributeStr("filename", element);		
    _mustRead = qtXmlBase::getAttributeBool("mustRead", element);		
  }
  
  void bVOReadMSH::preUpdate( void ) {
    dt__onlyMaster {
      if ( ptrBoundedVolume()->isMeshed() ) {
        dt__info(preUpdate(), << "Already meshed.");
        
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
        ".msh";
      }
//      else dt__throwUnexpected(preUpdate);
      
      if ( !systemHandling::fileExists(cFileName) && !_mustRead ) {
        dt__info(
          preUpdate(), 
          << "No file >" << cFileName << "<. Do not read!" << std::endl
          << "mustRead = " << _mustRead
        );
        return;
      }
      
      dtInt status = gm->readMSH(cFileName);
      
      dt__info(
        preUpdate(),
        << "Read MSH file = " << cFileName << "." << std::endl
        << "Return status = " << status << "."
      );
      dt__throwIf( !status, preUpdate() );
      
      ptrBoundedVolume()->setMeshed();
    }
  }
}
