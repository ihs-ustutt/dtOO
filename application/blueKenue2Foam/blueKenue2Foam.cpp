#include <stdio.h>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParser.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>

#include <dtArg.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <iostream>

#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>

#include <gmsh/MVertex.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MLine.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MPrism.h>

#include <meshEngine/dtOMMesh.h>
#include <meshEngine/dtOMVertexField.h>
#include <meshEngine/dtOMEdgeField.h>

#include "progHelper.h"
#include "interfaceHeaven/calculationTypeHandling.h"
#include "meshEngine/dtGmshRegion.h"
#include "meshEngine/dtFoamLibrary.h"
#include <meshEngine/dtMoabCore.h>
#include <interfaceHeaven/systemHandling.h>

#include <criticalHeaven/prepareOpenFOAM.h>
#include <argList.H>
#include <Time.H>
#include <polyMesh.H>
#include <volFields.H>

using namespace dtOO;

twoDArrayHandling< std::string > readBlock(
  int const & nLines, std::ifstream & in
) {
  dt__infoNoClass(readBlock(), << "Read block with " << nLines << " lines ...");   
  twoDArrayHandling< std::string > ret;
  std::string line;
  int ii = 0;
  while ( ii<nLines ) {
    getline(in, line);
    // ignore comments
    if (stringPrimitive::stringContains("#", line) ) continue;
    boost::algorithm::trim_all(line);
    std::vector< std::string > parts;
    boost::split(
      parts, line, boost::is_any_of("\t "), boost::token_compress_on
    );
    ret.push_back( parts );
    ii = ii + 1;
  }
  dt__infoNoClass(
    readBlock(), 
    << "Done. Block dimension -> " << ret.size(0) << " x " << ret.size(1)
  );   
  return ret;
}

void readT3s( 
  std::string const & filename, 
  twoDArrayHandling< float > & nodes,
  vectorHandling< float > & bathymetry,
  twoDArrayHandling< int > & elements
) {
  std::ifstream in( filename.c_str() );
  dt__throwIfNoClass(!in, readT3s());

  int nNodes = 0;
  int nElements = 0;
  
  std::string line;    
  while ( getline(in, line) ) {
    //
    // get number of nodes
    //
    if ( stringPrimitive::stringContains( ":NodeCount", line ) ) {
      nNodes
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );        
    }
    //
    // get number of elements
    //
    else if ( stringPrimitive::stringContains( ":ElementCount", line ) ) {
      nElements
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );
    }
    //
    // read nodes and elements if nNodes and nElements is not zero
    //
    if ( (nNodes!=0) && (nElements!=0) ) {
      if ( stringPrimitive::stringContains( ":EndHeader", line ) ) {
        //
        // read nodes and bathymetry
        //
        dt__infoNoClass(
          readT3s(), << "[Nodes] : Read " << nNodes << " nodes ..."
        );
        twoDArrayHandling< std::string > blockStr = readBlock(nNodes, in);
        nodes.resize(nNodes, 3);
        bathymetry.resize(nNodes);
        int ii = 0;
        dt__forAllRefAuto(blockStr, aStr) {
          nodes[ii][0] = stringPrimitive::stringToFloat( aStr[0] );
          nodes[ii][1] = stringPrimitive::stringToFloat( aStr[1] );
          nodes[ii][2] = 0.;
          bathymetry[ii] = stringPrimitive::stringToFloat( aStr[4] );
          ii = ii + 1;
        }
        dt__infoNoClass(readT3s(), << "[Nodes] : Done."); 
        
        //
        // read elements
        //
        dt__infoNoClass(
          readT3s(), << "[Elements] : Read " << nElements << " elements ..."
        );      
        twoDArrayHandling< std::string > blockEStr = readBlock(nElements, in);
        elements.resize(nElements, 3);
        int jj = 0;
        dt__forAllRefAuto(blockEStr, aStr) {
          elements[jj][0] = stringPrimitive::stringToInt( aStr[0] )-1;
          elements[jj][1] = stringPrimitive::stringToInt( aStr[1] )-1;
          elements[jj][2] = stringPrimitive::stringToInt( aStr[2] )-1;
          jj = jj + 1;
        }
        dt__infoNoClass(readT3s(), << "[Elements] : Done."); 
      }      
    }    
  }
  in.close();  
  
  dt__infoNoClass(
    readT3s(), 
    << "nodes      > " << nodes.size(0) << " x " << nodes.size(1) << std::endl
    << "bathymetry > " << bathymetry.size() << std::endl
    << "elements   > " << elements.size(0) << " x " << elements.size(1)
  );
  return;
}

void splitShore(
  std::list< std::string > & shoreLabel, std::list< int > & shoreline 
) {
  std::string lastLabel = shoreLabel.back();
  int lastNode = shoreline.back();
  if ( shoreLabel.front()==shoreLabel.back() ) {
    dt__infoNoClass(
      splitShore(), 
      << logMe::vecToString( progHelper::list2Vector( shoreline ) ) << std::endl
      << logMe::vecToString( progHelper::list2Vector( shoreLabel ) )
    );
    
    for (
      auto it 
      = 
      ::boost::make_zip_iterator(
        ::boost::make_tuple(shoreLabel.begin(), shoreline.begin())
      ),
      ie
      =
      ::boost::make_zip_iterator(
        ::boost::make_tuple(shoreLabel.end(), shoreline.end())
      );
      it!=ie; 
      ++it
    ) {
      std::string aLabel = *(it.get_iterator_tuple().get<0>());
      int aNode = *(it.get_iterator_tuple().get<1>());
      //
      // find split point -> label change
      //
      if ( aLabel != lastLabel ) {
        //
        // split and merge shoreLabel
        //
        std::list< std::string > shoreL0 
        = 
        std::list< std::string >(it.get_iterator_tuple().get<0>(), shoreLabel.end());
        std::list< std::string > shoreL1 
        = 
        std::list< std::string >(shoreLabel.begin(), it.get_iterator_tuple().get<0>());
        shoreLabel.clear();
        shoreLabel.insert(shoreLabel.end(), shoreL0.begin(), shoreL0.end());
        shoreLabel.insert(shoreLabel.end(), shoreL1.begin(), shoreL1.end());      
        //
        // split and merge shoreline
        //        
        std::list< int > shore0 
        = 
        std::list< int >(it.get_iterator_tuple().get<1>(), shoreline.end());
        std::list< int > shore1 
        = 
        std::list< int >(shoreline.begin(), it.get_iterator_tuple().get<1>());
        shoreline.clear();
        shoreline.insert(shoreline.end(), shore0.begin(), shore0.end());
        shoreline.insert(shoreline.end(), shore1.begin(), shore1.end());  
        
        //
        // break the loop
        //
        break;
      }
      lastLabel = aLabel;
      lastNode = aNode;
    }
    dt__infoNoClass(
      splitShore(), 
      << logMe::vecToString( progHelper::list2Vector( shoreline ) ) << std::endl
      << logMe::vecToString( progHelper::list2Vector( shoreLabel ) )
    );    
  }
}

void readBc2( 
  std::string const & filename, 
  std::map< std::string, int > & boundaryNumber,
  std::map< std::string, std::list< int > > & boundary
) {
  std::ifstream in( filename.c_str() );
  dt__throwIfNoClass(!in, readBc2());

  int nNodes = 0;
  int nElements = 0;
  int nBoundarys = 0;
  int nShorelines = 0;
  int nShorelineNodes = 0;
  std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > >  segments;
  std::string line;    
  while ( getline(in, line) ) {
    //
    // get number of nodes
    //
    if ( stringPrimitive::stringContains( ":NodeCount", line ) ) {
      nNodes
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );        
    }
    //
    // get number of elements
    //
    else if ( stringPrimitive::stringContains( ":ElementCount", line ) ) {
      nElements
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );
    }
    //
    // get number of elements
    //
    else if ( stringPrimitive::stringContains( ":BoundarySegmentCount", line ) ) {
      nBoundarys
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );
      int jj = 0;
      dt__forAllRefAuto(readBlock(nBoundarys, in), aStr) {
        segments.push_back(
          std::pair< std::pair<int, int>, std::pair<int, int> >(
            std::pair<int, int>(0,0), std::pair<int, int>(0,0)
          )      
        );
        segments.back().first.first = stringPrimitive::stringToInt( aStr[4] );
        segments.back().first.second = stringPrimitive::stringToInt( aStr[5] );
        segments.back().second.first = stringPrimitive::stringToInt( aStr[6] );
        segments.back().second.second = stringPrimitive::stringToInt( aStr[7] );        
        dt__infoNoClass(
          readBc2(), 
          << "Segment[ " << jj << " ] > " 
          << segments[jj].first.first << " -> "
          << segments[jj].first.second << " :: "
          << segments[jj].second.first << " -> "
          << segments[jj].second.second
        );
        jj = jj + 1;
      }
    }    
    else if ( stringPrimitive::stringContains( ":ShorelineCount", line ) ) {
      nShorelines
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );
    }    
    else if ( stringPrimitive::stringContains( ":ShorelineNodeCount", line ) ) {
      nShorelineNodes
      =
      stringPrimitive::stringToInt( 
        stringPrimitive::getStringBetween(" ", "", line)
      );
    }        
    //
    // read shorelines
    //
    if ( 
      (nNodes!=0) && (nElements!=0) && (nBoundarys!=0) 
      && 
      (nShorelines!=0) && (nShorelineNodes!=0)
    ) {
      if ( stringPrimitive::stringContains( ":BeginTable", line ) ) {
        line = stringPrimitive::replaceStringInString(":BeginTable ", "", line);
        int nLines
        =
        stringPrimitive::stringToInt( 
          stringPrimitive::getStringBetween("", " ", line)
        );
        int nColumns
        =
        stringPrimitive::stringToInt( 
          stringPrimitive::getStringBetween(" ", "", line)
        );                
        //
        // read nodes and bathymetry
        //
        dt__infoNoClass(
          readBc2(), 
          << "[Boundarys] : Read " << nLines 
          << " lines and " << nColumns << " columns ..."
        );
        int jj = 0;
        std::vector< std::list< int > > shorelines(
          nShorelines, std::list< int >(0)
        );
        std::vector< std::list< std::string > > shoreLabels(
          nShorelines, std::list< std::string >(0)
        );
        dt__forAllRefAuto(readBlock(nLines, in), aStr) {
          shorelines[ stringPrimitive::stringToInt( aStr[13] ) - 1 ].push_back(
            stringPrimitive::stringToInt( aStr[11] ) - 1
          );
          if ( aStr.size() == 15 ) {
            shoreLabels[ stringPrimitive::stringToInt( aStr[13] ) - 1 ].push_back(
              aStr[14]
            );            
          }
          else {
            shoreLabels[ stringPrimitive::stringToInt( aStr[13] ) - 1 ].push_back(
              ""
            );                       
          }
          jj = jj + 1;
        }
        
        //
        // label boundarys
        //
        int emptyC = 0;
        dt__forAllIndex(shorelines, jj) {
          //
          // adjust shore
          //
          splitShore(shoreLabels[jj], shorelines[jj]);
          
          std::list< int >::iterator shorelines_it = shorelines[jj].begin();
          std::list< std::string >::iterator shoreLabels_it = shoreLabels[jj].begin();
          bool lastEmpty = false;
          std::string lastBoundary = "";
          std::string thisBoundary = "";
          emptyC = emptyC + 1;
          dt__forAllIndex(shoreLabels[jj], ii) {
            if ( shoreLabels_it->empty() ) {
              lastEmpty = true;
              thisBoundary = "empty_"+stringPrimitive::intToString(emptyC);
            }
            else {
              if ( lastEmpty ) {
                emptyC = emptyC + 1;
                lastEmpty = false;
              }
              thisBoundary = *shoreLabels_it;
            }
            boundary[ thisBoundary ].push_back( *shorelines_it );
            boundaryNumber[ thisBoundary ] = jj;
            lastBoundary = thisBoundary;            
            
            shorelines_it++;
            shoreLabels_it++;
          }
        }
      }      
    }    
  }
  in.close();  
  
  dt__infoNoClass(
    readBc2(), 
    << "boundary      > " << boundary.size() << std::endl
    << "segments      > " << segments.size() << std::endl
  );
  dt__forAllRefAuto(boundary, aBoundary) {
    dt__infoNoClass(
      readBc2(), 
      << " > " << aBoundary.first << " < " << std::endl
      << logMe::vecToString( progHelper::list2Vector(aBoundary.second) )
    );
  }
  dt__forAllRefAuto(boundaryNumber, aBoundaryNumber) {
    dt__infoNoClass(
      readBc2(), 
      << "boundaryNumber[ "
      << aBoundaryNumber.first << " ] = " << aBoundaryNumber.second
    );
  }  
  return;
}

void createModel(
  dtGmshModel * gm, 
  dtOMMesh & om,
  twoDArrayHandling< float > const & nodes, 
  twoDArrayHandling< int > const & elements,
  vectorHandling< float > const & bathymetry,
  std::map< std::string, int > const & boundaryNumber,
  std::map< std::string, std::list< int > > const & boundarys
) {
  //
  // create fields
  //
  om.enqueueField( 
    new dtOMVertexField< std::string >("boundaryNameVertexField", om, "")
  );
  om.enqueueField( 
    new dtOMEdgeField< std::string >("boundaryNameEdgeField", om, "")  
  );
  om.enqueueField( 
    new dtOMVertexField< dtGmshVertex * >("gVertexField", om, NULL)  
  );  
  om.enqueueField( 
    new dtOMEdgeField< dtGmshEdge * >("gEdgeField", om, NULL)  
  );    
  om.enqueueField(
    new dtOMVertexField< int >("vIndexField", om, -1)
  );
  om.enqueueField(
    new dtOMFaceField< int >("fIndexField", om, -1)
  );  
  om.enqueueField( 
    new dtOMVertexField< float >(
      "bathymetryField", om, std::numeric_limits<float>::max()
    )
  );  
  dtOMVertexField< std::string > & boundaryNameVertexField 
  =
  *dtOMVertexField< std::string >::MustDownCast( 
    om["boundaryNameVertexField"] 
  );  
  dtOMEdgeField< std::string > & boundaryNameEdgeField 
  =
  *dtOMEdgeField< std::string >::MustDownCast( 
    om["boundaryNameEdgeField"] 
  );    
  dtOMVertexField< int > & vIndexField 
  =
  *dtOMVertexField< int >::MustDownCast( 
    om["vIndexField"] 
  );    
  dtOMFaceField< int > & fIndexField 
  =
  *dtOMFaceField< int >::MustDownCast( 
    om["fIndexField"] 
  );    
  dtOMVertexField< dtGmshVertex * > & gVertexField 
  =
  *dtOMVertexField< dtGmshVertex * >::MustDownCast( 
    om["gVertexField"] 
  );     
  dtOMEdgeField< dtGmshEdge * > & gEdgeField
  =
  *dtOMEdgeField< dtGmshEdge * >::MustDownCast( 
    om["gEdgeField"] 
  );     
  dtOMVertexField< float > & bathymetryField 
  =
  *dtOMVertexField< float >::MustDownCast( 
    om["bathymetryField"] 
  );  
  
  //
  // gmsh
  //
  dtGmshFace * gf = new dtGmshFace(gm, 1);
  gm->add( gf );
  gm->tagPhysical( gf, "up" );
  
  std::vector< MVertex * > vv;
  std::vector< MTriangle * > tt;
  std::vector< MLine * > ll;

  //
  // create mesh vertices
  //
  dt__forAllRefAuto(nodes, aNode) {
    vv.push_back( new MVertex(aNode[0], aNode[1], aNode[2], NULL) );
  }

  //
  // mesh elements
  //    
  dt__forAllRefAuto(elements, aElement) {
    tt.push_back( 
      new MTriangle(vv[aElement[0]], vv[aElement[1]], vv[aElement[2]]) 
    );
    om.addFace( tt.back() );
  }

  //
  // update fields and mesh
  //
  om.update();    
  
  int cc = 0;
  dt__forAllRefAuto(elements, aElement) {
    vIndexField[ vv[aElement[0]] ] = aElement[0];
    vIndexField[ vv[aElement[1]] ] = aElement[1];
    vIndexField[ vv[aElement[2]] ] = aElement[2];
    fIndexField[ om.at(tt[cc]) ] = cc;
    bathymetryField[ vv[aElement[0]] ] = bathymetry[ aElement[0] ];
    bathymetryField[ vv[aElement[1]] ] = bathymetry[ aElement[1] ];
    bathymetryField[ vv[aElement[2]] ] = bathymetry[ aElement[2] ];    
    cc = cc + 1;
  }

  //
  // tag boundary vertices -> boundaryNameVertexField
  //    
  dt__forAllRefAuto(boundarys, aBoundary) {
    std::string name = aBoundary.first;
    std::list< int > const & nodeList = aBoundary.second;
    dt__forAllRefAuto(nodeList, anIndex) {
      boundaryNameVertexField[ vv[anIndex] ] = name;
    }
  }

  //
  // tag boundary edges
  //
  dt__forFromToIter( omEdgeI, om.edges_begin(), om.edges_end(), eIt ) {
    if ( ! om.is_boundary( *eIt ) ) continue;

    omVertexH v0 = om.from_vertex_handle( om.halfedge_handle( *eIt, 0 ) );
    omVertexH v1 = om.to_vertex_handle( om.halfedge_handle( *eIt, 0 ) );

    // tag edge same as first boundary vertex
    boundaryNameEdgeField[*eIt] = boundaryNameVertexField.at(v0);

    //
    // different boundary vertices at an edge
    // -> add to empty patch
    //
    if ( boundaryNameVertexField.at(v0) != boundaryNameVertexField.at(v1) ) {
      if ( stringPrimitive::stringContains( 
              "empty", boundaryNameVertexField.at(v1)
           )
         ) {
        // correct boundary name
        boundaryNameEdgeField[*eIt] = boundaryNameVertexField.at(v1);
      }
    }
    dt__infoNoClass( 
      createModel(), 
      << "[boundaryEdge]"
      << boundaryNameVertexField.at(v0) 
      << " -- " 
      << boundaryNameVertexField.at(v1)
      << " => " << boundaryNameEdgeField[*eIt]
    );          
  }

  //
  // get number of edgeLoops
  //
  int nEdgeLoops = 0;
  dt__forAllRefAuto(boundaryNumber, aNumber) {
    nEdgeLoops = std::max( aNumber.second + 1, nEdgeLoops );
  }
  dt__infoNoClass( createModel(), << "nEdgeLoops = " << nEdgeLoops );

  //
  // get one vertex at edge loop
  //
  std::vector< int > firstVertexOnEdgeLoop(nEdgeLoops, -1);
  dt__forAllRefAuto(boundaryNumber, aNumber) {
    if ( firstVertexOnEdgeLoop[ aNumber.second ] != -1 ) continue;
    firstVertexOnEdgeLoop[ aNumber.second ] 
    = 
    boundarys.at( aNumber.first ).front();
    dt__infoNoClass( 
      createModel(), 
      << "firstVertexOnEdgeLoop[ " << aNumber.second << " ] = "
      << firstVertexOnEdgeLoop[ aNumber.second ]
    );
  }

  //
  // detect unique edge loops 
  // -> split and insert dtGmshVertex
  // adjust firstVertexOnEdgeLoop for non unique edge loops
  //
  std::vector< bool > uniqueEdgeLoop(nEdgeLoops, true);
  std::vector< omHalfedgeH > startHalfedgeOfEdgeLoop(nEdgeLoops, omHalfedgeH());
  dt__forAllIndex( firstVertexOnEdgeLoop, ii ) {
    int aVertex( firstVertexOnEdgeLoop[ii] );
    omHalfedgeH he0;
    dt__forFromToIter( 
      omVertexEdgeI, 
      om.ve_begin( om.at( vv.at(aVertex) ) ), 
      om.ve_end( om.at( vv.at(aVertex) ) ), 
      eIt
    ) {
      if ( om.is_boundary(*eIt) ) {
        he0 = om.halfedge_handle(*eIt, 0);
        if ( om.is_boundary( he0 ) ) {
          break;
        }
        else {
          he0 = om.opposite_halfedge_handle( he0 );
          break;
        }
      }
    }    
    dt__throwIfNoClass( !he0.is_valid(), createModel() );
    dt__throwIfNoClass( !om.is_boundary(he0), createModel() );

    omHalfedgeH he = he0;
    std::string bName( boundaryNameEdgeField.at( om.edge_handle(he0) ) );
    do {
      he = om.next_halfedge_handle(he0);
      if ( bName != boundaryNameEdgeField.at( om.edge_handle(he) ) ) {
        firstVertexOnEdgeLoop[ii] 
        = 
        vIndexField.at( om.from_vertex_handle(he) );
        startHalfedgeOfEdgeLoop[ii] = he;
        dt__infoNoClass(
          createModel(), 
          << "[firstVertexOnEdgeLoop] Correct vertex vv[ " 
          << firstVertexOnEdgeLoop[ii] << " ] for edgeLoop = " << ii
        );          
        uniqueEdgeLoop[ii] = false;
        break;
      }
    }
    while (he == he0);
    dt__infoNoClass( 
      createModel(), 
      << "uniqueEdgeLoop[ " << ii << " ] = " << uniqueEdgeLoop[ ii ]
    );
    if ( uniqueEdgeLoop[ii] ) {
      if ( gVertexField.at(vv[aVertex]) == NULL ) {
        omVertexH splitV = om.at( vv[aVertex] );
        gVertexField[ splitV ] = new dtGmshVertex(gm, gm->getNumVertices()+1);
        gVertexField[splitV]->setPosition( 
          dtPoint3(om[ splitV ]->x(), om[ splitV ]->y(), om[ splitV ]->z())
        );
        gm->add( gVertexField.at(splitV) );
        gVertexField[ splitV ]->addMeshVertex( om[ splitV ] );
        om[ splitV ]->setEntity( gVertexField[ splitV ] );
        dt__infoNoClass(
          createModel(), 
          << "[dtGmshVertex] Create dtGmshVertex at unique boundary: tag = " 
          << gm->getNumVertices() << ", " << "boundaryName = " 
          << bName
        );
        if ( om.from_vertex_handle(he0) == splitV ) {
          startHalfedgeOfEdgeLoop[ii] = he0;
        }
        else {
          startHalfedgeOfEdgeLoop[ii] = om.opposite_halfedge_handle(he0);
        }
      }        
    }
  }    

  //
  // create dtGmshVertex at vertices that connect more than one different 
  // boundary names
  //
  dt__forAllIndex( startHalfedgeOfEdgeLoop, ii) {
    omHalfedgeH he0 = startHalfedgeOfEdgeLoop[ii];
    omHalfedgeH he = he0;
    do {

      //
      // vector contains more than one element
      // -> two edges start at this vertex
      // -> create dtGmshVertex
      //
      std::string bName( boundaryNameEdgeField.at( om.edge_handle(he) ) );
      std::string bName_prev( 
        boundaryNameEdgeField.at( 
          om.edge_handle( om.prev_halfedge_handle(he) ) 
        ) 
      );
      omVertexH v_from( om.from_vertex_handle(he) );
      omVertexH v_to( om.to_vertex_handle(he) );        
      //bool onDtGmshVertex = 
      //
      // half edge that starts at a dtGmshVertex or lies on two boundarys
      //
      if (
        (bName_prev != bName) || dtGmshVertex::Is(gVertexField[ v_from]) 
      ) {
        if ( gVertexField.at(v_from) == NULL ) {
          gVertexField[v_from] = new dtGmshVertex(gm, gm->getNumVertices()+1);
          gVertexField[v_from]->setPosition( 
            dtPoint3(om[ v_from ]->x(), om[ v_from ]->y(), om[ v_from ]->z())
          );
          gm->add( gVertexField.at(v_from) );
          gVertexField.at(v_from)->addMeshVertex( om[ v_from ] );
          om[ v_from ]->setEntity( gVertexField[v_from] );
          dt__infoNoClass(
            createModel(), 
            << "[dtGmshVertex] Create dtGmshVertex : tag = " 
            << gm->getNumVertices() << ", " << "< bName_prev, bName > = < " 
            << bName_prev << ", " << bName << " >"
          );
        }
        if ( gEdgeField.at( om.edge_handle(he) ) == NULL ) {
          gEdgeField[ om.edge_handle(he) ] 
          = 
          new dtGmshEdge( gm, gm->getNumEdges()+1 );
          gm->tagPhysical(
            gEdgeField[ om.edge_handle(he) ], bName
          );
          if (ii=0) {
            gf->addEdge(gEdgeField[ om.edge_handle(he) ], 1);
          }
          else {
            gf->addEmbeddedEdge(gEdgeField[ om.edge_handle(he) ]);
          }
          gm->add( gEdgeField[ om.edge_handle(he) ] );
          ll.push_back( new MLine( om[ v_from ], om[ v_to ] )  );
          gEdgeField[ om.edge_handle( he ) ]->addElement(ll.back());            
        }
      }
      //
      // halfedge that is on a dtGmshEdge
      //
      else {
        if ( gEdgeField.at( om.edge_handle(he) ) == NULL ) {
          gEdgeField[ om.edge_handle(he) ] 
          = 
          gEdgeField.at( om.edge_handle( om.prev_halfedge_handle(he) ) );
          ll.push_back( new MLine( om[ v_from ], om[ v_to ] ) );
          gEdgeField[ om.edge_handle( he ) ]->addElement( ll.back() );
        }
        if ( om[ v_from ]->onWhat() == NULL ) {
          om[ v_from ]->setEntity( gEdgeField.at( om.edge_handle( he ) ) );
          gEdgeField[ om.edge_handle( he ) ]->addMeshVertex( om[ v_from ] );
        }          
      }
      he = om.next_halfedge_handle(he);
    }
    while (he != he0);
  }

  dt__forAllRefAuto(gm->dtEdges(), anEdge) {
    anEdge->setBeginVertex( 
      dtGmshVertex::MustDownCast(
        anEdge->lines.front()->getVertex(0)->onWhat()
      ) 
    );
    anEdge->setEndVertex( 
      dtGmshVertex::MustDownCast(
        anEdge->lines.back()->getVertex(1)->onWhat() 
      )
    );
  }
  dt__forFromToIter( 
    omVertexI, om.vertices_begin(), om.vertices_end(), vIt
  ) {
    if ( om[ *vIt ]->onWhat() == NULL ) {
      gf->addMeshVertex( om[*vIt] );
      om[*vIt]->setEntity( gf );          
    }
  }
  dt__forFromToIter( omFaceI, om.faces_begin(), om.faces_end(), fIt ) {
    gf->addElement( om[*fIt] );
  }  
}

void makeModel3d( dtGmshModel * gm, float const & ss ) {
  //
  // create 2d twin
  //
  std::map< ::GEntity *, ::GEntity * > newOldG
  =
  gm->createTwin(
    gm->getNumVertices(), 
    gm->getNumEdges(), 
    gm->getNumFaces(), 
    gm->getNumRegions() 
  );

  //
  // copy mesh vertices
  //
  std::vector< ::MVertex * > vv_c;
  std::map< ::MVertex *, ::MVertex * > newOldM;
  dt__forFromToIndex(0, gm->getNumMeshVertices(), ii) {
    ::MVertex * mv = gm->getMeshVertexByTag(ii+1);
    vv_c.push_back(
      new MVertex(
        mv->x(), mv->y(), mv->z() + ss, newOldG[ mv->onWhat() ]
      ) 
    );
    newOldM[ mv ] = vv_c.back();
  }
  
  //
  // copy mesh elements and set mesh vertices to twins
  // -> create edges between normal and twin dtGmshVertex
  //
  std::vector< MLine * > ll_c;
  std::vector< MTriangle * > tt_c;
  std::vector< dtGmshEdge * > sizeEdges;
  // vertices
  dt__forAllRefAuto(gm->dtVertices(), gv) {
    gm->add( newOldG[gv] );
    newOldG[gv]->setModel( gm );
    dtGmshVertex::MustDownCast(
      newOldG[gv]
    )->setPosition( dtPoint3(gv->x(), gv->y(), gv->z()+ss) );
    dt__forAllRefAuto( gv->mesh_vertices, mv ) {
      newOldM[mv]->setEntity( newOldG[gv] );
      newOldG[gv]->addMeshVertex( newOldM[mv] );          
    }
    sizeEdges.push_back( 
      new dtGmshEdge(
        NULL, 
        -1, //gm->getNumVertices()+sizeEdges.size()+1, 
        dtGmshVertex::MustDownCast(gv), 
        dtGmshVertex::MustDownCast(newOldG[gv])
      ) 
    );
    //gm->add( sizeEdges.back() );
    sizeEdges.back()->addLine( 
      new MLine(gv->mesh_vertices[0], newOldM[ gv->mesh_vertices[0] ]) 
    );
  }
  // edges
  dt__forAllRefAuto(gm->dtEdges(), ge) {
    gm->add( newOldG[ge] );
    newOldG[ge]->setModel( gm );
    dt__forAllRefAuto( ge->mesh_vertices, mv ) {
      newOldM[mv]->setEntity( newOldG[ge] );
      newOldG[ge]->addMeshVertex( newOldM[mv] );          
    }
    dt__forAllRefAuto( ge->lines, me ) {
      ll_c.push_back( 
        new MLine(newOldM[me->getVertex(0)], newOldM[me->getVertex(1)]) 
      );
      dtGmshEdge::MustDownCast(newOldG[ge])->addLine( ll_c.back() );
    }        
  }      
  // faces
  dt__forAllRefAuto(gm->dtFaces(), gf) {
    gm->add( newOldG[gf] );
    newOldG[gf]->setModel( gm );
    dt__forAllRefAuto( gf->mesh_vertices, mv ) {
      newOldM[mv]->setEntity( newOldG[gf] );
      newOldG[gf]->addMeshVertex( newOldM[mv] );          
    }
    dt__forAllRefAuto( gf->triangles, mf ) {
      tt_c.push_back( 
        new MTriangle(
          newOldM[mf->getVertex(0)], 
          newOldM[mf->getVertex(1)],
          newOldM[mf->getVertex(2)]
        ) 
      );
      dtGmshFace::MustDownCast(newOldG[gf])->addTriangle( tt_c.back() );
    }                
  }
  
  //
  // add edges in size direction to model
  //
  dt__forAllRefAuto(sizeEdges, ge) {
    ge->setTag( gm->getNumEdges()+1);
    ge->setModel(gm);
    gm->add(ge);
  }
  
  //
  // create region
  //
  gm->add( new dtGmshRegion(gm, 1) );
  
  //
  // create three dimensional elements
  //
  dt__forAllRefAuto(gm->getDtGmshFaceByTag(1)->triangles, aTri) {
    gm->getDtGmshRegionByTag(1)->addPrism(
      new MPrism(
        aTri->getVertex(0), 
        aTri->getVertex(1), 
        aTri->getVertex(2),
        newOldM[aTri->getVertex(0)], 
        newOldM[aTri->getVertex(1)], 
        newOldM[aTri->getVertex(2)]
      )
    );
  }
  
  //
  // create face for each physical tagged edge
  //
  dt__forAllRefAuto(gm->dtEdges(), aEdge) {
    std::string pys = gm->getPhysicalString( aEdge );
    if (pys.empty()) continue;
    
    dt__infoNoClass(makeModel3d(), << "Create face for physical = " << pys);
    
    gm->untagPhysical(aEdge);
    dtGmshFace * gf = new dtGmshFace(gm, gm->getNumFaces()+1);
    gf->addEdge(aEdge, 1);
    gf->addEdge( dtGmshEdge::MustDownCast(newOldG[aEdge]), 1);
    gm->tagPhysical(gf, pys);
    gm->add( gf );
    dt__forAllRefAuto(aEdge->lines, aLine) {
      gf->addQuadrangle( 
        new MQuadrangle(
          aLine->getVertex(0), aLine->getVertex(1),
          newOldM[aLine->getVertex(1)], newOldM[aLine->getVertex(0)]
        ) 
      );
    }
  }
  
  //
  // tag down face
  //
  gm->tagPhysical( newOldG[ gm->getDtGmshFaceByPhysical("up") ], "down" );
  
  //
  // label all empty as noLabel
  //
  dt__forAllRefAuto( gm->dtFaces(), aFace) {
    if ( 
      stringPrimitive::stringContains("empty", aFace->getPhysicalString() ) 
    ) {
      gm->untagPhysical( aFace );
      gm->tagPhysical( aFace, "noLabel");
    }
  }
}

void initMeshVectors(
  dtGmshModel * gm,
  std::vector< ::MVertex * > & allVerts, 
  std::vector< std::pair< ::MElement *, int > > & allElems,
  std::map< int, std::string > & physicalNames
) {
  // model, dimension, physical number
  std::map< std::tuple< dtGmshModel *, int, int >, int > globLoc;
  int nVerts = 0;
  int nElems = 0;    

  //
  // calculate number of elements and vertices
  //    
  std::vector< ::GEntity * > entities;
  gm->getEntities(entities);

  dt__forAllRefAuto(entities, anEntity) {
    nVerts = nVerts + anEntity->getNumMeshVertices();
    nElems = nElems + anEntity->getNumMeshElements();
  }

  //
  // allocate
  //    
  allVerts.resize(nVerts);
  allElems.resize(nElems);

  // vertex and element counter
  int vC = 0;
  int eC = 0;

  dt__forAllRefAuto(entities, anEntity) {
    //
    // get all mesh vertex
    //
    dt__forFromToIndex(0, anEntity->getNumMeshVertices(), ii) {
      allVerts[ vC ] = anEntity->getMeshVertex(ii);
      vC++;
    }

    //
    // local and global physical number
    //
    int dimension = anEntity->dim();
    int locPhysInt 
    = 
    gm->getPhysicalNumber(
      dimension, gm->getPhysicalString(anEntity)
    );
    int globPhysInt = -1;                
    // check if  it is already in map
    std::map< 
      std::tuple< dtGmshModel *, int, int >, int 
    >::iterator thisPhysical
    =
    globLoc.find( 
      std::tuple< dtGmshModel *, int, int >(gm, dimension, locPhysInt) 
    );
    // not in map
    if ( thisPhysical == globLoc.end() ) {
      globPhysInt = globLoc.size()+1;
      globLoc[ 
        std::tuple< dtGmshModel *, int, int >(gm, dimension, locPhysInt) 
      ]
      =
      globPhysInt; 

      physicalNames[globPhysInt] = gm->getPhysicalString(anEntity);
    }
    // in map
    else globPhysInt = thisPhysical->second;

    //
    // get all mesh elements
    // add global physical label for patch creation
    //
    dt__forFromToIndex(0, anEntity->getNumMeshElements(), ii) {
      allElems[ eC ] 
      = 
      std::pair< ::MElement * , int >(
        anEntity->getMeshElement(ii), globPhysInt
      );
      eC++;
    }
  }    
  dt__infoNoClass( initMeshVectors(), << "physicalNames = " << physicalNames );
}
  
int main( int ac, char* av[] ) {
  try {
    //
    // options
    //
    dtArg vm("blueKenue2Foam", ac, av);
       
    //
    // additional arguments
    //
    vm.description().add_options()
      (
        "bc2,b", 
        dtPO::value< std::string >(), 
        "path to bc2 file"
      )    
      (
        "t3s,t", 
        dtPO::value< std::string >(), 
        "path to t3s file"
      )        
      (
        "size,s", 
        dtPO::value< float >(), 
        "size of the cells"
      )            
      (
        "case,c", 
        dtPO::value< std::string >(), 
        "path to foam case"
      )         
      (
        "gmsh,g", 
        dtPO::value< std::string >()->default_value(""), 
        "write file <gmsh>.msh"
      )             
      (
        "extend,x", 
        dtPO::value< bool >()->default_value(false), 
        "extended interpolation"
      )      
    ;
    vm.update();
    
    //
    // create log files
    //
    logMe::initLog(vm["log"].as<std::string>()  );
    dt__infoNoClass(
      main(), 
      << "Call command:" << std::endl
      << vm.callCommand()
    );
    
    //
    // elemts, nodes, bathymetry
    //
    twoDArrayHandling< float > nodes;
    vectorHandling< float > bathymetry;
    twoDArrayHandling< int > elements;
        
    //
    // read t3s -> nodes, bathymetry, elements
    //
    readT3s(vm["t3s"].as< std::string >(), nodes, bathymetry, elements);

    std::map< std::string, int > boundaryNumber;
    std::map< std::string, std::list< int > > boundarys;

    //
    // read bc2 -> edges, edgeNames
    //
    readBc2(vm["bc2"].as< std::string >(), boundaryNumber, boundarys);


    dtGmshModel * gm = new dtGmshModel("blueKenue");
    dtOMMesh om;

    //
    // create two dimensional gmsh model
    //    
    createModel(gm, om, nodes, elements, bathymetry, boundaryNumber, boundarys);
    
    //
    // create three dimensional model
    //
    makeModel3d( gm, vm["size"].as< float >() );

    //
    // write gmsh file
    //    
    if ( vm["gmsh"].as< std::string >() != "" ) {
      gm->writeMSH( vm["gmsh"].as< std::string >()+".msh", 2.2, false, true );
    }
    //
    // foam
    //

    std::vector< ::MVertex * > allVerts;
    std::vector< std::pair< ::MElement *, int > > allElems;
    std::map< int, std::string > physicalNames;        
    //
    // read vertices and elements
    //
    initMeshVectors(gm, allVerts, allElems, physicalNames);
    
    // enable exception throwing
    ::Foam::FatalError.throwExceptions();    
    ::Foam::FatalIOError.throwExceptions();    

    ::Foam::argList::noParallel();
    int argc = 3;
    std::vector< std::string > argvStr(3);
    argvStr[0] = "blueKenue2Foam";
    argvStr[1] = std::string("-case");
    argvStr[2] = vm["case"].as< std::string >();

    char ** argv = new char*[3];
    argv[0] = const_cast< char *>(argvStr[0].c_str());
    argv[1] = const_cast< char *>(argvStr[1].c_str());
    argv[2] = const_cast< char *>(argvStr[2].c_str());

    //
    // create OpenFOAM rootCase and time
    //
    try {
      // disable floating point exception trapping
      systemHandling::unsetEnv("FOAM_SIGFPE");

      //
      // create rootCase
      //
      ::Foam::argList args(argc, argv);
      if (!args.checkRootCase()) {
        Foam::FatalError.exit();
      }

      //
      // create time
      //
      dt__infoNoClass(main(), << "Create time");

      ::Foam::Time runTime(
          Foam::Time::controlDictName,
          args.rootPath(),
          args.caseName(),
          "system",
          "constant",
          !args.optionFound("noFunctionObjects")
      );

      //
      // set runTime to timestep constant
      // - write mesh to constant
      //
      runTime.setTime( ::Foam::instant(runTime.constant()), 0 );

      //
      // create mesh
      //
      dt__pH(::Foam::polyMesh) mesh( 
        dtFoamLibrary::readMesh(
          allVerts, allElems, physicalNames, runTime, 
          true
        ) 
      );

      //
      // write mesh
      //
      mesh->write();

      //
      // set runTime to timestep 0
      // - write fields to 0
      //
      runTime.setTime( ::Foam::instant("0"), 1 );            

      //
      // create fvMesh for fields
      //
      ::Foam::fvMesh fvMesh(*mesh);

      //
      // bathymetry field
      //
      ::Foam::volScalarField volScalarBathy(
        ::Foam::IOobject(
          "h0",
          runTime.timeName(),
          runTime,
          ::Foam::IOobject::NO_READ,
          ::Foam::IOobject::AUTO_WRITE
        ),
        fvMesh,
        ::Foam::dimensionedScalar(
          "h0", 
          ::Foam::dimensionSet(::Foam::IStringStream("[0 1 0 0 0 0 0]")()), 
          ::Foam::scalar(0.)
        )
      );

      //
      // get bathymetry
      //
      dtOMVertexField< float > * bathymetryField 
      = 
      dtOMVertexField<float>::MustDownCast( om["bathymetryField"] );
      
      int cc = 0;
      dt__forAllRefAuto(gm->getDtGmshFaceByPhysical("up")->triangles, aPri) {
        omFaceH const & fH = om.at(aPri);
        float intValue = 0.;
        
        if ( vm["extend"].as< bool >() ) {
          std::vector< omVertexH > vH_v;        
          dt__forFromToIter(
            omFaceVertexI, 
            om.fv_begin(fH), om.fv_end(fH), v_it
          ) {
            vH_v.push_back( *v_it);
          }          
          dt__forFromToIter(
            omFaceHalfedgeI, 
            om.fh_begin(fH), om.fh_end(fH), he_it) {
            //omFaceHalfedgeI) {
            omFaceH neighbourFH 
            = 
            om.face_handle(
              om.opposite_halfedge_handle( *he_it )
            );          
            if ( om.is_boundary(neighbourFH) ) continue;
            dt__forFromToIter(
              omFaceVertexI, 
              om.fv_begin(neighbourFH), om.fv_end(neighbourFH), v_it
            ) {
              vH_v.push_back( *v_it);
            }
          }
          progHelper::removeBastardTwins( vH_v );
          dt__forAllRefAuto(vH_v, aH) {
            intValue = intValue + bathymetryField->at(aH);
          }
          intValue = intValue / vH_v.size();
        }
        else {
          SPoint3 bUVW = aPri->barycenterUVW();
          std::vector< double > val(3, 0.);         
          dt__forFromToIndex(0, 3, ii) {
            val[ii] = bathymetryField->at( om.at(fH)->getVertex(ii) );
          }          
          intValue
          = 
          aPri->interpolate( &(val[0]), bUVW.x(), bUVW.y(), bUVW.z(), 1, 0 );
        }
        volScalarBathy.internalField()[cc] 
        = 
        intValue;//aPri->interpolate( &(val[0]), bUVW.x(), bUVW.y(), bUVW.z(), 1, 0 );

        cc = cc + 1; 
      }

      //
      // write all fields
      //
      volScalarBathy.write();

      dt__infoNoClass(main(), << "Done");
    }
    catch (::Foam::error & err) {
      dt__throwNoClass(
        main(), 
        << "Instance of ::Foam::error thrown." << std::endl
        << dt__eval(err.what()) << std::endl
        << dt__eval(err.message())
      );
    }      
    dt__infoNoClass(main(), << "Successfull termination." << std::endl);
  }
  
  
  //
  // catch dtOO exceptions
  //
  catch (eGeneral & eGenRef) {
    dt__catchNoClass(main(), eGenRef.what());
  }
  //
  // catch std exceptions
  //
  catch (std::exception& e) {
    dtOO::FILELog().Get(dtOO::logERROR) 
      << "std::exception caught: " << e.what() << std::endl
      << dtOO::logMe::Backtrace();
  }
	
	return 0;
}