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

#include "analyticGeometryAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool analyticGeometryAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      analyticGeometryAGXmlBuilder, 
      new analyticGeometryAGXmlBuilder()
    )
  );
  
  analyticGeometryAGXmlBuilder::analyticGeometryAGXmlBuilder() {
  }

  analyticGeometryAGXmlBuilder::~analyticGeometryAGXmlBuilder() {
  }

  void analyticGeometryAGXmlBuilder::buildPart( 
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart()
    );

    //
    // copy
    //
    std::vector< ::QDomElement > wE
    = 
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild);     
    dt__forAllRefAuto(wE, anEl) {
      lvH_analyticGeometry toCopy;
      dtXmlParserBase::createAdvanced(&anEl, bC, cV, aF, aG, &toCopy);
      dt__forAllIndex(toCopy, ii) result->push_back( toCopy[ii] );
    }
  }
}
