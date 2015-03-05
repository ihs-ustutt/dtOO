#include "sca3PointMeanlineFunctionDecoratorXY.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <math.h>
#include <QtXml/QDomElement>

namespace dtOO {
  sca3PointMeanlineFunctionDecoratorXY::sca3PointMeanlineFunctionDecoratorXY() {
  }

  sca3PointMeanlineFunctionDecoratorXY::~sca3PointMeanlineFunctionDecoratorXY() {

  }

  void sca3PointMeanlineFunctionDecoratorXY::buildPart(
         QDomElement const & toBuildP, 
         vectorHandling< constValue * > const * const cValP, 
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP ) const {

    //
    // check input
    //
    bool hasAlphaOne = hasAttribute("alpha_one", toBuildP);
    bool hasAlphaTwo = hasAttribute("alpha_two", toBuildP);
    bool hasDeltaX= hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = hasAttribute("delta_y", toBuildP);
    bool hasOrder = hasAttribute("order", toBuildP);
    bool hasRatio = hasAttribute("ratio", toBuildP);


    //
    //
    //
    if ( hasAlphaOne && hasAlphaTwo && hasDeltaX && hasDeltaY && hasOrder ) {
      //
      // get necessary values
      //
      float alphaOne = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_one", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );      
      float alphaTwo = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_two", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );
      float deltaY = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float deltaX = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "delta_x", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     );      
      int order = muParseStringInt(
                    replaceUsedFunctions(
                      getAttributeStr(
                        "order", 
                        toBuildP
                      ), 
                      cValP, 
                      depSFunP
                    )
                  );       
      bool mirrorY = getAttributeBool("mirror_y", toBuildP);     
      
      //
      // check values
      //
      if ( alphaOne < 0.) {
        DTWARNINGWF(functionBuilderDecorator(),
                << "Adjusting alphaOne from " << DTLOGEVAL(alphaOne) 
                << " to " << 0. << "." );
        alphaOne = 0.;
      }
      if ( alphaTwo < 0.) {
        DTWARNINGWF(functionBuilderDecorator(),
                << "Adjusting alphaTwo from " << DTLOGEVAL(alphaTwo) 
                << " to " << 0. << "." );
        alphaTwo = 0.;        
      }            
      /*
      if ( (ratio < 0.) || (ratio > 1.) ) {
        dt__THROW(functionBuilderDecorator(),
                << DTLOGEVAL(ratio) << LOGDEL        
                << "Ratio is smaller than zero or bigger than one.");
      }
      */
      if (alphaOne == alphaTwo) {
          //
          // calculate deltaX
          //
          //float deltaX = deltaY * tan(alphaOne);
          //
          // calculate points
          //
          std::vector< dtPoint2 > pV;
          pV.push_back( dtPoint2(0.,0.) );
          if (!mirrorY) {
            pV.push_back( dtPoint2(deltaX, deltaY));
          }
          else {
            pV.push_back( dtPoint2(-deltaX, deltaY));
          }
          
          DTINFOWF(
            functionBuilderDecorator(),
            << DTLOGEVAL(alphaOne) << LOGDEL
            << DTLOGEVAL(alphaTwo) << LOGDEL              
            << DTLOGEVAL(deltaX) << LOGDEL 
            << DTLOGEVAL(deltaY) << LOGDEL
            << DTLOGPOI2D(pV[0]) << LOGDEL
            << DTLOGPOI2D(pV[1]) << LOGDEL
            << DTLOGEVAL(mirrorY)
          );
          
//          sFunBuilderP->setAttribute( &pV[0] );
//          sFunBuilderP->setAttribute( &pV[1] );
//          sFunBuilderP->setAttribute( &order ); 
					ptrHandling<dtCurve2d> dtC2d( 
					  bSplineCurve2d_pointConstructOCC(pV, 1).result() 
					);

					//
					// create scaCurve2dOneD
					//
					sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );						
      }
      else {
        //
        // calculate deltaX
        //
        float deltaXMax;
        float deltaXMin;
        if (alphaOne >= alphaTwo) {
          deltaXMax = deltaY / tan(alphaTwo);
          deltaXMin = deltaY / tan(alphaOne);
        }
        else if (alphaOne < alphaTwo) {
          deltaXMin = deltaY / tan(alphaTwo);
          deltaXMax = deltaY / tan(alphaOne);        
        }
//      else if ( alphaOne == M_PI_2 ) {
//        
//      }
//      else if ( alphaTwo == M_PI_2 ) {
//        
//      }

        //float deltaX = deltaXMin + ratio * (deltaXMax - deltaXMin);
	
	float ratio = (deltaX - deltaXMin) / (deltaXMax - deltaXMin);
      
        //
        // calculate points
        //
        std::vector< dtPoint2 > pV;
        pV.push_back( dtPoint2(0.,0.) );
        int ss = ( deltaY - deltaX * sin(alphaOne) / cos(alphaOne) )
                 /
                 ( sin(alphaTwo) - (cos(alphaTwo)*sin(alphaOne) / cos(alphaOne)) );
        int tt = ( deltaY - deltaX * sin(alphaTwo) / cos(alphaTwo) )
                 /
                 ( sin(alphaOne) - (cos(alphaOne)*sin(alphaTwo) / cos(alphaTwo)) );      
//      pV.push_back(
//        dtPoint2(
//          deltaX - cos(alphaTwo) * ss,
//          deltaY - sin(alphaTwo) * ss
//        )
//      );
        if (!mirrorY) {
          pV.push_back(
            dtPoint2(
              cos(alphaOne) * tt,
              sin(alphaOne) * tt
            )
          );      
          pV.push_back( dtPoint2(deltaX, deltaY) );
        }
        else {
          pV.push_back(
            dtPoint2(
              -cos(alphaOne) * tt,
              sin(alphaOne) * tt
            )
          );      
          pV.push_back( dtPoint2(-deltaX, deltaY) );          
        }
        DTINFOWF(
          functionBuilderDecorator(),
          << DTLOGEVAL(alphaOne) << LOGDEL
          << DTLOGEVAL(alphaTwo) << LOGDEL              
          << DTLOGEVAL(deltaXMin) << LOGDEL 
          << DTLOGEVAL(deltaXMax) << LOGDEL               
          << DTLOGEVAL(ratio) << LOGDEL
          << DTLOGEVAL(ss) << LOGDEL
          << DTLOGEVAL(tt) << LOGDEL
          << DTLOGEVAL(deltaX) << LOGDEL 
          << DTLOGEVAL(deltaY) << LOGDEL
          << DTLOGPOI2D(pV[0]) << LOGDEL
          << DTLOGPOI2D(pV[1]) << LOGDEL
          << DTLOGPOI2D(pV[2]) << LOGDEL
          << DTLOGEVAL(mirrorY) 
        );
           
				ptrHandling<dtCurve2d> dtC2d( 
					bSplineCurve2d_pointConstructOCC(pV, 2).result() 
				);

				//
				// create scaCurve2dOneD
				//
				sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );	      
      }
//      //
//      // call builder
//      //
//      sFunDirector.construct( sFunP );
    
    }
    
    else if ( hasAlphaOne && hasAlphaTwo && hasRatio && hasDeltaY && hasOrder ) {
      //
      // get necessary values
      //
      float alphaOne = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_one", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );      
      float alphaTwo = muParseString(
                         replaceUsedFunctions(
                           getAttributeStr(
                             "alpha_two", 
                             toBuildP
                           ), 
                           cValP, 
                           depSFunP
                         )
                       );
      float deltaY = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float ratio = muParseString(
                       replaceUsedFunctions(
                         getAttributeStr(
                           "ratio", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     );      
      int order = muParseStringInt(
                    replaceUsedFunctions(
                      getAttributeStr(
                        "order", 
                        toBuildP
                      ), 
                      cValP, 
                      depSFunP
                    )
                  );       
      bool mirrorY = getAttributeBool("mirror_y", toBuildP);     
      
      //
      // check values
      //
      if ( alphaOne < 0.) {
        DTWARNINGWF(functionBuilderDecorator(),
                << "Adjusting alphaOne from " << DTLOGEVAL(alphaOne) 
                << " to " << 0. << "." );
        alphaOne = 0.;
      }
      if ( alphaTwo < 0.) {
        DTWARNINGWF(functionBuilderDecorator(),
                << "Adjusting alphaTwo from " << DTLOGEVAL(alphaTwo) 
                << " to " << 0. << "." );
        alphaTwo = 0.;        
      }
      
      if (ratio < 0){
	ratio=fabs(ratio);
      }
      else if(ratio > 1){
      	ratio = (1/ratio);
      }
      
      /*if ( (ratio < 0.) || (ratio > 1.) ) {
        dt__THROW(functionBuilderDecorator(),
                << DTLOGEVAL(ratio) << LOGDEL        
                << "Ratio is smaller than zero or bigger than one.");
      }
      */
      if (alphaOne == alphaTwo) {
          //
          // calculate deltaX
          //
          float deltaX = deltaY * tan(alphaOne);
          //
          // calculate points
          //
          std::vector< dtPoint2 > pV;
          pV.push_back( dtPoint2(0.,0.) );
          if (!mirrorY) {
            pV.push_back( dtPoint2(deltaX, deltaY));
          }
          else {
            pV.push_back( dtPoint2(-deltaX, deltaY));
          }
          
          DTINFOWF(
            functionBuilderDecorator(),
            << DTLOGEVAL(alphaOne) << LOGDEL
            << DTLOGEVAL(alphaTwo) << LOGDEL              
            << DTLOGEVAL(deltaX) << LOGDEL 
            << DTLOGEVAL(deltaY) << LOGDEL
            << DTLOGPOI2D(pV[0]) << LOGDEL
            << DTLOGPOI2D(pV[1]) << LOGDEL
            << DTLOGEVAL(mirrorY)
          );
          
					ptrHandling<dtCurve2d> dtC2d( 
					  bSplineCurve2d_pointConstructOCC(pV, 1).result() 
					);

					//
					// create scaCurve2dOneD
					//
					sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );	
      }
      else {
        //
        // calculate deltaX
        //
        float deltaXMax;
        float deltaXMin;
        if (alphaOne >= alphaTwo) {
          deltaXMax = deltaY / tan(alphaTwo);
          deltaXMin = deltaY / tan(alphaOne);
        }
        else if (alphaOne < alphaTwo) {
          deltaXMin = deltaY / tan(alphaTwo);
          deltaXMax = deltaY / tan(alphaOne);        
        }
//      else if ( alphaOne == M_PI_2 ) {
//        
//      }
//      else if ( alphaTwo == M_PI_2 ) {
//        
//      }

        float deltaX = deltaXMin + ratio * (deltaXMax - deltaXMin);
      
        //
        // calculate points
        //
        std::vector< dtPoint2 > pV;
        pV.push_back( dtPoint2(0.,0.) );
        int ss = ( deltaY - deltaX * sin(alphaOne) / cos(alphaOne) )
                 /
                 ( sin(alphaTwo) - (cos(alphaTwo)*sin(alphaOne) / cos(alphaOne)) );
        int tt = ( deltaY - deltaX * sin(alphaTwo) / cos(alphaTwo) )
                 /
                 ( sin(alphaOne) - (cos(alphaOne)*sin(alphaTwo) / cos(alphaTwo)) );      
//      pV.push_back(
//        dtPoint2(
//          deltaX - cos(alphaTwo) * ss,
//          deltaY - sin(alphaTwo) * ss
//        )
//      );
        if (!mirrorY) {
          pV.push_back(
            dtPoint2(
              cos(alphaOne) * tt,
              sin(alphaOne) * tt
            )
          );      
          pV.push_back( dtPoint2(deltaX, deltaY) );
        }
        else {
          pV.push_back(
            dtPoint2(
              -cos(alphaOne) * tt,
              sin(alphaOne) * tt
            )
          );      
          pV.push_back( dtPoint2(-deltaX, deltaY) );          
        }
        DTINFOWF(
          functionBuilderDecorator(),
          << DTLOGEVAL(alphaOne) << LOGDEL
          << DTLOGEVAL(alphaTwo) << LOGDEL              
          << DTLOGEVAL(deltaXMin) << LOGDEL 
          << DTLOGEVAL(deltaXMax) << LOGDEL               
          << DTLOGEVAL(ratio) << LOGDEL
          << DTLOGEVAL(ss) << LOGDEL
          << DTLOGEVAL(tt) << LOGDEL
          << DTLOGEVAL(deltaX) << LOGDEL 
          << DTLOGEVAL(deltaY) << LOGDEL
          << DTLOGPOI2D(pV[0]) << LOGDEL
          << DTLOGPOI2D(pV[1]) << LOGDEL
          << DTLOGPOI2D(pV[2]) << LOGDEL
          << DTLOGEVAL(mirrorY) 
        );
           
				ptrHandling<dtCurve2d> dtC2d( 
					bSplineCurve2d_pointConstructOCC(pV, 2).result() 
				);

				//
				// create scaCurve2dOneD
				//
				sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );	     
      }
//      //
//      // call builder
//      //
//      sFunDirector.construct( sFunP );
    
    }
    
    
    else {
      dt__THROW(functionBuilderDecorator(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasAlphaOne) << LOGDEL
              << DTLOGEVAL(hasAlphaTwo) << LOGDEL
              << DTLOGEVAL(hasRatio) << LOGDEL
              << DTLOGEVAL(hasDeltaY) );
    }

		//
		// transform
		//
    ptrHandling< dtTransformer > cTransP(  
		  createTransformer(&toBuildP, cValP, depSFunP)
		);	
    if ( cTransP->isNecessary() ) {
      *sFunP = cTransP->apply(sFunP);
    }
  }
}
