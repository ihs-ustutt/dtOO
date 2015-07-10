#include "gmshVolume.h"

#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>

#include <gmsh/Context.h>
#include <gmsh/GRegion.h>
#include <gmsh/OpenFile.h>

#include "cardinalDirGModel.h"

#define gmshID(id) \
  id+1

namespace dtOO { 
  gmshVolume::gmshVolume() {
  }

  gmshVolume::~gmshVolume() {
  }

  void gmshVolume::init(
    ::QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP
  ) {
    //
    // init cardinalDirVolume
    //
    cardinalDirVolume::init(element, cValP, sFunP, depAGeoP, depBVolP); 
  }

  void gmshVolume::makeGrid( void ) {
    GmshInitialize();

    cardinalDirGModel * model = new cardinalDirGModel("gmshVolume");
    ::GModel::setCurrent( model );
   
    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }

    //
    // create boundedVolume in gmsh
    //
    model->attachToBoundedVolume( this );
    model->convertToGmshModel();
    
    //GEdge * test;
    //
    // create physical entities
    //
    model->getFaceByTag(gmshID(fStrToId("dtNorth")))->addPhysicalEntity(gmshID(fStrToId("dtNorth")));
    model->getFaceByTag(gmshID(fStrToId("dtWest")))->addPhysicalEntity(gmshID(fStrToId("dtWest")));
    model->getFaceByTag(gmshID(fStrToId("dtSouth")))->addPhysicalEntity(gmshID(fStrToId("dtSouth")));
    model->getFaceByTag(gmshID(fStrToId("dtEast")))->addPhysicalEntity(gmshID(fStrToId("dtEast")));
    model->getFaceByTag(gmshID(fStrToId("dtFront")))->addPhysicalEntity(gmshID(fStrToId("dtFront")));
    model->getFaceByTag(gmshID(fStrToId("dtBack")))->addPhysicalEntity(gmshID(fStrToId("dtBack")));
    if ( hasInternal() ) {
      model->getFaceByTag(gmshID(fStrToId("sInternalEast")))->addPhysicalEntity(gmshID(fStrToId("dtInternal")));
      model->getFaceByTag(gmshID(fStrToId("sInternalWest")))->addPhysicalEntity(gmshID(fStrToId("dtInternal")));
    }
    model->getRegionByTag(1)->addPhysicalEntity(gmshID(rStrToId("dtVolume")));
    model->setPhysicalName("north", 2, gmshID(fStrToId("dtNorth")));
    model->setPhysicalName("south", 2, gmshID(fStrToId("dtSouth")));
    model->setPhysicalName("front", 2, gmshID(fStrToId("dtFront")));
    model->setPhysicalName("back", 2, gmshID(fStrToId("dtBack")));
    model->setPhysicalName("west", 2, gmshID(fStrToId("dtWest")));
    model->setPhysicalName("east", 2, gmshID(fStrToId("dtEast")));
    if ( hasInternal() ) {
      model->setPhysicalName("internal", 2, gmshID(fStrToId("dtInternal")));
    }
    model->setPhysicalName("internalMesh", 3, gmshID(rStrToId("dtVolumeMesh")));
    
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();
    
    //
    // parse gmsh file if option exists
    //
    if ( hasOption("gmshMeshFile") ) {
      ParseFile( getOption("gmshMeshFile"), true, true );
    }

    //
    // write a geo file
    //
    model->writeGEO(labelHandling::getLabel()+".geo");

    //
    // create mesh and write each out
    //
    model->mesh(1);
    model->writeMSH(labelHandling::getLabel()+"_1d"+".msh");
    model->mesh(2);
    model->writeMSH(labelHandling::getLabel()+"_2d"+".msh");
    model->mesh(3);
    model->writeMSH(labelHandling::getLabel()+"_3d"+".msh");
   
    dt__debug(compute(), 
            << dt__eval( Msg().GetFirstError() ) << std::endl
            << dt__eval( Msg().GetFirstWarning() ) );

    delete model;

    GmshFinalize();    
  }

  covise::coDoSet * gmshVolume::toCoDoUnstructuredGrid(char const * const str) const {
    return NULL;
  }
  covise::coDoSet * gmshVolume::toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const {
    return NULL;
  }
}
