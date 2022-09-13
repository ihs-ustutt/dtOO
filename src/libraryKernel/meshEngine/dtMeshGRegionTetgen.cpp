#include "dtMeshGRegionTetgen.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include "dtMeshGRegion.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include "dtGmshVertex.h"

#define TETLIBRARY ;
#include "tetgen/tetgen.h"
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <gmsh/meshGRegion.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MTetrahedron.h>

namespace dtOO {  
  dtMeshGRegionTetgen::dtMeshGRegionTetgen(void) : dtMesh3DOperator() {
    
	}

	dtMeshGRegionTetgen::~dtMeshGRegionTetgen() {
    
	}
	
  void dtMeshGRegionTetgen::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _tetOpt 
    = 
    dtXmlParserBase::replaceDependencies(
      qtXmlBase::getAttributeStr("tetgenOption", element), cV
    );
    _tetOpt.append("Y");    
    dt__info( init(), << dt__eval(_tetOpt) );    
	}
	
  void dtMeshGRegionTetgen::operator() (dtGmshRegion * dtgr) {
    ::tetgenio tetIn;
    ::tetgenio tetOut;
     
    //
    // mapping between gmsh and tetgen
    //
    std::map< int, ::MVertex * > MVertexInt;
    std::map< ::MVertex *, dtInt > intMVertex;    

    //
    // debug output
    //
    const_cast<dtGmshModel&>(
      dtgr->refDtGmshModel()
    ).writeSTL(
      "transferToTetgen"
      +
      stringPrimitive::intToString(
        staticPropertiesHandler::getInstance()->thisRank()
      )
      +
      ".stl"
    );
    
    //
    // convert gmsh to tetgen
    //
    convertToTetgen(dtgr, &tetIn, MVertexInt, intMVertex);
    
    //
    // perform meshing
    //
    try {
      tetrahedralize(const_cast< char * >(_tetOpt.c_str()), &tetIn, &tetOut);
    }
    catch (int error) {
      dt__throw(operator(), << "Error in tetgen.");
    }
    
    //
    // convert tetgen to gmsh
    //
    convertFromTetgen(dtgr, &tetOut, MVertexInt, intMVertex);
    
    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
  }
  
  void dtMeshGRegionTetgen::convertToTetgen(
    dtGmshRegion * gr, tetgenio * in,
    std::map< int, ::MVertex * > & MVertexInt,
    std::map< ::MVertex *, dtInt > & intMVertex    
  ) {   
    //
    // start indexing with one
    //
    dtInt nodeIndex = 1;    
    std::vector< ::MTriangle * > triangle;
    //
    // faces
    //
    std::vector< dtGmshFace * > ff 
    = 
    dtGmshModel::cast2DtGmshFace( gr->faces() );
    dt__forAllRefAuto( ff, fit ) {
      // face vertices and elements
      enqueueMVertices(
        fit->mesh_vertices, nodeIndex, MVertexInt, intMVertex
      );
      enqueueMTriangles(fit->triangles, triangle);
      
      //
      // edges
      //
      std::vector< dtGmshEdge * > ee 
      = 
      dtGmshModel::cast2DtGmshEdge( fit->edges() );
      dt__forAllRefAuto( ee, eit ) {
        // edge vertices
        enqueueMVertices(
          eit->mesh_vertices, nodeIndex, MVertexInt, intMVertex
        );
        //
        // vertices
        //
        std::vector< dtGmshVertex * > vv
        =
        dtGmshModel::cast2DtGmshVertex( eit->vertices() );
        dt__forAllRefAuto( vv, vit ) {
          // vertex vertices
          enqueueMVertices(
            vit->mesh_vertices, nodeIndex, MVertexInt, intMVertex
          );
        }
      }              
    }      

    //
    // set tetgen structure
    //
    in->mesh_dim = 3;
    in->firstnumber = 1;
    in->numberofpoints = nodeIndex-1;
    in->pointlist = new REAL[in->numberofpoints * 3];
    in->pointmarkerlist = NULL;

    // vertices
    dt__forAllRefAuto( MVertexInt, it ) {
      in->pointlist[(it.first - 1) * 3 + 0] = (it.second)->x();
      in->pointlist[(it.first - 1) * 3 + 1] = (it.second)->y();
      in->pointlist[(it.first - 1) * 3 + 2] = (it.second)->z();
    }

    in->numberoffacets = triangle.size();
    in->facetlist = new tetgenio::facet[in->numberoffacets];
    in->facetmarkerlist = NULL;//new int[in.numberoffacets];

    // faces
    dtInt faceIndex = 0;
    dt__forAllRefAuto(triangle, aTri) {
      tetgenio::facet * f = &in->facetlist[faceIndex];
      tetgenio::init(f);
      f->numberofholes = 0;
      f->numberofpolygons = 1;
      f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
      tetgenio::polygon *p = &f->polygonlist[0];
      tetgenio::init(p);
      p->numberofvertices = 3;
      p->vertexlist = new int[p->numberofvertices];
      p->vertexlist[0] = intMVertex[ aTri->getVertex(0) ];
      p->vertexlist[1] = intMVertex[ aTri->getVertex(1) ];
      p->vertexlist[2] = intMVertex[ aTri->getVertex(2) ];
      faceIndex++;
    }
  }
  
  void dtMeshGRegionTetgen::convertFromTetgen(
    dtGmshRegion * gr, tetgenio * out,
    std::map< int, ::MVertex * > & MVertexInt,
    std::map< ::MVertex *, dtInt > & intMVertex    
  ) {
    //
    // get new vertices
    //
    const dtInt initialSize = (int)MVertexInt.size();
    for(int i = initialSize; i < out->numberofpoints; i++){
      MVertex *v = new MVertex(out->pointlist[i * 3 + 0],
                               out->pointlist[i * 3 + 1],
                               out->pointlist[i * 3 + 2], 
                               gr);
      gr->mesh_vertices.push_back(v);
      MVertexInt[ i+1 ] = v;
      intMVertex[ v ] = i+1;
    }

    dt__info(
      convertFromTetgen(), 
      << logMe::dtFormat(
        "%d points %d edges and %d faces in the initial mesh"
      ) % out->numberofpoints % out->numberofedges % out->numberoftrifaces
    );
  

    //
    // put all tetrahedra in ::GRegion
    //
    for(int i = 0; i < out->numberoftetrahedra; i++) {
      ::MVertex * v1 = MVertexInt[ out->tetrahedronlist[i * 4 + 0] ];
      ::MVertex * v2 = MVertexInt[ out->tetrahedronlist[i * 4 + 1] ];
      ::MVertex * v3 = MVertexInt[ out->tetrahedronlist[i * 4 + 2] ];
      ::MVertex * v4 = MVertexInt[ out->tetrahedronlist[i * 4 + 3] ];
      ::MTetrahedron * t = new  ::MTetrahedron(v1, v2, v3, v4);
      gr->tetrahedra.push_back(t);
    }
  }  
  
  void dtMeshGRegionTetgen::enqueueMVertices(
    std::vector< ::MVertex * > & vv,
    dtInt & nodeIndex,
    std::map< int, ::MVertex * > & MVertexInt,
    std::map< ::MVertex *, dtInt > & intMVertex
  ) {
    dt__forAllIndex(vv, ii) {
      ::MVertex * mv = vv[ii];
      MVertexInt[ nodeIndex ] = mv;
      intMVertex[ mv ] = nodeIndex;
      nodeIndex++;
    }
  }

  void dtMeshGRegionTetgen::enqueueMTriangles(
    std::vector< ::MTriangle * > & toEnqueue,
    std::vector< ::MTriangle * > & triangle
  ) {
    dt__forAllRefAuto(toEnqueue, aTri) triangle.push_back( aTri );
  }
}
