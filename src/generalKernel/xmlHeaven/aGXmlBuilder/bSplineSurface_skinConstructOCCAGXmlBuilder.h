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

#ifndef bSplineSurface_skinConstructOCCAGXmlBuilder_H
#define	bSplineSurface_skinConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_skinConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_skinConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineSurface_skinConstructOCCAGXmlBuilder);
      bSplineSurface_skinConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_skinConstructOCCAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const; 
    private: 
      static bool _registrated;            
  };
}
#endif	/* bSplineSurface_skinConstructOCCAGXmlBuilder_H */
