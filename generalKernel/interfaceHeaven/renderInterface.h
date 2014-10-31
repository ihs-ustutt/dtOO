#ifndef RENDERINTERFACE_H
#define	RENDERINTERFACE_H

#include <logMe/dtMacros.h>
#include "vectorHandling.h"

namespace dtOO { 
  class renderInterface {
  public:
    dt__CLASSNAME(renderInterface);  
    renderInterface();
    renderInterface(const renderInterface& orig);
    virtual ~renderInterface();
    void extRender( bool const & flag ) const;
	  bool mustExtRender( void ) const;
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    virtual vectorHandling< renderInterface * > getRender( void ) const;
	  void setMin(int const & dir, float const & value) const;
	  float getMin(int const & dir) const;	    
	  void setMax(int const & dir, float const & value) const;
	  float getMax(int const & dir) const;	     
  private:
    mutable bool _extRender;
    mutable float _u[2];
    mutable float _v[2];
    mutable float _w[2];
  };
}
#endif	/* RENDERINTERFACE_H */

