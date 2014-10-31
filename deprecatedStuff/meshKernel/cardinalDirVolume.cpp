#include "cardinalDirVolume.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO { 
  cardinalDirVolume::cardinalDirVolume() {
  }

  cardinalDirVolume::~cardinalDirVolume() {

  }
  
  void cardinalDirVolume::init(
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP
  ) {    
    //
    // init boundedVolume
    //
    boundedVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);

    //
    // vertex
    //
    addId("vertex", "dtSouthWestFront");
    addId("vertex", "dtSouthEastFront");
    addId("vertex", "dtSouthEastBack");
    addId("vertex", "dtSouthWestBack");
    addId("vertex", "dtNorthWestFront");
    addId("vertex", "dtNorthEastFront");
    addId("vertex", "dtNorthEastBack");
    addId("vertex", "dtNorthWestBack");
    addId("vertex", "vInternalSouthBack");
    addId("vertex", "vInternalSouthFront");
    addId("vertex", "vInternalNorthBack");
    addId("vertex", "vInternalNorthFront");

    //
    // edge
    //
    addId("edge", "dtSouthFront");
    addId("edge", "dtSouthBack");
    addId("edge", "dtNorthBack");
    addId("edge", "dtNorthFront");
    addId("edge", "dtSouthWest");
    addId("edge", "dtSouthEast");
    addId("edge", "dtNorthEast"); 
    addId("edge", "dtNorthWest"); 
    addId("edge", "dtWestFront"); 
    addId("edge", "dtEastFront"); 
    addId("edge", "dtEastBack"); 
    addId("edge", "dtWestBack"); 
    addId("edge", "_internalSouthEast");
    addId("edge", "_internalSouthWest");
    addId("edge", "_internalNorthEast");
    addId("edge", "_internalNorthWest");
    addId("edge", "_internalFront");
    addId("edge", "_internalBack");

    //
    // face
    //
    addId("face", "dtNorth");
    addId("face", "dtWest");
    addId("face", "dtSouth");
    addId("face", "dtEast");
    addId("face", "dtFront");
    addId("face", "dtBack");
    addId("face", "dtInternal");
    addId("face", "sInternalEast");
    addId("face", "sInternalWest");

    //
    // region
    //
    addId("region", "dtVolume");

    vectorHandling< map3dTo3d * > & rRefP = getRefToMap3dTo3dHandling();
    coDoSetHandling< map2dTo3d * > & sRefP = getRefToMap2dTo3dHandling();
    vectorHandling< map1dTo3d * > & eRefP = getRefToMap1dTo3dHandling();
    vectorHandling< dtPoint3 * > & vRefP = getRefToVertexHandling();
    
    //
    // set surfaces
    //
    QDomElement wElement = getChild("analyticGeometry", element);
    while ( !wElement.isNull() ) {
      //
      // get label of surface
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get position of surface
      //
      std::string position = getAttributeStr("position", wElement);
      
      if ( hasStrToId("face", position) ) {
        //
        // get surface
        //      
        analyticGeometry const * aG = depAGeoP->get(label);
        map2dTo3d const * mm2d;
        dt__MUSTDOWNCAST(aG, map2dTo3d const, mm2d);

        //
        // put surface at correct position and store pointer
        //
        sRefP[fStrToId(position)] = mm2d->clone();
      }
      //
      // go to next element
      //
      wElement = getNextSibling("analyticGeometry", wElement);
    }
    
    
    //
    // south
    //    
    vRefP[vStrToId("dtSouthWestFront")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(0., 0.) );
    vRefP[vStrToId("dtSouthEastFront")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(1., 0.) );
    vRefP[vStrToId("dtSouthEastBack")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(1., 1.) );
    vRefP[vStrToId("dtSouthWestBack")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(0., 1.) );
    eRefP[eStrToId("dtSouthFront")] = sRefP[fStrToId("dtSouth")]->pickConstVPercent(0., 0., 1.);
    eRefP[eStrToId("dtSouthEast")] = sRefP[fStrToId("dtSouth")]->pickConstUPercent(1., 0., 1.);
    eRefP[eStrToId("dtSouthBack")] = sRefP[fStrToId("dtSouth")]->pickConstVPercent(1., 0., 1.);
    eRefP[eStrToId("dtSouthWest")] = sRefP[fStrToId("dtSouth")]->pickConstUPercent(0., 0., 1.);

    //
    // north
    //    
    vRefP[vStrToId("dtNorthWestFront")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(0., 0.) );
    vRefP[vStrToId("dtNorthEastFront")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(1., 0.) );
    vRefP[vStrToId("dtNorthEastBack")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(1., 1.) );
    vRefP[vStrToId("dtNorthWestBack")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(0., 1.) );
    eRefP[eStrToId("dtNorthFront")] = sRefP[fStrToId("dtNorth")]->pickConstVPercent(0., 0., 1.);
    eRefP[eStrToId("dtNorthEast")] = sRefP[fStrToId("dtNorth")]->pickConstUPercent(1., 0., 1.);
    eRefP[eStrToId("dtNorthBack")] = sRefP[fStrToId("dtNorth")]->pickConstVPercent(1., 0., 1.);
    eRefP[eStrToId("dtNorthWest")] = sRefP[fStrToId("dtNorth")]->pickConstUPercent(0., 0., 1.);

    //
    // front
    //    
    eRefP[eStrToId("dtEastFront")] = sRefP[fStrToId("dtFront")]->pickConstUPercent(1., 0., 1.);
    eRefP[eStrToId("dtWestFront")] = sRefP[fStrToId("dtFront")]->pickConstUPercent(0., 0., 1.);

    //
    // back
    //    
    eRefP[eStrToId("dtEastBack")] = sRefP[fStrToId("dtBack")]->pickConstUPercent(1., 0., 1.);
    eRefP[eStrToId("dtWestBack")] = sRefP[fStrToId("dtBack")]->pickConstUPercent(0., 0., 1.);
    
  }
  
  bool cardinalDirVolume::hasInternal( void ) const {
    if (getConstRefToMap2dTo3dHandling()[fStrToId("dtInternal")]) {
      return true;
    }
    else {
      return false;
    }
  }

  covise::coDoSet * cardinalDirVolume::toCoDoSet(char const * const str) const {
    return getConstRefToMap2dTo3dHandling().toCoDoSet(
             getConstRefToMap2dTo3dHandling(), 
             str
           );
  }
}
