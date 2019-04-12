#include "customGmsh.h"
#include "xmlHeaven/dtXmlParserBase.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {
	customGmsh::customGmsh() : gmshBoundedVolume() {
	}

	customGmsh::~customGmsh() {
	}
  
  void customGmsh::init( 
    ::QDomElement const & element,
		baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV
	) {
    //
    // init gmshBoundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);
		
		//
		// set current model
		//
		::GModel::setCurrent( _gm );

		dt__forAllRefAuto(
      qtXmlPrimitive::getChildVector(element), anEl
    ) {    
      //
      // analyticGeometry - faces
      //
      if ( qtXmlPrimitive::is("analyticGeometry", anEl) ) {
        dt__pH(analyticGeometry) thisAG;
        thisAG.reset(
          dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG)
        );
        int thisId = 0;
        _gm->addIfToGmshModel( thisAG.get(), &thisId );
      }
      //
      // customFaces
      //
      else if ( qtXmlPrimitive::is("customFace", anEl) ) {
        std::list< ::GEdge * > edges;
        std::vector< int > ori;
        dt__forAllRefAuto(
          qtXmlPrimitive::getChildVector(
            "analyticGeometry", 
            qtXmlPrimitive::getChild("edgeLoop", anEl)
          ), 
          aEdgeLabel
        ) {
          dt__pH(analyticGeometry) thisEdge;
          thisEdge.reset(
            dtXmlParserBase::createAnalyticGeometry(aEdgeLabel, bC, cV, aF, aG)
          );
          int tag;
          _gm->addIfToGmshModel(thisEdge.get(), &tag);
          if ( tag<0 ) {
            ori.push_back(-1);
          }
          else {
            ori.push_back(1);
          }
          edges.push_back( _gm->getEdgeByTag( tag ) );
        }

        dt__pH(map2dTo3d) thisFace;
        thisFace.reset(
          map2dTo3d::MustDownCast(      
            dtXmlParserBase::createAnalyticGeometry(
              qtXmlPrimitive::getChild(
                "analyticGeometry", 
                qtXmlPrimitive::getChild("face", anEl)
              ), 
              bC, cV, aF, aG
            )
          )
        );
        int tag = 0;
        _gm->addIfFaceToGmshModel(thisFace.get(), &tag, edges, ori);
      }
	  }
    
    //
    // make a region
    //
    int tag = 0;
    _gm->addIfRegionToGmshModel(
      NULL,
      &tag,
      _gm->faces(),
      std::vector< int >(_gm->faces().size(), 1)
    );
  }
}
