#ifndef uVw_skewPhirMs_H
#define	uVw_skewPhirMs_H

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
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
    virtual std::vector< dtPoint3 > 
    apply( std::vector< dtPoint3 > const * const toTrans ) const;        
    std::vector< dtPoint3 > 
    retract(std::vector< dtPoint3 > const * const toRetract) const;    
  private:
    /**
     * @todo Should be on map3dto3d
     */
	  float m_uSVS(float const & uu, float const & vv) const;
    void transformPhirM( void );
  private:
    dt__pH(rotatingMap2dTo3d const) _rM2d;
    dtVector3 _ss;
    dt__pH( vec3dThreeD ) _phirMS_uvw;
    int _nU;
    dt__pH(scaOneD) _fU;
    dt__pH(scaOneD) _fV;
    int _nV;
    int _nW;
    float _relaxM;
    float _relaxPhir;
    int _nSteps;
    float _angle;
  };
}

#endif	/* uVw_skewPhirMs_H */
