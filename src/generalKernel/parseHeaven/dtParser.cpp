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

#include "dtParser.h"

#include <geometryEngine/dtMuParser.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <interfaceHeaven/intHandling.h>

#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/aFBuilder/x_vec3dClosestPointToPoint.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <dtTransformerHeaven/doNothing.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/aGBuilder/float_map1dTo3dPointConstCartesian.h>
#include <analyticGeometryHeaven/aGBuilder/pairUUV_map1dTo3dClosestPointToMap2dTo3d.h>

#include <interfaceHeaven/lVHOstateHandler.h>

namespace dtOO {
  std::string dtParser::_CALCSIGN = "`";
  std::string dtParser::_POINTSIGN = "!";
  std::string dtParser::_VECTORSIGN = "|";
  std::string dtParser::_DTTSIGN = "~";
  std::string dtParser::_CVSIGN = "#";
  std::string dtParser::_AFSIGN = "$";
  std::string dtParser::_AGSIGN = "@";
  
  dtParser::dtParser() {
    _bC = NULL;
    _cV = NULL;
    _aF = NULL;
    _aG = NULL;
    _bV = NULL;
    _dC = NULL;
  }

  dtParser::dtParser(
    baseContainer const * const bC,      
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC
  ) {
    _bC = bC;
    _cV = cV;
    _aF = aF;
    _aG = aG;
    _bV = bV;
    _dC = dC;
  }

  dtParser::~dtParser() {
  }


  std::string dtParser::operator[]( std::string const expression) const {
    std::string wExpression = expression;
    if ( stringPrimitive::stringContains( "STATE", wExpression ) ) {
      wExpression
      =
      stringPrimitive::replaceStringInString( 
        "STATE", lVHOstateHandler().commonState(), wExpression
      );
    }

    return dtParser::replaceDependencies(
      wExpression, _bC, _cV, _aF, _aG
    ); 
  }

  std::vector< dtReal > dtParser::operator()( 
    std::string const expression
  ) const {
    return dtParser::muParseCSString( expression ); 
  }

  std::string dtParser::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV
	) {
    if (!cV) return expression;

    std::string returnExpression;
    returnExpression = expression;
    dtUnsInt found;
    //
    // check if there is a constValue in expression
    // 1. * #cVLabel#
    //
    found = returnExpression.find(_CVSIGN);
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      dtUnsInt foundEnd = returnExpression.find(_CVSIGN, found+1);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      
      //
      // extract constValue label and option
      std::string cVLabel
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
      if ( stringPrimitive::stringContains("[", cVLabel) ) {
        std::string cVOption
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &cVLabel);
        dt__debug( replaceDependencies(), << "cVOption = " << cVOption );
        //
        // add array index i -> label_i
        //
        cVLabel 
        = 
        cVLabel 
        + 
        "_" 
        + 
        stringPrimitive::intToString( 
          intHandling::round( muParseString(cVOption) )
        );
      }
      
      //
      // replace constValue string by value
      //      
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        stringPrimitive::floatToString( cV->get(cVLabel)->getValue() )
      );

      //
      // go to next constValue
      //
      found = returnExpression.find(_CVSIGN);
    }
    //
    // check if there is an instruction to calculate in expression
    // `#cVLabel# + #cVLabel#`
    //
    found = returnExpression.find(_CALCSIGN);
    while ( found < returnExpression.size() ) {
      //
      // find start and end of function
      //
      dtUnsInt foundEnd = returnExpression.find(_CALCSIGN, found+1);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      
      //
      // muparse string
      //
      returnExpression.replace(
        replaceStart, 
        replaceEnd, 
        stringPrimitive::floatToString(
          muParseString(        
            returnExpression.substr(replaceStart+1, replaceEnd-2)
          )
        )
      );

      //
      // go to next constValue
      //
      found = returnExpression.find(_CALCSIGN);
    }    
    return returnExpression;
  }
  
  std::string dtParser::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF
	) {
    if (!aF) return replaceDependencies(expression, cV);

    std::string returnExpression = expression;
          
    //
    // crumble string down, respect brackets
    //
    dt__forAllRefAuto(
      stringPrimitive::crumbleDown("(", ")", expression),
      aCrumble
    ) {
      if ( !stringPrimitive::stringContains(_AFSIGN, aCrumble) ) continue;
      returnExpression 
      = 
      stringPrimitive::replaceStringInString(
        aCrumble, 
        replaceDependencies(aCrumble, cV, aF),
        returnExpression
      );
    }
    
    //
    // check if there is a function in expression
    // $functionName(value * #constValue#)$
    //
    dtUnsInt found = returnExpression.find(_AFSIGN);
    while ( (found < returnExpression.size()) ) {
      //
      // find start and end of function
      //
      //unsigned dtInt foundEnd = returnExpression.find_last_of(_AFSIGN);
      dtUnsInt foundEnd = returnExpression.find_first_of(_AFSIGN, found+1);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // replace in argument
      //
      std::string arg 
      = 
      replaceDependencies(
        stringPrimitive::getStringBetweenFirstLast(
          "(", ")", replaceString
        ), 
        cV, 
        aF
      );

      //
      // get and cast analyticFunction
      //
      std::string aFLabel 
      = 
      stringPrimitive::getStringBetween("", "(", replaceString);
      std::string aFOption = "";
      if ( stringPrimitive::stringContains("[", aFLabel) ) {
        aFOption 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &aFLabel);
      }
      analyticFunction const * const theAF = aF->get(aFLabel); 
      
      scaFunction const * const sF = scaFunction::ConstDownCast(theAF);
      vec2dFunction const * const v2dF = vec2dFunction::ConstDownCast(theAF);
      vec3dFunction const * const v3dF = vec3dFunction::ConstDownCast(theAF);
      
      std::vector< dtReal > pp; 
      std::vector< dtReal > argCS;
      if (!arg.empty()) {
        argCS
        = 
        muParseCSString( replaceDependencies(arg, cV, aF) );
      }

        
      if (sF) {
        if (aFOption == "") {
          pp = sF->Y( argCS ).stdVector();
        }        
        else if (aFOption == "%") {
          pp = sF->Y( sF->x_percent( aFX( argCS ) ) ).stdVector();
        }                
        else dt__throwUnexpected(replaceDependencies());        
      }
      else if (v2dF) {
        if (aFOption == "") {
          pp = v2dF->Y( argCS ).stdVector();
        }    
        else if (aFOption == "%x") {
          aFX xx = v2dF->Y( v2dF->x_percent( argCS ) ).stdVector();
          pp.resize(1);
          pp[0] = xx[0];
        }           
        else if (aFOption == "%y") {
          aFX xx = v2dF->Y( v2dF->x_percent( argCS ) ).stdVector();
          pp.resize(1);
          pp[0] = xx[1];
        }         
        else dt__throwUnexpected(replaceDependencies()); 
      }
      else if (v3dF) {
        if (aFOption == "") {
          pp = v3dF->Y( argCS ).stdVector();
        }       
        else if (aFOption == "%") {
          pp = v3dF->Y( v3dF->x_percent( argCS ) ).stdVector();
        }                   
        else if (aFOption == "%x") {
          aFX xx = v3dF->Y( v3dF->x_percent( argCS ) ).stdVector();
          pp.resize(1);
          pp[0] = xx[0];
        }           
        else if (aFOption == "%y") {
          aFX xx = v3dF->Y( v3dF->x_percent( argCS ) ).stdVector();
          pp.resize(1);
          pp[0] = xx[1];
        } 
        else if (aFOption == "%z") {
          aFX xx = v3dF->Y( v3dF->x_percent( argCS ) ).stdVector();
          pp.resize(1);
          pp[0] = xx[2];
        }
        else if ( 
          stringPrimitive::matchWildcard("%d*Yx", aFOption) 
          ||
          stringPrimitive::matchWildcard("%d*Yy", aFOption) 
          ||
          stringPrimitive::matchWildcard("%d*Yz", aFOption) 
        ) {
          std::string fDStr 
          = 
          stringPrimitive::getStringBetween("d", "Y", aFOption);
          dtReal fD = muParseString(fDStr);
          std::vector< dtReal > argCSUp(argCS);
          std::vector< dtReal > argCSDown(argCS);
          dt__forAllIndex(argCS, ii) {
            argCSUp[ii] = floatHandling::boundToRange(argCS[ii] + fD, 0., 1.);
            argCSDown[ii] = floatHandling::boundToRange(argCS[ii] - fD, 0., 1.);
          }
          aFY yyUp(v3dF->Y( v3dF->x_percent( argCSUp ) ));
          aFY yyDown(v3dF->Y( v3dF->x_percent( argCSDown ) ));
          
          aFY yy(yyUp);
          dtReal length = 0.;
          dt__forAllIndex(yy, ii) {
            yy[ii] = yyUp[ii] - yyDown[ii];
            length = length + yy[ii] * yy[ii];
          }
          length = length / sqrt(length);
          
          pp.resize(1);
          if ( stringPrimitive::stringContains("x", aFOption) ) {
            pp[0] = yy[0] / length;          
          }
          else if ( stringPrimitive::stringContains("y", aFOption) ) {
            pp[0] = yy[1] / length;          
          }
          else if ( stringPrimitive::stringContains("z", aFOption) ) {
            pp[0] = yy[2] / length;          
          }
          else dt__throwUnexpected(replaceDependencies());
        }
        else if (aFOption == "-1") {
          pp = v3dF->invY( aFY(argCS) ).toStdVector();
        }
        else if (aFOption == "xMin0") {
            pp.push_back(v3dF->xMin(0));
        }
        else if (aFOption == "xMin1") {
            pp.push_back(v3dF->xMin(1));
        }
        else if (aFOption == "xMax0") {
            pp.push_back(v3dF->xMax(0));
        }
        else if (aFOption == "xMax1") {
            pp.push_back(v3dF->xMax(1));
        }
        else dt__throwUnexpected(replaceDependencies()); 
      }
      else dt__throwUnexpected(replaceDependencies());
      
      if (pp.size() == 3) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
          +
          ","
          +
          stringPrimitive::floatToString(pp[2])
        );
      }
      else if (pp.size() == 2) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
        );
      }        
      else if (pp.size() == 1) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
        );
      }            
      else dt__throwUnexpected(replaceDependencies());
        
      //
      // go to next analyticGeometry
      //
      found = returnExpression.find(_AFSIGN);  
    }

    return replaceDependencies(returnExpression, cV);
  }
  
  std::string dtParser::replaceDependencies( 
	  std::string const expression, 
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG
	) {
    if (!aG) return replaceDependencies(expression, cV, aF);
    
    std::string returnExpression = expression;
          
    dt__forAllRefAuto(
      stringPrimitive::crumbleDown("(", ")", expression),
      aCrumble
    ) {
      if ( !stringPrimitive::stringContains(_AGSIGN, aCrumble) ) continue;
      returnExpression 
      = 
      stringPrimitive::replaceStringInString(
        aCrumble, 
        replaceDependencies(aCrumble, cV, aF, aG),
        returnExpression
      );
    }
    
    //
    // check if there is a analyticGeometry in expression
    // @analyticGeometryName(1. * #constValue#, 2.)@
    //
    dtUnsInt found = returnExpression.find(_AGSIGN);
    while ( (found < returnExpression.size()) ) {
      //
      // find start and end of function
      //
      dtUnsInt foundEnd = returnExpression.find_last_of(_AGSIGN);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // replace in argument
      //
      std::string arg 
      = 
      replaceDependencies(
        stringPrimitive::getStringBetweenFirstLast("(", ")", replaceString), 
        cV, 
        aF, 
        aG
      );

      //
      // get and cast analyticGeometry
      //
      std::string aGLabel 
      = 
      stringPrimitive::getStringBetween("", "(", replaceString);
      std::string aGOption = "";
      if ( stringPrimitive::stringContains("[", aGLabel) ) {
        aGOption 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &aGLabel);
      }
      analyticGeometry const * const theAG = aG->get(aGLabel); 
      
      map1dTo3d const * const m1d = map1dTo3d::ConstDownCast(theAG);
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(theAG);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(theAG);
      
      std::vector< dtReal > pp; 
      std::vector< dtReal > argCS 
      = 
      muParseCSString( replaceDependencies(arg, cV, aF) );

        
      if (m1d) {
        dt__throwIf(argCS.size()!=1, replaceDependencies());       

        if (aGOption == "") {
          pp = dtLinearAlgebra::toStdVector( m1d->getPoint( argCS[0] ) );
        }        
        else if (aGOption == "%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector( m1d->getPoint( (*m1d) % argCS[0] ) );
        }
        else if (aGOption == "%x") {
          pp.push_back( m1d->getPoint( (*m1d) % argCS[0] ).x() );
        }
        else if (aGOption == "%y") {
          pp.push_back( m1d->getPoint( (*m1d) % argCS[0] ).y() );
        }        
        else if (aGOption == "%z") {
          pp.push_back( m1d->getPoint( (*m1d) % argCS[0] ).z() );
        }
        else if (aGOption == "%dU") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              m1d->firstDerU( (*m1d) % argCS[0] )
            )
          );
        }        
        else if (aGOption == "%ndU") {
          pp
          =
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              dtLinearAlgebra::normalize( m1d->firstDerU( (*m1d) % argCS[0] ) )
            )
          );
        }        
        else if (aGOption == "%ndUx") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m1d->firstDerU( (*m1d) % argCS[0] ) 
            ).x()
          );
        }                  
        else if (aGOption == "%ndUy") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m1d->firstDerU( (*m1d) % argCS[0] ) 
            ).y()
          );
        }                 
        else if (aGOption == "%ndUz") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m1d->firstDerU( (*m1d) % argCS[0] ) 
            ).z()
          );
        }
        else if ( stringPrimitive::matchWildcard("-1%*X=", aGOption) ) {
          std::string initGuessStr 
          = 
          stringPrimitive::getStringBetween("%", "X", aGOption);
          if ( initGuessStr.empty() ) {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(m1d, 0, argCS[0]).result()
              )
            );
          }
          else {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(
                  m1d, 0, argCS[0], muParseString(initGuessStr)
                ).result()
              )
            );            
          }
        }
        else if ( stringPrimitive::matchWildcard("-1%*Y=", aGOption) ) {
          std::string initGuessStr 
          = 
          stringPrimitive::getStringBetween("%", "Y", aGOption);
          if ( initGuessStr.empty() ) {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(m1d, 1, argCS[0]).result()
              )
            );
          }
          else {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(
                  m1d, 1, argCS[0], muParseString(initGuessStr)
                ).result()
              )
            );            
          }
        }        
        else if ( stringPrimitive::matchWildcard("-1%*Z=", aGOption) ) {
          std::string initGuessStr 
          = 
          stringPrimitive::getStringBetween("%", "Z", aGOption);
          if ( initGuessStr.empty() ) {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(m1d, 2, argCS[0]).result()
              )
            );
          }
          else {
            pp.push_back(
              m1d->percent_u(
                float_map1dTo3dPointConstCartesian(
                  m1d, 2, argCS[0], muParseString(initGuessStr)
                ).result()
              )
            );            
          }
        }
        else if ( 
          stringPrimitive::matchWildcard("-1%ClosestPointTo*", aGOption) 
        ) {
          map2dTo3d const * const closeMap2d
          = 
          map2dTo3d::MustConstDownCast(
            aG->get( stringPrimitive::getStringBetween("@", "@", aGOption ) )
          );
          pp.push_back(
            m1d->percent_u(
              pairUUV_map1dTo3dClosestPointToMap2dTo3d(
                m1d, closeMap2d
              ).result().first
            )
          );
        }
        else if (aGOption == "length%") {
          pp.push_back( m1d->l_u( (*m1d) % argCS[0] ) );
        }        
        else dt__throwUnexpected(replaceDependencies());        
      }
      else if (m2d) {
        dt__throwIf(
          (argCS.size()!=2)&&(argCS.size()!=3), 
          replaceDependencies()
        );
        
        //
        // get dtPoint3
        //
        if (aGOption == "") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->getPoint( argCS[0], argCS[1] )
          );
        }        
        else if (aGOption == "%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->getPointPercent( argCS[0], argCS[1] )
          );          
        }
        else if (aGOption == "%x") {
          pp.push_back( m2d->getPointPercent( argCS[0], argCS[1] ).x() );          
        }        
        else if (aGOption == "%y") {
          pp.push_back( m2d->getPointPercent( argCS[0], argCS[1] ).y() );          
        }        
        else if (aGOption == "%z") {
          pp.push_back( m2d->getPointPercent( argCS[0], argCS[1] ).z() );          
        }                
        else if (aGOption == "dU") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              m2d->firstDerU( argCS[0], argCS[1] )
            )
          );
        }        
        else if (aGOption == "dV") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              m2d->firstDerV( argCS[0], argCS[1] )
            )
          );
        }  
        else if (aGOption == "%dU") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              m2d->firstDerU( 
                m2d->uv_percent( argCS[0], argCS[1] ).x(), 
                m2d->uv_percent( argCS[0], argCS[1] ).y() 
              )
            )
          );
        }        
        else if (aGOption == "%dV") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              m2d->firstDerV(  
                m2d->uv_percent( argCS[0], argCS[1] ).x(), 
                m2d->uv_percent( argCS[0], argCS[1] ).y() 
              )
            )
          );
        }          
        else if (aGOption == "ndU") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              dtLinearAlgebra::normalize( m2d->firstDerU( argCS[0], argCS[1] ) )
            )
          );
        }        
        else if (aGOption == "ndV") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              dtLinearAlgebra::normalize( m2d->firstDerV( argCS[0], argCS[1] ) )
            )
          );
        }        
        else if (aGOption == "%ndU") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              dtLinearAlgebra::normalize( 
                m2d->firstDerU( 
                  m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1])             
                ) 
              )
            )
          );
        }        
        else if (aGOption == "%ndV") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            dtLinearAlgebra::toDtPoint3(
              dtLinearAlgebra::normalize( 
                m2d->firstDerU( 
                  m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1])             
                ) 
              )
            )
          );
        }          
        else if (aGOption == "ndUx") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( argCS[0], argCS[1] ) 
            ).x()
          );
        }                  
        else if (aGOption == "ndUy") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( argCS[0], argCS[1] ) 
            ).y()
          );
        }                 
        else if (aGOption == "ndUz") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( argCS[0], argCS[1] ) 
            ).z()
          );
        }            
        else if (aGOption == "%ndUx") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).x()
          );
        }                  
        else if (aGOption == "%ndUy") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).y()
          );
        }                 
        else if (aGOption == "%ndUz") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerU( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).z()
          );
        }                    
        else if (aGOption == "ndVx") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( argCS[0], argCS[1] ) 
            ).x()
          );
        }                  
        else if (aGOption == "ndVy") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( argCS[0], argCS[1] ) 
            ).y()
          );
        }                 
        else if (aGOption == "ndVz") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( argCS[0], argCS[1] ) 
            ).z()
          );
        }
        else if (aGOption == "%ndVx") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).x()
          );
        }                  
        else if (aGOption == "%ndVy") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).y()
          );
        }                 
        else if (aGOption == "%ndVz") {
          pp.push_back(
            dtLinearAlgebra::normalize( 
              m2d->firstDerV( 
                m2d->u_percent(argCS[0]), m2d->v_percent(argCS[1]) 
              ) 
            ).z()
          );
        }        
        else if (aGOption == "-1") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->reparamOnFace( dtPoint3(argCS[0], argCS[1], argCS[2]) )
          );
        }
        else if (aGOption == "-1%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->reparamPercentOnFace( dtPoint3(argCS[0], argCS[1], argCS[2]) )
          );
        }
        else if (aGOption == "-a") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->approxOnFace( dtPoint3(argCS[0], argCS[1], argCS[2]) )
          );
        }
        else if (aGOption == "-a%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m2d->approxPercentOnFace( dtPoint3(argCS[0], argCS[1], argCS[2]) )
          );
        }            
        else if (aGOption == "-1%u") {
          pp.push_back(
            m2d->reparamPercentOnFace( 
              dtPoint3(argCS[0], argCS[1], argCS[2]) 
            ).x()
          );
        }        
        else if (aGOption == "-1%v") {
          pp.push_back(
            m2d->reparamPercentOnFace( 
              dtPoint3(argCS[0], argCS[1], argCS[2]) 
            ).y()
          );
        }                
        else dt__throwUnexpected(replaceDependencies());
      }
      else if (m3d) {
        dt__throwIf( argCS.size()!=3, replaceDependencies() );
        
        //
        // get dtPoint3
        //
        if (aGOption == "") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m3d->getPoint( argCS[0], argCS[1], argCS[2] )
          );
        }        
        else if (aGOption == "%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m3d->getPointPercent( argCS[0], argCS[1], argCS[2] )
          );
        }
        else if (aGOption == "%x") {
          pp.push_back(
            m3d->getPointPercent( argCS[0], argCS[1], argCS[2] ).x() 
          );
        }        
        else if (aGOption == "%y") {
          pp.push_back(
            m3d->getPointPercent( argCS[0], argCS[1], argCS[2] ).y() 
          );
        }        
        else if (aGOption == "%z") {
          pp.push_back(
            m3d->getPointPercent( argCS[0], argCS[1], argCS[2] ).z() 
          );
        }              
        else if (aGOption == "-1") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m3d->reparamInVolume(dtPoint3(argCS[0], argCS[1], argCS[2]))
          );
        }
        else if (aGOption == "-1%") {
          pp 
          = 
          dtLinearAlgebra::toStdVector(
            m3d->reparamPercentInVolume(dtPoint3(argCS[0], argCS[1], argCS[2]))
          );
        }
        else dt__throwUnexpected(replaceDependencies());
      }    
      else dt__throwUnexpected(replaceDependencies());
      
      if (pp.size() == 3) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
          +
          ","
          +
          stringPrimitive::floatToString(pp[2])
        );
      }
      else if (pp.size() == 2) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
          +
          ","
          +
          stringPrimitive::floatToString(pp[1])
        );
      }        
      else if (pp.size() == 1) {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(pp[0])
        );
      }            
      else dt__throwUnexpected(replaceDependencies());
        
      //
      // go to next analyticGeometry
      //
      found = returnExpression.find(_AGSIGN);
    }
    
    return replaceDependencies(returnExpression, cV, aF);
  }
  
  std::string dtParser::replaceDependencies( 
	  std::string const expression, 
    baseContainer const * const bC,       
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG
	) {
    if (!bC) return replaceDependencies(expression, cV, aF, aG);    
    
    std::string returnExpression;
    returnExpression = expression;
    dtUnsInt found;
    
    //
    // check if there is a point in expression
    // !aPoint! or !aPoint[x]!
    //
    found = returnExpression.find(_POINTSIGN);
    while ( (found < returnExpression.size()) && bC ) {
      //
      // find start and end
      //
      dtUnsInt foundEnd = returnExpression.find_first_of(_POINTSIGN, found+1);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // get option
      //
      std::string label = replaceString;
      std::string option = "";
      if ( stringPrimitive::stringContains("[", label) ) {
        option 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &label);
      }
      dtPoint3 const thePoint = bC->constPtrPointContainer()->get(label); 
      
      if (option == "x") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(thePoint.x())
        );        
      }
      else if (option == "y") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(thePoint.y())
        );        
      }
      else if (option == "z") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(thePoint.z())
        );        
      }      
      else if (option == "") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(thePoint.x())
          +
          ","
          +
          stringPrimitive::floatToString(thePoint.y())
          +
          ","
          +
          stringPrimitive::floatToString(thePoint.z())
        );        
      }
      else dt__throwUnexpected(replaceDependencies());        

      //
      // go to next transformer
      //
      found = returnExpression.find(_POINTSIGN);
    }
    
    //
    // check if there is a vector in expression
    // %aVector% or %aVector[x]%
    //
    found = returnExpression.find(_VECTORSIGN);
    while ( (found < returnExpression.size()) && bC ) {
      //
      // find start and end
      //
      dtUnsInt foundEnd 
      = 
      returnExpression.find_first_of(_VECTORSIGN, found+1);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // replace in argument
      //
      
      std::vector< dtReal > argCS;
      if ( stringPrimitive::stringContains("(", replaceString) ) {
        argCS 
        = 
        muParseCSString( 
          replaceDependencies(
            stringPrimitive::getStringBetweenAndRemove(
              "(", ")", &replaceString
            ), 
            bC, 
            cV, 
            aF, 
            aG
          ) 
        );
      }

      //
      // get option
      //
      std::string label = replaceString;
      std::string option = "";
      if ( stringPrimitive::stringContains("[", label) ) {
        option 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &label);
      }
      dtVector3 const theVector = bC->constPtrVectorContainer()->get(label); 
      
      if (option == "x") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(theVector.x())
        );        
      }
      else if (option == "y") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(theVector.y())
        );        
      }
      else if (option == "z") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(theVector.z())
        );        
      }
      else if (option == "length") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString( dtLinearAlgebra::length(theVector) )
        );        
      }
      else if (option == "crossProduct") {
        dtVector3 resVector 
        = 
        dtLinearAlgebra::crossProduct(
          theVector, dtVector3(argCS[0], argCS[1], argCS[2])
        );
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(resVector.x())
          +
          ","
          +
          stringPrimitive::floatToString(resVector.y())
          +
          ","
          +
          stringPrimitive::floatToString(resVector.z())
        );           
      }
      else if (option == "normalize") {
        dtVector3 resVector = dtLinearAlgebra::normalize(theVector);
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(resVector.x())
          +
          ","
          +
          stringPrimitive::floatToString(resVector.y())
          +
          ","
          +
          stringPrimitive::floatToString(resVector.z())
        );           
      }      
      else if (option == "") {
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(theVector.x())
          +
          ","
          +
          stringPrimitive::floatToString(theVector.y())
          +
          ","
          +
          stringPrimitive::floatToString(theVector.z())
        );        
      }
      else dt__throwUnexpected(replaceDependencies());        

      //
      // go to next transformer
      //
      found = returnExpression.find(_VECTORSIGN);
    }
    
    //
    // check if there is a transformer in expression
    // ~dtT(@aG_1[-1](@aG_0[%](0.00, 0.00)@)@)~
    //
    found = returnExpression.find(_DTTSIGN);
    while ( (found < returnExpression.size()) && bC ) {
      //
      // find start and end
      //
      dtUnsInt foundEnd = returnExpression.find_last_of(_DTTSIGN);
      dtInt replaceStart = found;
      dtInt replaceEnd = foundEnd-found+1;
      std::string replaceString 
      = 
      returnExpression.substr(replaceStart+1, replaceEnd-2);
     
      //
      // replace in argument
      //
      std::string arg 
      = 
      replaceDependencies(
        stringPrimitive::getStringBetweenFirstLast("(", ")", replaceString), 
        bC, 
        cV, 
        aF, 
        aG
      );

      //
      // get and cast analyticGeometry
      //
      std::string TLabel 
      = 
      stringPrimitive::getStringBetween("", "(", replaceString);
      std::string aGOption = "";
      if ( stringPrimitive::stringContains("[", TLabel) ) {
        aGOption 
        = 
        stringPrimitive::getStringBetweenAndRemove("[", "]", &TLabel);
      }
      dtTransformer const * const theT 
      = 
      bC->constPtrTransformerContainer()->get(TLabel); 
      
      std::vector< dtReal > argVec 
      = 
      muParseCSString( replaceDependencies(arg, cV, aF, aG) );
        
      if (argVec.size() == 3) {
        dtPoint3 p3(argVec[0], argVec[1], argVec[2]);
        if (aGOption == "-1") {
          p3 = theT->retract( p3 );
        }
        else if (aGOption == "") {
          p3 = theT->apply( p3 );
        }
        else dt__throwUnexpected(replaceDependencies());        
        
        returnExpression.replace(
          replaceStart, 
          replaceEnd, 
          stringPrimitive::floatToString(p3.x())
          +
          ","
          +
          stringPrimitive::floatToString(p3.y())
          +
          ","
          +
          stringPrimitive::floatToString(p3.z())
        );        
      }
      else dt__throwUnexpected(replaceDependencies());
      
      //
      // go to next transformer
      //
      found = returnExpression.find(_DTTSIGN);
    }
    
    return replaceDependencies(returnExpression, cV, aF, aG);    
  } 

  dtReal dtParser::muParseString( std::string const expression ) {
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      return ( (dtReal) parser.Eval() );
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseString(), 
        << dt__eval(expression) << std::endl
        << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }

  std::vector< dtReal > dtParser::muParseCSString( 
    std::string const expression 
  ) {
    if ( !stringPrimitive::stringContains(",", expression) ) {
      return std::vector< dtReal >(1, muParseString(expression));
    }
    
    dtMuParser parser;
    try {
      parser.SetExpr( expression );
      
      dtInt nDim;
      double * yyD = parser.Eval(nDim);
      std::vector< dtReal > yy(nDim, 0.);
			dt__forFromToIndex(0, nDim, ii) yy[ii] = static_cast<dtReal>(yyD[ii]);
      
      return yy;
    }
    catch (mu::Parser::exception_type &e) {
      dt__throw( 
        muParseCSString(), << "muParser returns: " << dt__eval(e.GetMsg())
      );
    }
  }  
}
