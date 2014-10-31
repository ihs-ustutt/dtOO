#include "cVolume.h"
#include "analyticGeometryHeaven/splineCurve3d.h"
#include "interfaceHeaven/systemHandling.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <functionHeaven/constValue.h>
#include <ReadFOAM.h>
#include <logMe/logMe.h>
#include <string>
#include <do/coDoSet.h>
#include <do/coDoUnstructuredGrid.h>

namespace dtOO {
  DTCLASSLOGMETHODI(cVolume,
          << "No member attributes.");
  
  cVolume::cVolume() {
    _surfP.resize(7);
    _val.resize(_nGridVal);
    _valCValP.resize(_nGridVal);    
    _int.resize(_nGridInt);
  }

  cVolume::cVolume(const cVolume& orig) {
  }

  cVolume::~cVolume() {
    _valCValP.clear();
    _intCValP.clear();
    _int.clear();
    _val.clear();
  }

  void cVolume::init( QDomElement const & element,
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
      //
      // get surface
      //      
      analyticGeometry const * aG = depAGeoP->get(label);
      analyticSurface const * aS = dynamic_cast< analyticSurface const * >(aG);
      if (!aS) {
        dt__THROW(init,
                DTLOGEVAL(aS) << LOGDEL
                << "dynamic_cast of analyticGeometry " << aG->getName() 
                << " to analyticSurface fails.");
      }
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
    // set constValues float
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
      std::string attribute = getAttributeStr("attribute", wElement);  
      //
      // get surface
      //      
      constValue * cV = cValP->get(label);

      if (attribute == "dtNode2") {
        _val[dtNode2] = cV->getValue();
        _valCValP[dtNode2] = cV;             
      }
      else if (attribute == "dtNode6") {
        _val[dtNode6] = cV->getValue();
        _valCValP[dtNode6] = cV;         
      }
      else if (attribute == "dtNode1") {
        _val[dtNode1] = cV->getValue();
        _valCValP[dtNode1] = cV;                    
      }
      else if (attribute == "dtNode5") {
        _val[dtNode5] = cV->getValue();
        _valCValP[dtNode5] = cV;      
      }
      else if (attribute == "dtNode9") {
        _val[dtNode9] = cV->getValue();
        _valCValP[dtNode9] = cV;     
      }
      else if (attribute == "dtNode11") {
        _val[dtNode11] = cV->getValue();
        _valCValP[dtNode11] = cV;      
      }
      else if (attribute == "dtNode8") {
        _val[dtNode8] = cV->getValue();
        _valCValP[dtNode8] = cV;        
      }
      else if (attribute == "dtNode10") {
        _val[dtNode10] = cV->getValue();
        _valCValP[dtNode10] = cV;   
      }
      else if (attribute == "dtNode13") {
        _val[dtNode13] = cV->getValue();
        _valCValP[dtNode13] = cV;
      }
      else if (attribute == "dtNode15") {
        _val[dtNode15] = cV->getValue();
        _valCValP[dtNode15] = cV;          
      }
      else if (attribute == "dtNode16") {
        _val[dtNode16] = cV->getValue();
        _valCValP[dtNode16] = cV;             
      }
      else if (attribute == "dtNode17") {
        _val[dtNode17] = cV->getValue();
        _valCValP[dtNode17] = cV;           
      }
      else if (attribute == "dtNode18u") {
        _val[dtNode18u] = cV->getValue();
        _valCValP[dtNode18u] = cV;
      }
      else if (attribute == "dtNode18v") {
        _val[dtNode18v] = cV->getValue();
        _valCValP[dtNode18v] = cV;     
      }
      else if (attribute == "dtNode19u") {
        _val[dtNode19u] = cV->getValue();
        _valCValP[dtNode19u] = cV;
      }
      else if (attribute == "dtNode19v") {
        _val[dtNode19v] = cV->getValue();
        _valCValP[dtNode19v] = cV;
      }
      else if (attribute == "dtNode22") {
        _val[dtNode22] = cV->getValue();
        _valCValP[dtNode22] = cV;            
      }
      else if (attribute == "dtNode23") {
        _val[dtNode23] = cV->getValue();
        _valCValP[dtNode23] = cV;
      }
      else if (attribute == "dtA") {
        _val[dtA] = cV->getValue();
        _valCValP[dtA] = cV;
      }
      else if (attribute == "dtB") {
        _val[dtB] = cV->getValue();
        _valCValP[dtB] = cV;
      }
      else if (attribute == "dtC") {
        _val[dtC] = cV->getValue();
        _valCValP[dtC] = cV;
      }
      else if (attribute == "dtD") {
        _val[dtD] = cV->getValue();
        _valCValP[dtD] = cV;
      }
      else if (attribute == "dtE") {
        _val[dtE] = cV->getValue();
        _valCValP[dtE] = cV;
      }
      else if (attribute == "dtF") {
        _val[dtF] = cV->getValue();
        _valCValP[dtF] = cV;
      }      
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
      std::string attribute = getAttributeStr("attribute", wElement);  
      //
      // get surface
      //      
      constValue * cV = cValP->get(label);
      if (!cV) {
        dt__THROW(init,
                DTLOGEVAL(cV) << LOGDEL
                << "constValue " << DTLOGEVAL(label) 
                << " not in vector _cValP.");
      }
      if (attribute == "dtAlpha") {
        _int[dtAlpha] = cV->getValue();
      }
      else if (attribute == "dtBeta") {
        _int[dtBeta] = cV->getValue();
      }
      else if (attribute == "dtGamma") {
        _int[dtGamma] = cV->getValue();
      }
      else if (attribute == "dtDelta") {
        _int[dtDelta] = cV->getValue();
      }
      else if (attribute == "dtEpsilon") {
        _int[dtEpsilon] = cV->getValue();
      }
      else if (attribute == "dtPhi") {
        _int[dtPhi] = cV->getValue();
      }
      else if (attribute == "dtPsi") {
        _int[dtPsi] = cV->getValue();
      }
      
      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);
    }

    //
    // automatic adjust
    //
    if ( optionTrue("auto_adjust") ) {
      autoAdjust();
    }

    dump();
  }  
  
  void cVolume::dump(void) const {
    DTBUFFERINIT();
    dt__FORALL(_surfP, ii,
      DTBUFFER( << "_surfP[" << ii << "] = " << _surfP[ii] 
            << " label = " << _surfP[ii]->getName() << LOGDEL );
    );
    dt__FORALL(_val, ii,
      DTBUFFER( << "_val[" << ii << "] = " << _val[ii] << LOGDEL );
    );       
    dt__FORALL(_int, ii,
      DTBUFFER( << "_int[" << ii << "] = " << _int[ii] << LOGDEL );
    );    
    DTDEBUGWF_BUFFER(init());
  }
  
  void cVolume::pushGridToBlockMeshDictWriter( void ) {
    int nPointsOnSpline = 100;

    //
    // clear
    //
    blockMeshDictWriter::clear();
    
    //--------------------------------------------------------------------------
    // front block
    //--------------------------------------------------------------------------
    //
    // vertices
    //
    addVertex( _surfP[dtSouth]->getPointPercent(1.,0.), 0 );
    addVertex( _surfP[dtSouth]->getPointPercent(1., _val[dtNode1]), 1 );
    addVertex( _surfP[dtSouth]->getPointPercent(0.,_val[dtNode2]), 2 );
    addVertex( _surfP[dtSouth]->getPointPercent(0.,0.), 3 );
    addVertex( _surfP[dtNorth]->getPointPercent(1.,0.), 4 );
    addVertex( _surfP[dtNorth]->getPointPercent(1., _val[dtNode5]), 5 );
    addVertex( _surfP[dtNorth]->getPointPercent(0.,_val[dtNode6]), 6 );
    addVertex( _surfP[dtNorth]->getPointPercent(0.,0.), 7 );
    //
    // edges
    //
    addEdgeInSurface(_surfP[dtSouth], 0, 1, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 3, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 7, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 4, 5, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 0, 3, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 1, 2, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 5, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 4, 7, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 0, 4, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 1, 5, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 2, 6, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtWest], 3, 7, nPointsOnSpline);

    //--------------------------------------------------------------------------
    // front internal block
    //--------------------------------------------------------------------------    
    //
    // vertices
    //
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode8], 0.), 8 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode9],0.), 9 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode10],1.), 10 );
    addVertex( _surfP[dtInternal]->getPointPercent(_val[dtNode11],1.), 11 );
    
    //
    // edges
    //
    addEdgeInSurface(_surfP[dtSouth], 8, 1, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtNorth], 10, 5, nPointsOnSpline);        
    addEdgeInSurface(_surfP[dtSouth], 9, 2, nPointsOnSpline);     
    addEdgeInSurface(_surfP[dtNorth], 11, 6, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtInternal], 8, 9, nPointsOnSpline);        
    addEdgeInSurface(_surfP[dtInternal], 10, 11, nPointsOnSpline);            
    addEdgeInSurface(_surfP[dtInternal], 9, 11, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtInternal], 8, 10, nPointsOnSpline);    
    //--------------------------------------------------------------------------
    // west internal block
    //--------------------------------------------------------------------------       
    //
    // vertices
    //
    addVertex(_surfP[dtInternal]->getPointPercent(0.,0.), 12);
    addVertex(_surfP[dtWest]->getPointPercent(_val[dtNode13], 0.), 13);
    addVertex(_surfP[dtInternal]->getPointPercent(0., 1.), 14);   
    addVertex(_surfP[dtWest]->getPointPercent(_val[dtNode15], 1.), 15);       
    //
    // edges
    //
    addEdge(_surfP[dtInternal]->getSplineCurve3dLinearPercent(_val[dtNode9],0.,1.,0.),0., 1., 9, 12, nPointsOnSpline );
    addEdgeInSurface(_surfP[dtSouth], 12, 13, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtSouth], 2, 13, nPointsOnSpline);    
    addEdge(_surfP[dtInternal]->getSplineCurve3dLinearPercent(_val[dtNode11],1.,1.,1.),0., 1., 11, 14, nPointsOnSpline );
    addEdgeInSurface(_surfP[dtNorth], 14, 15, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtNorth], 6, 15, nPointsOnSpline);    
    addEdge(_surfP[dtInternal]->getSplineCurve3dLinearPercent(0.,0.,0.,1.),0., 1., 12, 14, nPointsOnSpline );
    addEdgeInSurface(_surfP[dtWest], 13, 15, nPointsOnSpline);    
    //--------------------------------------------------------------------------
    // east internal block
    //--------------------------------------------------------------------------       
    //
    // vertices
    //
    addVertex(_surfP[dtEast]->getPointPercent(_val[dtNode16], 0.), 16);    
    addVertex(_surfP[dtEast]->getPointPercent(_val[dtNode17], 1.), 17);
    //
    // edges
    //
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode8],
                        0.,            
                        0,
                        0.
                      ),
      0., 1., 8, 12, nPointsOnSpline 
    );
    addEdgeInSurface(_surfP[dtSouth], 12, 16, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtEast], 1, 16, nPointsOnSpline);    
    addEdge(
      _surfP[dtInternal]->getSplineCurve3dLinearPercent(
                        _val[dtNode10],
                        1.,
                        0,
                        1.
                      ),
      0., 1., 10, 14, nPointsOnSpline 
    );
    addEdgeInSurface(_surfP[dtNorth], 14, 17, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtEast], 5, 17, nPointsOnSpline);    
    addEdgeInSurface(_surfP[dtEast], 16, 17, nPointsOnSpline);
    //--------------------------------------------------------------------------
    // back internal block
    //--------------------------------------------------------------------------       
    //
    // vertices
    //
    addVertex(_surfP[dtSouth]->getPointPercent(_val[dtNode18u], _val[dtNode18v]), 18);    
    addVertex(_surfP[dtNorth]->getPointPercent(_val[dtNode19u], _val[dtNode19v]), 19);
    //
    // edges
    //
    addEdgeInSurface(_surfP[dtSouth], 13, 18, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 16, 18, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 15, 19, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 17, 19, nPointsOnSpline);
    addEdgeNoSurface(18, 19, nPointsOnSpline);
    //--------------------------------------------------------------------------
    // back block east
    //--------------------------------------------------------------------------       
    //
    // vertices
    //
    addVertex(_surfP[dtSouth]->getPointPercent(1., 1.), 20);    
    addVertex(_surfP[dtNorth]->getPointPercent(1., 1.), 21);
    addVertex(_surfP[dtSouth]->getPointPercent(_val[dtNode22], 1.), 22);    
    addVertex(_surfP[dtNorth]->getPointPercent(_val[dtNode23], 1.), 23);        
    //
    // edges
    //
    addEdgeInSurface(_surfP[dtSouth], 16, 20, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 17, 21, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 18, 22, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 19, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 20, 22, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 21, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtEast], 20, 21, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtBack], 22, 23, nPointsOnSpline);

    //--------------------------------------------------------------------------
    // back block west
    //--------------------------------------------------------------------------       
    //
    // vertices
    //
    addVertex(_surfP[dtSouth]->getPointPercent(0.,1.), 24);    
    addVertex(_surfP[dtNorth]->getPointPercent(0.,1.), 25);
    //
    // edges
    //
    addEdgeInSurface(_surfP[dtSouth], 13, 24, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtSouth], 24, 22, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 15, 25, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtNorth], 25, 23, nPointsOnSpline);
    addEdgeInSurface(_surfP[dtBack], 24, 25, nPointsOnSpline);
    
    //
    // block
    //
    addBlock(0,1,2,3,4,5,6,7, "front");
    addBlock(8,9,2,1,10,11,6,5, "frontInternal");
    addBlock(9,12,13,2,11,14,15,6, "westInternal");
    addBlock(8,1,16,12,10,5,17,14, "eastInternal");
    addBlock(12,16,18,13,14,17,19,15, "backInternal");  
    addBlock(16,20,22,18,17,21,23,19, "backEast"); 
    addBlock(13,18,22,24,15,19,23,25, "backWest");
    
//    //
//    // number of element and grading
//    //
//    _alpha = 10;
//    _beta = 5;
//    _gamma = 10;
//    _delta = 20;
//    _epsilon = 20;
//    _phi = 5;    
//    _alphaGrading = 1;
//    _betaGrading = 1;
//    _gammaGrading = -10;
//    _deltaGrading = 5;
//    _epsilonGrading = 1;
//    _phiGrading = 1;    
    setNElements("front", _int[dtAlpha], _int[dtBeta], _int[dtGamma]);
    setNElements("frontInternal", _int[dtBeta], _int[dtDelta], _int[dtGamma]);
    setNElements("eastInternal", _int[dtDelta], _int[dtEpsilon], _int[dtGamma]);
    setNElements("westInternal", _int[dtPhi], _int[dtDelta], _int[dtGamma]);
    setNElements("backInternal", _int[dtDelta], _int[dtDelta], _int[dtGamma]);
    setNElements("backWest", _int[dtDelta], _int[dtPsi], _int[dtGamma]);
    setNElements("backEast", _int[dtPsi], _int[dtDelta], _int[dtGamma]);
    setEdgeGrading("front", 1, 1, 1, 1, _val[dtB], _val[dtB], _val[dtB], _val[dtB], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("frontInternal", _val[dtB], _val[dtB], _val[dtB], _val[dtB], _val[dtC], _val[dtD], _val[dtD], _val[dtC], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("westInternal", _val[dtF], 1, 1, _val[dtF], _val[dtD], _val[dtD], _val[dtD], _val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("eastInternal", _val[dtC], _val[dtC], _val[dtC], _val[dtC], _val[dtF], 1, 1, _val[dtF], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("backInternal", _val[dtC], _val[dtE], _val[dtE], _val[dtC], _val[dtD], _val[dtE], _val[dtE], _val[dtD], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("backEast", 1, 1, 1, 1, _val[dtE], _val[dtE], _val[dtE], _val[dtE], _val[dtA], _val[dtA], _val[dtA], _val[dtA]);
    setEdgeGrading("backWest", _val[dtE], _val[dtE], _val[dtE], _val[dtE], 1, 1, 1, 1, _val[dtA], _val[dtA], _val[dtA], _val[dtA]);

    //
    //patches
    //
    addPatch("front", 0, 4, 7, 3);
    addPatch("internal", 8, 9, 11, 10);
    addPatch("internal", 9, 12, 14, 11);
    addPatch("internal", 12, 8, 10, 14);
    addPatch("east", 3, 7, 6, 2);    
    addPatch("east", 2, 6, 15, 13);
    addPatch("east", 13, 15, 25, 24);
    addPatch("west", 0, 4, 5, 1);
    addPatch("west", 1, 5, 17, 16); 
    addPatch("west", 16, 17, 21, 20);
    addPatch("back", 24, 25, 23, 22);
    addPatch("back", 22, 23, 21, 20);
    addPatch("south", 2, 1, 0, 3);
    addPatch("south", 9, 8, 1, 2);
    addPatch("south", 13, 12, 9, 2);
    addPatch("south", 16, 1, 8, 12);
    addPatch("south", 18, 16, 12, 13);
    addPatch("south", 22, 20, 16, 18);
    addPatch("south", 24, 22, 18, 13);
    addPatch("north", 5, 6, 7, 4);
    addPatch("north", 10, 11, 6, 5);
    addPatch("north", 14, 15, 6, 11);
    addPatch("north", 17, 14, 10, 5);
    addPatch("north", 19, 15, 14, 17);
    addPatch("north", 21, 23, 19, 17);
    addPatch("north", 23, 25, 15, 19);
  }
  
  void cVolume::writeGrid( void ) {  
    //
    // push to blockMeshDictWriter
    //
    pushGridToBlockMeshDictWriter();
    
    std:string cmd;
    systemHandling systemH;
    
    //
    //prepare case
    //
    DTINFOWF(writeGrid(),
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

  void cVolume::writeCheapGrid( void ) {
    //
    // push to blockMeshDictWriter
    //
    pushGridToBlockMeshDictWriter();
  }
  
  covise::coDoSet * cVolume::toCoDoSet(char const * const str) const {
    coDoSet * retSet = blockMeshDictWriter::toCoDoSet(str);
    retSet->addAttribute("COLOR", "yellow");
    
    return retSet;
  }
  
  void cVolume::autoAdjust( void ) { 
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
    DTINFOWF(autoAdjust(),
            << "trailingEdgePoint (hub) >> " << DTLOGPOI2D(tPoiH) << LOGDEL
            << "trailingEdgePoint (shroud) >> " << DTLOGPOI2D(tPoiS) << LOGDEL
            << "leadingEdgePoint (hub) >> " << DTLOGPOI2D(lPoiH) << LOGDEL
            << "leadingEdgePoint (shroud) >> " << DTLOGPOI2D(lPoiS) );

    //
    //adjust constValues
    //
    _valCValP[dtNode2]->setRange(0., .99 * lPoiH.y());
    _valCValP[dtNode6]->setRange(0., .99 * lPoiS.y());
    _valCValP[dtNode1]->setRange(0., .99 * lPoiH.y());
    _valCValP[dtNode5]->setRange(0., .99 * lPoiS.y());
    _valCValP[dtNode9]->setRange(1.01 * lPoiInternalH.x(), 1.);
    _valCValP[dtNode11]->setRange(1.01 * lPoiInternalS.x(), 1.);
    _valCValP[dtNode8]->setRange(0., .99 * lPoiInternalH.x());
    _valCValP[dtNode10]->setRange(0., .99 * lPoiInternalS.x());
    _valCValP[dtNode13]->setRange(tPoiH.y(), .99);
    _valCValP[dtNode15]->setRange(tPoiS.y(), .99);
    _valCValP[dtNode16]->setRange(tPoiH.y(), .99);
    _valCValP[dtNode17]->setRange(tPoiS.y(), .99);
    _valCValP[dtNode18u]->setRange(.01, .99);
    _valCValP[dtNode19u]->setRange(.01, .99);
    _valCValP[dtNode18v]->setRange(1.01 * tPoiH.y(), .99);
    _valCValP[dtNode19v]->setRange(1.01 * tPoiS.y(), .99);
    _valCValP[dtNode22]->setRange(.01, .99);
    _valCValP[dtNode23]->setRange(.01, .99);
    
    DTBUFFERINIT();
    for(int ii=0;ii<_nGridVal;ii++) {
      DTBUFFER(
              << DTLOGEVAL(_valCValP[ii]->getMin()) << " " 
              << DTLOGEVAL(_valCValP[ii]->getMax()) << LOGDEL);
    }
    DTDEBUGWF_BUFFER(autoAdjust());
  }

  bool cVolume::parameterCheck(void) {
    if ( (_valCValP[dtNode18v]->getValue() <=  _valCValP[dtNode16]->getValue()) 
            || (_valCValP[dtNode18v]->getValue() <=  _valCValP[dtNode13]->getValue()) 
            || (_valCValP[dtNode19v]->getValue() <=  _valCValP[dtNode17]->getValue())
            || (_valCValP[dtNode19v]->getValue() <=  _valCValP[dtNode15]->getValue()) ) {
      
      DTINFOWF(parameterCheck(),
              << "parameterCheck fails." );
      return false;
    }
    return true;
  }  
  
  covise::coDoSet * cVolume::toCoDoUnstructuredGrid(char const * const str) const {
    covise::coDistributedObject ** elem = new covise::coDistributedObject*[2];
    
        
    elem[0] = ReadFOAM(0, NULL).loadMesh(
                _caseDirectory+"/constant/polyMesh", 
                _caseDirectory+"/constant/polyMesh",
                ("cVolume::coDoUnstructuredGrid::"+getName()).c_str()
              );
    elem[1] = NULL;
    return new covise::coDoSet(str, elem);
  }
  
 }
