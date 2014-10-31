#ifndef PERFROTATINGSPLINE_H
#define	PERFROTATINGSPLINE_H

#include "rotatingSpline.h"

#include <logMe/dtMacros.h>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class scaFunction;
  
  class perfRotatingSpline : public rotatingSpline {
    public:
      DTCLASSNAMEMETHOD(perfRotatingSpline);    
      DTCLASSLOGMETHOD(perfRotatingSpline);
      perfRotatingSpline();
      perfRotatingSpline(const perfRotatingSpline& orig);
      perfRotatingSpline(const rotatingSpline& orig);
      virtual ~perfRotatingSpline();
      void initInternalFunctions( void );
      virtual dtPoint2 uv_phiRadiusM(float const & phiRadius, float const & mm) const;
      virtual dtPoint2 uv_deltaPhiRadiusDeltaM(
                float const & bU,
                float const & bV,
                float const & deltaPhiRadius, 
                float const & deltaMm) const;
      virtual dtPoint2 uv_phiM(float const & phi, float const & mm) const;
      virtual float r_m(const float & meridLength) const;      
      virtual covise::coDoSet * packToCoDoSet( char const * const str) const;
      virtual void unpackFromCoDoSet( covise::coDoSet const * const set );       
      virtual void packToExtInfoContainer( extInfoContainer * const eIC ) const;      
    private:
      void createPhiU( void );
      void createMeridianV( void );
      void createMeridianR( void );
    private:
      scaFunction * _phiU;
      scaFunction * _uPhi;
      scaFunction * _meridianV;
      scaFunction * _vMeridian;      
      scaFunction * _meridianR;
      scaFunction * _rMeridian;            
      int _phiUNPoints;
      int _phiUOrder;
      int _meridianVNPoints;
      int _meridianVOrder;
      int _meridianRNPoints;
      int _meridianROrder;
  };
}
#endif	/* PERFROTATINGSPLINE_H */

