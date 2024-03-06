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

#include "offset.h"
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include "progHelper.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <logMe/logMe.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool offset::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(offset, new offset())
  );
  
  offset::offset() : dtTransformer() {
    _nPieces = 1;
    _xyPercent = false;
  }
  
  offset::offset( offset const & orig ) {
    _paraOneOffsetPercent = orig._paraOneOffsetPercent;
    _paraTwoOffsetPercent = orig._paraTwoOffsetPercent;
    _xyPercent = orig._xyPercent;
    _xOffset = orig._xOffset;
    _yOffset = orig._yOffset;
    _normalOffset = orig._normalOffset;
    _nPieces = orig._nPieces;
  }

  offset::~offset() {
  }

  dtTransformer * offset::clone( void ) const {
	  return new offset(*this);
	}
	
  dtTransformer * offset::create( void ) const {
		return new offset();
	}
	
  lvH_analyticGeometry offset::apply( 
    lvH_analyticGeometry const * const toTrans 
  ) const {
    lvH_analyticGeometry retAGeo;

    dt__forAllRefAuto(*toTrans, aTrans){
      //each piece
      dt__forFromToIndex(0, _nPieces, jj) {
        analyticSurface * aS 
        = 
        analyticSurface::MustConstDownCast(aTrans)->clone();

        aS->offsetNormal( _normalOffset );

        retAGeo.push_back( aS );
      }
    }
    return retAGeo;
  }

  lvH_analyticFunction offset::apply( 
    lvH_analyticFunction const * const toTrans 
  ) const { 
    lvH_analyticFunction retSFun;

    //each analyticGeometry
    dt__forAllRefAuto(*toTrans, aTrans) {
      scaCurve2dOneD  * s2d 
      = 
      scaCurve2dOneD::MustConstDownCast(aTrans)->clone();
      //
      // translation vector
      //
      dtVector2 transVec;
      if (_xyPercent == true ) {
        dtReal xRange = s2d->xMax(0) - s2d->xMin(0);
        dtReal yRange 
        = 
        s2d->YFloat(s2d->xMax(0)) - s2d->YFloat(s2d->xMin(0));
        transVec = dtVector2(_xOffset * xRange, _yOffset * yRange);
      }
      else {
        transVec = dtVector2(_xOffset, _yOffset);
      }

      //each piece
      dt__forFromToIndex(0, _nPieces, jj) {
        //convert integer to dtReal      
        dtReal jjF = (dtReal) (jj+1);
        s2d->translate( jjF * transVec );

        //
        // push translated geometry in vector
        //
        retSFun.push_back( s2d );
      }
    }
    return retSFun;    
  }

  bool offset::isNecessary( void ) const {
    return true;
  }

  void offset::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    if (dtXmlParserBase::hasAttribute("parameter_one_offset_percent", *tE)) {
      _paraOneOffsetPercent 
      = 
      dtXmlParserBase::muParseString( 
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
      _paraTwoOffsetPercent 
      = 
      dtXmlParserBase::muParseString( 
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
      _nPieces 
      = 
      dtXmlParserBase::muParseString( 
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
      _xOffset 
      = 
      dtXmlParserBase::muParseString( 
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
      _yOffset 
      = 
      dtXmlParserBase::muParseString( 
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
      _xOffset 
      = 
      dtXmlParserBase::muParseString( 
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
      _yOffset 
      = 
      dtXmlParserBase::muParseString( 
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
      _normalOffset 
      = 
      dtXmlParserBase::muParseString( 
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
    
    dt__info(
      init(),
      << dt__eval(_paraOneOffsetPercent) << std::endl
      << dt__eval(_paraTwoOffsetPercent) << std::endl
      << dt__eval(_normalOffset) << std::endl
      << dt__eval(_xOffset) << std::endl
      << dt__eval(_yOffset) << std::endl
      << dt__eval(_xyPercent) << std::endl
      << dt__eval(_nPieces) 
    );
  }
}
