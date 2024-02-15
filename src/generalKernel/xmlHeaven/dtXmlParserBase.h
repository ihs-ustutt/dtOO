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

#ifndef DTXMLPARSERBASE_H
#define	DTXMLPARSERBASE_H

#include <dtOOTypeDef.h>

#include <string>
#include <interfaceHeaven/vectorHandling.h>
#include "qtXmlBase.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class dtTransformer;
  class baseContainer;
  
  class dtXmlParserBase : public qtXmlBase {
    public:  
      dt__classOnlyName(dtXmlParserBase);
      virtual ~dtXmlParserBase();
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        dtTransformer const * const dtTransformerP, 
        std::vector< dtPointD > * basicP
      );    
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        dtTransformer const * const dtTransformerP, 
        std::vector< dtPoint3 > * basicP
      );
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        dtTransformer const * const dtTransformerP, 
        std::vector< dtPoint2 > * basicP
      );
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        std::vector< dtReal > * basicP
      );     
      static void createAdvanced( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        dtTransformer const * const dtTransformerP,                        
        lvH_analyticGeometry * advancedP 
      );      
      static void createAdvanced( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        dtTransformer const * const dtTransformerP,                          
        lvH_analyticFunction * advancedP 
      );    
      //
      // replace constValues, analyticFunctions and analyticGeometries
      //
      static std::string replaceDependencies(
        std::string const expression,
        lvH_constValue const * const cV
      );
      static std::string replaceDependencies(
        std::string const expression,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      );
      static std::string replaceDependencies( 
        std::string const expression,      
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );
      static std::string replaceDependencies( 
        std::string const expression,
        baseContainer const * const bC,      
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );
      static std::string replaceDependenciesRef(
        std::string const expression, 
        baseContainer const & bC,      
        lvH_constValue const & cV,
        lvH_analyticFunction const & aF,
        lvH_analyticGeometry const & aG
      );      
      static dtReal getAttributeFloatMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV
      );
      static dtReal getAttributeFloatMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );
      static dtReal getAttributeFloatMuParse(
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        dtReal const & def
      );    
      static dtInt getAttributeIntMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV
      );    
      static dtInt getAttributeIntMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );
      static dtInt getAttributeIntMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG        
      );      
      static dtInt getAttributeIntMuParse(
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        dtInt const & def
      );
      static std::vector< dtReal > getAttributeFloatVectorMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );        
      static std::vector< double > getAttributeDoubleVectorMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );            
      static std::vector< dtInt > getAttributeIntVectorMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );
      static std::vector< dtInt > getAttributeIntVectorMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );      
      static dtReal getAttributeFloatMuParse( 
        std::string const attName, 
        ::QDomElement const element, 
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG
      );  
      //
      // transformer
      //
      static dtTransformer * createTransformer(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );       
      static dtTransformer * createTransformer(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      );
      //
      // float
      //    
      static dtReal createFloat(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );        
      //
      // dtPointD
      //
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,      
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF, 
        std::vector< dtPointD > * basicP
      );
      //
      // dtPoint3
      //
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        std::vector< dtPoint3 > * basicP
      );
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,      
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF, 
        std::vector< dtPoint3 > * basicP
      );
      static dtPoint3 createDtPoint3(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );
      static dtPoint3 createDtPoint3(
        ::QDomElement const & toBuild,
        baseContainer * const bC,                   
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );
      static dtPoint3 createDtPoint3(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      );    
      static dtPoint3 getDtPoint3(
        ::QDomElement const * toBuildP,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );
      static dtPoint3 getDtPoint3(
        ::QDomElement const * toBuildP,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      );        
      static dtPoint3 getDtPoint3(
        std::string const & str, baseContainer const * const bC
      );    
      static dtPoint3 getDtPoint3(
        ::QDomElement const & toBuildP, baseContainer const * const bC
      );    

      //
      // dtPoint2
      //
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,
        std::vector< dtPoint2 > * basicP
      );
      static void createBasic(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,      
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        std::vector< dtPoint2 > * basicP
      );     
      static dtPoint2 createDtPoint2(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );
      static dtPoint2 createDtPoint2(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      );        
      //
      // dtVector2
      //
      static dtVector2 createDtVector2(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );       
      static dtVector2 getDtVector2(
        ::QDomElement const * toBuildP,
        baseContainer const * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );    
      //
      // dtVector3
      //
      static dtVector3 createDtVector3(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );    
      static dtVector3 createDtVector3(
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF
      );
      static dtVector3 getDtVector3(
        ::QDomElement const * toBuildP,
        baseContainer const * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );    
      static dtVector3 getDtVector3(
        ::QDomElement const * toBuildP,
        baseContainer const * const bC,
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF
      );    
      static dtVector3 getDtVector3(
        std::string const & str, baseContainer const * const bC
      );    
      static dtVector3 getDtVector3(
        ::QDomElement const & toBuildP, baseContainer const * const bC
      );
      //
      // analyticGeometry
      //    
      static void createAdvanced( 
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,            
        lvH_analyticGeometry * advancedP 
      );
      static void createAdvanced( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG,            
        lvH_analyticGeometry * advancedP 
      );
      static analyticGeometry * createAnalyticGeometry( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );
      static analyticGeometry * createAnalyticGeometry( 
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );    
      static analyticGeometry * createAnalyticGeometry( 
        ::QDomElement const * toBuildP,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );      
      static analyticGeometry * createAnalyticGeometry( 
        ::QDomElement const & toBuildP,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticGeometry const * const aG
      );          
      //
      // analyticFunction
      //
      static void createAdvanced( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticFunction * advancedP 
      );
      static void createAdvanced( 
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF, 
        lvH_analyticFunction * advancedP 
      );         
      static analyticFunction * createAnalyticFunction( 
        ::QDomElement const * toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      ); 
      static analyticFunction * createAnalyticFunction( 
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF 
      );
    protected:
      dtXmlParserBase();    
  };
}
#endif	/* DTXMLPARSERBASE_H */
