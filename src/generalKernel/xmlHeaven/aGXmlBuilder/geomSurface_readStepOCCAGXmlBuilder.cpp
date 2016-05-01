#include "geomSurface_readStepOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/geoBuilder/geomSurface_baseConstructOCC.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <STEPControl_Reader.hxx>
#include <Geom_Surface.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <Precision.hxx>
#include <BRep_Tool.hxx>

namespace dtOO {
  geomSurface_readStepOCCAGXmlBuilder
    ::geomSurface_readStepOCCAGXmlBuilder() {
  }

  geomSurface_readStepOCCAGXmlBuilder
    ::~geomSurface_readStepOCCAGXmlBuilder() {
  }

  void geomSurface_readStepOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("filename", toBuild), 
      buildPart()
    );

    std::string fn = dtXmlParserBase::getAttributeStr("filename", toBuild);

    ::STEPControl_Reader reader; 
    ::IFSelect_ReturnStatus stat = reader.ReadFile(fn.c_str()); 
    
    dt__info(
      buildPart(), 
      << dt__eval(stat) << std::endl
      << " {" << std::endl
      << "IFSelect_RetVoid," << std::endl
      << "IFSelect_RetDone," << std::endl
      << "IFSelect_RetError," << std::endl
      << "IFSelect_RetFail," << std::endl
      << "IFSelect_RetStop" << std::endl
      << "}"
    );
    reader.PrintCheckLoad( false, ::IFSelect_ItemsByEntity ); 
    
    dt__info( buildPart(), << dt__eval(reader.TransferRoots()) );
    
    dt__forFromToIndex(1, reader.NbShapes()+1, ii) {
      dt__info(
        buildPart(), 
        << dt__eval( reader.Shape(ii).ShapeType() ) << std::endl
        << "{" << std::endl
        << "TopAbs_COMPOUND," << std::endl
        << "TopAbs_COMPSOLID," << std::endl
        << "TopAbs_SOLID," << std::endl
        << "TopAbs_SHELL," << std::endl
        << "TopAbs_FACE," << std::endl
        << "TopAbs_WIRE," << std::endl
        << "TopAbs_EDGE," << std::endl
        << "TopAbs_VERTEX," << std::endl
        << "TopAbs_SHAPE" << std::endl
        << "}"
      );
      
      ::TopExp_Explorer explorer; 
      for( 
        explorer.Init(reader.Shape(ii),::TopAbs_FACE); 
        explorer.More(); 
        explorer.Next()
      ) {
        ::TopoDS_Face aFace( ::TopoDS::Face( explorer.Current() ) );

        Handle(Geom_Surface) aSurf = ::BRep_Tool::Surface( aFace );
        if ( !aSurf.IsNull() ) {
          dtOCCSurfaceBase base(aSurf);
          if ( geomSurface_baseConstructOCC::support( base ) ) {
            result->push_back( 
              new analyticSurface(
                dt__tmpPtr(
                  dtSurface, 
                  geomSurface_baseConstructOCC( base ).result()
                )
              )
            );
          }
        }
      }       
    }    
  }
}