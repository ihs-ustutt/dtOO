#ifndef RENDERINTERFACE_H
#define	RENDERINTERFACE_H

#include <dtOOTypeDef.h>

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
	  void geoBoundMin(int const & dir, dtReal const & value) const;
	  dtReal geoBoundMin(int const & dir) const;	    
	  void geoBoundMax(int const & dir, dtReal const & value) const;
	  dtReal geoBoundMax(int const & dir) const;
    dtReal characteristicLength( void ) const;
  private:
    mutable bool _extRender;
    mutable std::string _extRenderOption;
    mutable dtReal _u[2];
    mutable dtReal _v[2];
    mutable dtReal _w[2];
  };
}
#endif	/* RENDERINTERFACE_H */

