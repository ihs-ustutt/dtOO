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

#ifndef vec3dThreeD_triLinearPointConstructAFXmlBuilder_H
#define	vec3dThreeD_triLinearPointConstructAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class vec3dThreeD_triLinearPointConstructAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(vec3dThreeD_triLinearPointConstructAFXmlBuilder);
      dt__classSelfCreate(vec3dThreeD_triLinearPointConstructAFXmlBuilder);
      vec3dThreeD_triLinearPointConstructAFXmlBuilder();
      virtual ~vec3dThreeD_triLinearPointConstructAFXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cValP, 
        lvH_analyticFunction const * const depSFunP,
        lvH_analyticFunction * sFunP
      ) const;
    private: 
      static bool _registrated;      
    };
}
#endif	/* vec3dThreeD_triLinearPointConstructAFXmlBuilder_H */
