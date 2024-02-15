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

#ifndef bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H
#define	bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder);    
      bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder();
      virtual ~bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder();
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
#endif	/* bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H */
