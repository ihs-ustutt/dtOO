
#include "progHelper.h"

#include "bladeLayerVolume.h"
#include "analyticFunctionHeaven/constValue.h"
#include "analyticGeometryHeaven/analyticSurface.h"
#include <algorithm>


namespace dtOO {
  DTCLASSLOGMETHODIN(bladeLayerVolume);
  
  bladeLayerVolume::bladeLayerVolume() {
  }

  bladeLayerVolume::~bladeLayerVolume() {
    _valCValTwins.destroy();
  }
  
  void bladeLayerVolume::init(
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< scaFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP  
  ) {
    //
    // init cardinalDirVolume
    //
    cardinalDirVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);
    
    //
    // add extruded layer
    //
    addId("face", "dtBladeLayerExt");

    analyticGeometry * aG
    = 
    depAGeoP->get(
      getAttributeStr(
        "label",
        getChildByAttributeMatch(
          "analyticGeometry", 
          "position", 
          "dtBladeLayerExt",
          element
        )
      )
    );
    analyticSurface const * aS;
    dt__MUSTDOWNCAST(aG, analyticSurface const, aS);
    getRefToSurfHandling()[fStrToId("dtBladeLayerExt")] = aS->clone(); 
    
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
  
  void bladeLayerVolume::pushGridToSimpleBlockStructure( void ) {
    coDoSetHandling< analyticSurface * > & sRefP = getRefToSurfHandling();
    
    int nPointsOnSpline = 3;

    //
    // clear
    //
    simpleBlockStructure::clear();
    
//    float frontSouthU = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.50,0.00) 
//                        ).x();
//    float frontSouthV = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.50,0.00) 
//                        ).y();
//    float frontNorthU = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.50,1.00) 
//                        ).x();
//    float frontNorthV = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.50,1.00) 
//                        ).y();
//    float backSouthU = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.00,0.00) 
//                        ).x();
//    float backSouthV = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.00,0.00) 
//                        ).y();
//    float backNorthU = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.00,1.00) 
//                        ).x();
//    float backNorthV = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                          sRefP[fStrToId("dtInternal")]->getPointPercent(0.00,1.00) 
//                        ).y();
//    float frontEastSouthU = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("3u")->getValue(),0.00) 
//                            ).x();
//    float frontEastSouthV =  sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("3u")->getValue(),0.00) 
//                            ).y();
//    float frontEastNorthU = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("7u")->getValue(),1.00) 
//                            ).x();
//    float frontEastNorthV =  sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("7u")->getValue(),1.00) 
//                            ).y();
//    float frontWestSouthU = sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("13u")->getValue(),0.00) 
//                            ).x();
//    float frontWestSouthV =  sRefP[fStrToId("dtSouth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("13u")->getValue(),0.00) 
//                            ).y();
//    float frontWestNorthU = sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("15u")->getValue(),1.00) 
//                            ).x();
//    float frontWestNorthV =  sRefP[fStrToId("dtNorth")]->getClosestPointParameterPercent(
//                              sRefP[fStrToId("dtInternal")]->getPointPercent(_valCValTwins.get("15u")->getValue(),1.00) 
//                            ).y();    
//    float const val0u = frontEastSouthU + _valCValTwins.get("0u")->getValue() * (1. - frontEastSouthU);
//    float const val0v = frontEastSouthV - _valCValTwins.get("0v")->getValue() * (1. - frontEastSouthV);
//    float const val4u = frontEastNorthU + _valCValTwins.get("4u")->getValue() * (1. - frontEastNorthU);
//    float const val4v = frontEastNorthV - _valCValTwins.get("4v")->getValue() * (1. - frontEastNorthV);
//    float const val3u = _valCValTwins.get("3u")->getValue();
//    float const val3v = 0.;
//    float const val7u = _valCValTwins.get("7u")->getValue();
//    float const val7v = 1.;
//    float const val2u = 0.;
//    float const val2v = 0.;
//    float const val6u = 0.;
//    float const val6v = 1.;
//    float const val1u = backSouthU + _valCValTwins.get("1u")->getValue() * (1. - backSouthU);
//    float const val1v = backSouthV + _valCValTwins.get("1v")->getValue() * (1. - backSouthV);
//    float const val5u = backNorthU + _valCValTwins.get("5u")->getValue() * (1. - backNorthU);
//    float const val5v = backNorthV + _valCValTwins.get("5v")->getValue() * (1. - backNorthV);  
//    float const val8u = frontSouthU + _valCValTwins.get("8u")->getValue() * (1. - frontSouthU);
//    float const val8v = frontSouthV - _valCValTwins.get("8v")->getValue() * (1. - frontSouthV);
//    float const val10u = frontNorthU + _valCValTwins.get("10u")->getValue() * (1. - frontNorthU);
//    float const val10v = frontNorthV - _valCValTwins.get("10v")->getValue() * (1. - frontNorthV);
//    float const val9u = _valCValTwins.get("9u")->getValue();
//    float const val9v = 0.;
//    float const val11u = _valCValTwins.get("11u")->getValue();
//    float const val11v = 1.;
//    float const val12u = frontWestSouthU - _valCValTwins.get("12u")->getValue() * (1. - frontWestSouthU);
//    float const val12v = frontWestSouthV - _valCValTwins.get("12v")->getValue() * (1. - frontWestSouthV);
//    float const val14u = frontWestNorthU - _valCValTwins.get("14u")->getValue() * (1. - frontWestNorthU);
//    float const val14v = frontWestNorthV - _valCValTwins.get("14v")->getValue() * (1. - frontWestNorthV);
//    float const val13u = _valCValTwins.get("13u")->getValue();
//    float const val13v = 0.;
//    float const val15u = _valCValTwins.get("15u")->getValue();
//    float const val15v = 1.;    
//    float const val16u = backSouthU - _valCValTwins.get("16u")->getValue() * (1. - backSouthU);
//    float const val16v = backSouthV + _valCValTwins.get("16v")->getValue() * (1. - backSouthV);
//    float const val17u = backNorthU - _valCValTwins.get("17u")->getValue() * (1. - backNorthU);
//    float const val17v = backNorthV + _valCValTwins.get("17v")->getValue() * (1. - backNorthV);      
    float const val0u = .25;//frontEastSouthU + _valCValTwins.get("0u")->getValue() * (1. - frontEastSouthU);
    float const val0v = 0.;//frontEastSouthV - _valCValTwins.get("0v")->getValue() * (1. - frontEastSouthV);
    float const val4u = .25;//frontEastNorthU + _valCValTwins.get("4u")->getValue() * (1. - frontEastNorthU);
    float const val4v = 1.;//frontEastNorthV - _valCValTwins.get("4v")->getValue() * (1. - frontEastNorthV);
    float const val3u = _valCValTwins.get("3u")->getValue();
    float const val3v = 0.;
    float const val7u = _valCValTwins.get("7u")->getValue();
    float const val7v = 1.;
    float const val2u = 0.;
    float const val2v = 0.;
    float const val6u = 0.;
    float const val6v = 1.;
    float const val1u = 0.;//backSouthU + _valCValTwins.get("1u")->getValue() * (1. - backSouthU);
    float const val1v = 0.;//backSouthV + _valCValTwins.get("1v")->getValue() * (1. - backSouthV);
    float const val5u = 0.;//backNorthU + _valCValTwins.get("5u")->getValue() * (1. - backNorthU);
    float const val5v = 1.;//backNorthV + _valCValTwins.get("5v")->getValue() * (1. - backNorthV);  
    float const val8u = .5;//frontSouthU + _valCValTwins.get("8u")->getValue() * (1. - frontSouthU);
    float const val8v = 0.;//frontSouthV - _valCValTwins.get("8v")->getValue() * (1. - frontSouthV);
    float const val10u = .5;//frontNorthU + _valCValTwins.get("10u")->getValue() * (1. - frontNorthU);
    float const val10v = 1.;//frontNorthV - _valCValTwins.get("10v")->getValue() * (1. - frontNorthV);
    float const val9u = _valCValTwins.get("9u")->getValue();
    float const val9v = 0.;
    float const val11u = _valCValTwins.get("11u")->getValue();
    float const val11v = 1.;
    float const val12u = .75;//frontWestSouthU - _valCValTwins.get("12u")->getValue() * (1. - frontWestSouthU);
    float const val12v = 0.;//frontWestSouthV - _valCValTwins.get("12v")->getValue() * (1. - frontWestSouthV);
    float const val14u = .75;//frontWestNorthU - _valCValTwins.get("14u")->getValue() * (1. - frontWestNorthU);
    float const val14v = 1.;//frontWestNorthV - _valCValTwins.get("14v")->getValue() * (1. - frontWestNorthV);
    float const val13u = _valCValTwins.get("13u")->getValue();
    float const val13v = 0.;
    float const val15u = _valCValTwins.get("15u")->getValue();
    float const val15v = 1.;    
    float const val16u = 1.;//backSouthU - _valCValTwins.get("16u")->getValue() * (1. - backSouthU);
    float const val16v = 0.;//backSouthV + _valCValTwins.get("16v")->getValue() * (1. - backSouthV);
    float const val17u = 1.;//backNorthU - _valCValTwins.get("17u")->getValue() * (1. - backNorthU);
    float const val17v = 1.;//backNorthV + _valCValTwins.get("17v")->getValue() * (1. - backNorthV);      
    //
    // vertices
    //    
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val0u, val0v), 0, val0u, val0v, fStrToId("dtBladeLayerExt"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val1u, val1v), 1, val1u, val1v, fStrToId("dtBladeLayerExt"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val2u, val2v), 2, val2u, val2v, fStrToId("dtInternal"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val3u, val3v), 3, val3u, val3v, fStrToId("dtInternal"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val4u, val4v), 4, val4u, val4v, fStrToId("dtBladeLayerExt"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val5u, val5v), 5, val5u, val5v, fStrToId("dtBladeLayerExt"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val6u, val6v), 6, val6u, val6v, fStrToId("dtInternal"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val7u, val7v), 7, val7u, val7v, fStrToId("dtInternal"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val8u, val8v), 8, val8u, val8v, fStrToId("dtBladeLayerExt"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val9u, val9v), 9, val9u, val9v, fStrToId("dtInternal"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val10u, val10v), 10, val10u, val10v, fStrToId("dtBladeLayerExt"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val11u, val11v), 11, val11u, val11v, fStrToId("dtInternal"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val12u, val12v), 12, val12u, val12v, fStrToId("dtBladeLayerExt"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val13u, val3v), 13, val13u, val13v, fStrToId("dtInternal"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val14u, val14v), 14, val14u, val14v, fStrToId("dtBladeLayerExt"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtInternal")]->getPointPercent(val15u, val15v), 15, val15u, val15v, fStrToId("dtInternal"), fStrToId("dtNorth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val16u, val16v), 16, val16u, val16v, fStrToId("dtBladeLayerExt"), fStrToId("dtSouth") );
    addVertex( sRefP[fStrToId("dtBladeLayerExt")]->getPointPercent(val17u, val17v), 17, val17u, val17v, fStrToId("dtBladeLayerExt"), fStrToId("dtNorth") );    


    //
    // edges
    //
//    //iee
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val3u,
//                        val3v,
//                        val2u,
//                        val2v
//                      ),
//      0., 1., 3, 2, nPointsOnSpline 
//    ); 
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val7u,
//                        val7v,
//                        val6u,
//                        val6v
//                      ),
//      0., 1., 7, 6, nPointsOnSpline       
//    ); 
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 0, 3, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 4, 7, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 0, 1, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 4, 5, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 1, 2, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 5, 6, nPointsOnSpline); 
//    addEdgeInSurface(sRefP[fStrToId("dtBladeLayerExt")], 1, 5, nPointsOnSpline);
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        0.00,
//                        0.00,
//                        0.00,
//                        1.00
//                      ),
//      0., 1., 2, 6, nPointsOnSpline       
//    ); 
//    addEdgeInSurface(sRefP[fStrToId("dtBladeLayerExt")], 0, 4, nPointsOnSpline);
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val3u,
//                        0.00,
//                        val7u,
//                        1.00
//                      ),
//      0., 1., 3, 7, nPointsOnSpline       
//    ); 
//    //ie
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val3u,
//                        val3v,
//                        val9u,
//                        val9v
//                      ),
//      0., 1., 3, 9, nPointsOnSpline 
//    ); 
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val7u,
//                        val7v,
//                        val11u,
//                        val11v
//                      ),
//      0., 1., 7, 11, nPointsOnSpline       
//    ); 
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 8, 9, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 10, 11, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 0, 8, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 4, 10, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtBladeLayerExt")], 8, 10, nPointsOnSpline);
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val9u,
//                        val9v,
//                        val11u,
//                        val11v
//                      ),
//      0., 1., 9, 11, nPointsOnSpline       
//    ); 
//    //iw
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val9u,
//                        val9v,
//                        val13u,
//                        val13v
//                      ),
//      0., 1., 9, 13, nPointsOnSpline 
//    ); 
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val11u,
//                        val11v,
//                        val15u,
//                        val15v
//                      ),
//      0., 1., 11, 15, nPointsOnSpline       
//    ); 
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 12, 13, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 14, 15, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 8, 12, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 10, 14, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtBladeLayerExt")], 12, 14, nPointsOnSpline);
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val13u,
//                        val13v,
//                        val15u,
//                        val15v
//                      ),
//      0., 1., 13, 15, nPointsOnSpline       
//    );
//    //iww
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val13u,
//                        val13v,
//                        1.,
//                        val2v
//                      ),
//      0., 1., 13, 2, nPointsOnSpline 
//    ); 
//    addEdge(
//      sRefP[fStrToId("dtInternal")]->getSplineCurve3dLinearPercent(
//                        val15u,
//                        val15v,
//                        1.,
//                        val6v
//                      ),
//      0., 1., 15, 6, nPointsOnSpline       
//    ); 
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 12, 16, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 14, 17, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtSouth")], 16, 2, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtNorth")], 17, 6, nPointsOnSpline);
//    addEdgeInSurface(sRefP[fStrToId("dtBladeLayerExt")], 16, 17, nPointsOnSpline);

    addEdgeNoSurface(3, 2, nPointsOnSpline); 
    addEdgeNoSurface(7, 6, nPointsOnSpline); 
    addEdgeNoSurface(0, 3, nPointsOnSpline);
    addEdgeNoSurface(4, 7, nPointsOnSpline);
    addEdgeNoSurface(0, 1, nPointsOnSpline);
    addEdgeNoSurface(4, 5, nPointsOnSpline);
    addEdgeNoSurface(1, 2, nPointsOnSpline);
    addEdgeNoSurface(5, 6, nPointsOnSpline); 
    addEdgeNoSurface(1, 5, nPointsOnSpline);
    addEdgeNoSurface(2, 6, nPointsOnSpline); 
    addEdgeNoSurface(0, 4, nPointsOnSpline);
    addEdgeNoSurface(3, 7, nPointsOnSpline); 
    //ie
    addEdgeNoSurface(3, 9, nPointsOnSpline); 
    addEdgeNoSurface(7, 11, nPointsOnSpline); 
    addEdgeNoSurface(8, 9, nPointsOnSpline);
    addEdgeNoSurface(10, 11, nPointsOnSpline);
    addEdgeNoSurface(0, 8, nPointsOnSpline);
    addEdgeNoSurface(4, 10, nPointsOnSpline);
    addEdgeNoSurface(8, 10, nPointsOnSpline);
    addEdgeNoSurface(9, 11, nPointsOnSpline); 
    //iw
    addEdgeNoSurface(9, 13, nPointsOnSpline); 
    addEdgeNoSurface(11, 15, nPointsOnSpline); 
    addEdgeNoSurface(12, 13, nPointsOnSpline);
    addEdgeNoSurface(14, 15, nPointsOnSpline);
    addEdgeNoSurface(8, 12, nPointsOnSpline);
    addEdgeNoSurface(10, 14, nPointsOnSpline);
    addEdgeNoSurface(12, 14, nPointsOnSpline);
    addEdgeNoSurface(13, 15, nPointsOnSpline);
    //iww
    addEdgeNoSurface(13, 2, nPointsOnSpline); 
    addEdgeNoSurface(15, 6, nPointsOnSpline); 
    addEdgeNoSurface(12, 16, nPointsOnSpline);
    addEdgeNoSurface(14, 17, nPointsOnSpline);
    addEdgeNoSurface(16, 2, nPointsOnSpline);
    addEdgeNoSurface(17, 6, nPointsOnSpline);
    addEdgeNoSurface(16, 17, nPointsOnSpline);    
    
    //
    // block
    //
    addBlock(0,1,2,3,4,5,6,7, "iee");
    addBlock(0,3,9,8,4,7,11,10, "ie");
    addBlock(8,9,13,12,10,11,15,14, "iw");
    addBlock(12,13,2,16,14,15,6,17, "iww");
    
    //
    // number of element and grading
    //
    setNElements(
      "iee", 
      _valCValTwins.get("dtLayerNumElemTangent")->getValue(), 
      _valCValTwins.get("dtLayerNumElemNormal")->getValue(), 
      _valCValTwins.get("dtLayerNumElemRadial")->getValue()
    );
    setNElements(
      "ie", 
      _valCValTwins.get("dtLayerNumElemNormal")->getValue(), 
      _valCValTwins.get("dtLayerNumElemTangent")->getValue(), 
      _valCValTwins.get("dtLayerNumElemRadial")->getValue()
    );    
    setNElements(
      "iw", 
      _valCValTwins.get("dtLayerNumElemNormal")->getValue(), 
      _valCValTwins.get("dtLayerNumElemTangent")->getValue(), 
      _valCValTwins.get("dtLayerNumElemRadial")->getValue()
    );        
    setNElements(
      "iww", 
      _valCValTwins.get("dtLayerNumElemNormal")->getValue(), 
      _valCValTwins.get("dtLayerNumElemTangent")->getValue(), 
      _valCValTwins.get("dtLayerNumElemRadial")->getValue()
    );         
    setSimpleGrading(
      "iee", 
      1.,
      1.,      
      _valCValTwins.get("dtLayerGrading")->getValue() 
    );
    setSimpleGrading(
      "ie", 
      1., 
      1., 
      _valCValTwins.get("dtLayerGrading")->getValue()
    );    
    setSimpleGrading(
      "iw", 
      1., 
      1.,
      _valCValTwins.get("dtLayerGrading")->getValue()
    );        
    setSimpleGrading(
      "iww", 
      1., 
      1.,
      _valCValTwins.get("dtLayerGrading")->getValue()
    );          

    //
    //patches
    //
    addPatch("inField", 0, 4, 10, 8);
    addPatch("inField", 12, 8, 10, 14);
    addPatch("inField", 0, 1 , 5, 4);
    addPatch("inField", 12, 16, 17, 14);
    addPatch("inField", 1, 2, 6, 5);
    addPatch("inField", 2, 16, 17, 6);
    addCouplingPatchName("inField");
    addPatch("internal", 2, 3, 7, 6);
    addPatch("internal", 3, 9, 11, 7);
    addPatch("internal", 9, 13, 15, 11);
    addPatch("internal", 13, 2, 6, 15);
    addPatch("south", 0, 1, 2, 3);
    addPatch("south", 8, 0, 3, 9);
    addPatch("south", 12, 8, 9, 13);
    addPatch("south", 12, 13, 2, 16);
    addPatch("north", 4, 5, 6, 7);
    addPatch("north", 10, 4, 7, 11);
    addPatch("north", 14, 10, 11, 15);
    addPatch("north", 14, 15, 6, 17);
  }

  std::vector< std::vector< int > > bladeLayerVolume::getCouplingVertex( void ) const {
    std::vector< std::vector< int > > cV;
    cV.push_back( std::vector< int >() );
    cV.push_back( std::vector< int >() );
    
    cV[0].push_back(2);
    cV[0].push_back(1);
    cV[0].push_back(0);
    cV[0].push_back(8);
    cV[0].push_back(12);
    cV[0].push_back(16);

    cV[1].push_back(6);
    cV[1].push_back(5);
    cV[1].push_back(4);
    cV[1].push_back(10);
    cV[1].push_back(14);
    cV[1].push_back(17);    
  }
  
  covise::coDoSet * bladeLayerVolume::toCoDoSet(char const * const str) const {
    coDoSet * retSet = simpleBlockStructure::toCoDoSet(str);
    retSet->addAttribute("COLOR", "yellow");
    
    return retSet;
  }  
}
