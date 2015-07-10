#include "coVolume.h"
#include "analyticGeometryHeaven/splineCurve3d.h"
#include "interfaceHeaven/systemHandling.h"
#include "analyticFunctionHeaven/scaFunction.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticFunctionHeaven/constValue.h>
#include <ReadFOAM.h>
#include <logMe/logMe.h>
#include <string>
#include <do/coDoSet.h>
#include <do/coDoUnstructuredGrid.h>

#define grid__readCVal(constValue, attribute, location, floatToStore, constValueToStore) \
  if (attribute == #location) { \
    floatToStore[location] = constValue->getValue(); \
    constValueToStore[location] = constValue; \
}
#define grid__readInt(constValue, attribute, location, intToStore) \
      if (attribute == #location) { \
        intToStore[location] = constValue->getValue(); \
      }

      
namespace dtOO {
  DTCLASSLOGMETHODI(coVolume,
          << "No member attributes.");
  
  coVolume::coVolume() {
    _surfP.resize(7);
    _val.resize(_nGridVal);
    _valCValP.resize(_nGridVal);    
    _int.resize(_nGridInt);
  }

  coVolume::coVolume(const coVolume& orig) {
  }

  coVolume::~coVolume() {
    _valCValP.clear();
    _intCValP.clear();
    _int.clear();
    _val.clear();
    _markedEdgeScaFunction.clear();
    _markedEdgeFrom.clear();
    _markedEdgeTo.clear();
  }

  void coVolume::init( ::QDomElement const & element,
                      vectorHandling< constValue * > const * const cValP,
                      vectorHandling< scaFunction * > const * const sFunP,
                      vectorHandling< analyticGeometry * > const * const depAGeoP ) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, cValP, sFunP, depAGeoP);
    
    //
    // set label of boundenVolume
    //
    labelHandling::setName( getAttributeStr("label", element ) );

    //
    // case directory
    //
    _caseDirectory = getOption("workingDirectory")+"/"+getName();
    
    //
    // set surfaces
    //
    ::QDomElement wElement = getChild("analyticGeometry", element);
    while ( !wElement.isNull() ) {
      //
      // get label of surface
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get position of surface
      //
      std::string position = getAttributeStr("position", wElement);
      //
      // get surface
      //      
      analyticGeometry const * aG = depAGeoP->get(label);
      analyticSurface const * aS;
      dt__mustCast(aG, analyticSurface const, aS);
      
      //
      // put surface at correct position and store pointer
      //
      if (position == "dtNorth") {
        _surfP[dtNorth] = aS->clone();
      }
      else if (position == "dtWest") {
        _surfP[dtWest] = aS->clone();
      }
      else if (position == "dtSouth") {
        _surfP[dtSouth] = aS->clone();
      }
      else if (position == "dtEast") {
        _surfP[dtEast] = aS->clone();
      }
      else if (position == "dtFront") {
        _surfP[dtFront] = aS->clone();
      }
      else if (position == "dtBack") {
        _surfP[dtBack] = aS->clone();
      }
      else if (position == "dtInternal") {
        _surfP[dtInternal] = aS->clone();
      }
      //
      // go to next element
      //
      wElement = getNextSibling("analyticGeometry", wElement);
    }

    //
    // clear function vectors
    //
    _markedEdgeFrom.clear();
    _markedEdgeTo.clear();
    _markedEdgeScaFunction.clear();

    //
    // set scaFunctions
    //    
    wElement = getChild("scaFunction", element);
    while ( !wElement.isNull() ) {
      //
      // get label
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get from to
      //
      int from = getAttributeInt("from", wElement);
      int to = getAttributeInt("to", wElement);
      
      //
      // get function
      //      
      scaFunction * sF = sFunP->get(label);
      
      //
      // put scaFunction
      //
      scaFunction * sFClone = sF->clone();
      _markedEdgeScaFunction.push_back( sFClone );
      _markedEdgeFrom.push_back( from );
      _markedEdgeTo.push_back( to );
      
      //
      // go to next element
      //
      wElement = getNextSibling("scaFunction", wElement);
    }
    
    //
    // set constValues float
    //
    // fixed nodes
    _val[dtNode0u] = 1.;
    _val[dtNode0v] = 0.;
    _val[dtNode4u] = 1.;
    _val[dtNode4v] = 0.;
    _val[dtNode1u] = 1.;
    _val[dtNode5u] = 1.;
    _val[dtNode20u] = 1.;
    _val[dtNode22u] = 1.;
    _val[dtNode36u] = 1.;
    _val[dtNode36v] = 1.;
    _val[dtNode38u] = 1.;
    _val[dtNode38v] = 1.;
    _val[dtNode37v] = 1.;
    _val[dtNode39v] = 1.;
    _val[dtNode40v] = 1.;
    _val[dtNode41v] = 1.;
    _val[dtNode42v] = 1.;
    _val[dtNode43v] = 1.;
    _val[dtNode44u] = 0.;
    _val[dtNode44v] = 1.;
    _val[dtNode45u] = 0.;
    _val[dtNode45v] = 1.;
    _val[dtNode34u] = 0.;
    _val[dtNode35u] = 0.;
    _val[dtNode16u] = 0.;
    _val[dtNode18u] = 0.;
    _val[dtNode19u] = 0.;
    _val[dtNode19v] = 0.;
    _val[dtNode17u] = 0.;
    _val[dtNode17v] = 0.;
    _val[dtNode15v] = 0.;
    _val[dtNode13v] = 0.;
    _val[dtNode11v] = 0.;
    _val[dtNode9v] = 0.;
    _val[dtNode3v] = 0.;
    _val[dtNode7v] = 0.;
    _val[dtNode24u] = 0.;
    _val[dtNode24v] = 0.;
    _val[dtNode26u] = 0.;
    _val[dtNode26v] = 1.;
    _val[dtNode25v] = 0.;
    _val[dtNode27v] = 1.;
    _val[dtNode28v] = 0.;
    _val[dtNode29v] = 1.;    
    _val[dtNode30v] = 0.;
    _val[dtNode31v] = 1.;    
    _val[dtA] = 1.;
    _val[dtB] = 1.;
    _val[dtC] = 1.;
    _val[dtD] = 1.;
    _val[dtE] = 1.;
    
    // read other non-fixed values
    wElement = getChild("constValue", element);
    while ( !wElement.isNull() ) {
      //
      // get label of constValue
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get attribute
      //
      std::string attribute;
      if ( hasAttribute("attribute", wElement) ) {
        attribute = getAttributeStr("attribute", wElement);  
      }
      else {
        attribute = label;
      }
      
      //
      // get surface
      //      
      constValue * cV = cValP->get(label);
      if (!cV) {
        dt__THROW(init,
                dt__eval(cV) << std::endl
                << "constValue " << dt__eval(label) 
                << " not in vector _cValP.");
      }
      grid__readCVal(cV, attribute, dtNode3u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode7u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode9u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode11u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode13u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode15u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode1v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode5v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode2u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode2v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode6u, _val, _valCValP);      
      grid__readCVal(cV, attribute, dtNode6v, _val, _valCValP);      
      grid__readCVal(cV, attribute, dtNode8u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode8v, _val, _valCValP);      
      grid__readCVal(cV, attribute, dtNode10u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode10v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode12u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode12v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode14u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode14v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode16v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode18v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode20v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode22v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode21u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode21v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode23u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode23v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode32u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode32v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode33u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode33v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode34v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode35v, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode25u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode27u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode28u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode29u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode30u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode31u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode37u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode39u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode40u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode41u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode42u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtNode43u, _val, _valCValP);
      grid__readCVal(cV, attribute, dtA, _val, _valCValP);
      grid__readCVal(cV, attribute, dtB, _val, _valCValP);      
      grid__readCVal(cV, attribute, dtC, _val, _valCValP);      
      grid__readCVal(cV, attribute, dtD, _val, _valCValP);     
      grid__readCVal(cV, attribute, dtE, _val, _valCValP);     
      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);
    }
    
    //
    // set constValues int
    //
    wElement = getChild("constValue", element);
    while ( !wElement.isNull() ) {
      //
      // get label of constValue
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get attribute
      //
      std::string attribute;
      if ( hasAttribute("attribute", wElement) ) {
        attribute = getAttributeStr("attribute", wElement);  
      }
      else {
        attribute = label;
      }  
      //
      // get surface
      //      
      constValue * cV = cValP->get(label);
      if (!cV) {
        dt__THROW(init,
                dt__eval(cV) << std::endl
                << "constValue " << dt__eval(label) 
                << " not in vector _cValP.");
      }
      grid__readInt(cV, attribute, dtAlpha, _int);
      grid__readInt(cV, attribute, dtBeta, _int);
      grid__readInt(cV, attribute, dtGamma, _int);
      grid__readInt(cV, attribute, dtDelta, _int);
      grid__readInt(cV, attribute, dtEpsilon, _int);
      grid__readInt(cV, attribute, dtPhi, _int);
      grid__readInt(cV, attribute, dtPsi, _int);
      
      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);
    }

//    //
//    // automatic adjust
//    //
//    if ( optionTrue("auto_adjust") ) {
//      autoAdjust();
//    }

    dump();
  }  
  
  void coVolume::dump(void) const {
    DTBUFFERINIT();
    dt__FORALL(_surfP, ii,
      DTBUFFER( << "_surfP[" << ii << "] = " << _surfP[ii] 
            << " label = " << _surfP[ii]->getName() << std::endl );
    );
    dt__FORALL(_val, ii,
      DTBUFFER( << "_val[" << ii << "] = " << _val[ii] << std::endl );
    );       
    dt__FORALL(_int, ii,
      DTBUFFER( << "_int[" << ii << "] = " << _int[ii] << std::endl );
    );    
    dt__debug_BUFFER(init());
  }
  
  void coVolume::pushGridToBlockMeshDictWriter( void ) {
    int nPointsOnSpline = 100;

    //
    // clear
    //
    blockMeshDictWriter::clear();
    
    //
    // push marked edges
    //
    dt__FORALL(_markedEdgeScaFunction, ii,
      markSplineEdge(_markedEdgeFrom[ii], _markedEdgeTo[ii], _markedEdgeScaFunction[ii]);
    );
    
//    //
//    //auto adjust parameters
//    //
//    autoAdjust();
    
    //
    // vertices
    //    
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode0u], _val[dtNode0v]), 0 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode1u], _val[dtNode1v]), 1 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode2u], _val[dtNode2v]), 2 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode3u], _val[dtNode3v]), 3 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode4u], _val[dtNode4v]), 4 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode5u], _val[dtNode5v]), 5 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode6u], _val[dtNode6v]), 6 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode7u], _val[dtNode7v]), 7 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode8u], _val[dtNode8v]), 8 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode9u], _val[dtNode9v]), 9 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode10u], _val[dtNode10v]), 10 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode11u], _val[dtNode11v]), 11 );    
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode12u], _val[dtNode12v]), 12 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode13u], _val[dtNode13v]), 13 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode14u], _val[dtNode14v]), 14 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode15u], _val[dtNode15v]), 15 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode16u], _val[dtNode16v]), 16 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode17u], _val[dtNode17v]), 17 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode18u], _val[dtNode18v]), 18 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode19u], _val[dtNode19v]), 19 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode20u], _val[dtNode20v]), 20 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode21u], _val[dtNode21v]), 21 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode22u], _val[dtNode22v]), 22 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode23u], _val[dtNode23v]), 23 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode24u], _val[dtNode24v]), 24 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode25u], _val[dtNode25v]), 25 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode26u], _val[dtNode26v]), 26 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode27u], _val[dtNode27v]), 27 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode28u], _val[dtNode28v]), 28 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode29u], _val[dtNode29v]), 29 );    
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode30u], _val[dtNode30v]), 30 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode31u], _val[dtNode31v]), 31 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode32u], _val[dtNode32v]), 32 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode33u], _val[dtNode33v]), 33 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode34u], _val[dtNode34v]), 34 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode35u], _val[dtNode35v]), 35 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode36u], _val[dtNode36v]), 36 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode37u], _val[dtNode37v]), 37 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode38u], _val[dtNode38v]), 38 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode39u], _val[dtNode39v]), 39 );    
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode40u], _val[dtNode40v]), 40 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode41u], _val[dtNode41v]), 41 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode42u], _val[dtNode42v]), 42 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode43u], _val[dtNode43v]), 43 );
    addVertex( _surfP[dtSouth]->getPointPercent(_val[dtNode44u], _val[dtNode44v]), 44 );
    addVertex( _surfP[dtNorth]->getPointPercent(_val[dtNode45u], _val[dtNode45v]), 45 );
         
    //
    // edges
    //
    //fee
    addEdgeInSurface(_surfP[dtSouth], 0, 1, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 3, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 4, 5, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 7, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 0, 3, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 1, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 4, 7, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtNorth], 5, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 0, 4, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 1, 5, nPointsOnSpline);
    addEdgeNoSurface(2, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtFront], 3, 7, nPointsOnSpline);
    //fe
    addEdgeInSurface(_surfP[dtSouth], 3, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 9, 8, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 7, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 11, 10, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 3, 9, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 2, 8, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 7, 11, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtNorth], 6, 10, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 3, 7, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 2, 6, nPointsOnSpline);
    addEdgeNoSurface(8, 10, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtFront], 9, 11, nPointsOnSpline);
    //fw
    addEdgeInSurface(_surfP[dtSouth], 9, 8, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 11, 10, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 8, 12, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 10, 14, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 13, 12, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 15, 14, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 9, 13, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 11, 15, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtFront], 9, 11, nPointsOnSpline);    
    addEdgeNoSurface(8, 10, nPointsOnSpline);
    addEdgeNoSurface(12, 14, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtFront], 13, 15, nPointsOnSpline);   
    //fww
    addEdgeInSurface(_surfP[dtSouth], 13, 12, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 15, 14, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 12, 16, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 14, 18, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 17, 16, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 19, 18, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 13, 17, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 15, 19, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtFront], 13, 15, nPointsOnSpline);    
    addEdgeNoSurface(12, 14, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 16, 18, nPointsOnSpline);     
    addEdgeInSurface(_surfP[dtFront], 17, 19, nPointsOnSpline);     
    //ieee
    addEdgeInSurface(_surfP[dtSouth], 1, 20, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 5, 22, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 20, 21, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 22, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 2, 21, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 6, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 1, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 5, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 1, 5, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 20, 22, nPointsOnSpline);
    addEdgeNoSurface(21, 23, nPointsOnSpline);
    addEdgeNoSurface(2, 6, nPointsOnSpline);
    //iwww
    addEdgeInSurface(_surfP[dtSouth], 12, 32, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 14, 33, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 32, 34, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 33, 35, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 16, 34, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 18, 35, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 12, 16, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 14, 18, nPointsOnSpline);
    addEdgeNoSurface(12, 14, nPointsOnSpline);
    addEdgeNoSurface(32, 33, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 34, 35, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 16, 18, nPointsOnSpline);    
    //bee
    addEdgeInSurface(_surfP[dtSouth], 20, 36, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 22, 38, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 36, 37, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 38, 39, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 21, 37, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 23, 39, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 20, 21, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 22, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 20, 22, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 36, 38, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtBack], 37, 39, nPointsOnSpline);    
    addEdgeNoSurface(21, 23, nPointsOnSpline);
    //be
    addEdgeInSurface(_surfP[dtSouth], 21, 37, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 23, 39, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 37, 40, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 39, 41, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 24, 40, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 26, 41, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 21, 24, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 23, 26, nPointsOnSpline);
    addEdgeNoSurface(21, 23, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtBack], 37, 39, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtBack], 40, 41, nPointsOnSpline);    
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode24u],
                        _val[dtNode24v],            
                        _val[dtNode26u],
                        _val[dtNode26v]
                      ),
      0., 1., 24, 26, nPointsOnSpline 
    );
    //bw
    addEdgeInSurface(_surfP[dtSouth], 32, 24, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 33, 26, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 24, 40, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 26, 41, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 42, 40, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 43, 41, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 32, 42, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 33, 43, nPointsOnSpline);
    addEdgeNoSurface(32, 33, nPointsOnSpline);    
    //edge 24-26 already in mesh
    addEdgeNoSurface(21, 23, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtBack], 40, 41, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtBack], 42, 43, nPointsOnSpline);    
    //bww
    addEdgeInSurface(_surfP[dtSouth], 32, 42, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 33, 43, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 42, 44, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 43, 45, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 34, 44, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 35, 45, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 33, 35, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 32, 34, nPointsOnSpline);
    addEdgeNoSurface(32, 33, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtBack], 42, 43, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtBack], 44, 45, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 34, 35, nPointsOnSpline);
    //iee
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode25u],
                        _val[dtNode25v],
                        _val[dtNode24u],
                        _val[dtNode24v]
                      ),
      0., 1., 25, 24, nPointsOnSpline 
    ); 
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                       _val[dtNode27u],
                        _val[dtNode27v],
                        _val[dtNode26u],
                        _val[dtNode26v]
                      ),
      0., 1., 27, 26, nPointsOnSpline             
    ); 
    addEdgeInSurface(_surfP[dtSouth], 25, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 27, 6, nPointsOnSpline);    
    //ie
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode25u],
                        _val[dtNode25v],
                        _val[dtNode28u],
                        _val[dtNode28v]
                      ),
      0., 1., 25, 28, nPointsOnSpline 
    ); 
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                       _val[dtNode27u],
                        _val[dtNode27v],
                        _val[dtNode29u],
                        _val[dtNode29v]
                      ),
      0., 1., 27, 29, nPointsOnSpline             
    ); 
    addEdgeInSurface(_surfP[dtSouth], 8, 28, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 10, 29, nPointsOnSpline);  
    //iw
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode28u],
                        _val[dtNode28v],
                        _val[dtNode30u],
                        _val[dtNode30v]
                      ),
      0., 1., 28, 30, nPointsOnSpline 
    ); 
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                       _val[dtNode29u],
                        _val[dtNode29v],
                        _val[dtNode31u],
                        _val[dtNode31v]
                      ),
      0., 1., 29, 31, nPointsOnSpline             
    );     
    addEdgeInSurface(_surfP[dtSouth], 12, 30, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 14, 31, nPointsOnSpline);  
    //iww
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode30u],
                        _val[dtNode30v],
                        1.,
                        _val[dtNode24v]
                      ),
      0., 1., 30, 24, nPointsOnSpline 
    ); 
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                       _val[dtNode31u],
                        _val[dtNode31v],
                        1.,
                        _val[dtNode26v]
                      ),
      0., 1., 31, 26, nPointsOnSpline             
    );     
    
    //
    // block
    //
    addBlock(0,1,2,3,4,5,6,7, "fee");
    addBlock(3,2,8,9,7,6,10,11, "fe");
    addBlock(9,8,12,13,11,10,14,15, "fw");
    addBlock(13,12,16,17,15,14,18,19, "fww");
    addBlock(1,20,21,2,5,22,23,6, "ieee");  
    addBlock(12,32,34,16,14,33,35,18, "iwww"); 
    addBlock(20,36,37,21,22,38,39,23, "bee");
    addBlock(21,37,40,24,23,39,41,26, "be");
    addBlock(32,24,40,42,33,26,41,43, "bw");
    addBlock(32,42,44,34,33,43,45,35, "bww");
    addBlock(2,21,24,25,6,23,26,27, "iee");
    addBlock(2,25,28,8,6,27,29,10, "ie");
    addBlock(8,28,30,12,10,29,31,14, "iw");
    addBlock(12,30,24,32,14,31,26,33, "iww");
    
    //
    // number of element and grading
    //
    setNElements("fee", _int[dtAlpha], _int[dtEpsilon], _int[dtGamma]);
    setNElements("fe", _int[dtAlpha], _int[dtBeta], _int[dtGamma]);
    setNElements("fw", _int[dtAlpha], _int[dtBeta], _int[dtGamma]);
    setNElements("fww", _int[dtAlpha], _int[dtEpsilon], _int[dtGamma]);
    setNElements("ieee", _int[dtPhi], _int[dtEpsilon], _int[dtGamma]);
    setNElements("iwww", _int[dtPhi], _int[dtEpsilon], _int[dtGamma]);
    setNElements("bee", _int[dtPsi], _int[dtEpsilon], _int[dtGamma]);
    setNElements("be", _int[dtPsi], _int[dtDelta], _int[dtGamma]);
    setNElements("bw", _int[dtDelta], _int[dtPsi], _int[dtGamma]);
    setNElements("bww", _int[dtPsi], _int[dtEpsilon], _int[dtGamma]);
    setNElements("iee", _int[dtPhi], _int[dtDelta], _int[dtGamma]);
    setNElements("ie", _int[dtDelta], _int[dtBeta], _int[dtGamma]);
    setNElements("iw", _int[dtDelta], _int[dtBeta], _int[dtGamma]);
    setNElements("iww", _int[dtDelta], _int[dtPhi], _int[dtGamma]);
    setEdgeGrading("fee", _val[dtE], _val[dtE], _val[dtE], _val[dtE],  _val[dtD], _val[dtD], _val[dtD], _val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("fe", _val[dtE], _val[dtE], _val[dtE], _val[dtE], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("fw", _val[dtE], _val[dtE], _val[dtE], _val[dtE], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("fww", _val[dtE], _val[dtE], _val[dtE], _val[dtE], 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("ieee", 1, 1, 1, 1, _val[dtD], _val[dtD], _val[dtD], _val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("iee", 1, 1, 1, 1, _val[dtB], _val[dtB], _val[dtB], _val[dtB], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("ie", _val[dtB], _val[dtB], _val[dtB], _val[dtB], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("iw", _val[dtB], _val[dtB], _val[dtB], _val[dtB], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("iww", _val[dtB], _val[dtB], _val[dtB], _val[dtB], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("iwww", 1, 1, 1, 1, 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("bee", _val[dtC], _val[dtC], _val[dtC], _val[dtC], _val[dtD], _val[dtD], _val[dtD], _val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("be", _val[dtC], _val[dtC], _val[dtC], _val[dtC], _val[dtB], 1, 1, _val[dtB], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("bw", _val[dtB], 1, 1, _val[dtB], _val[dtC], _val[dtC], _val[dtC], _val[dtC], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("bww", _val[dtC], _val[dtC], _val[dtC], _val[dtC], 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], 1./_val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);

    //
    //patches
    //
    addPatch("front", 0, 4, 7, 3);
    addPatch("front", 3, 7, 11, 9);
    addPatch("front", 9, 11 , 15, 13);
    addPatch("front", 13, 15, 19, 17);
    addPatch("south", 17, 13, 12, 16);
    addPatch("south", 13, 9, 8, 12);
    addPatch("south", 9, 3, 2, 8);
    addPatch("south", 3, 0, 1, 2);
    addPatch("south", 16, 12, 32, 34);
    addPatch("south", 12, 30, 24, 32);
    addPatch("south", 12, 8, 28, 30);
    addPatch("south", 8, 2, 25, 28);
    addPatch("south", 25, 2, 21, 24);
    addPatch("south", 2, 1, 20, 21);
    addPatch("south", 34, 32, 42, 44);
    addPatch("south", 32, 24, 40, 42);
    addPatch("south", 24, 21, 37, 40);
    addPatch("south", 21, 20, 36, 37);
    addPatch("north", 19, 15, 14, 18);
    addPatch("north", 15, 11, 10, 14);
    addPatch("north", 11, 7, 6, 10);
    addPatch("north", 7, 4, 5, 6);
    addPatch("north", 18, 14, 33, 35);
    addPatch("north", 14, 31, 26, 33);
    addPatch("north", 14, 10, 29, 31);
    addPatch("north", 10, 6, 27, 29);
    addPatch("north", 6, 23, 26, 27);
    addPatch("north", 6, 5, 22, 23);
    addPatch("north", 35, 33, 43, 45);
    addPatch("north", 33, 26, 41, 43);
    addPatch("north", 26, 23, 39, 41);
    addPatch("north", 23, 22, 38, 39);
    addPatch("back", 44, 42, 43, 45);    
    addPatch("back", 42, 40, 41, 43);    
    addPatch("back", 40, 37, 39, 41);    
    addPatch("back", 37, 36, 38, 39);
    addPatch("internal", 30, 31, 26, 24);
    addPatch("internal", 30, 28, 29, 31);
    addPatch("internal", 28, 25, 27, 29);
    addPatch("internal", 25, 24, 26, 27);
    addPatch("east", 4, 5, 1, 0);
    addPatch("east", 1, 20, 22, 5);
    addPatch("east", 20, 36, 38, 22);
    addPatch("west", 18, 19, 17, 16);
    addPatch("west", 34, 16, 18, 35);
    addPatch("west", 44, 34, 35, 45);
  }
  
  void coVolume::writeGrid( void ) {  
    //
    // push to blockMeshDictWriter
    //
    pushGridToBlockMeshDictWriter();
    
    std:string cmd;
    systemHandling systemH;
    
    //
    //prepare case
    //
    dt__info(writeGrid(),
            << "Preparing case: " << _caseDirectory );    
    cmd = "sh " + getOption("prepareCase_script") + " " + _caseDirectory;    
    systemH.command(cmd);

    //
    // write blockMeshDict
    //    
    writeDict( _caseDirectory );

    //
    // create grid with shell script
    //
    cmd = "sh " + _caseDirectory + "/run_blockMesh.sh";
    systemH.command(cmd);
  }

  void coVolume::writeCheapGrid( void ) {
    //
    // push to blockMeshDictWriter
    //
    pushGridToBlockMeshDictWriter();
  }
  
  covise::coDoSet * coVolume::toCoDoSet(char const * const str) const {
    coDoSet * retSet = blockMeshDictWriter::toCoDoSet(str);
    retSet->addAttribute("COLOR", "yellow");
    
    return retSet;
  }
  
  void coVolume::autoAdjust( void ) { 
    // t: trailingEdge (Hinterkante)
    // l: leadingEdge (Vorderkante)
    // H: hub (Nabe)
    // S: shroud (Kranz)
    dtPoint2 tPoiInternalH = _surfP[dtInternal]->getClosestPointParameterPercent(
                              _surfP[dtSouth]->getPointPercent(0.5,1.)
                            );         
    dtPoint2 tPoiH = _surfP[dtSouth]->getClosestPointParameterPercent(
                      _surfP[dtInternal]->getPointPercent(
                                            tPoiInternalH.x(),
                                            tPoiInternalH.y()
                      )
                    );
    dtPoint2 lPoiInternalH = _surfP[dtInternal]->getClosestPointParameterPercent(
                              _surfP[dtSouth]->getPointPercent(0.5,0)
                            );     
    dtPoint2 lPoiH = _surfP[dtSouth]->getClosestPointParameterPercent(
                      _surfP[dtInternal]->getPointPercent(
                                            lPoiInternalH.x(),
                                            lPoiInternalH.y()
                      )
                    );
    
    dtPoint2 tPoiInternalS = _surfP[dtInternal]->getClosestPointParameterPercent(
                              _surfP[dtNorth]->getPointPercent(0.5,1.)
                            );         
    dtPoint2 tPoiS = _surfP[dtNorth]->getClosestPointParameterPercent(
                      _surfP[dtInternal]->getPointPercent(
                                            tPoiInternalS.x(),
                                            tPoiInternalS.y()
                      )
                    );
    dtPoint2 lPoiInternalS = _surfP[dtInternal]->getClosestPointParameterPercent(
                              _surfP[dtNorth]->getPointPercent(0.5,0)
                            );     
    dtPoint2 lPoiS = _surfP[dtNorth]->getClosestPointParameterPercent(
                      _surfP[dtInternal]->getPointPercent(
                                            lPoiInternalS.x(),
                                            lPoiInternalS.y()
                      )
                    );
    dt__info(autoAdjust(),
            << "trailingEdgePoint (hub) >> " << dt__point2d(tPoiH) << std::endl
            << "trailingEdgePoint (shroud) >> " << dt__point2d(tPoiS) << std::endl
            << "leadingEdgePoint (hub) >> " << dt__point2d(lPoiH) << std::endl
            << "leadingEdgePoint (shroud) >> " << dt__point2d(lPoiS) );

    //
    //adjust constValues
    //
    _val[dtNode16v] = lPoiH.y() * _val[dtNode16v];
    _val[dtNode12v] = lPoiH.y() * _val[dtNode12v];
    _val[dtNode8v] = lPoiH.y() * _val[dtNode8v];
    _val[dtNode2v] = lPoiH.y() * _val[dtNode2v];
    _val[dtNode1v] = lPoiH.y() * _val[dtNode1v];
    _val[dtNode18v] = lPoiS.y() * _val[dtNode18v];
    _val[dtNode14v] = lPoiS.y() * _val[dtNode14v];
    _val[dtNode10v] = lPoiS.y() * _val[dtNode10v];
    _val[dtNode6v] = lPoiS.y() * _val[dtNode6v];
    _val[dtNode5v] = lPoiS.y() * _val[dtNode5v];
    _val[dtNode35v] = tPoiS.y() + (1. - tPoiS.y()) * _val[dtNode35v];
    _val[dtNode33v] = tPoiS.y() + (1. - tPoiS.y()) * _val[dtNode33v];
    _val[dtNode23v] = tPoiS.y() + (1. - tPoiS.y()) * _val[dtNode23v];
    _val[dtNode22v] = tPoiS.y() + (1. - tPoiS.y()) * _val[dtNode22v];
    _val[dtNode34v] = tPoiH.y() + (1. - tPoiH.y()) * _val[dtNode34v];
    _val[dtNode32v] = tPoiH.y() + (1. - tPoiH.y()) * _val[dtNode32v];
    _val[dtNode21v] = tPoiH.y() + (1. - tPoiH.y()) * _val[dtNode21v];
    _val[dtNode20v] = tPoiH.y() + (1. - tPoiH.y()) * _val[dtNode20v];    
  }  
  
  covise::coDoSet * coVolume::toCoDoUnstructuredGrid(char const * const str) const {
    covise::coDistributedObject ** elem = new covise::coDistributedObject*[2];
    
        
    elem[0] = ReadFOAM(0, NULL).loadMesh(
                _caseDirectory+"/constant/polyMesh", 
                _caseDirectory+"/constant/polyMesh",
                ("coVolume::coDoUnstructuredGrid::"+getName()).c_str()
              );
    elem[1] = NULL;
    return new covise::coDoSet(str, elem);
  }  
 }
