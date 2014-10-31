#include "surfacePointConstruct.h"
#include "geometryEngine/dtSislCurve.h"
#include "progHelper.h"
#include "geometryEngine/dtSislSurf.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>

#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

namespace dtOO {	
  surfacePointConstruct::surfacePointConstruct(
      std::vector< std::vector< dtPoint3 > > const & points, 
      int const & orderOne, 
      int const & orderTwo
    ) {
    int openOneInt = 0;
    int openTwoInt = 0;
    std::vector< double > PointList;
    int nPointsFD = points.size();
    int nPointsSD = points.at(0).size();
    for(int ii=0;ii<nPointsFD;ii++) {
      int SDChecker = points.at(ii).size();
      if (SDChecker != nPointsSD) {
        dt__THROW(surfacePointConstruct(),
                << "different dimensions" << LOGDEL
                << "nPointsFD = " << nPointsFD << LOGDEL
                << "nPointsSD = " << nPointsSD << LOGDEL
                << "SDChecker = " << SDChecker);
      }
      for(int jj=0;jj<SDChecker;jj++) {
        PointList.push_back( (double) points.at(ii).at(jj).x() );
        PointList.push_back( (double) points.at(ii).at(jj).y() );
        PointList.push_back( (double) points.at(ii).at(jj).z() );
      }
      if ( CGAL::compare_xyz( points.at(ii).at(0), 
                          points.at(ii).at(SDChecker-1) ) != CGAL::EQUAL ) {
        openOneInt++;
      }
    }
    for(int ii=0;ii<nPointsFD;ii++) {
      if ( CGAL::EQUAL != CGAL::compare_xyz( points.at(0).at(ii), points.back().at(ii) ) ) {
        openTwoInt++;
      }
    }
    int openOne = 0;
    int openTwo = 0;
    if (openTwoInt == nPointsFD) {
      openTwo = 1;
      }
    if (openOneInt == nPointsSD) {
      openOne = 1;
    }    
    
    int ipar = 2;
    int errFlag;
		SISLSurf * surf;
    s1620(&(PointList[0]), 
          nPointsSD, 
          nPointsFD, 
          ipar, 
          openOne, 
          openTwo, 
          orderOne, 
          orderTwo, 
          3, 
          &surf, 
          &errFlag);

    dtSislBase().allRight(errFlag, __FILE__, __LINE__);
		
		_dtS.reset( new dtSislSurf(*surf) );
		
		delete surf;
	}
	
  surfacePointConstruct::~surfacePointConstruct() {
		
	}
  
	dtSurface * surfacePointConstruct::result( void ) {
		return _dtS->clone();
	}

}
