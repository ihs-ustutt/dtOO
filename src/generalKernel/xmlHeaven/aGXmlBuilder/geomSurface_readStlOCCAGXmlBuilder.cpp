#include "geomSurface_readStlOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <progHelper.h>

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <gmsh/MLine.h>
#include <meshEngine/dtOMMesh.h>
#include <meshEngine/dtOMDynamicVertexField.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
  bool geomSurface_readStlOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomSurface_readStlOCCAGXmlBuilder, 
      new geomSurface_readStlOCCAGXmlBuilder()
    )
  );
  
  geomSurface_readStlOCCAGXmlBuilder
    ::geomSurface_readStlOCCAGXmlBuilder() {
  }

  geomSurface_readStlOCCAGXmlBuilder
    ::~geomSurface_readStlOCCAGXmlBuilder() {
  }

  void geomSurface_readStlOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("filename", toBuild), 
      buildPart()
    );

    std::string fn = dtXmlParserBase::getAttributeStr("filename", toBuild);

    dtOMMesh om;
    om.readMesh(fn);
    dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), vIt) {
      omPoint const & pp = om.point(*vIt); 
      om.data(*vIt).MVertex( new ::MVertex(pp[0], pp[1], pp[2], NULL) );
    }
    om.createNewElements();

      
    //
    // find a boundary vertex
    //
    omHalfedgeH heH;
    dt__forFromToIter(omVertexI, om.vertices_begin(), om.vertices_end(), vIt) {
      dt__quickdebug( << dt__eval( om.is_boundary( *vIt ) ) );
      dt__forFromToIter(
        omVertexOHalfedgeI, om.voh_begin(*vIt), om.voh_end(*vIt), heIt
      ) {
        if ( om.is_boundary(*heIt) ) {
          heH = *heIt;
          break;
        }
      }
      if ( heH.is_valid() ) break;
    }
    dt__throwIf( !heH.is_valid(), buildPart() );
    
    //
    // create angle field
    //
    dtOMDynamicVertexField< dtReal > boundAngle("boundAngle", om, 0.);
    std::pair< omHalfedgeH, omHalfedgeH > hePair( 
      heH, om.next_halfedge_handle( heH ) 
    );

    //
    // calculate max angles
    //
    std::vector< dtReal > maxAngle( 4, -std::numeric_limits<dtReal>::max() );
    std::vector< omVertexH > maxAnglePos(4);
    do {
      omVertexH vH0 = om.from_vertex_handle( hePair.first );
      omVertexH vH1 = om.to_vertex_handle( hePair.first );
      omVertexH vH2 = om.to_vertex_handle( hePair.second );
      dtVector3 v0 
      =
      dtGmshModel::extractPosition( om.at(vH1) )
      -
      dtGmshModel::extractPosition( om.at(vH0) );
      dtVector3 v1
      =
      dtGmshModel::extractPosition( om.at(vH2) )
      -
      dtGmshModel::extractPosition( om.at(vH1) );
      boundAngle[ vH1 ]
      = 
      acos(
        dtLinearAlgebra::dotProduct(v0, v1)
        /
        ( dtLinearAlgebra::length(v0) * dtLinearAlgebra::length(v1) )
      );
      
      dt__warnIfWithSolution(
        std::isnan(boundAngle[ vH1 ]), boundAngle[ vH1 ] = 0., buildPart()
      );
      
      if ( boundAngle[ vH1 ] > maxAngle[0] ) {
        maxAngle[ 0 ] = boundAngle[ vH1 ];
        maxAnglePos[ 0 ] = vH1;
      }
      else if ( boundAngle[ vH1 ] > maxAngle[1] ) {
        maxAngle[ 1 ] = boundAngle[ vH1 ];
        maxAnglePos[ 1 ] = vH1;
      }      
      else if ( boundAngle[ vH1 ] > maxAngle[2] ) {
        maxAngle[ 2 ] = boundAngle[ vH1 ];
        maxAnglePos[ 2 ] = vH1;
      }      
      else if ( boundAngle[ vH1 ] > maxAngle[3] ) {
        maxAngle[ 3 ] = boundAngle[ vH1 ];
        maxAnglePos[ 3 ] = vH1;
      }
      dt__quickdebug( << dt__eval( boundAngle[ vH1 ] ) );
      hePair.first = om.next_halfedge_handle( hePair.first );
      hePair.second = om.next_halfedge_handle( hePair.second );
    }
    while ( om.to_vertex_handle( heH ) != om.to_vertex_handle(hePair.first) );
    
    //
    //
    //
    dt__info(
      buildPart(), 
      << logMe::dtFormat("max %8.3e %8.3e %8.3e %8.3e")
      % maxAngle[0] % maxAngle[1] % maxAngle[2] % maxAngle[3]
      << std::endl
      << logMe::dtFormat("pos %8i %8i %8i %8i")
      % maxAnglePos[0] % maxAnglePos[1] % maxAnglePos[2] % maxAnglePos[3]      
    );
    maxAngle.push_back(maxAngle[0]);
    maxAnglePos.push_back(maxAnglePos[0]);
    
//    //
//    // create vertices
//    //
//    dt__forFromToIndex(0, 4, ii) {
//      om[ maxAnglePos[ii] ]->setEntity( vv[ii] );
//      vv[ii]->setPosition( 
//        dtGmshModel::extractPosition(om[ maxAnglePos[ii] ]) 
//      );
//      vv[ii]->mesh_vertices.push_back(
//        om[ maxAnglePos[ii] ]
//      );
//    }
    
    //
    // create edges
    //
    dt__forFromToIndex(0, 4, ii) {
      std::vector< dtPoint3 > xyzG;
      //
      // find start halfedge
      //
      omHalfedgeH startHE;
      dt__forFromToIter(
        omVertexOHalfedgeI, 
        om.voh_begin(maxAnglePos[ii]), 
        om.voh_end(maxAnglePos[ii]), 
        heIt
      ) {
        if ( om.is_boundary(*heIt) ) {
          startHE = *heIt;
          break;
        }
      }
      dt__throwIf(!startHE.is_valid(), buildPart());
      
      xyzG.push_back( 
        dtGmshModel::extractPosition( om[ om.from_vertex_handle(startHE) ] ) 
      );     
      do {
        omVertexH to = om.to_vertex_handle( startHE );
        
        xyzG.push_back( dtGmshModel::extractPosition(om[to]) );             
        startHE = om.next_halfedge_handle(startHE);
      }
      while ( om.from_vertex_handle( startHE ) != maxAnglePos[ii+1] );
      
      dt__info(buildPart(), << dt__eval(xyzG.size()));
      result->push_back( 
        new analyticCurve(
          dt__tmpPtr(
            dtCurve, bSplineCurve_pointInterpolateConstructOCC( xyzG ).result()
          )
        )
      );           
    }
  }
}