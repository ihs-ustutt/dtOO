#include "bVOReadMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/lVHOstateHandler.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include "bVOInterfaceFactory.h"
#include "jsonHeaven/jsonPrimitive.h"

namespace dtOO {  
  bool bVOReadMSH::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOReadMSH, new bVOReadMSH())
  );
  
  bVOReadMSH::bVOReadMSH() {
  }

  bVOReadMSH::~bVOReadMSH() {
    
  }
  
  void bVOReadMSH::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {		
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver name="bVOReadMSH" 
//		  filename="mesh.msh"
//		/>
  	jsonPrimitive jE;
    jE.append< std::string >(
      "_filename",
      qtXmlBase::getAttributeStr("filename", element)
    );		
    jE.append< bool >(
      "_mustRead",
      qtXmlBase::getAttributeBool("mustRead", element)
    );		
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
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
      std::string cFileName = config().lookup< std::string >("_filename");
      if ( cFileName == "" ) {
        cFileName 
        = 
        lVHOstateHandler().commonState()
        +
        "_"
        +
        ptrBoundedVolume()->getLabel()
        +
        ".msh";
      }
      
      if ( 
        !systemHandling::fileExists(cFileName) 
        && 
        !config().lookup< bool >("_mustRead") 
      ) {
        dt__info(
          preUpdate(), 
          << "No file >" << cFileName << "<. Do not read!" << std::endl
          << "mustRead = " << config().lookup< bool >("_mustRead") 
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
