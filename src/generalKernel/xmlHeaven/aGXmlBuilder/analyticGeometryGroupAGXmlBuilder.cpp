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

#include "analyticGeometryGroupAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticGeometryGroup.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <boost/assign.hpp>

namespace dtOO {
  bool analyticGeometryGroupAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      analyticGeometryGroupAGXmlBuilder, 
      new analyticGeometryGroupAGXmlBuilder()
    )
  );
  
  analyticGeometryGroupAGXmlBuilder::analyticGeometryGroupAGXmlBuilder() {

  }
  analyticGeometryGroupAGXmlBuilder::~analyticGeometryGroupAGXmlBuilder() {

  }
    
  std::vector< std::string > analyticGeometryGroupAGXmlBuilder
    ::factoryAlias( void ) const {
    return 
      ::boost::assign::list_of
        ("map1dTo3dGroup")
        ("map2dTo3dGroup")
        ("map3dTo3dGroup");
  }
  
  void analyticGeometryGroupAGXmlBuilder::buildPart(
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

    vectorHandling< analyticGeometry const * > mXd;
    
    dt__forAllRefAuto( 
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), wE
    ) {
      if ( 
        !dtXmlParserBase::isWildcard( 
          dtXmlParserBase::getAttributeStr("label", wE) 
        )
      ) {
        mXd.push_back( 
            dtXmlParserBase::createAnalyticGeometry(wE, bC, cV, aF, aG)
        );
      }
      else {
        std::string wCard = dtXmlParserBase::getAttributeStr("label", wE);
        dt__forAllRefAuto(*aG, anAG) {
          if (
            dtXmlParserBase::matchWildcard( wCard, anAG->getLabel() )
          ) {
            mXd.push_back( anAG->clone() );            
          }
        }
      }
    }
    
    result->push_back( new analyticGeometryGroup(mXd) );
      
    //
    // free memory
    //
    mXd.destroy();      
  }
}
