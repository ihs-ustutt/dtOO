#include "c14Volume.h"

#include "progHelper.h"

#include <interfaceHeaven/profilingHandling.h>

#include <gmsh/Context.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/GModel.h>
#include <gmsh/GEdge.h>
#include <gmsh/OpenFile.h>

//#include <limits>
#include <dtGmshVertex.h>
#include <dtGmshEdge.h>
#include <dtGmshFace.h>
#include <dtGmshRegion.h>
#include <dtGmshRegionHex.h>
#include <constValueHeaven/constValue.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <debugHeaven/extInfoContainer.h>
#include "blockGridModel.h"

#include <interfaceHeaven/floatHandling.h>


#define __addVertex(surf1, uu, vv, id) \
  addVertex( sRefP[fStrToId(#surf1)]->getPointPercent( _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue() ), id, _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue(), fStrToId(#surf1) )
#define __addVertex2(surf1, uu, vv, id, surf2) \
  addVertex( sRefP[fStrToId(#surf1)]->getPointPercent( _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue() ), id, _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue(), fStrToId(#surf1), fStrToId(#surf2) )
#define __addVertex3(surf1, uu, vv, id, surf2, surf3) \
  addVertex( sRefP[fStrToId(#surf1)]->getPointPercent( _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue() ), id, _valCValTwins.get(#uu)->getValue(), _valCValTwins.get(#vv)->getValue(), fStrToId(#surf1), fStrToId(#surf2), fStrToId(#surf3) )


namespace dtOO {
  c14Volume::c14Volume() {    
    GmshInitialize();
  }

  c14Volume::~c14Volume() {
   _valCValTwins.destroy();  
   GmshFinalize();
  }

  void c14Volume::init(
          QDomElement const & element,
          vectorHandling< constValue * > const * const cValP,
          vectorHandling< analyticFunction * > const * const sFunP,
          vectorHandling< analyticGeometry * > const * const depAGeoP,
          vectorHandling< boundedVolume * > const * const depBVolP
        ) {
    //
    // init cardinalDirVolume
    //
    cardinalDirTransfiniteVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);
    
    //
    // destroy
    //
    _valCValTwins.destroy();
    
    //
    // set constValues float
    //
    QDomElement wElement = getChild("constValue", element);
    while ( !wElement.isNull() ) {
      std::string label = getAttributeStr("label", wElement);      
      _valCValTwins.push_back( cValP->get(label)->clone() );

      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);      
    }    
  }

  covise::coDoSet * c14Volume::toCoDoSet(char const * const str) const {
    return _gModel->toCoDoSet(str);
  }

  void c14Volume::makePreGrid( void ) {
    //
    // clear
    //
    dtMacroMesh::clear();
    
//    if ( !parametersOk() ) return;
    
    coDoSetHandling< map2dTo3d * > & sRefP = getRefToMap2dTo3dHandling();

    __addVertex3( dtSouth, 0u, 0v, 0, dtEast, dtFront );
    __addVertex2( dtSouth, 1u, 1v,  1, dtEast );
    __addVertex( dtSouth, 2u, 2v,  2 );
    __addVertex2( dtSouth, 3u, 3v,  3, dtFront );
    __addVertex3( dtNorth, 4u, 4v,  4, dtEast, dtFront );
    __addVertex2( dtNorth, 5u, 5v,  5, dtEast );
    __addVertex( dtNorth, 6u, 6v,  6 );
    __addVertex2( dtNorth, 7u, 7v,  7, dtFront );
    __addVertex( dtSouth, 8u, 8v,  8 );
    __addVertex2( dtSouth, 9u, 9v,  9, dtFront );
    __addVertex( dtNorth, 10u, 10v,  10 );
    __addVertex2( dtNorth, 11u, 11v,  11, dtFront );    
    __addVertex( dtSouth, 12u, 12v,  12 );
    __addVertex2( dtSouth, 13u, 13v,  13, dtFront );
    __addVertex( dtNorth, 14u, 14v,  14 );
    __addVertex2( dtNorth, 15u, 15v,  15, dtFront );
    __addVertex2( dtSouth, 16u, 16v,  16, dtWest );
    __addVertex3( dtSouth, 17u, 17v,  17, dtWest, dtFront );
    __addVertex2( dtNorth, 18u, 18v,  18, dtWest );
    __addVertex3( dtNorth, 19u, 19v,  19, dtWest, dtFront );
    __addVertex2( dtSouth, 20u, 20v,  20, dtEast );
    __addVertex( dtSouth, 21u, 21v,  21 );
    __addVertex2( dtNorth, 22u, 22v,  22, dtEast );
    __addVertex( dtNorth, 23u, 23v,  23 );
    __addVertex2( dtInternal, 24u, 24v,  24, dtSouth );
    __addVertex2( dtInternal, 25u, 25v,  25, dtSouth );
    __addVertex2( dtInternal, 26u, 26v,  26, dtNorth );
    __addVertex2( dtInternal, 27u, 27v,  27, dtNorth );
    __addVertex2( dtInternal, 28u, 28v,  28, dtSouth );
    __addVertex2( dtInternal, 29u, 29v,  29, dtNorth );    
    __addVertex2( dtInternal, 30u, 30v,  30, dtSouth );
    __addVertex2( dtInternal, 31u, 31v,  31, dtNorth );
    __addVertex( dtSouth, 32u, 32v,  32 );
    __addVertex( dtNorth, 33u, 33v,  33 );
    __addVertex2( dtSouth, 34u, 34v,  34, dtWest );
    __addVertex2( dtNorth, 35u, 35v,  35, dtWest );
    __addVertex3( dtSouth, 36u, 36v,  36, dtEast, dtBack );
    __addVertex2( dtSouth, 37u, 37v,  37, dtBack );
    __addVertex3( dtNorth, 38u, 38v,  38, dtEast, dtBack );
    __addVertex2( dtNorth, 39u, 39v,  39, dtBack );    
    __addVertex2( dtSouth, 40u, 40v,  40, dtBack );
    __addVertex2( dtNorth, 41u, 41v,  41, dtBack );
    __addVertex2( dtSouth, 42u, 42v,  42, dtBack );
    __addVertex2( dtNorth, 43u, 43v,  43, dtBack );
    __addVertex3( dtSouth, 44u, 44v,  44, dtWest, dtBack );
    __addVertex3( dtNorth, 45u, 45v,  45, dtWest, dtBack );
    
    //
    // snap all vertices in volume
    //
    this->snapAllVerticesToMap3dTo3d( rStrToId("dtVolume") );
       
//    addBlock(0,1,2,3,4,5,6,7, "fee");
//    addBlock(3,2,8,9,7,6,10,11, "fe");
//    addBlock(9,8,12,13,11,10,14,15, "fw");
//    addBlock(13,12,16,17,15,14,18,19, "fww");
//    addBlock(1,20,21,2,5,22,23,6, "ieee");  
//    addBlock(12,32,34,16,14,33,35,18, "iwww"); 
//    addBlock(20,36,37,21,22,38,39,23, "bee");
//    addBlock(21,37,40,24,23,39,41,26, "be");
//    addBlock(32,24,40,42,33,26,41,43, "bw");
//    addBlock(32,42,44,34,33,43,45,35, "bww");
    addBlock(2,21,24,25,6,23,26,27, "iee");
    addBlock(2,25,28,8,6,27,29,10, "ie");
    addBlock(8,28,30,12,10,29,31,14, "iw");
    addBlock(12,30,24,32,14,31,26,33, "iww");          
    
    dtMacroMesh::dump();

    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }
    
    //
    // parse gmsh file if option exists
    //
    if ( hasOption("gmshMeshFile") ) {
      ParseFile( getOption("gmshMeshFile"), true, true );
    }
    
    //
    // create model and attach a boundedVolume
    //
    _gModel.reset( new blockGridModel("blockGridModel") );
    ::GModel::setCurrent( _gModel.get() );
    _gModel->attachToBoundedVolume( this );
    
    //
    // add vertices
    //
    for (int ii=0;ii<getNVertices();ii++) {
      _gModel->addDtGmshVertex(ii+1);
      std::vector< int > bN = getVertexSnapSurfId(ii);
      _gModel->snapVertexPercent(ii+1, bN[0], getVertexUV(ii));
      if (getVertexSnapSurfId(ii).size() > 1) {
        for(int jj=1; jj<bN.size(); jj++) {
          _gModel->snapVertex(ii+1, bN[jj]);
        }
      }
      if (getVertexSnapVolumeId(ii).size() == 1) {
        _gModel->snap3dVertex(ii+1, getVertexSnapVolumeId(ii)[0]);
      }
    }		

    //
    // snap edges to surfaces
    //
    for(int ii=0; ii<getNEdges();ii++) {
      _gModel->addDtGmshEdge(ii+1, getFromByEdgeIndex(ii)+1, getToByEdgeIndex(ii)+1 );
      bool snapped = _gModel->snapEdge(ii+1);
      if (!snapped) {
        bool forceSnapped = _gModel->forceSnapEdge(ii+1);
        if (!forceSnapped) {
          dt__THROW(writeGrid(), << "Cannot snap edge " << DTLOGEVAL(ii+1) );
        }
      }      
    }
		
    //
    // add faces
    //
    for(int ii=0; ii<getNFaces(); ii++) {
      std::vector< int > fV = getFaceVertices(ii);
      std::vector< int > eI(4,0);
      eI[0] = getEdgeFromToIndex(fV[0], fV[1]);
      eI[1] = getEdgeFromToIndex(fV[1], fV[2]);
      eI[2] = getEdgeFromToIndex(fV[2], fV[3]);
      eI[3] = getEdgeFromToIndex(fV[3], fV[0]);
      _gModel->addDtGmshFace(
        ii+1, 
        eI[0]+1, eI[1]+1, eI[2]+1, eI[3]+1
      );
      
      //
      // add mesh attribute if transfinite meshing
      //
      _gModel->getDtGmshEdgeByTag( eI[0]+1 )->setNElements(3);
      _gModel->getDtGmshEdgeByTag( eI[1]+1 )->setNElements(3);
      _gModel->getDtGmshEdgeByTag( eI[2]+1 )->setNElements(3);
      _gModel->getDtGmshEdgeByTag( eI[3]+1 )->setNElements(3);

      _gModel->getDtGmshEdgeByTag( eI[0]+1 )->meshTransfinite(1, 1.);
      _gModel->getDtGmshEdgeByTag( eI[1]+1 )->meshTransfinite(1, 1.);
      _gModel->getDtGmshEdgeByTag( eI[2]+1 )->meshTransfinite(1, 1.);
      _gModel->getDtGmshEdgeByTag( eI[3]+1 )->meshTransfinite(1, 1.);
      
      _gModel->getDtGmshFaceByTag(ii+1)->meshTransfinite();
    }

    //
    // add regions
    //
    std::vector< int > ori;
    std::list< ::GFace * > faces;    
    for(int ii=0; ii<getNBlocks();ii++) {
      ori.clear();
      faces.clear();
      std::vector< int > fI = getBlockFaces(ii);
      dt__FORALL(fI, jj,
        faces.push_back( _gModel->getFaceByTag( fI[jj]+1 ) );
        ori.push_back( 1 );
      );
      //
      // add dtGmshRegionHex
      //
      if (faces.size() == 6) {
        std::vector< int > hexVert = getBlockVertices(ii);
        std::vector< int > hexEdges(12,0);
        hexEdges[0]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[0]+1, hexVert[1]+1);
        hexEdges[1]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[3]+1, hexVert[2]+1);
        hexEdges[2]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[7]+1, hexVert[6]+1);
        hexEdges[3]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[4]+1, hexVert[5]+1);
        hexEdges[4]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[0]+1, hexVert[3]+1);
        hexEdges[5]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[1]+1, hexVert[2]+1);
        hexEdges[6]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[5]+1, hexVert[6]+1);
        hexEdges[7]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[4]+1, hexVert[7]+1);
        hexEdges[8]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[0]+1, hexVert[4]+1);
        hexEdges[9]  = _gModel->getDtGmshEdgeTagByFromTo(hexVert[1]+1, hexVert[5]+1);
        hexEdges[10] = _gModel->getDtGmshEdgeTagByFromTo(hexVert[2]+1, hexVert[6]+1);
        hexEdges[11] = _gModel->getDtGmshEdgeTagByFromTo(hexVert[3]+1, hexVert[7]+1);
        
        _gModel->add( new dtGmshRegionHex(_gModel.get(), ii+1, hexEdges, faces, ori) );
      }
      else {
        _gModel->add( new dtGmshRegion(_gModel.get(), ii+1, faces, ori) );
      }
      _gModel->getDtGmshRegionByTag(ii+1)->meshTransfinite();
    }

    //
    // set gradings and number of elements
    //
    int alpha = _valCValTwins.get("nElemAlpha")->getValue(); //alpha
    int beta = _valCValTwins.get("nElemBeta")->getValue(); //beta
    int gamma = _valCValTwins.get("nElemGamma")->getValue();//gamma
    int delta = _valCValTwins.get("nElemDelta")->getValue(); //delta
    int epsilon = _valCValTwins.get("nElemEpsilon")->getValue();//epsilon
    int phi = _valCValTwins.get("nElemPhi")->getValue(); //phi
    int psi = _valCValTwins.get("nElemPsi")->getValue(); //psi
    float gA = _valCValTwins.get("gradingA")->getValue(); //gA
    float gB = _valCValTwins.get("gradingB")->getValue(); //gB
    float gC = _valCValTwins.get("gradingC")->getValue(); //gC
    float gD = _valCValTwins.get("gradingD")->getValue(); //gD		
//    // north-south grading
//    _gModel->getDtGmshEdgeByFromTo(0+1 , 4+1 )->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(3+1 , 7+1 )->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(9+1 , 11+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(13+1, 15+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(17+1, 19+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(1+1 , 5+1 )->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(2+1 , 6+1 )->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(8+1 , 10+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(12+1, 14+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(16+1, 18+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(25+1, 27+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(28+1, 29+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(30+1, 31+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(20+1, 22+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(21+1, 23+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(24+1, 26+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(32+1, 33+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(34+1, 35+1)->meshTransfiniteWNElements(2, gA, gamma); 
//    _gModel->getDtGmshEdgeByFromTo(36+1, 38+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(37+1, 39+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(40+1, 41+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(42+1, 43+1)->meshTransfiniteWNElements(2, gA, gamma);
//    _gModel->getDtGmshEdgeByFromTo(44+1, 45+1)->meshTransfiniteWNElements(2, gA, gamma);
//    // blade grading
//    _gModel->meshEdgeTransfiniteFromTo(37+1, 40+1, 1, 1., delta);
//    _gModel->meshEdgeTransfiniteFromTo(39+1, 41+1, 1, 1., delta);
//    _gModel->meshEdgeTransfiniteFromTo(21+1, 24+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(21+1, 24+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(23+1, 26+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(6+1 , 27+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(2+1 , 25+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(10+1, 29+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(8+1 , 28+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(14+1, 31+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(12+1, 30+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(32+1, 24+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(33+1, 26+1, 1, gB, delta);
//    _gModel->meshEdgeTransfiniteFromTo(42+1, 40+1, 1, 1., delta);
//    _gModel->meshEdgeTransfiniteFromTo(43+1, 41+1, 1, 1., delta);
//    // inlet grading
//    _gModel->meshEdgeTransfiniteFromTo(0+1 , 1+1 , 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(3+1 , 2+1 , 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(4+1 , 5+1 , 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(7+1 , 6+1 , 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(9+1 , 8+1 , 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(11+1, 10+1, 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(13+1, 12+1, 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(15+1, 14+1, 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(17+1, 16+1, 1, 1., alpha);
//    _gModel->meshEdgeTransfiniteFromTo(19+1, 18+1, 1, 1., alpha);
//    // outlet grading
//    _gModel->meshEdgeTransfiniteFromTo(20+1, 36+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(22+1, 38+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(21+1, 37+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(23+1, 39+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(24+1, 40+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(26+1, 41+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(32+1, 42+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(33+1, 43+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(34+1, 44+1, 1, gC, psi);
//    _gModel->meshEdgeTransfiniteFromTo(35+1, 45+1, 1, gC, psi);    
//    // periodic grading
//    _gModel->meshEdgeTransfiniteFromTo(3+1 , 0+1 , 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(7+1 , 4+1 , 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(2+1 , 1+1 , 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(6+1 , 5+1 , 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(23+1, 22+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(21+1, 20+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(39+1, 38+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(37+1, 36+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(13+1, 17+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(15+1, 19+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(14+1, 18+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(12+1, 16+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(33+1, 35+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(32+1, 34+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(43+1, 45+1, 1, gD, epsilon);
//    _gModel->meshEdgeTransfiniteFromTo(42+1, 44+1, 1, gD, epsilon);
//    // blade grading
//    _gModel->meshEdgeTransfiniteFromTo(1+1 , 20+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(5+1 , 22+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(2+1 , 21+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(6+1 , 23+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(25+1, 24+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(27+1, 26+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(30+1, 24+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(31+1, 26+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(14+1, 33+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(12+1, 32+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(16+1, 34+1, 1, 1., phi);
//    _gModel->meshEdgeTransfiniteFromTo(18+1, 35+1, 1, 1., phi);
//    // inlet blade grading
//    _gModel->meshEdgeTransfiniteFromTo(9+1 , 3+1 , 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(11+1 ,7+1 , 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(8+1 , 2+1 , 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(10+1, 6+1 , 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(29+1, 27+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(28+1, 25+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(9+1 , 13+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(11+1, 15+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(10+1, 14+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(8+1 , 12+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(29+1, 31+1, 1, 1., beta);
//    _gModel->meshEdgeTransfiniteFromTo(28+1, 30+1, 1, 1., beta);
    
    //
    // call observers
    //
    this->notify();		
  }
  
  void c14Volume::makeGrid( void ) {
		//
		// make pre grid
		//
		makePreGrid();
    
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();
    
//    //
//    // gmsh output
//    //
//    _gModel->writeGEO(labelHandling::getLabel()+".geo"); 

    _gModel->meshVertex();
		
    for (int ii=1;ii<=_gModel->getNumRegions();ii++) {//14;ii++) {
		  _gModel->meshRegion( ii );
		}
    
    _gModel->writeMSH(labelHandling::getLabel()+"_3d.msh");
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();
  }
  
  covise::coDoSet * c14Volume::toCoDoUnstructuredGrid(char const * const str) const {
    return _gModel->toCoDoUnstructuredGrid(str);
  }
  
  covise::coDoSet * c14Volume::toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const {
    return _gModel->toCoDoSurfUnsGrid(str, boundFaceNum);
  }
    
  blockGridModel * c14Volume::refToBlockGridModel( void ) {
    return _gModel.get();
  }
}

