#include "cardinalDirTransfiniteVolume.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <geometryEngine/geoBuilder/bezierSurface_pointConstructOCC.h>
#include <geometryEngine/dtSurface.h>

namespace dtOO {  
  cardinalDirTransfiniteVolume::cardinalDirTransfiniteVolume() {
  }

  cardinalDirTransfiniteVolume::~cardinalDirTransfiniteVolume() {

  }
  
  void cardinalDirTransfiniteVolume::init(
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

//    //
//    // vertex
//    //
//    addId("vertex", "dtSouthWestFront");
//    addId("vertex", "dtSouthEastFront");
//    addId("vertex", "dtSouthEastBack");
//    addId("vertex", "dtSouthWestBack");
//    addId("vertex", "dtNorthWestFront");
//    addId("vertex", "dtNorthEastFront");
//    addId("vertex", "dtNorthEastBack");
//    addId("vertex", "dtNorthWestBack");
//    addId("vertex", "vInternalSouthBack");
//    addId("vertex", "vInternalSouthFront");
//    addId("vertex", "vInternalNorthBack");
//    addId("vertex", "vInternalNorthFront");

//    //
//    // edge
//    //
//    addId("edge", "dtSouthFront");
//    addId("edge", "dtSouthBack");
//    addId("edge", "dtNorthBack");
//    addId("edge", "dtNorthFront");
//    addId("edge", "dtSouthWest");
//    addId("edge", "dtSouthEast");
//    addId("edge", "dtNorthEast"); 
//    addId("edge", "dtNorthWest"); 
//    addId("edge", "dtWestFront"); 
//    addId("edge", "dtEastFront"); 
//    addId("edge", "dtEastBack"); 
//    addId("edge", "dtWestBack"); 
//    addId("edge", "_internalSouthEast");
//    addId("edge", "_internalSouthWest");
//    addId("edge", "_internalNorthEast");
//    addId("edge", "_internalNorthWest");
//    addId("edge", "_internalFront");
//    addId("edge", "_internalBack");

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

    //
    // region
    //
    addId("region", "dtVolume");

    vectorHandling< map3dTo3d * > & rRefP = getRefToMap3dTo3dHandling();
    coDoSetHandling< map2dTo3d * > & sRefP = getRefToMap2dTo3dHandling();
    vectorHandling< map1dTo3d * > & eRefP = getRefToMap1dTo3dHandling();
    vectorHandling< dtPoint3 * > & vRefP = getRefToVertexHandling();

    //
    // set 6 sided cube
    //
    QDomElement wElement = getChild("map3dTo3d", element);
    std::string label = getAttributeStr("label", wElement);
    std::string position = getAttributeStr("position", wElement);      
    analyticGeometry const * aG = depAGeoP->get(label);
    map3dTo3d const * mm3d;
    dt__MUSTDOWNCAST(aG, map3dTo3d const, mm3d);
    rRefP[rStrToId(position)] = mm3d->clone();
    
    //
    // set internal
    //
    wElement = getChild("map2dTo3d", element);
    label = getAttributeStr("label", wElement);
    position = getAttributeStr("position", wElement);      
    aG = depAGeoP->get(label);
    map2dTo3d const * mm2d;
    dt__MUSTDOWNCAST(aG, map2dTo3d const, mm2d);
    sRefP[fStrToId(position)] = mm2d->clone();//mm3d->reparamInVolume( mm2d );
    
    //
    // create bounded surfaces
    //
	  twoDArrayHandling< dtPoint3 > p3(2,2);
    // north
		p3[0][0] = dtPoint3(0,0,1);
		p3[0][1] = dtPoint3(1,0,1);
		p3[1][0] = dtPoint3(0,1,1);
		p3[1][1] = dtPoint3(1,1,1);   
    ptrHandling< vec3dTwoD > northUV(
      new vec3dSurfaceTwoD(
				ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );
    northUV->setLabel("dtNorth");
    // south
		p3[0][0] = dtPoint3(0,0,0);
		p3[0][1] = dtPoint3(1,0,0);
		p3[1][0] = dtPoint3(0,1,0);
		p3[1][1] = dtPoint3(1,1,0);   		
    ptrHandling< vec3dTwoD > southUV(
      new vec3dSurfaceTwoD(
        ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );
    southUV->setLabel("dtSouth");
    // west
		p3[0][0] = dtPoint3(0,0,0);
		p3[0][1] = dtPoint3(0,1,0);
		p3[1][0] = dtPoint3(0,0,1);
		p3[1][1] = dtPoint3(0,1,1);   				
    ptrHandling< vec3dTwoD > westUV(
      new vec3dSurfaceTwoD(
        ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );
    westUV->setLabel("dtWest");
    // east
		p3[0][0] = dtPoint3(1,0,0);
		p3[0][1] = dtPoint3(1,1,0);
		p3[1][0] = dtPoint3(1,0,1);
		p3[1][1] = dtPoint3(1,1,1);   						
    ptrHandling< vec3dTwoD > eastUV(
      new vec3dSurfaceTwoD(
        ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );    
    eastUV->setLabel("dtEast");
    // front
		p3[0][0] = dtPoint3(0,0,0);
		p3[0][1] = dtPoint3(1,0,0);
		p3[1][0] = dtPoint3(0,0,1);
		p3[1][1] = dtPoint3(1,0,1);
    ptrHandling< vec3dTwoD > frontUV(
      new vec3dSurfaceTwoD(
        ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );        
    frontUV->setLabel("dtFront");
    // back
		p3[0][0] = dtPoint3(0,1,0);
		p3[0][1] = dtPoint3(1,1,0);
		p3[1][0] = dtPoint3(0,1,1);
		p3[1][1] = dtPoint3(1,1,1);
    ptrHandling< vec3dTwoD > backUV(
      new vec3dSurfaceTwoD(
        ptrHandling<dtSurface>(bezierSurface_pointConstructOCC(p3).result()).get()
      )
    );
    backUV->setLabel("dtBack");
    sRefP[fStrToId("dtNorth")] = new vec3dTwoDInMap3dTo3d( northUV.get(), rRefP[rStrToId("dtVolume")] );
    sRefP[fStrToId("dtWest")] = new vec3dTwoDInMap3dTo3d( westUV.get(), rRefP[rStrToId("dtVolume")] );
    sRefP[fStrToId("dtSouth")] = new vec3dTwoDInMap3dTo3d( southUV.get(), rRefP[rStrToId("dtVolume")] );
    sRefP[fStrToId("dtEast")] = new vec3dTwoDInMap3dTo3d( eastUV.get(), rRefP[rStrToId("dtVolume")] );
    sRefP[fStrToId("dtFront")] = new vec3dTwoDInMap3dTo3d( frontUV.get(), rRefP[rStrToId("dtVolume")] );
    sRefP[fStrToId("dtBack")] = new vec3dTwoDInMap3dTo3d( backUV.get(), rRefP[rStrToId("dtVolume")] );
    
    
//    //
//    // south
//    //    
//    vRefP[vStrToId("dtSouthWestFront")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(0., 0.) );
//    vRefP[vStrToId("dtSouthEastFront")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(1., 0.) );
//    vRefP[vStrToId("dtSouthEastBack")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(1., 1.) );
//    vRefP[vStrToId("dtSouthWestBack")] = new dtPoint3( sRefP[fStrToId("dtSouth")]->getPointPercent(0., 1.) );
//    eRefP[eStrToId("dtSouthFront")] = sRefP[fStrToId("dtSouth")]->pickConstVPercent(0., 0., 1.);
//    eRefP[eStrToId("dtSouthEast")] = sRefP[fStrToId("dtSouth")]->pickConstUPercent(1., 0., 1.);
//    eRefP[eStrToId("dtSouthBack")] = sRefP[fStrToId("dtSouth")]->pickConstVPercent(1., 0., 1.);
//    eRefP[eStrToId("dtSouthWest")] = sRefP[fStrToId("dtSouth")]->pickConstUPercent(0., 0., 1.);
//
//    //
//    // north
//    //    
//    vRefP[vStrToId("dtNorthWestFront")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(0., 0.) );
//    vRefP[vStrToId("dtNorthEastFront")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(1., 0.) );
//    vRefP[vStrToId("dtNorthEastBack")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(1., 1.) );
//    vRefP[vStrToId("dtNorthWestBack")] = new dtPoint3( sRefP[fStrToId("dtNorth")]->getPointPercent(0., 1.) );
//    eRefP[eStrToId("dtNorthFront")] = sRefP[fStrToId("dtNorth")]->pickConstVPercent(0., 0., 1.);
//    eRefP[eStrToId("dtNorthEast")] = sRefP[fStrToId("dtNorth")]->pickConstUPercent(1., 0., 1.);
//    eRefP[eStrToId("dtNorthBack")] = sRefP[fStrToId("dtNorth")]->pickConstVPercent(1., 0., 1.);
//    eRefP[eStrToId("dtNorthWest")] = sRefP[fStrToId("dtNorth")]->pickConstUPercent(0., 0., 1.);
//
//    //
//    // front
//    //    
//    eRefP[eStrToId("dtEastFront")] = sRefP[fStrToId("dtFront")]->pickConstUPercent(1., 0., 1.);
//    eRefP[eStrToId("dtWestFront")] = sRefP[fStrToId("dtFront")]->pickConstUPercent(0., 0., 1.);
//
//    //
//    // back
//    //    
//    eRefP[eStrToId("dtEastBack")] = sRefP[fStrToId("dtBack")]->pickConstUPercent(1., 0., 1.);
//    eRefP[eStrToId("dtWestBack")] = sRefP[fStrToId("dtBack")]->pickConstUPercent(0., 0., 1.);
    
  }
  
  bool cardinalDirTransfiniteVolume::hasInternal( void ) const {
    if (getConstRefToMap2dTo3dHandling()[fStrToId("dtInternal")]) {
      return true;
    }
    else {
      return false;
    }
  }
}
