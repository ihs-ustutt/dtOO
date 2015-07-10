#include "bladeLayerGmshVolume.h"

#include <gmsh/Context.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/GModel.h>
#include <gmsh/GEdge.h>
#include <dtGmshVertex.h>
#include <dtGmshEdge.h>
#include <dtGmshFace.h>
#include <dtGmshRegion.h>

#include <logMe/logMe.h>
#include <progHelper.h>
#include <gmsh/OpenFile.h>
#include "blockGridModel.h"
#include "cardinalDirGModel.h"
#include "analyticGeometryHeaven/analyticSurface.h"
#include "debugHeaven/extInfoContainer.h"
#include "simpleBlockStructure.h"
#include <debugHeaven/extInfoContainer.h>
#include <list>

namespace dtOO {
  DTCLASSLOGMETHODIN(bladeLayerGmshVolume);   
  
  bladeLayerGmshVolume::bladeLayerGmshVolume() {
    _strucModel = NULL;
    _eIC = new extInfoContainer;
  }

  bladeLayerGmshVolume::~bladeLayerGmshVolume() {
    delete _eIC;
  }
  
  void bladeLayerGmshVolume::init(
    ::QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< scaFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP
  ) {
    //
    // init cardinalDirVolume
    //
    bladeLayerVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);    
  }
  
  void bladeLayerGmshVolume::writeGrid( void ) {
    bladeLayerVolume::pushGridToSimpleBlockStructure();
    
    _eIC->clear();
    
    GmshInitialize();

    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }
    
    //
    // create model and attach a boundedVolume
    //
    _strucModel = new blockGridModel("bladeLayerStrucGmshVolume");
    ::GModel::setCurrent( _strucModel );
    _strucModel->attachToBoundedVolume( this );
    
    //
    // add vertices
    //
    for (int ii=0;ii<getNVertices();ii++) {
      _strucModel->addDtGmshVertex(ii+1);
      blockNumbering bN = getVertexSnapSurfId(ii);
      _strucModel->snapVertexPercent(ii+1, bN[0], getVertexUV(ii));
      if (getVertexSnapSurfId(ii).size() > 1) {
        for(int jj=1; jj<bN.size(); jj++) {
          _strucModel->snapVertex(ii+1, bN[jj]);
        }
      }
    }

    //
    // snap edges to surfaces
    //
    for(int ii=0; ii<getNEdges();ii++) {
      _strucModel->addDtGmshEdge(ii+1, getFromByEdgeIndex(ii)+1, getToByEdgeIndex(ii)+1 );
      bool snapped = _strucModel->snapEdge(ii+1);
      if (!snapped) {
        bool forceSnapped = _strucModel->forceSnapEdge(ii+1);
        if (!forceSnapped) {
          dt__THROW(writeGrid(),
                  << "Cannot snap edge " << dt__eval(ii+1) );
        }
      }
      _strucModel->getEdgeByTag(ii+1)->meshAttributes.method = MESH_TRANSFINITE;
      _strucModel->getEdgeByTag(ii+1)->meshAttributes.typeTransfinite = 1;
//      _strucModel->getEdgeByTag(ii+1)->meshAttributes.coeffTransfinite = 1.;
//      _strucModel->getEdgeByTag(ii+1)->meshAttributes.nbPointsTransfinite = 10;       
    }

    //
    // add faces
    //
    for(int ii=0; ii<getNFaces();ii++) {
      blockNumbering const & eI = getFaceNumbering(ii);
      _strucModel->addDtGmshFace(
        ii+1, 
        getEdgeFromToIndex(eI[0], eI[1])+1,
        getEdgeFromToIndex(eI[0], eI[3])+1,
        getEdgeFromToIndex(eI[3], eI[2])+1,
        getEdgeFromToIndex(eI[1], eI[2])+1
      );     
      _strucModel->getFaceByTag(ii+1)->meshAttributes.method = MESH_TRANSFINITE;
//      _strucModel->getFaceByTag(ii+1)->meshAttributes.recombine = 1;
    }

    std::vector< int > ori;
    std::list< ::GFace * > faces;    
    //
    // add regions
    //
    for(int ii=0; ii<getNBlocks();ii++) {
      ori.clear();
      faces.clear();
      blockNumbering const & fI = getBlockFaceNumbering(ii);
      blockNumbering const & eI = getBlockEdgeNumbering(ii);
      blockNumberingFloat const & bGrading = getBlockGrading(ii);
      blockNumbering const & bNElem = getBlockNElements(ii);

      //
      // grading
      //
      if (bGrading.size() == 8) {
        dt__FORALL(bGrading, jj,
          ::GEdge * ge = _strucModel->getEdgeByTag( eI[jj]+1 );
          ge->meshAttributes.coeffTransfinite = bGrading[jj];
        );
      }
      else {
        ::GEdge * ge;
        ge = _strucModel->getEdgeByTag(eI[0]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[0];
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[1]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[0];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[2]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[0];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[3]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[0];                
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[4]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[1];
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[5]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[1];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[6]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[1];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[7]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[1];                        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[8]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[2];
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[9]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[2];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[10]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[2];        
        ge->meshAttributes.typeTransfinite = 2;
        ge = _strucModel->getEdgeByTag(eI[11]+1);
        ge->meshAttributes.coeffTransfinite = bGrading[2];        
        ge->meshAttributes.typeTransfinite = 2;
      }
      
      //
      // number of elements
      //
      ::GEdge * ge;
      ge = _strucModel->getEdgeByTag(eI[0]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[0];
      ge = _strucModel->getEdgeByTag(eI[1]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[0];        
      ge = _strucModel->getEdgeByTag(eI[2]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[0];        
      ge = _strucModel->getEdgeByTag(eI[3]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[0];                
      ge = _strucModel->getEdgeByTag(eI[4]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[1];
      ge = _strucModel->getEdgeByTag(eI[5]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[1];        
      ge = _strucModel->getEdgeByTag(eI[6]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[1];        
      ge = _strucModel->getEdgeByTag(eI[7]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[1];                        
      ge = _strucModel->getEdgeByTag(eI[8]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[2];
      ge = _strucModel->getEdgeByTag(eI[9]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[2];        
      ge = _strucModel->getEdgeByTag(eI[10]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[2];        
      ge = _strucModel->getEdgeByTag(eI[11]+1);
      ge->meshAttributes.nbPointsTransfinite = bNElem[2];           
      
      dt__FORALL(fI, jj,
        faces.push_back( _strucModel->getFaceByTag( fI[jj]+1 ) );
        ori.push_back( 1 );
      );
      _strucModel->add( new dtGmshRegion(_strucModel, ii+1, faces, ori) );
      _strucModel->getRegionByTag(ii+1)->meshAttributes.method = MESH_TRANSFINITE;
    }
    
    //
    // unstructured part
    //
    // create unstructured volume
    cardinalDirVolumeDefines cDDef(100);
    _strucModel->convertOuterToGmshModel( &cDDef );

    ori.clear();
    faces.clear();
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtNorth"))) );
    ori.push_back(1);
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtWest"))) );
    ori.push_back(1);
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtSouth"))) );
    ori.push_back(1);
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtEast"))) );
    ori.push_back(1);
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtFront"))) );
    ori.push_back(1);
    faces.push_back( _strucModel->getFaceByTag(cDDef.transId(fStrToId("dtBack"))) );
    ori.push_back(1);
    
    std::list< ::GEdge * > northEmb;
    std::list< ::GEdge * > southEmb;
    for (int ii=0;ii<getNCouplingPatches();ii++) {
      std::vector< blockNumbering > cPatches = getPatch( getCouplingPatchName(ii) );
      
      for (int jj=0;jj<cPatches.size();jj++) {
        int fId = getFaceIndex( cPatches[jj] );
        ::GFace * gf = _strucModel->getFaceByTag(fId+1);
        faces.push_back(gf);//_unstrucModel->add( gf->clone(_unstrucModel)->cast2Face() );
        ori.push_back(-1);
        
        std::list< ::GEdge * > eList = gf->edges();
        for (std::list<::GEdge *>::const_iterator it = eList.begin();it != eList.end(); ++it) {
          ::GEdge * ge = (*it);
          dtGmshVertex * dtGV0 = _strucModel->cast2DtGmshVertex( ge->getBeginVertex() );
          dtGmshVertex * dtGV1 = _strucModel->cast2DtGmshVertex( ge->getEndVertex() );
          
          if ( dtGV0->snappedTo(getPtrToSurf(fStrToId("dtNorth"))) && dtGV1->snappedTo(getPtrToSurf(fStrToId("dtNorth"))) ) {
            northEmb.push_back( ge );
          }
          if ( dtGV0->snappedTo(getPtrToSurf(fStrToId("dtSouth"))) && dtGV1->snappedTo(getPtrToSurf(fStrToId("dtSouth"))) ) {
            southEmb.push_back( ge );
          }
        }
      }
    }
    _strucModel->getDtGmshFaceByTag( cDDef.transId(fStrToId("dtNorth")) )->addEdgeLoop( northEmb );  
    _strucModel->getDtGmshFaceByTag( cDDef.transId(fStrToId("dtSouth")) )->addEdgeLoop( southEmb );
    _strucModel->add( new dtGmshRegion(_strucModel, cDDef.transId(1), faces, ori) );
    
    //
    // additional information
    //
    for (::GModel::fiter it = _strucModel->firstFace(); it != _strucModel->lastFace(); ++it) {
      _strucModel->cast2DtGmshFace((*it))->packToExtInfoContainer(_eIC);
    }
    
    
    //
    // parse gmsh file if option exists
    //
    if ( hasOption("gmshMeshFile") ) {
      ParseFile( getOption("gmshMeshFile"), true, true );
    }
    
    _strucModel->writeGEO(labelHandling::getName()+".geo");
    
    _strucModel->mesh(1);
    _strucModel->writeMSH(labelHandling::getName()+"_1d.msh");    
    _strucModel->mesh(2);
    _strucModel->writeMSH(labelHandling::getName()+"_2d.msh");
    _strucModel->mesh(3);
    _strucModel->writeMSH(labelHandling::getName()+"_3d.msh");    
        
    delete _strucModel;
    
    GmshFinalize();
  }
  
  void bladeLayerGmshVolume::writeCheapGrid( void ) {
    bladeLayerVolume::pushGridToSimpleBlockStructure();
  }
  
  covise::coDoSet * bladeLayerGmshVolume::toCoDoUnstructuredGrid(char const * const str) const {
    return NULL;
  }

  void bladeLayerGmshVolume::packToExtInfoContainer( extInfoContainer * const eIC ) const {
    if ( _eIC != NULL ) {
      eIC->add( _eIC );
    }
  }
}
