#ifndef RENDERINTERFACE_H
#define	RENDERINTERFACE_H

#include <logMe/dtMacros.h>
#include "vectorHandling.h"
#include <string>

namespace dtOO { 
  class renderInterface {
  public:
    dt__classOnlyName(renderInterface);  
    renderInterface();
    renderInterface(const renderInterface& orig);
    virtual ~renderInterface();
    void extRender( bool const & flag ) const;
	  void extRender( bool const & flag, std::string const & option ) const;
	  bool mustExtRender( void ) const;
	  std::string extRenderWhat( void ) const;
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    virtual vectorHandling< renderInterface * > getRender( void ) const;
	  void geoBoundMin(int const & dir, float const & value) const;
	  float geoBoundMin(int const & dir) const;	    
	  void geoBoundMax(int const & dir, float const & value) const;
	  float geoBoundMax(int const & dir) const;	     
  private:
    mutable bool _extRender;
    mutable std::string _extRenderOption;
    mutable float _u[2];
    mutable float _v[2];
    mutable float _w[2];
  };
}
#endif	/* RENDERINTERFACE_H */

