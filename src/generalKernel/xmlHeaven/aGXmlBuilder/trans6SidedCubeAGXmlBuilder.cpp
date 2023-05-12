#include "trans6SidedCubeAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h>
#include <geometryEngine/dtSurface.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool trans6SidedCubeAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      trans6SidedCubeAGXmlBuilder, 
      new trans6SidedCubeAGXmlBuilder()
    )
  );
  
  trans6SidedCubeAGXmlBuilder::trans6SidedCubeAGXmlBuilder() {
  }

  trans6SidedCubeAGXmlBuilder::~trans6SidedCubeAGXmlBuilder() {
  }

  void trans6SidedCubeAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    lvH_constValue const * const cV,  
    lvH_analyticFunction const * const aF,  
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );
    
    dt__pVH(map2dTo3d) mm;
    std::vector< ::QDomElement > wElementVec 
    = 
    dtXmlParserBase::getChildVector( "analyticGeometry", toBuild );
    if (wElementVec.size() == 6) {
      std::vector< bool > exchange(6, false);
      std::vector< std::string > reverse(6,"");
      if ( dtXmlParserBase::hasAttribute("exchange", toBuild) ) {
        exchange 
        = 
        dtXmlParserBase::getAttributeBoolVector( "exchange", toBuild );    
      }
      if ( dtXmlParserBase::hasAttribute("reverse", toBuild) ) {
        reverse
        = 
        dtXmlParserBase::getAttributeStrVector( "reverse", toBuild );    
      }      
      dt__info(buildPart(), << "exchange = " << exchange);
      dt__info(buildPart(), << "reverse = " << reverse);
      
      dtInt counter = 0;
      dt__forAllIter(std::vector< ::QDomElement >, wElementVec, it) {
        
        //
        // get analyticGeometry
        //
        dt__pH(analyticGeometry const) aG_t(
          dtXmlParserBase::createAnalyticGeometry(*it, bC, cV, aF, aG)
        );          
        //
        // check if it is a map2dTo3d
        //
        dt__pH(dtSurface) dtS(
          analyticSurface::MustConstDownCast(
            aG_t.get()
          )->ptrConstDtSurface()->clone()
        );
        if ( exchange[counter] ) {
          dtS.reset(
            bSplineSurface_exchangeSurfaceConstructOCC( dtS.get() ).result()
          );          
        }
        if ( reverse[counter] != "" ) {
          if ( reverse[counter] == "u" ) {
            dtS.reset(
              geomSurface_surfaceReverseConstructOCC( 
                dtS.get(), true, false 
              ).result()
            );          
          }
          else if ( reverse[counter] == "v" ) {
            dtS.reset(
              geomSurface_surfaceReverseConstructOCC( 
                dtS.get(), false, true 
              ).result()            
            );
          }
          else if ( reverse[counter] == "uv" ) {
            dtS.reset(
              geomSurface_surfaceReverseConstructOCC( 
                dtS.get(), true, true
              ).result()            
            );
          }
          else dt__throwUnexpected(buildPart());
        }        
        mm.push_back( 
          new analyticSurface( dtS.get() )//map2dTo3d::MustDownCast(aG_t->clone()) 
        );
        counter++;
      }
    }
    else if (wElementVec.size() == 2) {     
      std::vector< bool > exchange(2, false);
      std::vector< std::string > reverse(2,"");
      if ( dtXmlParserBase::hasAttribute("exchange", toBuild) ) {
        exchange 
        = 
        dtXmlParserBase::getAttributeBoolVector( "exchange", toBuild );    
      }
      if ( dtXmlParserBase::hasAttribute("reverse", toBuild) ) {
        reverse
        = 
        dtXmlParserBase::getAttributeStrVector( "reverse", toBuild );    
      }        
      dt__info(buildPart(), << "exchange = " << exchange);
      
      vectorHandling< dtSurface const * > cDtS;
      dt__forFromToIndex(0,2,ii) {
        dt__pH(dtSurface const) thisSurface( 
          analyticSurface::ConstDownCast(
            dt__tmpPtr(
              analyticGeometry,
              dtXmlParserBase::createAnalyticGeometry(
                &wElementVec[ii], bC, cV, aF, aG
              )
            )
          )->ptrConstDtSurface()->clone()
        );
        
        if ( exchange[ii] ) {
          thisSurface.reset(
            bSplineSurface_exchangeSurfaceConstructOCC(thisSurface.get()).result()
          );
        }
        if ( reverse[ii] != "" ) {
          if ( reverse[ii] == "u" ) {
            thisSurface.reset(
              geomSurface_surfaceReverseConstructOCC( 
                thisSurface.get(), true, false 
              ).result()
            );          
          }
          else if ( reverse[ii] == "v" ) {
            thisSurface.reset(
              geomSurface_surfaceReverseConstructOCC( 
                thisSurface.get(), false, true 
              ).result()            
            );
          }
          else if ( reverse[ii] == "uv" ) {
            thisSurface.reset(
              geomSurface_surfaceReverseConstructOCC( 
                thisSurface.get(), true, true
              ).result()            
            );
          }
          else dt__throwUnexpected(buildPart());
        }         
        cDtS.push_back( thisSurface->clone() );
      }

      vectorHandling< dtSurface * > dtS
      = 
      bSplineSurfaces_bSplineSurfaceSkinConstructOCC(cDtS).result();
      dt__forAllIndex(dtS, ii) {
        mm.push_back( new analyticSurface(dtS[ii]) );
      }
      cDtS.destroy();
      dtS.destroy();
    }			
    else {
      dt__throw(
        buildPart(), 
        << dt__eval(wElementVec.size()) << std::endl
        << "Supported only 2 and 6." << std::endl
      );
    }

    //
    // create analyticGeometry
    //      
    result->push_back( 
      new trans6SidedCube(
        &(mm[0]), &(mm[1]), &(mm[2]), &(mm[3]), &(mm[4]), &(mm[5])
      ) 
    );
  }
}
