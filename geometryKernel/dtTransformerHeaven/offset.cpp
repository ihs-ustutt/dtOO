#include "offset.h"
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include "progHelper.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <logMe/logMe.h>

namespace dtOO {
  offset::offset() : dtTransformer() {
    _nPieces = 1;
    _xyPercent = false;
  }

  offset::~offset() {
  }

  dtTransformer * offset::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * offset::create( void ) const {
		return new offset();
	}
	
  vectorHandling< analyticGeometry * > offset::apply( 
    vectorHandling< analyticGeometry * > const * const aGeoVecP 
  ) const {
    vectorHandling< analyticGeometry * > retAGeo;

    //each analyticGeometry
    for (int ii=0;ii< aGeoVecP->size();ii++) {
        //each piece
        for (int jj=1;jj<=_nPieces;jj++) {
          //
          // clone and cast analyticGeometry
          //
          analyticGeometry * aGeoP = aGeoVecP->at(ii)->clone();
          analyticSurface * aS = dynamic_cast< analyticSurface * >(aGeoP);

          //
          // perform translation
          //
          if ( aS ) {
            aS->offsetNormal( _normalOffset );
          }
          else {
            dt__warning(
              apply(),
              << dt__eval(aS) << std::endl
              << "Unknown type"
            );
          }

          //
          // push translated geometry in vector
          //
          retAGeo.push_back( aGeoP );
        }
    }
    return retAGeo;
  }

  vectorHandling< analyticFunction * > offset::apply( 
    vectorHandling< analyticFunction * > const * const sFunP 
  ) const { 
    vectorHandling< analyticFunction * > retSFun;

    //each analyticGeometry
    for (int ii=0;ii< sFunP->size();ii++) {
        //
        // clone and cast scaFunction
        //
        dt__ptrAss( 
          scaOneD * cloneP, scaOneD::DownCast( sFunP->at(ii)->clone() ) 
        );
        dt__ptrAss( 
          scaCurve2dOneD  * splineP, scaCurve2dOneD::DownCast(cloneP) 
        );
          
        //
        // translation vector
        //
        dtVector2 transVec;
        if (_xyPercent == true ) {
          float xRange = splineP->xMax(0) - splineP->xMin(0);
          float yRange 
          = 
          splineP->YFloat(splineP->xMax(0)) - splineP->YFloat(splineP->xMin(0));
          transVec = dtVector2(_xOffset * xRange, _yOffset * yRange);
        }
        else {
          transVec = dtVector2(_xOffset, _yOffset);
        }

        //each piece
        for (int jj=1;jj<=_nPieces;jj++) {
          //convert integer to float      
          float jjF = (float) jj;

          //
          // perform translation
          //
//          if ( splineP ) {
          splineP->translate( jjF * transVec );
//          }
//          else {
//            dt__warning(apply(),
//                    << dt__eval(splineP) << std::endl
//                    << "Unknown type");
//          }

          //
          // push translated geometry in vector
          //
          retSFun.push_back( cloneP );
        }
    }
    return retSFun;    
  }

  bool offset::isNecessary( void ) const {
    return true;
  }

  void offset::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    if (dtXmlParserBase::hasAttribute("parameter_one_offset_percent", *tE)) {
      _paraOneOffsetPercent = dtXmlParserBase::muParseString( 
                                dtXmlParserBase::replaceDependencies(
                                  dtXmlParserBase::getAttributeStr(
                                    "parameter_one_offset_percent", 
                                    *tE
                                  ),
                                  cV, 
                                  aF
                                ) 
                              );
    }
    if (dtXmlParserBase::hasAttribute("parameter_two_offset_percent", *tE)) {
      _paraTwoOffsetPercent = dtXmlParserBase::muParseString( 
                                dtXmlParserBase::replaceDependencies(
                                  dtXmlParserBase::getAttributeStr(
                                    "parameter_two_offset_percent", 
                                    *tE
                                  ),
                                  cV, 
                                  aF
                                ) 
                              );
    }  
    if (dtXmlParserBase::hasAttribute("number_pieces", *tE)) {
      _nPieces = dtXmlParserBase::muParseString( 
                   dtXmlParserBase::replaceDependencies(
                     dtXmlParserBase::getAttributeStr(
                       "number_pieces", 
                       *tE
                     ),
                     cV, 
                     aF
                   ) 
                 );
    }  
    if (dtXmlParserBase::hasAttribute("x_offset_percent", *tE)) {
      _xOffset = dtXmlParserBase::muParseString( 
                          dtXmlParserBase::replaceDependencies(
                            dtXmlParserBase::getAttributeStr(
                              "x_offset_percent", 
                              *tE
                            ),
                            cV, 
                            aF
                          ) 
                        );
      _xyPercent = true;
    }
    if (dtXmlParserBase::hasAttribute("y_offset_percent", *tE)) {
      _yOffset = dtXmlParserBase::muParseString( 
                          dtXmlParserBase::replaceDependencies(
                            dtXmlParserBase::getAttributeStr(
                              "y_offset_percent", 
                              *tE
                            ),
                            cV, 
                            aF
                          ) 
                        );
      _xyPercent = true;
    }      
    if (dtXmlParserBase::hasAttribute("x_offset", *tE)) {
      _xOffset = dtXmlParserBase::muParseString( 
                          dtXmlParserBase::replaceDependencies(
                            dtXmlParserBase::getAttributeStr(
                              "x_offset", 
                              *tE
                            ),
                            cV, 
                            aF
                          ) 
                        );
    }
    if (dtXmlParserBase::hasAttribute("y_offset", *tE)) {
      _yOffset = dtXmlParserBase::muParseString( 
                          dtXmlParserBase::replaceDependencies(
                            dtXmlParserBase::getAttributeStr(
                              "y_offset", 
                              *tE
                            ),
                            cV, 
                            aF
                          ) 
                        );
    }      
    if (dtXmlParserBase::hasAttribute("normal_offset", *tE)) {
      _normalOffset = dtXmlParserBase::muParseString( 
                          dtXmlParserBase::replaceDependencies(
                            dtXmlParserBase::getAttributeStr(
                              "normal_offset", 
                              *tE
                            ),
                            cV, 
                            aF
                          ) 
                        );
    }          
    
    dt__info(init(),
            << dt__eval(_paraOneOffsetPercent) << std::endl
            << dt__eval(_paraTwoOffsetPercent) << std::endl
            << dt__eval(_normalOffset) << std::endl
            << dt__eval(_xOffset) << std::endl
            << dt__eval(_yOffset) << std::endl
            << dt__eval(_xyPercent) << std::endl
            << dt__eval(_nPieces) );
  }
}