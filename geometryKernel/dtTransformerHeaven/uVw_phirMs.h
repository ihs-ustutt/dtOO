#ifndef uVw_phirMs_H
#define	uVw_phirMs_H

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
  class uVw_phirMs : public dtTransformer {
  public:      
    dt__classOnlyName(uVw_phirMs);
    uVw_phirMs();
    uVw_phirMs( uVw_phirMs const & orig );
    virtual ~uVw_phirMs();
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
    virtual void handleAnalyticGeometry(
      std::string const name, analyticGeometry const * value
    );
    virtual void handleDtVector3(std::string const name, dtVector3 const value);
    virtual void handleInt(std::string const name, int const value);
  private:
    /**
     * @todo Should be on map3dto3d
     */
	  float m_uSVS(float const & uu, float const & vv) const;
    /**
     * @todo Should be on map3dto3d
     */    
    float s_uSVS(float const & uu, float const & vv) const;     
    /**
     * @todo Should be on map3dto3d
     */    
    float uV_phirVVWV(
      float const & phir, float const & vv, float const & ww
    ) const;
    /**
     * @todo Should be on map3dto3d
     */
    float vV_ms(float const & mm, float const & ss) const;
    /**
     * @todo Should be on map3dto3d
     */    
    float wV_ms(float const & mm, float const & ss) const;
    /**
     * @todo Should be on map3dto3d
     */    
    float phir_uVvVwV(
      float const & uu, float const & vv, float const & ww
    ) const;
    /**
     * @todo Should be on map3dto3d
     */
    float m_vVs(float const & vv, float const & ss) const;      
  private:
    dt__pH(rotatingMap2dTo3d const) _rM2d;
    dtVector3 _ss;
    dt__pH(vec2dMultiBiLinearTwoD) _ms_uSPercentVSPercent;
    int _nU;
    int _nV;
  };
}

#endif	/* uVw_phirMs_H */

