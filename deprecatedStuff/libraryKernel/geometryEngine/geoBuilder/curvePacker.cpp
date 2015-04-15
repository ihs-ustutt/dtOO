#include "curvePacker.h"
#include "progHelper.h"

#include <logMe/logMe.h>

#include <geometryEngine/dtSislCurve.h>
#include <sisl.h>
#include <sisl-copyright.h>
#include <sislP.h>

namespace dtOO {
	curvePacker::~curvePacker() {
	}

  curvePacker::curvePacker() {
		
	}    
	
  dtCurve * curvePacker::result( void ) {
		return _dtC->clone();
	}

  void curvePacker::pack(dtCurve const & cc, std::vector< int > * intArr, std::vector< double > * doubleArr) const {
    dtSislCurve const * dtCPtr;
		
		dt__CANDOWNCAST(&cc, dtSislCurve const, dtCPtr);
		
		if (dtCPtr) {
			//
			// get size and values of arraies
			//
			int etSize;
			int rcoefSize;
			int ecoefSize;
			sisl_newCurve(
				dtCPtr->SISLRef().in, 
				dtCPtr->SISLRef().ik, 
			  dtCPtr->SISLRef().et, 
				dtCPtr->SISLRef().ecoef, 
				dtCPtr->SISLRef().ikind, 
				dtCPtr->SISLRef().idim, 
				dtCPtr->SISLRef().icopy, 
				&etSize, &rcoefSize, &ecoefSize
			);

			//
			// fill int-array
			//
			intArr->clear();
			intArr->push_back(dtCPtr->SISLRef().ik);
			intArr->push_back(dtCPtr->SISLRef().in);
			intArr->push_back(dtCPtr->SISLRef().ikind);
			intArr->push_back(dtCPtr->SISLRef().idim);
			intArr->push_back(dtCPtr->SISLRef().icopy);
			intArr->push_back(dtCPtr->SISLRef().cuopen);
			intArr->push_back(etSize);
			intArr->push_back(rcoefSize);
			intArr->push_back(ecoefSize);

			//
			// fill double array
			//
			doubleArr->clear();
			int nPoints;
			//
			//et
			//
			nPoints = etSize;
			for (int ii=0;ii<nPoints;ii++) {
				doubleArr->push_back( dtCPtr->SISLRef().et[ii] );
			}
			//
			//ecoef
			//
			nPoints = ecoefSize;
			for (int ii=0;ii<nPoints;ii++) {
				doubleArr->push_back( dtCPtr->SISLRef().ecoef[ii] );
			}
			if ( (dtCPtr->SISLRef().ikind == 2) || (dtCPtr->SISLRef().ikind == 4) ) {
				//
				//rcoef
				//
				nPoints = rcoefSize;
				for (int ii=0;ii<nPoints;ii++) {
					doubleArr->push_back( dtCPtr->SISLRef().rcoef[ii] );
				}
			}
		}
		else {
			dt__THROW(
			  pack(), 
			  << "Unknown type." << std::endl
				<< "Should be dtSislCurve."
			);
		}
  }  

  curvePacker::curvePacker(int const * intArr, double const * doubleArr) {
		//
		// new curve object
		//
    SISLCurve curve;

    //
    // fill int-array
    //    
    curve.ik = intArr[0];
    curve.in = intArr[1];
    curve.ikind = intArr[2];
    curve.idim = intArr[3];
    curve.icopy = intArr[4];
    curve.cuopen = intArr[5];
    int etSize = intArr[6];
    int rcoefSize = intArr[7];
    int ecoefSize = intArr[8];
    curve.ecoef = NULL;
    curve.rcoef = NULL;
    curve.et = NULL;
    curve.pdir = NULL;
    curve.pbox = NULL;
    
    //
    // fill double array
    //
    int nPointsStart;
    int nPointsEnd;
    int counter;
    //
    //et1
    //
    nPointsStart = 0;
    nPointsEnd = etSize;
    counter = 0;
    curve.et = newarray(etSize, double);
    for (int ii=nPointsStart;ii<nPointsEnd;ii++) {
      curve.et[counter] = doubleArr[ii];
      counter++;
    }
    //
    //ecoef
    //
    nPointsStart = nPointsEnd;
    nPointsEnd = nPointsEnd + ecoefSize;
    counter = 0;
    curve.ecoef = newarray(ecoefSize, double);
    for (int ii=nPointsStart;ii<nPointsEnd;ii++) {
      curve.ecoef[counter] = doubleArr[ii];
      counter++;
    }
    if ( (curve.ikind == 2) || (curve.ikind == 4) ) {
      //
      //rcoef
      //
      nPointsStart = nPointsEnd;
      nPointsEnd = nPointsEnd + rcoefSize;
      counter = 0;
      curve.rcoef = newarray(rcoefSize, double);
      for (int ii=nPointsStart;ii<nPointsEnd;ii++) {
        curve.rcoef[counter] = doubleArr[ii];
        counter++;
      }
    }
		
		//
		// create curve
		//
		_dtC.reset( new dtSislCurve(curve) );
  }  
	
	void curvePacker::sisl_newCurve(
    int in, int ik, double const * const et, double const * const ecoef, int const ikind, int const idim, int const icopy, 
    int * const etSize, int * const rcoefSize, int * const ecoefSize
  ) const {
    int i, j, J, jj, k;		/* loop variables               */
    int k1,k2;                    /* Superflous knots in the ends. */
    int kdim;			/* Dimension of space (also including potential
             homogenous coordinate        */

    if (ikind == 2 || ikind == 4)
      kdim = idim + 1;
    else
      kdim = idim;

    /* Count superflous knots in the start.  */

    for (k1=0; k1<in; k1++)
       if (et[ik-1] < et[ik+k1]) break;

    /* Count superflous knots in the end.  */

    for (k2=0; k2<in; k2++)
       if (et[in] > et[in-1-k2]) break;

    /* Reduce knots and vertices according to k1 and k2.  */

    in -= (k1+k2);

    /* Initialize new curve.  */

    *etSize = in+ik;

    if (ikind == 2 || ikind == 4)
      {
        for (i = 0, j = 0, J = 0, k = idim; i < in; i++, k += kdim)
    {
      for (jj = 0; jj < idim; jj++, j++, J++)
      j++;
    }
        *ecoefSize = in*idim;
        *rcoefSize = in*kdim;
      }
    else
      {
        *ecoefSize = in*kdim;
        *rcoefSize = 0;        
      }
  }	
}
