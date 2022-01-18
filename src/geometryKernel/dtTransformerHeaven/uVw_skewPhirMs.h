#ifndef uVw_skewPhirMs_H
#define	uVw_skewPhirMs_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

//#include <analyticFunctionHeaven/vec3dMultiThreeD.h>
//#include <analyticFunctionHeaven/vec3dTriLinearThreeD.h>

namespace dtOO {
  class rotatingMap2dTo3d;
  class vec3dThreeD;
//  class vec3dTriLinearThreeD;
  class scaOneD;
  
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
  class uVw_skewPhirMs : public dtTransformer {
    public:      
      dt__classOnlyName(uVw_skewPhirMs);
      uVw_skewPhirMs();
      uVw_skewPhirMs( uVw_skewPhirMs const & orig );
      virtual ~uVw_skewPhirMs();
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
      void transformPhirM( void );
    private:
      dt__pH(rotatingMap2dTo3d const) _rM2d;
      dtVector3 _ss;
      dt__pH( vec3dThreeD ) _phirMS_uvw;
      dtInt _nU;
      dt__pH(scaOneD) _fU;
      dt__pH(scaOneD) _fV;
      dtInt _nV;
      dtInt _nW;
      dtReal _relaxM;
      dtReal _relaxPhir;
      dtInt _nSteps;
      dtReal _angle;
      static bool _registrated;    
  };
}

#endif	/* uVw_skewPhirMs_H */

