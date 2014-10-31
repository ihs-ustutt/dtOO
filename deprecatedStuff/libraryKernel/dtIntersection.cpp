#include "dtIntersection.h"
#include "geometryEngine/dtCurve.h"

#include <logMe/logMe.h>

#include <sisl.h>

namespace dtOO {
  DTCLASSLOGMETHODIN(dtIntersection);
  
  dtIntersection::dtIntersection() {
  }

  dtIntersection::dtIntersection( dtSislSurf const & surf1, dtSislSurf const & surf2 ) {
    int errFlag;

    s1859(
      const_cast< SISLSurf * >( surf1.getSISLSurf() ), 
      const_cast< SISLSurf * >( surf2.getSISLSurf() ), 
      0.,
      dtSislBase::refToGeometricResolution() * 100., 
      &_nIntPt, 
      &_uPt,
      &_vPt,
      &_nIntCr, 
      &_intCurves, 
      &errFlag
    );
      
    allRight(errFlag);
    
    _surf[0] = dtSislSurf( surf1 );
    _surf[1] = dtSislSurf( surf2 );

    for (int ii=0; ii<_nIntCr;ii++) {
      s1310(
        const_cast< SISLSurf * >(_surf[0].getSISLSurf()), 
        const_cast< SISLSurf * >(_surf[1].getSISLSurf()), 
        _intCurves[ii], 
        dtSislBase::refToGeometricResolution() * 100, 
        0., 
        2, 
        0, 
        &errFlag
      );
    }
    allRight(errFlag);    
//    dump();
  }
  
  void dtIntersection::dump( void ) const {
    DTBUFFERINIT();
    DTBUFFER( << DTLOGEVAL(_nIntPt) << LOGDEL );
    for (int ii=0; ii<_nIntPt; ii++) {
      DTBUFFER(<< "intPoint = ( " << _uPt[ii] << ", " << _vPt[ii] << " )" << LOGDEL);
    }
    DTBUFFER( << DTLOGEVAL(_nIntCr) << LOGDEL );
    for (int ii=0; ii<_nIntCr; ii++) {
      DTBUFFER( << "_intCurves[" << ii << "]->ipoint = " << _intCurves[ii]->ipoint << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->ipar1 = " << _intCurves[ii]->ipar1 << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->ipar2 = " << _intCurves[ii]->ipar2 << LOGDEL );
      for (int jj=0; jj<_intCurves[ii]->ipoint; jj++) {
        DTBUFFER( << "_intCurves[" << ii << "]->epar1[" << jj << "] = " << _intCurves[ii]->epar1[jj] << LOGDEL );
      }
      for (int jj=0; jj<_intCurves[ii]->ipoint; jj++) {
        DTBUFFER( << "_intCurves[" << ii << "]->epar2[" << jj << "] = " << _intCurves[ii]->epar2[jj] << LOGDEL );
      }
      DTBUFFER( << "_intCurves[" << ii << "]->pgeom = " << _intCurves[ii]->pgeom << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->ppar1 = " << _intCurves[ii]->ppar1 << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->ppar2 = " << _intCurves[ii]->ppar2 << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->itype = " << _intCurves[ii]->itype << LOGDEL
                << " (" << intCurveKindStr[_intCurves[ii]->itype] << ") " << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->pretop[0] = " << _intCurves[ii]->pretop[0] << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->pretop[1] = " << _intCurves[ii]->pretop[1] << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->pretop[2] = " << _intCurves[ii]->pretop[2] << LOGDEL );
      DTBUFFER( << "_intCurves[" << ii << "]->pretop[3] = " << _intCurves[ii]->pretop[3] << LOGDEL );
    }
    DTDEBUGWF_BUFFER(dump());
  }
  
  int dtIntersection::getNCurves( void ) const {
    return _nIntCr;
  }

  dtCurve dtIntersection::getCurve( int const num ) const {    
    if (_nIntCr == 0) {    
      dt__THROW(getCurve(),
        << "No intersection curve!");
    }
    
    return dtCurve( *(_intCurves[num]->pgeom) );
  }

  dtCurve dtIntersection::getEquiResplinedCurve( int const cId, int const sId, int const nPoints, int const order ) const {
    dtCurve dtC = getCurve(cId);
    
    std::vector< dtPoint2 > pUV;
    for (int ii=0; ii<nPoints; ii++) {
      float iiF = static_cast< float >(ii);
      float nPointsF = static_cast< float >(nPoints);
      
      pUV.push_back(
        _surf[sId].getClosestPointParameter( 
          dtC.getPointLengthPercent3d( iiF / (nPointsF-1.) )
        )
      );
      
    }
    dtCurve dtCUV( &pUV, order);
    
    return *(_surf[sId].getCurveSpline( &dtCUV ));
    
  }
  
  dtIntersection::~dtIntersection() {
  }
}
