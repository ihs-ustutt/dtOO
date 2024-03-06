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

#ifndef bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder_H
#define	bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder);
      dt__classSelfCreate(bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder);
      bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder();
      virtual ~bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticFunction * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder_H */
