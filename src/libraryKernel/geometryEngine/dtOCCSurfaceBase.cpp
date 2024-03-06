/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtOCCSurfaceBase.h"

#include <logMe/logMe.h>
#include <Geom_Surface.hxx>

namespace dtOO {
  struct dtOCCSurfaceBase::OCCHanWrap {
    Handle_Geom_Surface occHandle;
  };
		
	dtOCCSurfaceBase::dtOCCSurfaceBase() 
    : _hanWrap(new OCCHanWrap()) {	
	}
	
	dtOCCSurfaceBase::dtOCCSurfaceBase(Handle_Geom_Surface orig) 
    : _hanWrap(new OCCHanWrap()) {
	  _hanWrap->occHandle = orig;
	}

  void dtOCCSurfaceBase::setOCC(Handle_Geom_Surface occSurface) {
    _hanWrap->occHandle = occSurface;
  }

  Handle_Geom_Surface const dtOCCSurfaceBase::getOCC( void ) const {
    return _hanWrap->occHandle;
  }
  
 std::string dtOCCSurfaceBase::dumpToString(void) const {
		std::stringstream ss;
    
    Handle(Geom_Surface) const & sH = getOCC();
    Handle(Standard_Type) const & stH = sH->DynamicType();

    ss 
    << dt__dumpToString(
      << dt__eval(stH->Name()) << std::endl
      << dt__eval( _hanWrap->occHandle.get() )
    );    
    
    return ss.str();
  }

	dtOCCSurfaceBase::~dtOCCSurfaceBase() {

	}
}
