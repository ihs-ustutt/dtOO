#include "simpleBlockStructure.h"

#include <progHelper.h>
#include <logMe/logMe.h>

#include <do/coDoSet.h>

#include <gmsh/MHexahedron.h>
#include <gmsh/MQuadrangle.h>

#include <interfaceHeaven/stringPrimitive.h>

#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>

#include <analyticFunctionHeaven/scaFunction.h>
#include <analyticFunctionHeaven/scaSISLSplineTwoD.h>

#include <geometryEngine/dtSislCurve.h>

#include <math.h>
#include <algorithm>

namespace dtOO {
  DTCLASSLOGMETHODIN(simpleBlockStructure);
  
  simpleBlockStructure::simpleBlockStructure() {
//    _macroMesh = new dtMacroMesh();
  }

  simpleBlockStructure::~simpleBlockStructure() {
    clear();
  }
  
  void simpleBlockStructure::clear( void ) {
//    _vertex.clear();
//    _vertexId.clear();
//    _vertexU.clear();
//    _vertexV.clear();
//    dt__FORALL(_vertexSnapSurfId, ii,
//      _vertexSnapSurfId[ii].clear();
//    );    
//    _vertexSnapSurfId.clear();
    dt__FORALL(_block, ii,
      _block[ii].clear();
    );
    _block.clear();
    dt__FORALL(_blockEdge, ii,
      _blockEdge[ii].clear();
    );    
    _blockEdge.clear();
    dt__FORALL(_blockFace, ii,
      _blockFace[ii].clear();
    );        
    _blockFace.clear();
    _blockName.clear();
    dt__FORALL(_blockNElem, ii,
      _blockNElem[ii].clear();
    );
    _blockNElem.clear();
    dt__FORALL(_blockGrading, ii,
      _blockGrading[ii].clear();
    );
    _blockGrading.clear();    
    dt__FORALL(_edge, ii,
      delete _edge[ii];
    );
    _edge.clear();
    _edgeUStart.clear();
    _edgeUEnd.clear();
    _edgeFrom.clear();
    _edgeTo.clear();
    _edgeNPointsOnSpline.clear();
    dt__FORALL(_faceEdgeNumbering, ii,
      _faceEdgeNumbering[ii].clear();
    );
    _faceEdgeNumbering.clear();
    dt__FORALL(_faceEdgeNumberingSorted, ii,
      _faceEdgeNumberingSorted[ii].clear();
    );
    _faceEdgeNumberingSorted.clear();
    _patchName.clear();
    _couplingPatchName.clear();
    dt__FORALL(_patchVertex, ii,
      _patchVertex[ii].clear();
    );    
    
    _markedEdgeFrom.clear();
    _markedEdgeTo.clear();
    _markedEdgeScaFunction.clear();
    dt__FORALL(_edgeBelongsToSurf, ii,
      delete _edgeBelongsToSurf[ii];
    );    
    _edgeBelongsToSurf.clear();

    dt__FORALL(_blockOption, ii,
      _blockOption[ii].clear();
    );    
    _blockOption.clear();
    
    dt__FORALL(_blockFaceVertex, ii,
      for (int jj=0; jj<6; jj++) {
        _blockFaceVertex[ii][jj].clear();
      }
      _blockFaceVertex[ii].clear();
    );
    _blockFaceVertex.clear();
    
//    delete _macroMesh;
//    _macroMesh = new dtMacroMesh();
  }
  
//  void simpleBlockStructure::addVertex(dtPoint3 const vertex, int const id) {
//    _vertex.push_back( vertex );
//    _vertexId.push_back( id );
//  }
//
//  void simpleBlockStructure::addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId) {
//    _vertex.push_back( vertex );
//    _vertexId.push_back( id );
//    _vertexU.push_back( uu );
//    _vertexV.push_back( vv );
//    _vertexSnapSurfId.push_back( blockNumbering(1, snapSurfId) );
//  }
//
//  void simpleBlockStructure::addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId, int const snapSurfId2) {
//    addVertex(vertex,id, uu, vv, snapSurfId);
//    _vertexSnapSurfId.back().push_back( snapSurfId2 );
//  }
//
//  void simpleBlockStructure::addVertex(dtPoint3 const vertex, int const id, float const uu, float const vv, int const snapSurfId, int const snapSurfId2, int const snapSurfId3) {
//    addVertex(vertex,id, uu, vv, snapSurfId, snapSurfId2);
//    _vertexSnapSurfId.back().push_back( snapSurfId3 );  
//  }  
    
//  dtPoint3 const & simpleBlockStructure::getVertex( int const id ) const {
//    for (int ii=0;ii<_vertex.size();ii++) {
//      if (_vertexId[ii] == id) {
//        return _vertex[ii];
//      }
//    }
//    dt__THROW(getVertex(),
//            << dt__eval(id) << " not in vector _vertex.");
//  }

//  dtPoint2 simpleBlockStructure::getVertexUV( int const id ) const {
//    for (int ii=0;ii<_vertex.size();ii++) {
//      if (_vertexId[ii] == id) {
//        return dtPoint2(_vertexU[ii], _vertexV[ii]);
//      }
//    }
//    dt__THROW(getVertexUV(),
//            << dt__eval(id) << " not in vector _vertex.");
//  }
//  
//  blockNumbering simpleBlockStructure::getVertexSnapSurfId( int const id ) const {
//    return _vertexSnapSurfId[id];
//  }
  
//  int simpleBlockStructure::getNVertices( void ) const {
//    return _vertex.size(); 
//  }  
  
//  void simpleBlockStructure::addBlock(
//                              int const zero, 
//                              int const one,
//                              int const two,
//                              int const three,
//                              int const four,
//                              int const five,
//                              int const six,
//                              int const seven,
//                              std::string blockName
//                            ) {
//    blockNumbering block;
//    
//    block.push_back(zero);
//    block.push_back(one);
//    block.push_back(two);
//    block.push_back(three);
//    block.push_back(four);
//    block.push_back(five);
//    block.push_back(six);
//    block.push_back(seven);
//    
//    _block.push_back( block );
//
//    addEdgeNoSurface(block[3], block[2], 2); 
//    addEdgeNoSurface(block[7], block[6], 2); 
//    addEdgeNoSurface(block[0], block[3], 2);
//    addEdgeNoSurface(block[4], block[7], 2);
//    addEdgeNoSurface(block[0], block[1], 2);
//    addEdgeNoSurface(block[4], block[5], 2);
//    addEdgeNoSurface(block[1], block[2], 2);
//    addEdgeNoSurface(block[5], block[6], 2); 
//    addEdgeNoSurface(block[1], block[5], 2);
//    addEdgeNoSurface(block[2], block[6], 2); 
//    addEdgeNoSurface(block[0], block[4], 2);
//    addEdgeNoSurface(block[3], block[7], 2); 
//    
//    _blockOption.push_back( std::vector< std::string >() );
//    
//    //
//    // name
//    //
//    if ( blockName == "" ) {
//      blockName = "block_"+stringPrimitive().intToString(_block.size());
//    }
//    _blockName.push_back( blockName );
//    
//    //
//    // number of elements and grading
//    //
//    _blockNElem.push_back( std::vector< int >(3, 10) );
//    _blockGrading.push_back( blockNumberingFloat(3,1) );
//    
//    _blockEdge.push_back(blockNumbering(0,0));
//    _blockFace.push_back(blockNumbering(0,0));
//    
//    std::vector< blockNumbering > faceVec(6, blockNumbering(4,0) );
//    faceVec[2][0] = zero;
//    faceVec[2][1] = one;
//    faceVec[2][2] = two;
//    faceVec[2][3] = three;
//    faceVec[0][0] = four;
//    faceVec[0][1] = five;
//    faceVec[0][2] = six;
//    faceVec[0][3] = seven;
//    faceVec[3][0] = one;
//    faceVec[3][1] = two;
//    faceVec[3][2] = six;
//    faceVec[3][3] = five;
//    faceVec[1][0] = zero;
//    faceVec[1][1] = three;
//    faceVec[1][2] = seven;
//    faceVec[1][3] = four;
//    faceVec[4][0] = zero;
//    faceVec[4][1] = one;
//    faceVec[4][2] = five;
//    faceVec[4][3] = four;
//    faceVec[5][0] = three;
//    faceVec[5][1] = two;
//    faceVec[5][2] = six;
//    faceVec[5][3] = seven;  
//      
//    _blockFaceVertex.push_back(faceVec);
//    
//    blockNumbering sorted(4,0);
//    for (int ii=0;ii<6;ii++) {
//      if ( !vecInVec(faceVec[ii], &sorted, _faceEdgeNumberingSorted) ) {
//        _faceEdgeNumberingSorted.push_back( sorted );
//        _faceEdgeNumbering.push_back( faceVec[ii] );
//      }
//    }   
//   dtMacroMesh::addBlock(zero, one, two, three, four, five, six, seven, blockName);
//                              
//  }
  
//  void simpleBlockStructure::addBlockOption(std::string const blockName, std::string const optionName) {
//    _blockOption[ getBlockIndex(blockName) ].push_back( optionName ); 
//  }
//  
//  bool simpleBlockStructure::hasBlockOption(int const blockId, std::string const optionName) const {
//    std::vector< std::string > const & options = _blockOption[blockId];
//    dt__FORALL(options, ii,
//      if ( options[ii] == optionName ) {
//        return true;
//      }
//    );
//    return false;
//  }
  
  void simpleBlockStructure::setNElements(std::string const blockName, 
                                         int const nX, 
                                         int const nY, 
                                         int const nZ) {
    for (int ii=0;ii<_blockName.size();ii++) {
      if (_blockName[ii] == blockName) {
        _blockNElem[ii].resize(3);
        _blockNElem[ii][0] = nX;
        _blockNElem[ii][1] = nY;
        _blockNElem[ii][2] = nZ;
        return;
      }
    }
  }
  
//    void simpleBlockStructure::setSimpleGrading(std::string const blockName, 
//                                         float const nX, 
//                                         float const nY, 
//                                         float const nZ) {
//    for (int ii=0;ii<_blockName.size();ii++) {
//      if (_blockName[ii] == blockName) {
//        _blockGrading[ii].resize(3);
//        _blockGrading[ii][0] = nX;
//        _blockGrading[ii][1] = nY;
//        _blockGrading[ii][2] = nZ;
//        return;
//      }
//    }
//  }
    
//  void simpleBlockStructure::setEdgeGrading(
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
//  ) {
//    for (int ii=0;ii<_blockName.size();ii++) {
//      if (_blockName[ii] == blockName) {
//        _blockGrading[ii].resize(12);
//        _blockGrading[ii][0] = zero;
//        _blockGrading[ii][1] = one;
//        _blockGrading[ii][2] = two;
//        _blockGrading[ii][3] = three;
//        _blockGrading[ii][4] = four;
//        _blockGrading[ii][5] = five;
//        _blockGrading[ii][6] = six;
//        _blockGrading[ii][7] = seven;
//        _blockGrading[ii][8] = eight;
//        _blockGrading[ii][9] = nine;
//        _blockGrading[ii][10] = ten;
//        _blockGrading[ii][11] = eleven;
//        return;
//      }
//    }    
//  }    

  bool simpleBlockStructure::alreadyAddedEdge(float const uuStart, 
                              float const uuEnd, 
                              int const from, 
                              int const to,
                              int const nPointsSpline) const {
    //
    // check if edge already exists
    //
    for (int ii=0;ii<_edge.size();ii++) {
      if ( (_edgeFrom[ii] == from) 
              && (_edgeTo[ii] == to) 
              && (_edgeUStart[ii] == uuStart) 
              && (_edgeUEnd[ii] == uuEnd)
              && (_edgeNPointsOnSpline[ii] == nPointsSpline) ) {
        
        //
        // do nothing
        //
        return true;
      }
      //
      // check for edge in reverse order
      //
      if ( (_edgeFrom[ii] == to) 
              && (_edgeTo[ii] == from) 
              && (_edgeUStart[ii] == uuEnd) 
              && (_edgeUEnd[ii] == uuStart)
              && (_edgeNPointsOnSpline[ii] == nPointsSpline) ) {
        dt__THROW(alreadyAddedEdge(),
                << dt__eval(from) << std::endl
                << dt__eval(to) << std::endl
                << "Try to add an edge twice in reverse order.");
      }
    }
    
    return false;
  }
  
  void simpleBlockStructure::addEdge(
                              analyticGeometry const * edgeAG, 
                              float const uuStart, 
                              float const uuEnd, 
                              int const from, 
                              int const to,
                              int const nPointsSpline,
                              analyticSurface const * aS) {
    //
    // check if edge already exists
    //
    if ( alreadyAddedEdge(uuStart, uuEnd, from, to, nPointsSpline) ) {
      delete edgeAG;
      return;
    }
    
    //
    // add edge
    //    
    _edge.push_back( edgeAG );
    _edgeUStart.push_back( uuStart );
    _edgeUEnd.push_back( uuEnd );
    _edgeFrom.push_back( from );
    _edgeTo.push_back( to );
    _edgeNPointsOnSpline.push_back( nPointsSpline );
    if (aS != NULL) {
      _edgeBelongsToSurf.push_back( aS->clone() );
      dt__info(addEdge(),
              << "Adding edge " << dt__eval(from) << " " << dt__eval(to) << "." << std::endl
              << "Edge lies in " << _edgeBelongsToSurf.back()->getName() << ".");
    }
    else {
      _edgeBelongsToSurf.push_back( NULL );
    }
  }

//  void simpleBlockStructure::addSplineEdge(
//                              analyticSurface const * surfAS, 
//                              int const fromVertex, 
//                              int const toVertex, 
//                              int const nPointsSpline) {
//    
//    dt__warning(addSplineEdge(),
//            << "Marked edge " << dt__eval(fromVertex) << " " << dt__eval(toVertex) );          
//    
//    for (int ii=0;ii<_markedEdgeScaFunction.size();ii++) {
//      if ( (_markedEdgeFrom[ii] == fromVertex)
//              && (_markedEdgeTo[ii] == toVertex) ) {
//        dt__info(addSplineEdge(),
//                << dt__eval(ii) << std::endl
//                << dt__eval(_markedEdgeFrom[ii]) << std::endl 
//                << dt__eval(_markedEdgeTo[ii]) << std::endl
//                << _markedEdgeScaFunction[ii]->getName() );
//
//        //
//        // add spline edge
//        //
//        dtPoint2 pTwoFrom = surfAS->getClosestPointParameterPercent(getVertex(fromVertex));
//        dtPoint2 pTwoTo = surfAS->getClosestPointParameterPercent(getVertex(toVertex));
//
//        //
//        // get dtSislCurve
//        //
//        scaFunction * sF;
//        _markedEdgeScaFunction[ii]->copy( &sF );
//        scaSISLSplineTwoD * spline;
//        dt__mustCast(sF, scaSISLSplineTwoD, spline);
//        dtSislCurve * dtC = const_cast< dtSislCurve * >(spline->getDtSislCurve());
//
//        
//        for ( int ii=0;ii<dtC->getNControlPoints();ii++) {
//          dtPoint2 cP = dtC->getControlPoint2d(ii);
//          dtPoint2 cPNew(
//            pTwoFrom.x() + cP.x() * ( pTwoTo.x() - pTwoFrom.x() ),
//            pTwoFrom.y() + cP.y() * ( pTwoTo.y() - pTwoFrom.y() )
//          );
//          dtC->setControlPoint2d(ii, cPNew);
//        }
////        surfAS->getSplineCurve3dSplinePercent(dtC)->dump();
//        
//        addEdge(
//          surfAS->getSplineCurve3dSplinePercent(dtC),
//          0., 
//          1., 
//          fromVertex, 
//          toVertex, 
//          nPointsSpline,
//          surfAS
//        );
//        
//        delete sF;
//        
//        return;
//      }    
//    }
//  }
  
//  void simpleBlockStructure::addEdgeInSurface(
//                              analyticSurface const * surfAS, 
//                              int const fromVertex, 
//                              int const toVertex, 
//                              int const nPointsSpline
//                            ) {    
//      //
//      // check if edge is marked as a spline edge
//      // 
//      if ( markedEdge(fromVertex, toVertex) ) {
//        dt__warning(addSpline(),
//                << "Marked edge " << dt__eval(fromVertex) << " " << dt__eval(toVertex) );
//        addSplineEdge(surfAS, fromVertex, toVertex, nPointsSpline);
//        return;
//      }
//      else {
//        //
//        // add straight edge
//        //
//        dtPoint2 fromP2 = surfAS->getClosestPointParameterPercent(getVertex(fromVertex));
//        dtPoint2 toP2 = surfAS->getClosestPointParameterPercent(getVertex(toVertex));
//        
//        /**
//         *  @todo: fix this bug. Strange bug, produces a segmentation fault when 
//         * creating the linear straight spline out of the two points
//         */
//        if (CGAL::compare_xy(fromP2, toP2) == CGAL::EQUAL) {
//          dt__THROW(addEdgeInSurface(),
//                  << dt__eval(fromVertex) << std::endl
//                  << dt__eval(toVertex) << std::endl
//                  << dt__eval(getVertex(fromVertex)) << std::endl
//                  << dt__eval(getVertex(toVertex)) << std::endl
//                  << dt__eval(fromP2) << std::endl
//                  << dt__eval(toP2) << std::endl
//                  << "fromP2 == toP2" << std::endl
//                  << "Unexpected error.");
//        }
//        
//        //
//        // add edge
//        //
//        addEdge(
//          surfAS->getSplineCurve3dLinearPercent(
//                    fromP2,
//                    toP2
//                  ),
//          0., 
//          1., 
//          fromVertex, 
//          toVertex, 
//          nPointsSpline,
//          surfAS
//        );      
//      }
//    }

  void simpleBlockStructure::addEdgeNoSurface(
                              int const fromVertex, 
                              int const toVertex, 
                              int const nPointsSpline
                            ) {
    std::vector< dtPoint3 > points;
    points.push_back(getVertex(fromVertex));
    points.push_back(getVertex(toVertex));
    addEdge(
      new splineCurve3d(&points,2),
      0., 
      1., 
      fromVertex, 
      toVertex, 
      nPointsSpline 
    );             
  }
  
//  void simpleBlockStructure::markSplineEdge( int const fromVertex, int const toVertex, scaFunction const * sF) {
//    _markedEdgeFrom.push_back( fromVertex );
//    _markedEdgeTo.push_back( toVertex );
////    scaFunction * sFClone;
////    sF->copy( &sFClone );
//    _markedEdgeScaFunction.push_back( sF );
//  }

//  bool simpleBlockStructure::markedEdge( int const fromVertex, int const toVertex) const {
//
//    for (int ii=0;ii<_markedEdgeFrom.size();ii++) {
//      if ( (_markedEdgeFrom[ii] == fromVertex) 
//              && (_markedEdgeTo[ii] == toVertex) ) {
//        return true;
//      }
//    }
//    return false;
//  }
  
//  int simpleBlockStructure::getNEdges( void ) const {
//    return _edgeFrom.size();
//  }
  
//  int simpleBlockStructure::getFromByEdgeIndex(int const eId) const {
//    return _edgeFrom[eId];
//  }
//  
//  int simpleBlockStructure::getToByEdgeIndex(int const eId) const {
//    return _edgeTo[eId];
//  }
  
//  std::vector< dtPoint3 > simpleBlockStructure::getPointsOnEdge( analyticGeometry const * aG, float const uuStart, float const uuEnd, int const nPoints ) const {
//    std::vector< dtPoint3 > pointOnEdge;
//
//    float nPointsF = (float) nPoints;
//    float delta = (uuEnd - uuStart) / (nPointsF-1.);
//
//      map1dTo3d const * map;
//      dt__mustCast(aG, map1dTo3d const, map);
//      pointOnEdge.push_back( map->getPointPercent(0.) );
//    for (int ii=1; ii<nPoints-1; ii++) {
//      float iiF = (float) ii;
//      float value = uuStart + iiF * delta;
//      pointOnEdge.push_back( map->getPointPercent(value) );
//    }
//    pointOnEdge.push_back( map->getPointPercent(1.) );
//    return pointOnEdge;
//  }
  
//  std::vector< dtPoint3 > simpleBlockStructure::getPointsOnEdge( 
//    analyticGeometry const * aG, 
//    float const uuStart, 
//    float const uuEnd, 
//    int const nPoints,
//    int const startVertex,
//    int const endVertex
//  ) const {
//    std::vector< dtPoint3 > pointOnEdge;
//
//    float nPointsF = (float) nPoints;
//    float delta = (uuEnd - uuStart) / (nPointsF-1.);
//
//      map1dTo3d const * map;
//      dt__mustCast(aG, map1dTo3d const, map);
////      pointOnEdge.push_back( map->getPointPercent(0.) );
//      pointOnEdge.push_back( getVertex(startVertex) );
//    for (int ii=1; ii<nPoints-1; ii++) {
//      float iiF = (float) ii;
//      float value = uuStart + iiF * delta;
//      pointOnEdge.push_back( map->getPointPercent(value) );
//    }
//    pointOnEdge.push_back( getVertex(endVertex) );
//    return pointOnEdge;
//  }
  
//  void simpleBlockStructure::addPatch(std::string const patchName, 
//                                     int const zero, 
//                                     int const one, 
//                                     int const two, 
//                                     int const three) {
//    for (int ii=0;ii<_patchName.size();ii++) {
//      if (patchName == _patchName[ii]) {
//        _patchVertex[ii].push_back(zero);
//        _patchVertex[ii].push_back(one);
//        _patchVertex[ii].push_back(two);
//        _patchVertex[ii].push_back(three);
//        return;
//      }
//    }
//    _patchName.push_back(patchName);
//    blockNumbering vertex;
//     vertex.push_back(zero);
//    vertex.push_back(one);
//    vertex.push_back(two);
//    vertex.push_back(three);
//    _patchVertex.push_back(vertex);
//  }
  
//  covise::coDoSet * simpleBlockStructure::toCoDoSet(char const * const str) const {
//    coDistributedObject ** obj = new coDistributedObject*[_edge.size()+1];
//    for (int ii=0;ii<_edge.size();ii++) {
//      obj[ii] = _edge[ii]->toCoDoSet( _edge[ii]->getName().c_str() );
//    }
//    obj[_edge.size()] = NULL;
//    
//    coDoSet * retSet = new coDoSet(str, (coDistributedObject **)obj);
//    
//    for (int ii=0;ii<_edge.size();ii++) {
//      delete obj[ii];
//    }
//    delete [] obj;
//    
//    return retSet;
//  }

//  int simpleBlockStructure::getEdgeFromToIndex( int const from, int const to) const {
//    for (int ii=0;ii<_edgeFrom.size();ii++) {
//      if (_edgeFrom[ii] == from) {
//        if (_edgeTo[ii] == to) {
//          return ii;
//        }
//      }
//    }
//    dt__THROW(getEdgeFromToIndex(),
//            << "No edge " << dt__eval(from) << " " 
//            << dt__eval(to) << " in instance.");
//  }
  
  int simpleBlockStructure::getBlockIndex( std::string const blockName ) const {
    for (int ii=0;ii<_blockName.size();ii++) {
      if (_blockName[ii] == blockName) {
        return ii;
      }
    }
    dt__THROW(getBlockIndex(),
            << "No block " << dt__eval(blockName) << " in instance.");    
  }
  
//  std::string simpleBlockStructure::getBlockName( int const blockIndex ) const {
//    return _blockName[blockIndex];
//  }
  
//  analyticGeometry const * simpleBlockStructure::getEdgeFromTo( int const from, int const to) const {
//    int pos = getEdgeFromToIndex(from, to);
//    
//    return _edge[pos];
//  }

//  int simpleBlockStructure::getNFaces( void ) const {
//    return _faceEdgeNumbering.size();
//  }
  
//  blockNumbering const & simpleBlockStructure::getFaceNumbering( int const fId ) const {
//    return _faceEdgeNumbering[fId];
//  }
  
  int simpleBlockStructure::getFaceIndex( blockNumbering const bN ) const {
    int fId = getVecInVecPos(bN, _faceEdgeNumberingSorted);
    
    if ( fId == -1) {
      dt__THROW(getFaceIndex(),
              << "Face = " << intVecToString(bN) << " not in model");
    }
    return fId;
  }
  
  int simpleBlockStructure::getFaceIndex( int const e0, int const e1, int const e2, int const e3) const {
    blockNumbering bN(4,0);
    
    bN[0] = e0;
    bN[1] = e1;
    bN[2] = e2;
    bN[3] = e3;
    
    return getFaceIndex(bN);
  }
  
//  std::vector< float > simpleBlockStructure::getAnglesInBlock( std::string const blockName ) const {
//    return getAnglesDegInBlock( getBlockIndex(blockName) );
//  }
  
  std::vector< float > simpleBlockStructure::getAnglesInBlock( int const blockIndex ) const {
    std::vector< float > angles;
//    
//    int pos = blockIndex;
//    
//    splineCurve3d const * sCOne;
//    splineCurve3d const * sCTwo;
//    splineCurve3d const * sCThree;
//    dtPoint3 oo;
//    dtPoint3 pp;
//    dtPoint3 qq;
//    dtPoint3 rr;
//
//    //
//    // 4  
//    // | 3
//    // |/
//    // 0----1
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][1]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][3]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][4]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getFirstControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//    
//    //
//    //      5  
//    //      | 2
//    //      |/
//    // 0----1
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][1]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][1], _block[pos][2]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][1], _block[pos][5]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getLastControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getPreLastControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//
//    //
//    //      6  
//    //      |
//    //      |
//    // 3----2
//    //     /
//    //    1
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][3], _block[pos][2]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][1], _block[pos][2]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][2], _block[pos][6]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getLastControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getPreLastControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getPreLastControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//
//    //
//    //   7  
//    //   |
//    //   |
//    //   3----2
//    //  /
//    // 0
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][3], _block[pos][2]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][3]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][3], _block[pos][7]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getFirstControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getPreLastControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//
//    //   
//    //   7
//    //  /
//    // 4----5
//    // |
//    // |
//    // 0
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][4], _block[pos][5]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][4], _block[pos][7]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][0], _block[pos][4]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getFirstControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getPreLastControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//    
//    //
//    //        
//    //        6
//    //       /
//    // 4----5
//    //      |
//    //      |
//    //      1
//    dt__mustCast( getEdgeFromTo(_block[pos][4], _block[pos][5]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][5], _block[pos][6]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][1], _block[pos][5]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getLastControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getPreLastControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getPreLastControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//     
//    //      
//    // 7----6
//    //     /|
//    //    5 |
//    //      2
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][7], _block[pos][6]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][5], _block[pos][6]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][2], _block[pos][6]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getLastControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getPreLastControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getPreLastControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getPreLastControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);
//
//    //
//    //   7----6
//    //  /|
//    // 4 |
//    //   3
//    //
//    dt__mustCast( getEdgeFromTo(_block[pos][7], _block[pos][6]), splineCurve3d const, sCOne );
//    dt__mustCast( getEdgeFromTo(_block[pos][4], _block[pos][7]), splineCurve3d const, sCTwo );
//    dt__mustCast( getEdgeFromTo(_block[pos][3], _block[pos][7]), splineCurve3d const, sCThree );
//    oo = sCOne->getConstDtSislCurve()->getFirstControlPoint3d();
//    pp = sCOne->getConstDtSislCurve()->getFirstNextControlPoint3d();
//    qq = sCTwo->getConstDtSislCurve()->getPreLastControlPoint3d();
//    rr = sCThree->getConstDtSislCurve()->getPreLastControlPoint3d();
//    calculateAnglesFromPoints(oo, pp, qq, rr, angles);    
//
//    
    return angles;
  }

  float simpleBlockStructure::getJacobianInBlockFace( int const blockIndex, int const faceIndex ) const {
    std::vector< int > const & bfi = _blockFaceVertex[blockIndex][faceIndex];
    
    std::vector< ::MVertex > vv;
    for (int ii=0; ii<4; ii++) {
      dtPoint3 vertex = getVertex(bfi[ii]);
      vv.push_back( ::MVertex( vertex.x(), vertex.y(), vertex.z() ) );
    }
    
    ::MQuadrangle quad( &vv[0], &vv[1], &vv[2] , &vv[3] );
    
    std::vector< double > det;
    det.push_back( quad.getJacobianDeterminant(-1, -1, 0) );
    det.push_back( quad.getJacobianDeterminant(1, 1, 0) );
    det.push_back( quad.getJacobianDeterminant(-1, 1, 0) );
    det.push_back( quad.getJacobianDeterminant(1, -1, 0) ); 

    return static_cast< float >( 
             *std::min_element(det.begin(), det.end()) / *std::max_element(det.begin(), det.end()) 
           );
  } 
  
//  float simpleBlockStructure::getJacobianInBlockFace( std::string const blockName, int const faceIndex ) const {
//    return getJacobianInBlockFace(getBlockIndex(blockName), faceIndex);
//  }

//  std::vector< float > simpleBlockStructure::getAnglesDegInBlock( std::string const blockName ) const {
//    std::vector< float > angles = getAnglesInBlock(blockName);
//    
//    dt__FORALL(angles, ii,
//      angles[ii] = 180. * angles[ii]/M_PI;
//    );
//    
//    return angles;
//  }
  
  std::vector< float > simpleBlockStructure::getAnglesDegInBlock( int const blockIndex ) const {
    std::vector< float > angles = getAnglesInBlock(blockIndex);
    
    dt__FORALL(angles, ii,
      angles[ii] = 180. * angles[ii]/M_PI;
    );
    
    return angles;    
  }
  
//  float simpleBlockStructure::getJacobianOfBlock( std::string const blockName ) const {
//    std::vector< ::MVertex > vertex;
//    blockNumbering const & nodes = _block[getBlockIndex(blockName)];
//    
//    for (int ii=0; ii<nodes.size(); ii++) {
//      dtPoint3 const pp = getVertex(nodes[ii]);
//      vertex.push_back( ::MVertex( pp.x(), pp.y(), pp.z() ) );
//    }
//    
//    MHexahedron hex( &vertex[3], &vertex[2], &vertex[6], &vertex[7],
//                     &vertex[0], &vertex[1], &vertex[5], &vertex[4]);
//    
//    std::vector< double > det;
//    det.push_back( hex.getJacobianDeterminant(-1, -1, -1) );
//    det.push_back( hex.getJacobianDeterminant( 1, -1, -1) );
//    det.push_back( hex.getJacobianDeterminant(-1,  1, -1) );
//    det.push_back( hex.getJacobianDeterminant( 1,  1, -1) );
//    det.push_back( hex.getJacobianDeterminant(-1, -1,  1) );
//    det.push_back( hex.getJacobianDeterminant( 1, -1,  1) );
//    det.push_back( hex.getJacobianDeterminant(-1,  1,  1) );
//    det.push_back( hex.getJacobianDeterminant( 1,  1,  1) );
//    
//    return static_cast< float >( 
//             *std::min_element(det.begin(), det.end()) / *std::max_element(det.begin(), det.end()) 
//           );
//  }  

//  std::vector< float > simpleBlockStructure::getAnglesDegInAllBlocks( void ) const {
//    std::vector< float > angles;
//    
//    dt__FORALL(_blockName, ii,
//       std::vector< float > blockAngles = getAnglesInBlock( _blockName[ii] );
//       dt__FORALL(blockAngles, jj,
//         angles.push_back( blockAngles[jj] );
//       );
//    );
//    
//    dt__FORALL(angles, ii,
//      angles[ii] = 180. * angles[ii]/M_PI;
//    );
//    
//    return angles;
//  }  
  
//  std::vector< float > simpleBlockStructure::getJacobiansOfAllBlocks( void ) const {
//    std::vector< float > jac;
//    
//    dt__FORALL(_blockName, ii,
//      jac.push_back( getJacobianOfBlock(_blockName[ii]) );
//    );
//        
//    return jac;
//  }
  
  
  void simpleBlockStructure::calculateAnglesFromPoints(
           dtPoint3 const oo, 
           dtPoint3 const pp, 
           dtPoint3 const qq, 
           dtPoint3 const rr,
           std::vector< float > & angles          
         ) const {

    dtVector3 uu = pp - oo;
    dtVector3 vv = qq - oo;
    dtVector3 ww = rr - oo;
    
    angles.push_back(
      acos( uu * vv / sqrt(uu.squared_length() * vv.squared_length()) )
    );

    angles.push_back(
      acos( uu * ww / sqrt(uu.squared_length() * ww.squared_length()) )
    );

    angles.push_back(
      acos( vv * ww / sqrt(vv.squared_length() * ww.squared_length()) )
    );
  }
  
//  int simpleBlockStructure::getNBlocks( void ) const {
//    return _block.size();
//  }
  
//  blockNumbering const & simpleBlockStructure::getBlockNumbering(int const blockId) const {
//    return _block[blockId];
//  }

//  blockNumbering const & simpleBlockStructure::getBlockEdgeNumbering(int const blockId) {
//    if (_blockEdge[blockId].size() == 0) {
//      blockNumbering blockVertex = getBlockNumbering(blockId);
//      _blockEdge[blockId].resize(12);
//      _blockEdge[blockId][0] = getEdgeFromToIndex(blockVertex[0], blockVertex[1]);
//      _blockEdge[blockId][1] = getEdgeFromToIndex(blockVertex[3], blockVertex[2]);
//      _blockEdge[blockId][2] = getEdgeFromToIndex(blockVertex[7], blockVertex[6]);
//      _blockEdge[blockId][3] = getEdgeFromToIndex(blockVertex[4], blockVertex[5]);
//      _blockEdge[blockId][4] = getEdgeFromToIndex(blockVertex[0], blockVertex[3]);
//      _blockEdge[blockId][5] = getEdgeFromToIndex(blockVertex[1], blockVertex[2]);
//      _blockEdge[blockId][6] = getEdgeFromToIndex(blockVertex[5], blockVertex[6]);
//      _blockEdge[blockId][7] = getEdgeFromToIndex(blockVertex[4], blockVertex[7]);
//      _blockEdge[blockId][8] = getEdgeFromToIndex(blockVertex[0], blockVertex[4]);
//      _blockEdge[blockId][9] = getEdgeFromToIndex(blockVertex[1], blockVertex[5]);
//      _blockEdge[blockId][10] = getEdgeFromToIndex(blockVertex[2], blockVertex[6]);      
//      _blockEdge[blockId][11] = getEdgeFromToIndex(blockVertex[3], blockVertex[7]);
//    }
//    
//    return _blockEdge[blockId];
//  }  
  
//  blockNumbering const & simpleBlockStructure::getBlockFaceNumbering(int const blockId) {
//    if (_blockFace[blockId].size() == 0) {
//      blockNumbering bV = getBlockNumbering(blockId);
//      _blockFace[blockId].resize(6);
//      _blockFace[blockId][0] = getFaceIndex(bV[0], bV[1], bV[2], bV[3]);
//      _blockFace[blockId][1] = getFaceIndex(bV[1], bV[2], bV[6], bV[5]);
//      _blockFace[blockId][2] = getFaceIndex(bV[4], bV[5], bV[6], bV[7]);
//      _blockFace[blockId][3] = getFaceIndex(bV[0], bV[3], bV[7], bV[4]);
//      _blockFace[blockId][4] = getFaceIndex(bV[0], bV[1], bV[5], bV[4]);
//      _blockFace[blockId][5] = getFaceIndex(bV[3], bV[2], bV[6], bV[7]);
//    }
//    
//    return _blockFace[blockId];    
//  }
  
  bool simpleBlockStructure::vecInVec( blockNumbering bN, blockNumbering * bNSorted, std::vector< blockNumbering > const & vecBn) const {
    if (bNSorted->size() != bN.size()) {
      bNSorted->resize( bN.size() );
    }

    int pos = getVecInVecPos(bN, vecBn);

    std::sort( bN.begin(), bN.begin()+bN.size() );
    *bNSorted = bN;          
    
    if (pos >= 0) {
      return true;
    }
    return false;
  }

  int simpleBlockStructure::getVecInVecPos( blockNumbering bN, std::vector< blockNumbering > const & vecBn) const {
    std::sort( bN.begin(), bN.begin()+bN.size() );
    
    dt__FORALL(vecBn, ii,
      if ( vecBn[ii] == bN ) {
        return ii;
      }
    );
    return -1;
  }  
  
//  void simpleBlockStructure::addCouplingPatchName( std::string const patchName ) {
//    _couplingPatchName.push_back( patchName );
//  }

//  int simpleBlockStructure::getNCouplingPatches( void ) const {
//    return _couplingPatchName.size();
//  }

//  std::string simpleBlockStructure::getCouplingPatchName( int const couplingPatchIndex ) const {
//    return _couplingPatchName[couplingPatchIndex];
//  }
  
//  std::vector< blockNumbering > simpleBlockStructure::getPatch( std::string const patchName ) const {
//    std::vector< blockNumbering > ret;
//    
////    dt__FORALL(_patchName, ii,
//    for (int ii=0;ii<_patchName.size();ii++) {
//      if (_patchName[ii] == patchName) {
//        for (int jj=0;jj<_patchVertex[ii].size();jj=jj+4) {
//          blockNumbering tmp;
//          tmp.push_back( _patchVertex[ii][jj] );
//          tmp.push_back( _patchVertex[ii][jj+1] );
//          tmp.push_back( _patchVertex[ii][jj+2] );
//          tmp.push_back( _patchVertex[ii][jj+3] );
//          ret.push_back( tmp );
//        }
//        
//      }
//  }
//    
//    return ret;
//  }
  
  blockNumberingFloat const & simpleBlockStructure::getBlockGrading(int const blockId) const {
    return _blockGrading[blockId];
  }
  
  blockNumbering const & simpleBlockStructure::getBlockNElements(int const blockId) const {
    return _blockNElem[blockId];
  }
  
//  void simpleBlockStructure::dump( void ) const {
//    DTBUFFERINIT();
//    dt__FORALL(_vertexId, ii,
//      DTBUFFER( << "Vertex " << _vertexId[ii] << ": " << getVertex( _vertexId[ii] ) << std::endl);
//    );
//    dt__FORALL(_block, ii,
//      DTBUFFER( << "Block: ");
//      dt__FORALL(_block[ii], jj,
//        DTBUFFER( << _block[ii][jj] << " " );
//      );
//      DTBUFFER( << std::endl );
//    );    
//    dt__info_BUFFER(dump());
    
//    _macroMesh->dump();
//  }
}
