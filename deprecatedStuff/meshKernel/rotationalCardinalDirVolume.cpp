#include "rotationalCardinalDirVolume.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <analyticFunctionHeaven/constValue.h>
#include <libraryKernel/dtIntersection.h>
#include <progHelper.h>

namespace dtOO {
  rotationalCardinalDirVolume::rotationalCardinalDirVolume() {
  }

  rotationalCardinalDirVolume::~rotationalCardinalDirVolume() {
  }

  void rotationalCardinalDirVolume::init(
    QDomElement const & element,
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< scaFunction * > const * const sFunP,
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< boundedVolume * > const * const depBVolP
  ) {
    //
    //reset first
    //
    _rS.destroy();
    
    //
    // init cardinalDir
    //
    cardinalDirVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);

    //
    // radial distribution
    //
    scaFunction * sF = sFunP->get( getOption("radial_distribution" ) );
    std::vector< float > fV = sF->getValueSet( getOptionInt("number_radial_points") );
    analyticSurface const * aS;
    rotatingSpline const * rS;
    
    aS = getPtrTM(fStrToId("dtNorth"));
    dt__mustCast(aS, rotatingSpline const, rS);
    aS = getPtrToSurf(fStrToId("dtSouth"));
    dt__mustCast(aS, rotatingSpline const, rS);
    
    aS = getPtrToSurf( fStrToId("dtWest") );
    
    for (int ii=1; ii<(fV.size()-1); ii++) {
      ptrHandling< analyticGeometry > sC3d( aS->getSplineCurve3dConstVPercent( fV[ii] ) );
      _rS.push_back( new rotatingSpline(sC3d.get(), rS->getOrigin(), rS->getRotVector(), rS->getAngle()) );
      dtIntersection dtInt(
        *(_rS.back()->getDtSislSurf()), 
        *( getPtrToSurf( fStrToId("dtInternal") )->getDtSislSurf() ) 
      );
      for (int jj=0; jj<dtInt.getNCurves(); jj++) {
        _s3d.push_back( new splineCurve3d( dtInt.getCurve(jj) ) );
        _s3d.push_back( new splineCurve3d( dtInt.getEquiResplinedCurve(jj, 0, 5, 2) ) );
        _s3d.back()->dump();
      }
    }
  }
  
  void rotationalCardinalDirVolume::writeGrid( void ) {
    
  }
  
  covise::coDoSet * rotationalCardinalDirVolume::toCoDoUnstructuredGrid(char const * const str) const {
  
  }   

  covise::coDoSet * rotationalCardinalDirVolume::toCoDoSet(char const * const str) const {
    coDoSet * cDirSet = cardinalDirVolume::toCoDoSet(str);
    cDirSet = _rS.toCoDoSet( cDirSet, _rS.toCoDoSet(_rS, str), str );
    return _s3d.toCoDoSet( cDirSet, _s3d.toCoDoSet(_s3d, str), str );
    
  }
}
