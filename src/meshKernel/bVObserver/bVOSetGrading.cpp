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

#include "bVOSetGrading.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include "bVOInterfaceFactory.h"
#include "jsonHeaven/jsonPrimitive.h"

namespace dtOO {  
  bool bVOSetGrading::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOSetGrading, new bVOSetGrading())
  );
  
  bVOSetGrading::bVOSetGrading() {
  }

  bVOSetGrading::~bVOSetGrading() {
    
  }
  
  void bVOSetGrading::init( 
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
		//   name="bVOSetGrading"
    //   regionLabel="{REG1}{REG2}"
		//   grading="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"
		//   type="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"    
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    jsonPrimitive jE;
    jE.append< std::vector< std::string > >(
      "_regionLabel",
		  dtXmlParserBase::getAttributeStrVector("regionLabel", element)
    );    
    
    jE.append< std::vector< std::string > >(
      "_faceLabel",
		  dtXmlParserBase::getAttributeStrVector("faceLabel", element)
    ); 
    jE.append< std::vector< std::string > >(
      "_edgeLabel",
		  dtXmlParserBase::getAttributeStrVector("edgeLabel", element)
    ); 
    
    jE.append< std::vector< dtReal > >(
      "_grading",
		  dtXmlParserBase::getAttributeFloatVectorMuParse(
        "grading", element, cV, aF
      )
    );
    jE.append< std::vector< dtReal > >(
      "_type",
		  dtXmlParserBase::getAttributeFloatVectorMuParse("type", element, cV, aF)
    );
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOSetGrading::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

    dt__throwIf(
      ( 
        config().lookup< std::vector< std::string > >("_regionLabel").size()
        + 
        config().lookup< std::vector< std::string > >("_faceLabel").size()
        + 
        config().lookup< std::vector< std::string > >("_edgeLabel").size()
      )
      >
      std::max(
        config().lookup< std::vector< std::string > >("_regionLabel").size(),
        std::max(
          config().lookup< std::vector< std::string > >("_faceLabel").size(),
          config().lookup< std::vector< std::string > >("_edgeLabel").size()
        )
      ), 
      preUpdate()
    );
   
		//
		// set current model
		//
		::GModel::setCurrent( gm );
	
    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_regionLabel"), aLabel
    ) {
      dt__info( preUpdate(), << "Handling regions " << aLabel );
      dt__forAllRefAuto(gm->getDtGmshRegionListByPhysical(aLabel), aReg) {
        dt__info( preUpdate(), << "Set grading to region " << aReg );
        aReg->setGrading(
          config().lookup< std::vector< dtReal > >("_grading"), 
          config().lookup< std::vector< dtReal > >("_type")
        );
      }
    }    
    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_faceLabel"), aLabel
    ) {
      dt__info( preUpdate(), << "Handling faces " << aLabel );
      dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace) {
        dt__info( preUpdate(), << "Set grading to face " << aFace );
        aFace->setGrading(
          config().lookup< std::vector< dtReal > >("_grading"), 
          config().lookup< std::vector< dtReal > >("_type")
        );
      }
    }        
    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_edgeLabel"), aLabel
    ) {
      gm->getDtGmshEdgeByPhysical(aLabel)->setGrading(
        config().lookup< std::vector< dtReal > >("_grading")[0], 
        config().lookup< std::vector< dtReal > >("_type")[0]
      );
    }
  }
}
