#include "geomShape_readIgesOCCAGXmlBuilder.h"
#include "geometryEngine/dtOCCCurveBase.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/geoBuilder/geomSurface_baseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_baseConstructOCC.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <IGESControl_Reader.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <Precision.hxx>
#include <BRep_Tool.hxx>

namespace dtOO {
  geomShape_readIgesOCCAGXmlBuilder
    ::geomShape_readIgesOCCAGXmlBuilder() {
  }

  geomShape_readIgesOCCAGXmlBuilder
    ::~geomShape_readIgesOCCAGXmlBuilder() {
  }

  void geomShape_readIgesOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("filename", toBuild), 
      buildPart()
    );

    std::string fn = dtXmlParserBase::getAttributeStr("filename", toBuild);

    ::IGESControl_Reader reader; 
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
//      ::TopExp_Explorer explorer; 
      for( 
        explorer.Init(reader.Shape(ii),::TopAbs_EDGE); 
        explorer.More(); 
        explorer.Next()
      ) {
        ::TopoDS_Edge aEdge( ::TopoDS::Edge( explorer.Current() ) );
        
        Standard_Real U0;
        Standard_Real U1;
        ::BRep_Tool::Range(aEdge, U0, U1);
        
        Handle(Geom_Curve) aCurve = ::BRep_Tool::Curve( aEdge, U0, U1 );
        if ( !aCurve.IsNull() ) {
          dtOCCCurveBase base(aCurve);
          result->push_back(
            new analyticCurve(
              dt__tmpPtr(
                dtCurve, geomCurve_baseConstructOCC( base ).result()
              )
            )
          );
        }
      }         
    }    
  }
}