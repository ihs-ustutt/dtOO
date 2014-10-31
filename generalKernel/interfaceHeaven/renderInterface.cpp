#include "renderInterface.h"

#include <logMe/logMe.h>

namespace dtOO {  
  renderInterface::renderInterface() {
		_extRender = false;
  }

  renderInterface::renderInterface(const renderInterface& orig) {
		_extRender = orig._extRender;
  }

  renderInterface::~renderInterface() {
  }
  
	vectorHandling< renderInterface * > renderInterface::getExtRender( void ) const {
		return vectorHandling< renderInterface * >(0);
	}
	
  vectorHandling< renderInterface * > renderInterface::getRender( void ) const {
		return vectorHandling< renderInterface * >(0);
	}
	
	void renderInterface::extRender( bool const & flag ) const {
		_extRender = flag;
	}  
	
	bool renderInterface::mustExtRender( void ) const {
		return _extRender;
	}  
	
	void renderInterface::setMin(int const & dir, float const & value) const {
    switch (dir) {
      case 0:
				_u[0] = value;
        return;
      case 1:
				_v[0] = value;
        return;
      case 2:
				_w[0] = value;
        return;
      default:
        dt__THROW(setMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }		
	}	

	float renderInterface::getMin(int const & dir) const {
    switch (dir) {
      case 0:
				return _u[0];
      case 1:
				return _v[0];
      case 2:
				return _w[0];
      default:
        dt__THROW(getMin(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }		
	}		

	void renderInterface::setMax(int const & dir, float const & value) const {
    switch (dir) {
      case 0:
				_u[1] = value;
        return;
      case 1:
				_v[1] = value;
        return;
      case 2:
				_w[1] = value;
        return;
      default:
        dt__THROW(setMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }		
	}	

	float renderInterface::getMax(int const & dir) const {
    switch (dir) {
      case 0:
				return _u[1];
      case 1:
				return _v[1];
      case 2:
				return _w[1];
      default:
        dt__THROW(getMax(),
              << DTLOGEVAL(dir) << LOGDEL
              << "dir should be 0, 1 or 2.");
    }		
	}			
}
