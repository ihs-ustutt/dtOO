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

#ifndef bSplineCurve_pointConstructOCCAFXmlBuilder_H
#define	bSplineCurve_pointConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_pointConstructOCCAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineCurve_pointConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineCurve_pointConstructOCCAFXmlBuilder);    
      bSplineCurve_pointConstructOCCAFXmlBuilder();
      virtual ~bSplineCurve_pointConstructOCCAFXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;        
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineCurve_pointConstructOCCAFXmlBuilder_H */
