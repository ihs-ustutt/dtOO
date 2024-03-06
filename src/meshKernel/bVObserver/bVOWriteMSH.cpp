/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "bVOWriteMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/lVHOstateHandler.h>
#include <meshEngine/dtGmshModel.h>
#include "bVOInterfaceFactory.h"
#include <parseHeaven/dtParser.h>

namespace dtOO {
  bool bVOWriteMSH::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOWriteMSH, new bVOWriteMSH())
  );
  
  bVOWriteMSH::bVOWriteMSH() {
  }

  bVOWriteMSH::~bVOWriteMSH() {
    
  }
  
  void bVOWriteMSH::init( 
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
    
//		<bVObserver name="bVOWriteMSH" 
//		  filename="mesh.msh"
//		  saveAll="false"
//		/>
		jsonPrimitive jE;
		jE.append< std::string >(
      "_filename",
		  qtXmlBase::getAttributeStr("filename", element)
    );		
    
    jE.append< bool >(  
      "_saveAll",
      qtXmlBase::getAttributeBool("saveAll", element, false)
    );
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOWriteMSH::postUpdate( void ) {
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
      std::string cFileName 
      = 
      dtParser()[config().lookup< std::string >("_filename")];
   
      if ( cFileName == "" ) {
        cFileName = lVHOstateHandler().commonState();
        if (cFileName != "") {
          cFileName = cFileName + "_";
        }
        cFileName = cFileName + ptrBoundedVolume()->getLabel() + ".msh";
      }
      
      dt__info(
        postUpdate(),
        << "Write >" << cFileName << "<." << std::endl
        << dt__eval(config().lookup< bool >("_saveAll"))
      );
      
		  ptrBoundedVolume()->getModel()->writeMSH(
        cFileName, 
        2.2, 
        false, 
        config().lookup< bool >("_saveAll"), 
        false
      );
    }
  }
}
