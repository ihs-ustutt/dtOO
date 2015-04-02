#ifndef DTMACROMESH_H
#define	DTMACROMESH_H

#include <interfaceHeaven/stringPrimitive.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <OpenVolumeMesh/Geometry/VectorT.hh>
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#include <OpenVolumeMesh/Mesh/HexahedralMeshIterators.hh>
#include <OpenVolumeMesh/Core/PropertyDefines.hh>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  typedef OpenVolumeMesh::VertexHandle ovmVertexH;
  typedef OpenVolumeMesh::HalfEdgeHandle ovmHalfEdgeH;
  typedef OpenVolumeMesh::EdgeHandle ovmEdgeH;
  typedef OpenVolumeMesh::HalfFaceHandle ovmHalfFaceH;
  typedef OpenVolumeMesh::FaceHandle ovmFaceH;
  typedef OpenVolumeMesh::CellHandle ovmCellH;
  typedef OpenVolumeMesh::VertexIter ovmVertexI;
  typedef OpenVolumeMesh::EdgeIter ovmEdgeI;
  typedef OpenVolumeMesh::HalfFaceIter ovmHalfFaceI;
  typedef OpenVolumeMesh::FaceIter ovmFaceI;
  typedef OpenVolumeMesh::CellIter ovmCellI;
  typedef OpenVolumeMesh::CellHalfFaceIter ovmCellHalfFaceI;
  typedef OpenVolumeMesh::CellHalfEdgeIter ovmCellHalfEdgeI;
  typedef OpenVolumeMesh::HalfFaceVertexIter ovmHalfFaceVertexI;
  typedef OpenVolumeMesh::HexVertexIter ovmHexVertexI;
  typedef OpenVolumeMesh::CellVertexIter ovmCellVertexI;
  typedef OpenVolumeMesh::VertexVertexIter ovmVertexVertexI;
  typedef OpenVolumeMesh::HalfFaceSheetHalfFaceIter ovmHalfFaceSheetHalfFaceI;
  typedef OpenVolumeMesh::HalfEdgeHalfFaceIter ovmHalfEdgeHalfFaceI;
  
  
  /**
   * @brief Interface to OpenVolumeMesh
   * 
   * Manages interface to OpenVolumeMesh. Please add new functions of
   * of OpenVolumeMesh only in this class.
   * See <a href="http://www.openvolumemesh.org">OpenVolumeMesh</a>.
   */  
  class dtMacroMesh : public OpenVolumeMesh::GeometricHexahedralMeshV3f {
      friend class bVOAngleBasedMacroMeshSmoothing;
  public:
    dt__classOnlyName(dtMacroMesh);
    dtMacroMesh();
    dtMacroMesh(const dtMacroMesh& orig);
    virtual ~dtMacroMesh();
    template< class T >
    void addEdgeProperty(std::string const propName, T const val) {
      OpenVolumeMesh::EdgePropertyT< T > prop
      = 
      this->request_edge_property< T >(propName);
      this->set_persistent(prop);
    }
    template< class T >
    void setEdgeProperty(std::string const propName, T const val) {    
      OpenVolumeMesh::EdgePropertyT< T > prop
      = 
      this->request_edge_property< T >(propName);      
      for (
        ovmEdgeI e_it = this->edges_begin();
        e_it != this->edges_end();
        ++e_it
      ) {
        prop[*e_it] = val;
      }
    }    
    void addVertex( dtPoint3 const vertex, const int id );
    void addVertex( 
      dtPoint3 const vertex, 
      int const id, 
      float const uu, 
      float const vv, 
      int const sId0 = -1, 
      int const sId1 = -1, 
      int const sId2 = -1 
    );
    void snapAllVerticesToMap3dTo3d( int const mId);
    dtPoint3 const getVertex( int const id ) const;
    
    dtPoint2 getVertexUV( int const id ) const;
    std::vector< int > getVertexSnapSurfId( int const id ) const;  
    std::vector< int > getVertexSnapVolumeId( int const id ) const;  
    int getNVertices( void ) const;
    int getNEdges( void ) const;
    int getNBlocks( void ) const;
    int getNFaces( void ) const;
    void addBlock( 
      int const zero, 
      int const one,
      int const two,
      int const three,
      int const four,
      int const five,
      int const six,
      int const seven,
      std::string const blockName = ""
    );
    std::string const getBlockLabel(int const id) const;
    void addBlockOption(std::string const blockName, std::string const optionName);
    bool hasBlockOption(std::string const blockName, std::string const optionName) const;
    int getFromByEdgeIndex(int const eId) const;
    int getToByEdgeIndex(int const eId) const;
    int getEdgeFromToIndex( int const from, int const to) const;
    std::vector< int > getFaceVertices( int const fId ) const;
    std::vector< int > getBlockFaces( int const bId ) const;
    std::vector< int > getBlockVertices( int const bId ) const;
//    void setNElements(
//      std::string const blockName, 
//      int const nX, 
//      int const nY, 
//      int const nZ 
//    );
//    void setSimpleGrading(
//      std::string const blockName, 
//      float const nX, 
//      float const nY, 
//      float const nZ 
//    );    
    template< class T >
    void setEPropertyInBlock(
      std::string const propName, std::string const blockName, 
      T const pX, T const pY, T const pZ 
    ) {
      OpenVolumeMesh::HalfEdgePropertyT< std::vector < std::string > > cPos
      =
      this->request_halfedge_property< std::vector< std::string > >("position");

      OpenVolumeMesh::EdgePropertyT< T > prop
      = 
      this->request_edge_property< T >(propName);

      ovmCellH cH = getCellHandle(blockName);
      ovmCellHalfEdgeI che_it = this->che_iter(cH);
      for (che_it; che_it.valid(); ++che_it) {
        for (int ii=0; ii<cPos[*che_it].size(); ii++) {
          std::string tmpName 
          = 
          stringPrimitive().getStringBetween("@", "", cPos[*che_it][ii]);
          if (tmpName == blockName) {
            std::string tmpPos = stringPrimitive().getStringBetween("", "@", cPos[*che_it][ii]);
            if (tmpPos == "0" || tmpPos == "1" || tmpPos == "2" || tmpPos == "3") {
              prop[this->edge_handle(*che_it)] = pX;
            }
            else if (tmpPos == "4" || tmpPos == "5" || tmpPos == "6" || tmpPos == "7") {
              prop[this->edge_handle(*che_it)] = pY;
            }
            else if (tmpPos == "8" || tmpPos == "9" || tmpPos == "10" || tmpPos == "11") {
              prop[this->edge_handle(*che_it)] = pZ;
            }
          }
        }
      }
    }     
    template< typename T >
    T getEdgeProperty(std::string const name, int const eId) {
      OpenVolumeMesh::EdgePropertyT< T > eP 
      =
      this->request_edge_property< T >(name);

      return eP[getEdgeHandle(eId)];
    }
    covise::coDoSet * toCoDoSet(char const * const str) const;
    void dump( void ) const;
    void clear( void );
  private:
    ovmVertexH const getVertexHandle( int const id ) const;
    dtPoint3 toDtPoint3( ovmVertexH const vH ) const;
    int const getVertexId( ovmVertexH const vH ) const;
    ovmCellH const getCellHandle( std::string const id ) const;
    ovmCellH const getCellHandle( int const id ) const;
    ovmEdgeH const getEdgeHandle( int const id ) const;
    int getEdgeIndex( OpenVolumeMesh::EdgeHandle const eH ) const;
    ovmFaceH const getFaceHandle( int const id ) const;
    ovmCellH addCellByVerts(
      const ovmVertexH& _v0, 
      const ovmVertexH& _v1,
      const ovmVertexH& _v2,
      const ovmVertexH& _v3,
      const ovmVertexH& _v4,
      const ovmVertexH& _v5,
      const ovmVertexH& _v6,
      const ovmVertexH& _v7, 
      bool _topologyCheck = false
    );
  private:
  };
}
#endif	/* DTMACROMESH_H */
