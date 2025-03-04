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

#ifndef bVOSetPrescribedElementSize_H
#define	bVOSetPrescribedElementSize_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

#include <analyticFunctionHeaven/analyticFunctionCompoundTrojanHorse.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class scaOneDPolyInterface;
  class dtGmshEdge;

  class bVOSetPrescribedElementSize : public bVOInterface {
    public:
      dt__class(bVOSetPrescribedElementSize, bVOInterface);
      dt__classSelfCreate(bVOSetPrescribedElementSize);
      bVOSetPrescribedElementSize();
      virtual ~bVOSetPrescribedElementSize();
      virtual std::vector< std::string > factoryAlias( void ) const;         
      void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );  
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      analyticFunctionCompoundTrojanHorse< scaOneDPolyInterface > _grading;
      dt__pH(scaOneDPolyInterface) _polyI;
      static bool _registrated;
  };
}
#endif	/* bVOSetPrescribedElementSize_H */
