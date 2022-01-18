#ifndef uVw_phiMs_H
#define	uVw_phiMs_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotatingMap2dTo3d;
  class vec2dMultiBiLinearTwoD;
  
  /**
   * \brief Coordinate transformation for rotatingMap2dTo3d mapping according to
   * \f[
   *   (\phi r, m, s) \rightarrow (u_0,v_0,w_0)
   * \f]
   * \f$\phi r\f$ is the real circumferential length and \f$m\f$ is the real
   * merdional length. \f$s\f$ is the spanwise direction, but not treated
   * as real length. Scales the coordinates \f$ (\phi r, m, s) \f$ with given
   * vector \f& \underline s \f$.
   * 
   */
  class uVw_phiMs : public dtTransformer {
    public:      
      dt__classOnlyName(uVw_phiMs);
      uVw_phiMs();
      uVw_phiMs( uVw_phiMs const & orig );
      virtual ~uVw_phiMs();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aFPtrVec 
      apply( aFPtrVec const * const aFP ) const;
      virtual std::vector< dtPoint3 > 
      apply( std::vector< dtPoint3 > const * const toTrans ) const;        
      std::vector< dtPoint3 > 
      retract(std::vector< dtPoint3 > const * const toRetract) const;    
    private:
      /**
       * @todo Should be on map3dto3d
       */
      dtReal m_uSVS(dtReal const & uu, dtReal const & vv) const;
      /**
       * @todo Should be on map3dto3d
       */    
      dtReal s_uSVS(dtReal const & uu, dtReal const & vv) const;     
      /**
       * @todo Should be on map3dto3d
       */    
      dtReal uV_phirVVWV(
        dtReal const & phir, dtReal const & vv, dtReal const & ww
      ) const;
      /**
       * @todo Should be on map3dto3d
       */
      dtReal vV_ms(dtReal const & mm, dtReal const & ss) const;
      /**
       * @todo Should be on map3dto3d
       */    
      dtReal wV_ms(dtReal const & mm, dtReal const & ss) const;
      /**
       * @todo Should be on map3dto3d
       */    
      dtReal phir_uVvVwV(
        dtReal const & uu, dtReal const & vv, dtReal const & ww
      ) const;
    private:
      dt__pH(rotatingMap2dTo3d const) _rM2d;
      dtVector3 _ss;
      dt__pH(vec2dMultiBiLinearTwoD) _ms_uSPercentVSPercent;
      dtInt _nV;
      dtInt _nW;
      static bool _registrated;    
  };
}

#endif	/* uVw_phiMs_H */

