#include "dtMacroMesh.h"

#include <logMe/logMe.h>
#include <algorithm>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/systemHandling.h>
#include <baseContainer/pointContainer.h>
#include <vector>
#include <do/coDoSet.h>
#include <do/coDoUnstructuredGrid.h>

#define __caCThis \
  const_cast< dtMacroMesh * >(this)

#define __forAllVert(in) \
    for ( \
      ovmVertexI v_it = this->vertices_begin(); \
      v_it != this->vertices_end(); \
      ++v_it \
    ) { \
      in \
    }

#define __forAllCell(in) \
    for ( \
      ovmCellI c_it = this->cells_begin(); \
      c_it != this->cells_end(); \
      ++c_it \
    ) { \
      in \
    }

#define __forAllFace(in) \
    for ( \
      ovmFaceI f_it = this->faces_begin(); \
      f_it != this->faces_end(); \
      ++f_it \
    ) { \
      in \
    }

#define __forAllEdge(in) \
    for ( \
      ovmEdgeI e_it = this->edges_begin(); \
      e_it != this->edges_end(); \
      ++e_it \
    ) { \
      in \
    }

namespace dtOO { 
  dtMacroMesh::dtMacroMesh() {
    // vertex id
    OpenVolumeMesh::VertexPropertyT< int > vId 
    = 
    this->request_vertex_property< int >("id");
    this->set_persistent(vId);
    // snap
    OpenVolumeMesh::VertexPropertyT< std::vector< int > > snap
    = 
    this->request_vertex_property< std::vector< int > >("snap");
    this->set_persistent(snap);
    // snap3d
    OpenVolumeMesh::VertexPropertyT< std::vector< int > > snap3d
    = 
    this->request_vertex_property< std::vector< int > >("snap3d");
    this->set_persistent(snap3d);    
    // uu
    OpenVolumeMesh::VertexPropertyT< std::vector< float > > uv
    = 
    this->request_vertex_property< std::vector< float > >("uv");
    this->set_persistent(uv);
    // cell name
    OpenVolumeMesh::CellPropertyT< std::string > cLabel
    = 
    this->request_cell_property< std::string >("label");
    this->set_persistent(cLabel);
    
    OpenVolumeMesh::CellPropertyT< std::vector< std::string > > cOption
    = 
    this->request_cell_property< std::vector< std::string > >("option");
    this->set_persistent(cOption);
    
    OpenVolumeMesh::HalfEdgePropertyT< std::vector< std::string > > hePosition
    = 
    this->request_halfedge_property< std::vector< std::string > >("position");
    this->set_persistent(hePosition);

//    OpenVolumeMesh::EdgePropertyT< int > eNElements
//    = 
//    this->request_edge_property< int >("nElements");
//    this->set_persistent(eNElements);
//    
//    OpenVolumeMesh::EdgePropertyT< float > eGrading
//    = 
//    this->request_edge_property< float >("grading");
//    this->set_persistent(eGrading);    
  }

  dtMacroMesh::dtMacroMesh(const dtMacroMesh& orig) {
  }

  dtMacroMesh::~dtMacroMesh() {
  }
  
  void dtMacroMesh::addVertex( dtPoint3 const vertex, int const id) {
    ovmVertexH vv 
    = 
    this->add_vertex( 
      OpenVolumeMesh::Vec3f( 
        vertex.x(), 
        vertex.y(), 
        vertex.z() 
      ) 
    );
    
    this->request_vertex_property< int >("id")[vv] = id;
  }

  void dtMacroMesh::addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const sId0, int const sId1, int const sId2) {
    this->addVertex(vertex, id);
    ovmVertexH const vH = getVertexHandle(id);
    this->request_vertex_property< std::vector< float > >("uv")[vH].push_back(uu);
    this->request_vertex_property< std::vector< float > >("uv")[vH].push_back(vv);
    
    if (sId0 != -1) {
      this->request_vertex_property< std::vector< int > >("snap")[vH].push_back(sId0);
    }
    if (sId1 != -1) {
      this->request_vertex_property< std::vector< int > >("snap")[vH].push_back(sId1);
    }
    if (sId2 != -1) {
      this->request_vertex_property< std::vector< int > >("snap")[vH].push_back(sId2);
    }
  }  

  void dtMacroMesh::snapAllVerticesToMap3dTo3d( int const mId) {
    OpenVolumeMesh::VertexPropertyT< std::vector< int > > snap3d
    =
    this->request_vertex_property< std::vector< int > >("snap3d");
    
    __forAllVert(
      snap3d[*v_it].push_back(mId);    
    );
  }
  
  dtPoint3 const dtMacroMesh::getVertex( int const id ) const {
    ovmVertexH vH = getVertexHandle(id);
    OpenVolumeMesh::Vec3f vv = this->vertex(vH);
    return dtPoint3(vv[0], vv[1], vv[2]);
  }
  
  dtPoint2 dtMacroMesh::getVertexUV( int const id ) const {
    ovmVertexH vH = getVertexHandle(id);
    
    return dtPoint2(
             __caCThis->request_vertex_property< std::vector< float > >("uv")[vH][0],
             __caCThis->request_vertex_property< std::vector< float > >("uv")[vH][1]
           );
  }
  
  std::vector< int > dtMacroMesh::getVertexSnapSurfId( int const id ) const {
    ovmVertexH vH = getVertexHandle(id);
    
    return __caCThis->request_vertex_property< std::vector< int > >("snap")[vH];
  }

  std::vector< int > dtMacroMesh::getVertexSnapVolumeId( int const id ) const {
    ovmVertexH vH = getVertexHandle(id);
    
    return __caCThis->request_vertex_property< std::vector< int > >("snap3d")[vH];
  }  
  
  int dtMacroMesh::getNVertices( void ) const {
    return this->n_vertices();
  }

  int dtMacroMesh::getNEdges( void ) const {
    return this->n_edges();
  }

  int dtMacroMesh::getNBlocks( void ) const {
    return this->n_cells();
  }  

  int dtMacroMesh::getNFaces( void ) const {
    return this->n_faces();
  }
  
  void dtMacroMesh::addBlock( 
         int const zero, 
         int const one,
         int const two,
         int const three,
         int const four,
         int const five,
         int const six,
         int const seven,
         std::string const blockName
  ) {
    
    std::vector< ovmVertexH > vH;
    vH.push_back( getVertexHandle(zero) );
    vH.push_back( getVertexHandle(one) );
    vH.push_back( getVertexHandle(two) );
    vH.push_back( getVertexHandle(three) );
    vH.push_back( getVertexHandle(four) );
    vH.push_back( getVertexHandle(five) );
    vH.push_back( getVertexHandle(six) );
    vH.push_back( getVertexHandle(seven) );
    
    ovmCellH cH 
    = 
    this->addCellByVerts(
      vH[0], vH[1], vH[5], vH[4], vH[3], vH[7], vH[6], vH[2], true
    );
        
    this->request_cell_property< std::string >("label")[cH] = blockName;

    OpenVolumeMesh::HalfEdgePropertyT< std::vector < std::string > > cPos
    =
    this->request_halfedge_property< std::vector< std::string > >("position");
    
    std::vector< ovmHalfEdgeH > heH;
    heH.push_back( this->halfedge( vH[0], vH[1] ) );
    heH.push_back( this->halfedge( vH[3], vH[2] ) );
    heH.push_back( this->halfedge( vH[7], vH[6] ) );
    heH.push_back( this->halfedge( vH[4], vH[5] ) );
    heH.push_back( this->halfedge( vH[0], vH[3] ) );
    heH.push_back( this->halfedge( vH[1], vH[2] ) );
    heH.push_back( this->halfedge( vH[5], vH[6] ) );
    heH.push_back( this->halfedge( vH[4], vH[7] ) );
    heH.push_back( this->halfedge( vH[0], vH[4] ) );
    heH.push_back( this->halfedge( vH[1], vH[5] ) );
    heH.push_back( this->halfedge( vH[2], vH[6] ) );
    heH.push_back( this->halfedge( vH[3], vH[7] ) );
    
    dt__FORALL(heH, ii,
      std::string pos = stringPrimitive().intToString(ii)+"@"+blockName;
      cPos[heH[ii]].push_back(pos);
      cPos[this->opposite_halfedge_handle(heH[ii])].push_back(pos);
    );
  }
  std::string const dtMacroMesh::getBlockLabel(int const id) const {
    return __caCThis->request_cell_property< std::string >("label")[getCellHandle(id)];
  }
  
  void dtMacroMesh::addBlockOption(std::string const blockName, std::string const optionName) {
    ovmCellH cH = getCellHandle(blockName);
    
    this->request_cell_property< std::vector< std::string > >("option")[cH].push_back(optionName);
  }
  
  bool dtMacroMesh::hasBlockOption(std::string const blockName, std::string const optionName) const {
    ovmCellH cH = getCellHandle(blockName);
    
    std::vector< std::string > sV 
    = 
    __caCThis->request_cell_property< std::vector< std::string > >("option")[cH];
    
    return ( std::find(sV.begin(), sV.end(), optionName) != sV.end() );
  }
  
  covise::coDoSet * dtMacroMesh::toCoDoSet(char const * const str) const {
    ptrHandling< covise::coDoUnstructuredGrid > ug(
      new covise::coDoUnstructuredGrid(
            "preGrid", 
            getNBlocks(),
            8*getNBlocks(), 
            getNVertices(),
            1
      )
    );
    int * elem;
    int * conn;
    float * xx;
    float * yy;
    float * zz;
    int * tl;
   
    ug->getAddresses(&elem, &conn, &xx, &yy, &zz);
    ug->getTypeList(&tl);
    
    int ii = 0;
    __forAllVert(
      dtPoint3 vv = toDtPoint3(*v_it);
      xx[ii] = vv.x();
      yy[ii] = vv.y();
      zz[ii] = vv.z();
      ii++;
    );
    int cellC = 0;
    int connC = 0;
    for (
      ovmCellI c_it = this->cells_begin();
      c_it != this->cells_end();
      ++c_it
    ) {
      elem[cellC] = connC;
      tl[cellC] = TYPE_HEXAEDER;
      ovmHexVertexI hv_it(*c_it, this);
      conn[elem[cellC]+0] = (hv_it+0)->idx();
      conn[elem[cellC]+1] = (hv_it+1)->idx();
      conn[elem[cellC]+2] = (hv_it+7)->idx();
      conn[elem[cellC]+3] = (hv_it+4)->idx();
      conn[elem[cellC]+4] = (hv_it+3)->idx();
      conn[elem[cellC]+5] = (hv_it+2)->idx();
      conn[elem[cellC]+6] = (hv_it+6)->idx();
      conn[elem[cellC]+7] = (hv_it+5)->idx();
      connC = connC + 8;
      cellC++;
    }
//    );    
    
    ptrHandling< covise::coDistributedObject * > cdo( new covise::coDistributedObject*[2] );
    cdo.get()[0] = ug.get();
    cdo.get()[1] = NULL;
    
    covise::coDoSet * set = new covise::coDoSet(str, cdo.get());
    return set;
  }
  
  void dtMacroMesh::dump( void ) const {
    DTBUFFERINIT();
    DTBUFFER(
      << dt__eval(this->n_vertices()) << std::endl
      << dt__eval(this->n_edges()) << std::endl
      << dt__eval(this->n_halfedges()) << std::endl
      << dt__eval(this->n_faces()) << std::endl
      << dt__eval(this->n_halffaces()) << std::endl
      << dt__eval(this->n_cells()) << std::endl
    );
    
    for (ovmCellI c_it = this->cells_begin(); c_it!=this->cells_end(); ++c_it) {
      DTBUFFER( 
        << "cell " << dt__eval(c_it)
        << ", label = " << __caCThis->request_cell_property< std::string >("label")[*c_it] 
        << std::endl
      );
      DTBUFFER( << "CellVertexIter" << std::endl);//std::endl);
      ovmCellVertexI cv_it = this->cv_iter( *c_it );
      for (cv_it; cv_it.valid(); ++cv_it) {
        OpenVolumeMesh::Vec3f vv = this->vertex( *cv_it );
        DTBUFFER( 
          << vv
          << " id = " << __caCThis->request_vertex_property< int >("id")[*cv_it] 
          << std::endl 
        );
      }
      DTBUFFER( << std::endl );

      DTBUFFER( << "HalfFaceVertexIter" << std::endl);
      //ovmHalfFaceI hf_it = this->cell( *c_it ).hf_iter();
      std::vector< ovmHalfFaceH > hfVec = __caCThis->cell( *c_it ).halffaces();
      for (int ii=0; ii<hfVec.size(); ii++) {
        ovmHalfFaceVertexI hfv_it = __caCThis->hfv_iter( hfVec[ii] );
        for (hfv_it; hfv_it.valid(); ++hfv_it) {
          DTBUFFER( 
            << __caCThis->request_vertex_property< int >("id")[*hfv_it] 
            << " "
          );
        }
        DTBUFFER( << std::endl );
      }
      DTBUFFER( << std::endl );
      DTBUFFER( << "CellHalfEdgeIter" << std::endl);
      ovmCellHalfEdgeI che_it = this->che_iter( *c_it );
      for (che_it; che_it.valid(); ++che_it) {
        DTBUFFER( 
        << "halfedge = " << *che_it
        << ", fromVertex = " 
        << __caCThis->request_vertex_property< int >("id")[this->halfedge(*che_it).from_vertex()] 
        << " -> toVertex = "
        << __caCThis->request_vertex_property< int >("id")[this->halfedge(*che_it).to_vertex()]
        << std::endl        );
      }
    }
    
    ovmEdgeI e_it = this->e_iter();
    for (e_it; e_it.valid(); ++e_it) {
      DTBUFFER( 
        << "edge = " << *e_it
        << ", fromVertex = " 
        << __caCThis->request_vertex_property< int >("id")[this->edge(*e_it).from_vertex()] 
        << " -> toVertex = "
        << __caCThis->request_vertex_property< int >("id")[this->edge(*e_it).to_vertex()]
        << std::endl
      );
    }        
    DTBUFFER( << std::endl );    
    dt__info_BUFFER(dump());
    
//    OpenVolumeMesh::IO::FileManager().writeFile("dtMacroMesh.ovm", *this);
    
  }
    
  ovmVertexH const dtMacroMesh::getVertexHandle( int const id ) const {
    OpenVolumeMesh::VertexPropertyT< int > const idProp
    =
    __caCThis->request_vertex_property< int >("id");
          
    __forAllVert(
      if ( idProp[*v_it] == id ) {
        return *v_it;
      }
    );
    
    dt__THROW(getVertexHandle(),
            << "Vertex " << dt__eval(id) << " not in mesh.");
  }
  
  int const dtMacroMesh::getVertexId( ovmVertexH const vH ) const {
    OpenVolumeMesh::VertexPropertyT< int > const idProp
    =
    __caCThis->request_vertex_property< int >("id");
          
    return idProp[vH];
  }
    
  ovmCellH const dtMacroMesh::getCellHandle( std::string const id ) const {
    OpenVolumeMesh::CellPropertyT< std::string > const name
    =
    __caCThis->request_cell_property< std::string >("label");
          
    __forAllCell(
      if ( name[*c_it] == id ) {
        return *c_it;
      }
    );
    
    dt__THROW(getCellHandle(), << "Cell " << dt__eval(id) << " not in mesh.");    
  }
  
  ovmCellH const dtMacroMesh::getCellHandle( int const id ) const {
    ovmCellI cv_it = __caCThis->cells_begin() + id;
    
    if ( cv_it->is_valid() ) {
      return *cv_it;
    }
    
    dt__THROW(getCellHandle(), << "Cell " << dt__eval(id) << " not in mesh.");        
  }
  
  ovmEdgeH const dtMacroMesh::getEdgeHandle( int const id ) const {
    ovmEdgeI e_it = __caCThis->edges_begin() + id;
    
    if ( e_it->is_valid() ) {
      return *e_it;
    }
    
    dt__THROW(getEdgeHandle(), << "Edge " << dt__eval(id) << " not in mesh.");    
  }

  ovmFaceH const dtMacroMesh::getFaceHandle( int const id ) const {
    ovmFaceI f_it = __caCThis->faces_begin() + id;
    
    if ( f_it->is_valid() ) {
      return *f_it;
    }
    
    dt__THROW(getEdgeHandle(), << "Edge " << dt__eval(id) << " not in mesh.");    
  }
  
  int dtMacroMesh::getFromByEdgeIndex(int const eId) const {
    ovmEdgeH eH = getEdgeHandle(eId);
    
    return __caCThis->request_vertex_property< int >("id")[
            this->edge(eH).from_vertex()
    ];
  }
  
  int dtMacroMesh::getToByEdgeIndex(int const eId) const {
    ovmEdgeH eH = getEdgeHandle(eId);
    
    return __caCThis->request_vertex_property< int >("id")[
            this->edge(eH).to_vertex()
    ];    
  }
  
  int dtMacroMesh::getEdgeFromToIndex( int const from, int const to) const {
    ovmVertexH vHFrom = getVertexHandle(from);
    ovmVertexH vHTo = getVertexHandle(to);
    
    ovmHalfEdgeH heH = this->halfedge(vHFrom, vHTo);
    ovmEdgeH eH = this->edge_handle(heH);
    
    return getEdgeIndex(eH);
  }
  
  std::vector< int > dtMacroMesh::getFaceVertices( int const fId ) const {
    ovmFaceH fH = getFaceHandle(fId);
    ovmHalfFaceH hfH = this->halfface_handle(fH, 0);
    
    std::vector< int > vId;
    ovmHalfFaceVertexI hfv_it = this->hfv_iter(hfH);
    for (hfv_it; hfv_it.valid(); ++hfv_it) {
      vId.push_back( getVertexId( *hfv_it ) );
    }
    
//    dt__debug(getFaceVertices(),
//            << dt__eval(fId) << std::endl
//            << "vID = " << vecToString<int>(vId) );
    return vId;
  }
  
  std::vector< int > dtMacroMesh::getBlockFaces( int const bId ) const {
    ovmCellH cH = getCellHandle(bId);
    
    ovmCellHalfFaceI chf_it = this->chf_iter(cH);
    std::vector< int > ret;
    for (chf_it; chf_it.valid(); ++chf_it) {
      ret.push_back( this->face_handle( *chf_it ).idx() );
    }
    
    return ret;
  }
  
  std::vector< int > dtMacroMesh::getBlockVertices( int const bId ) const {
    ovmCellH cH = getCellHandle(bId);
    
    ovmHexVertexI hv_it = this->hv_iter(cH);
    std::vector< int > ret;
    for (hv_it; hv_it.valid(); ++hv_it) {
      ret.push_back( getVertexId( *hv_it ) );
    }
    return ret;
  }  
      
//  void dtMacroMesh::setNElements(
//    std::string const blockName, int const nX, int const nY, int const nZ 
//  ) {
//    setEPropertyInBlock("nElements", blockName, nX, nY, nZ);
//    OpenVolumeMesh::HalfEdgePropertyT< std::vector < std::string > > cPos
//    =
//    this->request_halfedge_property< std::vector< std::string > >("position");
//
//    OpenVolumeMesh::EdgePropertyT< int > eNElements
//    = 
//    this->request_edge_property< int >("nElements");
//
//    
//    
//    ovmCellH cH = getCellHandle(blockName);
//    ovmCellHalfEdgeI che_it = this->che_iter(cH);
//    for (che_it; che_it.valid(); ++che_it) {
//      for (int ii=0; ii<cPos[*che_it].size(); ii++) {
//        std::string tmpName 
//        = 
//        stringPrimitive().getStringBetween("@", "", cPos[*che_it][ii]);
//        if (tmpName == blockName) {
//          std::string tmpPos = stringPrimitive().getStringBetween("", "@", cPos[*che_it][ii]);
//          if (tmpPos == "0" || tmpPos == "1" || tmpPos == "2" || tmpPos == "3") {
//            eNElements[this->edge_handle(*che_it)] = nX;
//          }
//          else if (tmpPos == "4" || tmpPos == "5" || tmpPos == "6" || tmpPos == "7") {
//            eNElements[this->edge_handle(*che_it)] = nY;
//          }
//          else if (tmpPos == "8" || tmpPos == "9" || tmpPos == "10" || tmpPos == "11") {
//            eNElements[this->edge_handle(*che_it)] = nZ;
//          }
//        }
//      }
//    }
//  }

//  void dtMacroMesh::setSimpleGrading(
//    std::string const blockName, float const nX, float const nY, float const nZ 
//  ) {
//    setEPropertyInBlock("grading", blockName, nX, nY, nZ);  
//  };
      
  int dtMacroMesh::getEdgeIndex( ovmEdgeH const eH ) const {
    int counter = 0;
    __forAllEdge(
      if ( *e_it == eH ) {
        return counter;
      }
      counter++;
    );    
    dt__THROW(getEdgeIndex(), << "Edge " << dt__eval(eH) << " not in mesh.");    
  }
  
  dtPoint3 dtMacroMesh::toDtPoint3( ovmVertexH const vH ) const {
    OpenVolumeMesh::Vec3f vv = this->vertex(vH);
    return dtPoint3(vv[0], vv[1], vv[2]);
  }
    
  ovmCellH dtMacroMesh::addCellByVerts(
    const ovmVertexH& _v0, 
    const ovmVertexH& _v1,
    const ovmVertexH& _v2,
    const ovmVertexH& _v3,
    const ovmVertexH& _v4,
    const ovmVertexH& _v5,
    const ovmVertexH& _v6,
    const ovmVertexH& _v7, 
    bool _topologyCheck
  ) {
    std::vector< ovmVertexH > vec;
    vec.push_back(_v0);
    vec.push_back(_v1);
    vec.push_back(_v2);
    vec.push_back(_v3);
    vec.push_back(_v4);
    vec.push_back(_v5);
    vec.push_back(_v6);
    vec.push_back(_v7);
    
    return this->add_cell(vec, _topologyCheck);
  }
  
  void dtMacroMesh::clear( void ) {
    OpenVolumeMesh::GeometricHexahedralMeshV3f::clear(false);
  }
}
