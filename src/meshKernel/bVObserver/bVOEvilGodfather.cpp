#include "bVOEvilGodfather.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>

#include <TDatime.h>
#include <TRandom3.h>

#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <gmsh/MVertex.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MPrism.h>
#include <gmsh/MElementOctree.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOEvilGodfather::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOEvilGodfather, new bVOEvilGodfather())
  );
  
  bVOEvilGodfather::bVOEvilGodfather() {
  }

  bVOEvilGodfather::~bVOEvilGodfather() {
    
  }
  
  void bVOEvilGodfather::bVOEvilGodfather::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {		
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver name="bVOEvilGodfather" 
//		  perTet="0.10"
//      perturbTet="0.5"
//		  perHex="0.10"
//		/>
		_perHex 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("perHex", element, cV, aF);		
    _perTet
    = 
    dtXmlParserBase::getAttributeFloatMuParse("perTet", element, cV, aF);		
    _perturbTet 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("perturbTet", element, cV, aF);	    
    _perturbHex 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("perturbHex", element, cV, aF);	        
  }
  
  void bVOEvilGodfather::postUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
      
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dt__onlyMaster {
      //
      // collect all elements
      //
      std::vector< ::MHexahedron * > hex;
      std::vector< ::MTetrahedron * > tet;
      std::vector< ::MPyramid * > pyr;
      std::vector< ::MPrism * > pri;
      std::vector< ::MElement * > me;
      dt__forAllRefAuto( gm->regions(), aReg ) {
        dt__forAllRefAuto( aReg->hexahedra, aHex ) {
          hex.push_back( aHex );
          me.push_back( aHex );
        }
        dt__forAllRefAuto( aReg->tetrahedra, aTet ) {
          tet.push_back( aTet );
          me.push_back( aTet );
        }
        dt__forAllRefAuto( aReg->pyramids, aPyr ) {
          pyr.push_back( aPyr );
          me.push_back( aPyr );
        }
        dt__forAllRefAuto( aReg->prisms, aPri ) {
          pri.push_back( aPri );  
          me.push_back( aPri );          
        }
      }
      
      dt__info(
        postUpdate(), 
        << "MHexahedron: " << hex.size() << std::endl
        << "MTetrahedron: " << tet.size() << std::endl
        << "MPyramid: " << pyr.size() << std::endl
        << "MPrism: " << pri.size() << std::endl
        << "MElement: " << me.size()
      );

      //
      // create octree
      //
      ::MElementOctree oct(me);
    
      //
      // random generator
      //
      ::TRandom3 rand( ::TDatime().Get() );
      
      //
      // tetrahedra
      //
      std::vector< bool > perturbedTet(tet.size(), false);
      std::vector< 
        std::pair< std::vector< ::MTetrahedron * >, ::GRegion * > 
      > newTetrahedra;
      std::vector< ::MVertex * > newVertices;
      std::vector< std::pair< ::MTetrahedron *, ::GRegion * > > oldTetrahedra;
      dt__forFromToIndex( 0, static_cast< dtInt >( _perTet * tet.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        dtInt toPerturb = rand.Integer( tet.size() );        
        //
        // check if already perturbed
        //
        if ( perturbedTet[ toPerturb ] ) {
          logC() << "Jinx already perturbed." << std::endl; 
          continue;
        }
        
        perturbedTet[ toPerturb ] = true;
        
        ::MTetrahedron * jinx = tet[ toPerturb ];
        
        std::vector< ::MVertex * > jinxNode;
        jinx->getVertices( jinxNode );
        
        bool isFree = true;
        dt__forAllRefAuto(jinxNode, aNode) {
          if ( aNode->onWhat()->dim() != 3 ) {
            isFree = false;
            logC() 
              << "Jinx not free, because node is not in region." 
              << std::endl;            
            isFree = false;
          }
          if (!isFree) break;          
          std::vector< ::MElement * > meVec 
          =           
          oct.findAll( aNode->x(), aNode->y(), aNode->z(), -1 );
          dt__forAllRefAuto(meVec, anMe) {
            if (
              dynamic_cast< ::MPrism * >(anMe)
              ||
              dynamic_cast< ::MPyramid * >(anMe)
            ) {
            logC() 
              << "Jinx not free, because pyramid or prism is connected." 
              << std::endl;                          
              isFree = false;
              break;
            }
          }
          if (!isFree) break;
        }
        
        //
        // do not change not free elements
        //
        if (!isFree) continue;
        
        logC() 
          << logMe::dtFormat(
            "(0) MTetrahedron[ %6i ]: V = %8.3e, "
            "skew = %8.3e angleShapeMeasure = %8.3e"
          ) 
          % jinx->getNum() 
          % jinx->getVolume() % jinx->skewness()
          % jinx->angleShapeMeasure()
          << std::endl;
        
        //
        // do perturbation
        //
        if (_perturbTet > 0.) {
          perturbElement(jinx, _perturbTet);
          
          logC() 
            << logMe::dtFormat(
              "(1) MTetrahedron[ %6i ]: V = %8.3e, "
              "skew = %8.3e angleShapeMeasure = %8.3e"
            ) 
            % jinx->getNum() 
            % jinx->getVolume() % jinx->skewness()
            % jinx->angleShapeMeasure()
            << std::endl;  
        }
        else if (_perturbTet < 0.) {
          //
          // divide tetrahedron
          //
          std::pair< std::vector< ::MTetrahedron * >, ::GRegion * > nT;
          ::MVertex * nV;
          divideTetrahedron(jinx, nT, nV);
          //
          // store old and new tetrahedra and vertex
          //
          newVertices.push_back( nV );
          oldTetrahedra.push_back( 
            std::pair< ::MTetrahedron *, ::GRegion * >(jinx, nT.second )
          );
          //dt__forAllIndex(nT, jj) newTetrahedra.push_back( nT[jj] );
          newTetrahedra.push_back( nT );
          
          //
          // output
          //
          dt__forAllRefAuto(nT.first, aTet) {
            logC() 
              << logMe::dtFormat(
                "(1a) MTetrahedron[ %6i ]: V = %8.3e, "
                "skew = %8.3e angleShapeMeasure = %8.3e"
              ) 
              % aTet->getNum() 
              % aTet->getVolume() % aTet->skewness()
              % aTet->angleShapeMeasure()
              << std::endl; 
          }
        }     
      }
      
      //
      // hexahedra
      //
      std::vector< bool > perturbedHex(hex.size(), false);    
      std::vector< 
        std::pair< std::vector< ::MPyramid * >, ::GRegion * > 
      > newPyramids;
      std::vector< std::pair< ::MHexahedron *, ::GRegion * > > oldHexahedra;      
      dt__forFromToIndex( 0, static_cast< dtInt >( _perHex * hex.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        dtInt toPerturb = rand.Integer( hex.size() );
        //
        // check if already perturbed
        //
        if ( perturbedHex[ toPerturb ] ) {
          logC() << "Jinx already perturbed." << std::endl; 
          continue;
        }
        
        perturbedHex[ toPerturb ] = true;        
        
        ::MHexahedron * jinx = hex[ toPerturb ];
        
        std::vector< ::MVertex * > jinxNode;
        jinx->getVertices( jinxNode );
        
        bool isFree = true;
        dt__forAllRefAuto(jinxNode, aNode) {
          if ( aNode->onWhat()->dim() != 3 ) {
            isFree = false;
            logC() 
              << "Jinx not free, because node is not in region." 
              << std::endl;            
            isFree = false;
          }
          if (!isFree) break;          
          std::vector< ::MElement * > meVec 
          =           
          oct.findAll( aNode->x(), aNode->y(), aNode->z(), -1 );
          dt__forAllRefAuto(meVec, anMe) {
            if (
              dynamic_cast< ::MPrism * >(anMe)
              ||
              dynamic_cast< ::MPyramid * >(anMe)
            ) {
            logC() 
              << "Jinx not free, because pyramid or prism is connected." 
              << std::endl;                          
              isFree = false;
              break;
            }
          }
          if (!isFree) break;
        }
        
        //
        // do not change not free elements
        //
        if (!isFree) continue;
        
        logC() 
          << logMe::dtFormat(
            "(0) MHexahedron[ %6i ]: V = %8.3e, "
            "skew = %8.3e angleShapeMeasure = %8.3e"
          ) 
          % jinx->getNum() 
          % jinx->getVolume() % jinx->skewness()
          % jinx->angleShapeMeasure()
          << std::endl;
        
        //
        // do perturbation
        //
        if (_perturbHex > 0.) {
          perturbElement(jinx, _perturbHex);

          logC() 
            << logMe::dtFormat(
              "(1) MHexahedron[ %6i ]: V = %8.3e, "
              "skew = %8.3e angleShapeMeasure = %8.3e"
            ) 
            % jinx->getNum() 
            % jinx->getVolume() % jinx->skewness()
            % jinx->angleShapeMeasure()
            << std::endl;              
        }
        else if (_perturbHex < 0.) {
          //
          // divide hexahedron
          //
          std::pair< std::vector< ::MPyramid * >, ::GRegion * > nP;
          ::MVertex * nV;
          divideHexahedron(jinx, nP, nV);
          //
          // store old and new tetrahedra and vertex
          //
          newVertices.push_back( nV );
          oldHexahedra.push_back( 
            std::pair< ::MHexahedron *, ::GRegion * >(jinx, nP.second )          
          );
          newPyramids.push_back( nP );
          
          //
          // output
          //
          dt__forAllRefAuto(nP.first, aPyr) {
            logC() 
              << logMe::dtFormat(
                "(1a) MPyramid[ %6i ]: V = %8.3e, "
                "skew = %8.3e angleShapeMeasure = %8.3e"
              ) 
              % aPyr->getNum() 
              % aPyr->getVolume() % aPyr->skewness()
              % aPyr->angleShapeMeasure()
              << std::endl; 
          }
        }
      }
      
      //
      // add new vertices
      //
      dt__info(postUpdate(), << "Add new vertices.");
      dt__forAllRefAuto(newVertices, aVert) {
        dt__ptrAss( 
          ::GRegion * gr, dynamic_cast< ::GRegion * >(aVert->onWhat())
        );

        gr->mesh_vertices.push_back( aVert );
      }
      //
      // delete old tetrahedra
      //
      dt__info(postUpdate(), << "Remove old tetrahedra.");
      dt__forAllRefAuto(oldTetrahedra, aTetPair) {
        progHelper::removeChildren( 
          aTetPair.second->tetrahedra, aTetPair.first 
        );
        delete aTetPair.first;
      }
      //
      // delete old hexahedra
      //      
      dt__info(postUpdate(), << "Remove old hexahedra.");
      dt__forAllRefAuto(oldHexahedra, aHexPair) {
        progHelper::removeChildren( 
          aHexPair.second->hexahedra, aHexPair.first 
        );
        delete aHexPair.first;
      }
      //
      // add new tetrahedra
      //
      dt__info(postUpdate(), << "Add new tetrahedra.");
      dt__forAllRefAuto(newTetrahedra, aTetPair) {
        dt__forAllRefAuto( aTetPair.first, aTet) {
          aTetPair.second->tetrahedra.push_back(aTet);
        }
      }      
      //
      // add new pyramids
      //
      dt__info(postUpdate(), << "Add new pyramids.");
      dt__forAllRefAuto(newPyramids, aPyrPair) {
        dt__forAllRefAuto( aPyrPair.first, aPyr) {
          aPyrPair.second->pyramids.push_back(aPyr);
        }
      }    

      //
      // force rebuilding mesh caches
      //
      gm->destroyMeshCaches();
    }
    else gm->destroy();
    
    staticPropertiesHandler::mpiBarrier();
  }
  
  void bVOEvilGodfather::perturbElement(
    ::MElement * const jinx, dtReal const & perturbFactor
  ) {
    std::vector< dtVector3 > perturbVector;
    dt__forFromToIndex(0, jinx->getNumEdges(), jj) {
      std::vector< ::MVertex * > jinxEdge;
      jinx->getEdgeVertices(jj, jinxEdge);
      perturbVector.push_back(
        dtVector3(
          jinxEdge[1]->x() - jinxEdge[0]->x(),
          jinxEdge[1]->y() - jinxEdge[0]->y(),
          jinxEdge[1]->z() - jinxEdge[0]->z()
        )
      );          
    }
    dt__forFromToIndex(0, jinx->getNumEdges(), jj) {
      std::vector< ::MVertex * > jinxEdge;
      jinx->getEdgeVertices(jj, jinxEdge);          
      jinxEdge[0]->setXYZ(
        jinxEdge[0]->x() + perturbFactor * perturbVector[jj].x(),
        jinxEdge[0]->y() + perturbFactor * perturbVector[jj].y(),
        jinxEdge[0]->z() + perturbFactor * perturbVector[jj].z()
      );
      jinxEdge[1]->setXYZ(
        jinxEdge[1]->x() - perturbFactor * perturbVector[jj].x(),
        jinxEdge[1]->y() - perturbFactor * perturbVector[jj].y(),
        jinxEdge[1]->z() - perturbFactor * perturbVector[jj].z()
      );            
    }            
  }

  void bVOEvilGodfather::divideTetrahedron(
    ::MTetrahedron * const jinx, 
    std::pair< std::vector< ::MTetrahedron * >, ::GRegion * > & newTets,
    ::MVertex * & newVertex
  ) {
    //
    // determine GRegion
    //
    bool found = false;
    dt__forFromToIndex(0, jinx->getNumVertices(), ii) {
      ::GRegion * gr
      = 
      dynamic_cast< ::GRegion * >(jinx->getVertex(ii)->onWhat());
      
      if (!gr) continue;
      
      std::vector< ::MTetrahedron * >::iterator hIt
      =
      std::find(gr->tetrahedra.begin(), gr->tetrahedra.end(), jinx);

      if ( hIt != gr->tetrahedra.end() ) {
        found = true;
        newTets.second = gr;
        break;
      }
    }
    dt__throwIf(!found, divideTetrahedron());
    
    ::SPoint3 baryUVW = jinx->barycenterUVW();
    ::SPoint3 baryXYZ;
    jinx->pnt(baryUVW.x(), baryUVW.y(), baryUVW.z(), baryXYZ);
    
    newVertex
    =
    new ::MVertex(
      baryXYZ.x(), baryXYZ.y(), baryXYZ.z(), newTets.second
    );
    
    dt__forFromToIndex(0, jinx->getNumFaces(), ii) {
      std::vector< ::MVertex * > verts;
      jinx->getFaceVertices(ii, verts);
      newTets.first.push_back(
        new ::MTetrahedron( verts[0], verts[1], verts[2], newVertex )
      );
    }
    
    if (jinx->getVolume() > 0.) {
      dt__forFromToIndex(0, newTets.first.size(), ii) {
        if (newTets.first[ii]->getVolume() < 0.) newTets.first[ii]->reverse();
      }
    }
    else {
      dt__forFromToIndex(0, newTets.first.size(), ii) {
        if (newTets.first[ii]->getVolume() > 0.) newTets.first[ii]->reverse();
      }
    }             
  }  
    
  void bVOEvilGodfather::divideHexahedron(
    ::MHexahedron * const jinx,
    std::pair< std::vector< ::MPyramid * >, ::GRegion * > & newPyrs,
    ::MVertex * & newVertex
  ) {
    //
    // determine GRegion
    //
    bool found = false;
    dt__forFromToIndex(0, jinx->getNumVertices(), ii) {
      ::GRegion * gr
      =
      dynamic_cast< ::GRegion * >(jinx->getVertex(ii)->onWhat());
      
      if (!gr) continue;
      
      std::vector< ::MHexahedron * >::iterator hIt
      =
      std::find(gr->hexahedra.begin(), gr->hexahedra.end(), jinx);

      if ( hIt != gr->hexahedra.end() ) {
        found = true;
        newPyrs.second = gr;
        break;
      }
    }
    dt__throwIf(!found, divideHexahedron());
        
    //
    //
    //
    ::SPoint3 baryUVW = jinx->barycenterUVW();
    ::SPoint3 baryXYZ;
    jinx->pnt(baryUVW.x(), baryUVW.y(), baryUVW.z(), baryXYZ);
    
    newVertex
    =
    new ::MVertex(
      baryXYZ.x(), baryXYZ.y(), baryXYZ.z(), newPyrs.second
    );
    
    dt__forFromToIndex(0, jinx->getNumFaces(), ii) {
      std::vector< ::MVertex * > verts;
      jinx->getFaceVertices(ii, verts);
      newPyrs.first.push_back(
        new ::MPyramid( verts[0], verts[1], verts[2], verts[3], newVertex )
      );
    }
    
    if (jinx->getVolume() > 0.) {
      dt__forFromToIndex(0, newPyrs.first.size(), ii) {
        if (newPyrs.first[ii]->getVolume() < 0.) newPyrs.first[ii]->reverse();
      }
    }
    else {
      dt__forFromToIndex(0, newPyrs.first.size(), ii) {
        if (newPyrs.first[ii]->getVolume() > 0.) newPyrs.first[ii]->reverse();
      }
    }           
  }
}
