#include "bVOEvilGodfather.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
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

namespace dtOO {  
  bVOEvilGodfather::bVOEvilGodfather() {
  }

  bVOEvilGodfather::~bVOEvilGodfather() {
    
  }
  
  void bVOEvilGodfather::bVOEvilGodfather::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
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
		_perHex = qtXmlBase::getAttributeFloatMuParse("perHex", element, cV, aF);		
    _perTet = qtXmlBase::getAttributeFloatMuParse("perTet", element, cV, aF);		
    _perturbTet 
    = 
    qtXmlBase::getAttributeFloatMuParse("perturbTet", element, cV, aF);	    
    _perturbHex 
    = 
    qtXmlBase::getAttributeFloatMuParse("perturbHex", element, cV, aF);	        
  }
  
  void bVOEvilGodfather::postUpdate( void ) {
    dt__onlyMaster {
		  dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
      
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
      std::vector< ::MTetrahedron * > newTetrahedra;
      std::vector< ::MVertex * > newVertices;
      std::vector< ::MTetrahedron * > oldTetrahedra;
      dt__forFromToIndex( 0, static_cast< int >( _perTet * tet.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        int toPerturb = rand.Integer( tet.size() );        
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
            "(0) MTetrahedron[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
          ) 
          % jinx->getNum() 
          % jinx->getVolume() % jinx->skewness()
          % jinx->rhoShapeMeasure()
          << std::endl;
        
        //
        // do perturbation
        //
        if (_perturbTet > 0.) {
          perturbElement(jinx, _perturbTet);
          
          logC() 
            << logMe::dtFormat(
              "(1) MTetrahedron[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
            ) 
            % jinx->getNum() 
            % jinx->getVolume() % jinx->skewness()
            % jinx->rhoShapeMeasure()
            << std::endl;  
        }
        else if (_perturbTet < 0.) {
          //
          // divide tetrahedron
          //
          std::vector< ::MTetrahedron * > nT;
          ::MVertex * nV;
          divideTetrahedron(jinx, nT, nV);
          //
          // store old and new tetrahedra and vertex
          //
          newVertices.push_back( nV );
          oldTetrahedra.push_back( jinx );
          dt__forAllIndex(nT, jj) newTetrahedra.push_back( nT[jj] );
          
          //
          // output
          //
          dt__forAllIndex(nT, jj) {
            logC() 
              << logMe::dtFormat(
                "(1a) MTetrahedron[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
              ) 
              % nT[jj]->getNum() 
              % nT[jj]->getVolume() % nT[jj]->skewness()
              % nT[jj]->rhoShapeMeasure()
              << std::endl; 
          }
        }     
      }
      
      //
      // hexahedra
      //
      std::vector< bool > perturbedHex(hex.size(), false);    
      std::vector< ::MPyramid * > newPyramids;
      std::vector< ::MHexahedron * > oldHexahedra;      
      dt__forFromToIndex( 0, static_cast< int >( _perHex * hex.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        int toPerturb = rand.Integer( hex.size() );
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
            "(0) MHexahedron[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
          ) 
          % jinx->getNum() 
          % jinx->getVolume() % jinx->skewness()
          % jinx->rhoShapeMeasure()
          << std::endl;
        
        //
        // do perturbation
        //
        if (_perturbHex > 0.) {
          perturbElement(jinx, _perturbHex);

          logC() 
            << logMe::dtFormat(
              "(1) MHexahedron[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
            ) 
            % jinx->getNum() 
            % jinx->getVolume() % jinx->skewness()
            % jinx->rhoShapeMeasure()
            << std::endl;              
        }
        else if (_perturbHex < 0.) {
          //
          // divide hexahedron
          //
          std::vector< ::MPyramid * > nP;
          ::MVertex * nV;
          divideHexahedron(jinx, nP, nV);
          //
          // store old and new tetrahedra and vertex
          //
          newVertices.push_back( nV );
          oldHexahedra.push_back( jinx );
          dt__forAllIndex(nP, jj) newPyramids.push_back( nP[jj] );
          
          //
          // output
          //
          dt__forAllIndex(nP, jj) {
            logC() 
              << logMe::dtFormat(
                "(1a) MPyramid[ %6i ]: V = %8.3e, skew = %8.3e rhoShapeMeasure = %8.3e"
              ) 
              % nP[jj]->getNum() 
              % nP[jj]->getVolume() % nP[jj]->skewness()
              % nP[jj]->rhoShapeMeasure()
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
      dt__forAllRefAuto(oldTetrahedra, aTet) {
        bool found = false;
        dt__forFromToIndex(0, aTet->getNumVertices(), ii) {
          dt__ptrAss(
            ::GRegion * gr, 
            dynamic_cast< ::GRegion * >(aTet->getVertex(ii)->onWhat())
          );
          std::vector< ::MTetrahedron * >::iterator tIt
          =
          std::find(gr->tetrahedra.begin(), gr->tetrahedra.end(), aTet);
  
          if ( tIt != gr->tetrahedra.end() ) {
            found = true;
            gr->tetrahedra.erase(tIt);
            break;
          }
        }
        dt__throwIf(!found, postUpdate());

        delete aTet;
      }
      //
      // delete old hexahedra
      //      
      dt__info(postUpdate(), << "Remove old hexahedra.");
      dt__forAllRefAuto(oldHexahedra, aHex) {
        bool found = false;
        dt__forFromToIndex(0, aHex->getNumVertices(), ii) {
          dt__ptrAss(
            ::GRegion * gr, 
            dynamic_cast< ::GRegion * >(aHex->getVertex(ii)->onWhat())
          );
          std::vector< ::MHexahedron * >::iterator hIt
          =
          std::find(gr->hexahedra.begin(), gr->hexahedra.end(), aHex);
  
          if ( hIt != gr->hexahedra.end() ) {
            found = true;
            gr->hexahedra.erase(hIt);
            break;
          }
        }
        dt__throwIf(!found, postUpdate());

        delete aHex;
      }
      //
      // add new tetrahedra
      //
      dt__info(postUpdate(), << "Add new tetrahedra.");
      dt__forAllRefAuto(newTetrahedra, aTet) {
        dt__ptrAss( 
          ::GRegion * gr, 
          dynamic_cast< ::GRegion * >(aTet->getVertex(0)->onWhat())
        );
        
        gr->tetrahedra.push_back(aTet);
      }      
      //
      // add new pyramids
      //
      dt__info(postUpdate(), << "Add new pyramids.");
      dt__forAllRefAuto(newPyramids, aPyr) {
        dt__ptrAss( 
          ::GRegion * gr, 
          dynamic_cast< ::GRegion * >(aPyr->getVertex(0)->onWhat())
        );
        
        gr->pyramids.push_back(aPyr);
      }
      
      gm->destroyMeshCaches();
      
    }
  }
  
  void bVOEvilGodfather::perturbElement(
    ::MElement * const jinx, float const & perturbFactor
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
    std::vector< ::MTetrahedron * > & newTets,
    ::MVertex * & newVertex
  ) {
    ::SPoint3 baryUVW = jinx->barycenterUVW();
    ::SPoint3 baryXYZ;
    jinx->pnt(baryUVW.x(), baryUVW.y(), baryUVW.z(), baryXYZ);
    
    newVertex
    =
    new ::MVertex(
      baryXYZ.x(), baryXYZ.y(), baryXYZ.z(), jinx->getVertex(0)->onWhat()
    );
    
    dt__forFromToIndex(0, jinx->getNumFaces(), ii) {
      std::vector< ::MVertex * > verts;
      jinx->getFaceVertices(ii, verts);
      newTets.push_back(
        new ::MTetrahedron( verts[0], verts[1], verts[2], newVertex )
      );
    }
    
    if (jinx->getVolume() > 0.) {
      dt__forFromToIndex(0, newTets.size(), ii) {
        if (newTets[ii]->getVolume() < 0.) newTets[ii]->reverse();
      }
    }
    else {
      dt__forFromToIndex(0, newTets.size(), ii) {
        if (newTets[ii]->getVolume() > 0.) newTets[ii]->reverse();
      }
    }             
  }  
    
  void bVOEvilGodfather::divideHexahedron(
    ::MHexahedron * const jinx,
    std::vector< ::MPyramid * > & newPyrs,
    ::MVertex * & newVertex
  ) {
    ::SPoint3 baryUVW = jinx->barycenterUVW();
    ::SPoint3 baryXYZ;
    jinx->pnt(baryUVW.x(), baryUVW.y(), baryUVW.z(), baryXYZ);
    
    newVertex
    =
    new ::MVertex(
      baryXYZ.x(), baryXYZ.y(), baryXYZ.z(), jinx->getVertex(0)->onWhat()
    );
    
    dt__forFromToIndex(0, jinx->getNumFaces(), ii) {
      std::vector< ::MVertex * > verts;
      jinx->getFaceVertices(ii, verts);
      newPyrs.push_back(
        new ::MPyramid( verts[0], verts[1], verts[2], verts[3], newVertex )
      );
    }
    
    if (jinx->getVolume() > 0.) {
      dt__forFromToIndex(0, newPyrs.size(), ii) {
        if (newPyrs[ii]->getVolume() < 0.) newPyrs[ii]->reverse();
      }
    }
    else {
      dt__forFromToIndex(0, newPyrs.size(), ii) {
        if (newPyrs[ii]->getVolume() > 0.) newPyrs[ii]->reverse();
      }
    }           
  }
}
