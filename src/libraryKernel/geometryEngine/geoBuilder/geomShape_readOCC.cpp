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

#include "geomShape_readOCC.h"
#include "bSplineSurface_convertOCC.h"

#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_baseConstructOCC.h>
#include <geometryEngine/geoBuilder/geomCurve_baseConstructOCC.h>

#include <XSControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <Precision.hxx>
#include <BRep_Tool.hxx>

namespace dtOO {
	geomShape_readOCC::geomShape_readOCC() {

	}

	geomShape_readOCC::geomShape_readOCC( std::string const & filename ) {
    //::IGESControl_Reader igesReader; 
    dt__pH(::XSControl_Reader) reader( new ::IGESControl_Reader() );
    
    if ( 
      stringPrimitive::stringContains(".stp", filename)
      ||
      stringPrimitive::stringContains(".step", filename) 
      ||
      stringPrimitive::stringContains(".STP", filename) 
      ||
      stringPrimitive::stringContains(".STEP", filename) 
    ) {
      reader.reset( new ::STEPControl_Reader() );
    }
    
    ::IFSelect_ReturnStatus stat = reader->ReadFile(filename.c_str()); 
    
    dt__info(
      geomShape_readOCC(), 
      << dt__eval(stat) << std::endl
      << " {" << std::endl
      << "IFSelect_RetVoid," << std::endl
      << "IFSelect_RetDone," << std::endl
      << "IFSelect_RetError," << std::endl
      << "IFSelect_RetFail," << std::endl
      << "IFSelect_RetStop" << std::endl
      << "}"
    );
    reader->PrintCheckLoad( false, ::IFSelect_ItemsByEntity ); 
    
    dtInt nGeos = reader->TransferRoots();
    
    dt__info( geomShape_readOCC(), << nGeos << " successful translations." );
    
    dt__forFromToIndex(1, reader->NbShapes()+1, ii) {
      dt__info(
        geomShape_readOCC(), 
        << dt__eval( reader->Shape(ii).ShapeType() ) << std::endl
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
        explorer.Init(reader->Shape(ii),::TopAbs_FACE); 
        explorer.More(); 
        explorer.Next()
      ) {
        ::TopoDS_Face aFace( ::TopoDS::Face( explorer.Current() ) );

        Handle(Geom_Surface) aSurf = ::BRep_Tool::Surface( aFace );
        if ( !aSurf.IsNull() ) {
          dtOCCSurfaceBase base(aSurf);
          _dtS.push_back(
            geomSurface_baseConstructOCC( base ).result()
          );
        }
        else dt__throwUnexpected(geomShape_readOCC());
      }       
      for( 
        explorer.Init(reader->Shape(ii),::TopAbs_EDGE); 
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
          _dtC.push_back(
            geomCurve_baseConstructOCC( base ).result()
          );
        }
        else dt__throwUnexpected(geomShape_readOCC());
      }         
    }  
	}

	geomShape_readOCC::~geomShape_readOCC() {
		
	}
	
  vectorHandling< dtCurve * > 
  geomShape_readOCC::resultCurveVectorHandling( void ) const {
    vectorHandling< dtCurve * > ret;
    dt__forAllRefAuto(_dtC, aCurv) ret.push_back( aCurv.clone() );
    return ret;
  }
  
  vectorHandling< dtSurface * > 
  geomShape_readOCC::resultSurfaceVectorHandling( void ) const {
    vectorHandling< dtSurface * > ret;
    dt__forAllRefAuto(_dtS, aSurf) ret.push_back( aSurf.clone() );
    return ret;
  }
  
  ptrVectorHandling< dtCurve > const & 
  geomShape_readOCC::resultCurveRef( void ) const {
    return _dtC;
  }
  
  ptrVectorHandling< dtSurface > const & 
  geomShape_readOCC::resultSurfaceRef( void ) const {
    return _dtS;
  }
}
