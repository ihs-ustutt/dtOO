#include "bVONameFaces.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVONameFaces::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVONameFaces, new bVONameFaces())
  );
  
  bVONameFaces::bVONameFaces() {
  }

  bVONameFaces::~bVONameFaces() {
    
  }
  
  void bVONameFaces::bVONameFaces::init( 
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
		//   name="bVONameFaces" 
		//   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
		_face
		= 
		dtXmlParserBase::getAttributeStrVector("face", element);    
  }
  
  void bVONameFaces::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
		//
		// set current model
		//
		::GModel::setCurrent( gm );
    
    logContainer< bVONameFaces > logC(logINFO, "preUpdate()");
    if ( _faceLabel.empty() ) {
      logC() << "Apply automatic naming" << std::endl;
      dt__forAllRefAuto(gm->regions(), aReg) {
        dtInt cc = 0;
        logC() << "Region : " << gm->getPhysicalString(aReg) << std::endl;
        dt__forAllRefAuto( aReg->faces(), aFace ) {
          if ( gm->getPhysicalString(aFace) == "" ) {
            gm->tagPhysical( 
              aFace,
              gm->getPhysicalString(aReg)+"_F_"+stringPrimitive::intToString(cc)
            );
          }          
          logC() 
            << "  Face " << cc << " : " << gm->getPhysicalString(aFace) 
            << std::endl;
          cc++;
          
        }
      }
    }
    else {
      if ( _face.size() == _faceLabel.size() ) {
        //int cc = 0;
        dt__forAllIndex(_face, cc) {
          dtGmshFace * gf = gm->getDtGmshFaceByPhysical( _face[cc] );
          gm->tagPhysical( gf, _faceLabel[cc] );
          logC()
            << logMe::dtFormat(
              "Tag face : %s -> %s"
            ) 
            % _face[cc] % _faceLabel[cc]
            << std::endl;          
        }
      }
      else {
        dtInt counter = 0;      
        dt__forAllRefAuto(gm->faces(), aFace) {
          std::vector< dtInt > pInt = aFace->getPhysicalEntities();
          dt__throwIf(pInt.size()!=0, preUpdate());

          std::string newL = _faceLabel[counter];
          if (newL != "") {
            dtInt pTag = aFace->model()->setPhysicalName(newL, 2, 0);
            aFace->addPhysicalEntity(pTag);
            dtGmshModel::intGEntityVMap map;
            gm->getPhysicalGroups(2, map);          
            logC()
              << logMe::dtFormat(
                "Physical group %d / %s ( %d faces ) -> add face %d"
              ) 
              % newL % pTag % map[ pTag ].size() % aFace->tag() 
              << std::endl;
          }
          counter++;				
        }
      }
    }
  }
}
