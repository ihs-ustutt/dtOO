#include "dtMeshGRegionTetgen.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <xmlHeaven/qtXmlBase.h>

#include "dtMeshGRegion.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include "dtGmshVertex.h"

#define TETLIBRARY ;
#include "tetgen/tetgen.h"

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
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _tetOpt = qtXmlBase::getAttributeStr("tetgenOption", element);
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
    std::map< ::MVertex *, int > intMVertex;    

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
    std::map< ::MVertex *, int > & intMVertex    
  ) {   
    //
    // start indexing with one
    //
    int nodeIndex = 1;    
    std::vector< ::MTriangle * > triangle;
    //
    // faces
    //
    std::list< dtGmshFace * > ff 
    = 
    dtGmshModel::cast2DtGmshFace( gr->faces() );
    dt__forFromToIter(
      std::list< dtGmshFace * >::iterator, ff.begin(), ff.end(), fit
    ) {
      // face vertices and elements
      enqueueMVertices(
        (*fit)->mesh_vertices, nodeIndex, MVertexInt, intMVertex
      );
      enqueueMTriangles((*fit)->triangles, triangle);
      
      //
      // edges
      //
      std::list< dtGmshEdge * > ee 
      = 
      dtGmshModel::cast2DtGmshEdge( (*fit)->edges() );
      dt__forFromToIter(
        std::list< dtGmshEdge * >::iterator, ee.begin(), ee.end(), eit
      ) {
        // edge vertices
        enqueueMVertices(
          (*eit)->mesh_vertices, nodeIndex, MVertexInt, intMVertex
        );
        //
        // vertices
        //
        std::list< dtGmshVertex * > vv
        =
        dtGmshModel::cast2DtGmshVertex( (*eit)->vertices() );
        dt__forFromToIter(
          std::list< dtGmshVertex * >::iterator, vv.begin(), vv.end(), vit
        ) {
          // vertex vertices
          enqueueMVertices(
            (*vit)->mesh_vertices, nodeIndex, MVertexInt, intMVertex
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
    int faceIndex = 0;
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
    std::map< ::MVertex *, int > & intMVertex    
  ) {
    //
    // get new vertices
    //
    const int initialSize = (int)MVertexInt.size();
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
    int & nodeIndex,
    std::map< int, ::MVertex * > & MVertexInt,
    std::map< ::MVertex *, int > & intMVertex
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