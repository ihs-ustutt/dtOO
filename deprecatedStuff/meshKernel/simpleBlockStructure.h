#ifndef SIMPLEBLOCKSTRUCTURE_H
#define	SIMPLEBLOCKSTRUCTURE_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <dtMacroMesh.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  typedef std::vector< int > blockNumbering;
  typedef std::vector< float > blockNumberingFloat;
  
  class analyticGeometry;
  class analyticSurface;
  class scaFunction;
  
  class simpleBlockStructure : public dtMacroMesh {
  public:
    DTCLASSLOGMETHOD(simpleBlockStructure);
    DTCLASSNAMEMETHOD(simpleBlockStructure);
    simpleBlockStructure();
    virtual ~simpleBlockStructure();    
//    void addBlock(int const zero, int const one, int const two, int const three,
//             int const four, int const five, int const six, int const seven, 
//             std::string blockName = "");
//    void addBlockOption(std::string const blockName, std::string const optionName);
//    bool hasBlockOption(int const blockId, std::string const optionName) const;
    void setNElements(std::string const blockName, int const nX, int const nY, int const nZ );    
//    int getNEdges( void ) const;
//    int getFromByEdgeIndex(int const eId) const;
//    int getToByEdgeIndex(int const eId) const;
    void clear( void );
//    covise::coDoSet * toCoDoSet(char const * const str) const;
//    float getJacobianInBlockFace( int const blockIndex, int const faceIndex ) const;    
//    std::vector< float > getAnglesDegInBlock( int const blockIndex ) const;
//    int getNBlocks( void ) const;
//    blockNumbering const & getBlockEdgeNumbering( int const blockId );
//    blockNumbering const & getBlockFaceNumbering( int const blockId );
//    int getNFaces( void ) const;
//    blockNumbering const & getFaceNumbering( int const fId ) const;
//    blockNumberingFloat const & getBlockGrading(int const blockId) const;
//    blockNumbering const & getBlockNElements(int const blockId) const;
//    int getEdgeFromToIndex( int const from, int const to) const;
//    std::string getBlockName( int const blockIndex ) const;    
  private:
    void addEdge(analyticGeometry const * edgeAG, float const uuStart, float const uuEnd,
            int const from, int const to, int const nPointsSpline,
            analyticSurface const * aS = NULL);    
    blockNumbering const & getBlockNumbering( int const blockId ) const;
    int getFaceIndex( blockNumbering const bN ) const;
    int getFaceIndex( int const e0, int const e1, int const e2, int const e3) const;    
    std::vector< float > getAnglesInBlock( int const blockIndex ) const;
    void addEdgeNoSurface(int const fromVertex, int const toVertex, int const nPointsSpline );
    bool alreadyAddedEdge(float const uuStart, float const uuEnd, int const from, 
                          int const to, int const nPointsSpline) const;    
    analyticGeometry const * getEdgeFromTo( int const from, int const to) const;
    int getBlockIndex( std::string const blockName ) const;
    void calculateAnglesFromPoints(
           dtPoint3 const oo, 
           dtPoint3 const pp, 
           dtPoint3 const qq, 
           dtPoint3 const rr,
           std::vector< float > & angles
         ) const;            
  private:
    bool vecInVec( 
           blockNumbering bN, 
           blockNumbering * bNSorted, 
           std::vector< blockNumbering > const & vecBn
         ) const;
  int getVecInVecPos( 
        blockNumbering bN, 
        std::vector< blockNumbering > const & vecBn
  ) const;
  private:
//    vectorHandling< dtPoint3 > _vertex;
//    vectorHandling< int > _vertexId;
//    vectorHandling< float > _vertexU;
//    vectorHandling< float > _vertexV;
//    vectorHandling< blockNumbering > _vertexSnapSurfId;
    vectorHandling< blockNumbering > _block;
    vectorHandling< blockNumbering > _blockEdge;
    vectorHandling< blockNumbering > _blockFace;
    vectorHandling< std::vector< blockNumbering > > _blockFaceVertex;
    vectorHandling< std::string > _blockName;
    vectorHandling< std::vector< std::string > > _blockOption;
    vectorHandling< std::vector< int > > _blockNElem;
    vectorHandling< blockNumberingFloat > _blockGrading;
    vectorHandling< analyticGeometry const * > _edge;
    vectorHandling< float > _edgeUStart;
    vectorHandling< float > _edgeUEnd;
    vectorHandling< int > _edgeFrom;
    vectorHandling< int > _edgeTo;
    vectorHandling< int > _edgeNPointsOnSpline;
    vectorHandling< analyticSurface * > _edgeBelongsToSurf;
    vectorHandling< blockNumbering > _faceEdgeNumbering;
    vectorHandling< blockNumbering > _faceEdgeNumberingSorted;
    vectorHandling< std::string > _patchName;
    vectorHandling< blockNumbering > _patchVertex;
    vectorHandling< std::string > _couplingPatchName;
    vectorHandling< int > _markedEdgeFrom;
    vectorHandling< int > _markedEdgeTo;
    vectorHandling< scaFunction const * > _markedEdgeScaFunction;
    dtMacroMesh * _macroMesh;
  public:
//    std::vector< dtPoint3 > getPointsOnEdge( 
//      analyticGeometry const * aG, 
//      float const uuStart, 
//      float const uuEnd, 
//      int const nPoints ) const;
//    std::vector< dtPoint3 > getPointsOnEdge( analyticGeometry const * aG, 
//      float const uuStart, 
//      float const uuEnd, 
//      int const nPoints,
//      int const startVertex,
//      int const endVertex ) const;     
//    void addVertex( dtPoint3 const vertex, const int id );
//    void addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId);
//    void addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId, int const snapSurfId2);
//    void addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId, int const snapSurfId2, int const snapSurfId3);
//    void setSimpleGrading(std::string const blockName, float const nX, float const nY, float const nZ );
//    void setEdgeGrading(
//      std::string const blockName, 
//      float const zero, 
//      float const one, 
//      float const two, 
//      float const three,
//      float const four, 
//      float const five, 
//      float const six, 
//      float const seven,
//      float const eight,
//      float const nine,
//      float const ten,
//      float const eleven
//    );    
//    void addSplineEdge( analyticSurface const * surfAS, int const fromVertex, 
//                        int const toVertex, int const nPointsSpline);
//    void addEdgeInSurface(analyticSurface const * surfAS, 
//                          int const fromVertex, 
//                          int const toVertex, 
//                          int const nPointsSpline);  
//    void markSplineEdge( int const fromVertex, int const toVertex, scaFunction const * sF);
//    bool markedEdge( int const fromVertex, int const toVertex) const;
//    void addPatch(std::string const patchName, int const zero, int const one, int const two, int const three);
//    void addCouplingPatchName( std::string const patchName );
//    int getNCouplingPatches( void ) const;
//    std::vector< blockNumbering > getPatch( std::string const patchName ) const;
//    std::string getCouplingPatchName( int const couplingPatchIndex ) const;
//    int getNVertices( void ) const;
//    dtPoint3 const & getVertex( int const id ) const;
//    dtPoint2 getVertexUV( int const id ) const;
//    blockNumbering getVertexSnapSurfId( int const id ) const;
//    std::vector< float > getAnglesInBlock( std::string const blockName ) const;
//    float getJacobianInBlockFace( std::string const blockName, int const faceIndex ) const;
//    std::vector< float > getAnglesDegInBlock( std::string const blockName ) const;
//    std::vector< float > getAnglesDegInAllBlocks( void ) const;
//    float getJacobianOfBlock( std::string const blockName ) const;
//    std::vector< float > getJacobiansOfAllBlocks( void ) const;            
//    void dump( void ) const;    
  };
}
#endif	/* SIMPLEBLOCKSTRUCTURE_H */

