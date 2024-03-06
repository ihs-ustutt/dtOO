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

#include "bVOWriteSU2.h"

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
  bool bVOWriteSU2::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOWriteSU2, new bVOWriteSU2())
  );
  
  bVOWriteSU2::bVOWriteSU2() {
  }

  bVOWriteSU2::~bVOWriteSU2() {
    
  }
  
  void bVOWriteSU2::init( 
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
    
//		<bVObserver name="bVOWriteSU2" 
//		  filename="mesh.msh"
//		  saveAll="false"
//		/>									
		_filename 
		= 
		qtXmlBase::getAttributeStr("filename", element);		
    
    _saveAll = false;
    if (qtXmlBase::hasAttribute("saveAll", element)) {
      _saveAll = qtXmlBase::getAttributeBool("saveAll", element);
    }
//    _saveParametric = false;
//    if (qtXmlBase::hasAttribute("saveParametric", element)) {
//      _saveParametric = qtXmlBase::getAttributeBool("saveParametric", element);
//    } 
  }
  
  void bVOWriteSU2::postUpdate( void ) {
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
        ".su2";
      }
      
      dt__info(
        postUpdate(),
        << "Write >" << cFileName << "<." << std::endl
        << dt__eval(_saveAll)
      );
      
		  ptrBoundedVolume()->getModel()->writeSU2( cFileName, _saveAll, 1.0 );
    }
  }
}
