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

#include "bVOAddFace.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOAddFace::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOAddFace, new bVOAddFace())
  );
  
  bVOAddFace::bVOAddFace() {
  }

  bVOAddFace::~bVOAddFace() {
    
  }
 
  void bVOAddFace::init( 
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
    
		// <bVObserver 
		//   name="bVOAddFace" 
    //   regionLabel="labelA"
		//   faceLabel="{labelA}{labelB}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    jsonPrimitive jE;
    jE.append< std::string >(
      "_regionLabel", 
      dtXmlParserBase::getAttributeStr("regionLabel", element)
    );
		jE.append< std::vector< std::string > >(
      "_faceLabel",
      dtXmlParserBase::getAttributeStrVector("faceLabel", element)
    );    
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOAddFace::preUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dtGmshRegion * const gr 
    = 
    gm->getDtGmshRegionByPhysical(
      config().lookup< std::string >("_regionLabel")
    );

    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_faceLabel"), aFace
    ) {
      gr->addFace( gm->getDtGmshFaceByPhysical(aFace), 1 );
    }
  }
}
