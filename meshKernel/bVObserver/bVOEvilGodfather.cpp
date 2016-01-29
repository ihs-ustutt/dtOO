#include "bVOEvilGodfather.h"

#include <logMe/logMe.h>
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
    dt__throwIf( !ptrBoundedVolume()->isMeshed(), postUpdate() );
    
    
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
      dt__forFromToIndex( 0, static_cast< int >( _perTet * tet.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        ::MTetrahedron * jinx = tet[ rand.Integer( tet.size() ) ];
        
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
      
      //
      // hexahedra
      //
      dt__forFromToIndex( 0, static_cast< int >( _perHex * hex.size() ), ii ) {
        logContainer< bVOEvilGodfather > logC( logDEBUG, "postUpdate()" );
        
        ::MHexahedron * jinx = hex[ rand.Integer( hex.size() ) ];
        
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
}