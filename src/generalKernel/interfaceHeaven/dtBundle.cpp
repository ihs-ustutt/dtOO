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

#include "dtBundle.h"

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <dtCase.h>
#include <dtPlugin.h>

namespace dtOO {
  dtBundle::dtBundle() {
    _bC.reset( new baseContainer() );
    _cV.reset( new lvH_constValue() );
    _aF.reset( new lvH_analyticFunction() );
    _aG.reset( new lvH_analyticGeometry() );
    _bV.reset( new lvH_boundedVolume() );
    _dC.reset( new lvH_dtCase() );
    _dP.reset( new lvH_dtPlugin() );
  }

  dtBundle::~dtBundle() {
    this->clear();    
  }

  baseContainer * const dtBundle::cptr_bC( void ) const {
    return _bC.get();
  }

  lvH_constValue * const dtBundle::cptr_cV( void ) const {
    return _cV.get();
  }

  lvH_analyticFunction * const dtBundle::cptr_aF( void ) const {
    return _aF.get();
  }

  lvH_analyticGeometry * const dtBundle::cptr_aG( void ) const {
    return _aG.get();
  }

  lvH_boundedVolume * const dtBundle::cptr_bV( void ) const {
    return _bV.get();
  }

  lvH_dtCase * const dtBundle::cptr_dC( void ) const {
    return _dC.get();
  }

  lvH_dtPlugin * const dtBundle::cptr_dP( void ) const {
    return _dP.get();
  }

  void dtBundle::clear( bool const keepConstValue ) {
    _bC->clear();
    if ( !keepConstValue ) _cV->clear();
    _aF->clear();
    _aG->clear();
    _bV->clear();
    _dC->clear();
    _dP->clear();
  }
}
