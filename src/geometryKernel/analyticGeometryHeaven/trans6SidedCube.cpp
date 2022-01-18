#include "trans6SidedCube.h"

#include <logMe/logMe.h>
#include <analyticFunctionHeaven/transIntCube.h>
#include "map2dTo3d.h"
#include "analyticSurface.h"
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/stringPrimitive.h>
#include "map3dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO { 
  trans6SidedCube::trans6SidedCube() : map3dTo3d() {
    
  }

  trans6SidedCube::trans6SidedCube(const trans6SidedCube& orig) 
    : map3dTo3d(orig) {
    _tI.reset( new transIntCube( *(orig._tI.get()) ) );
  }
  
  trans6SidedCube::trans6SidedCube(
    map2dTo3d const * const aS0, 
    map2dTo3d const * const aS1,
    map2dTo3d const * const aS2, 
    map2dTo3d const * const aS3, 
    map2dTo3d const * const aS4,
    map2dTo3d const * const aS5
  ) : map3dTo3d() {
    _tI.reset( new transIntCube(aS0, aS1, aS2, aS3, aS4, aS5) );
  }

  trans6SidedCube * trans6SidedCube::create( void ) const {
    return new trans6SidedCube();
  }
  
  trans6SidedCube * trans6SidedCube::clone( void ) const {
    return new trans6SidedCube(*this);
  }
  
  trans6SidedCube * trans6SidedCube::cloneTransformed( 
    dtTransformer const * const dtT  
  ) const {
    return new map3dTo3dTransformed< trans6SidedCube >(*this, dtT);
  }
    
  trans6SidedCube::~trans6SidedCube() {
    
  }
  
  dtPoint3 trans6SidedCube::getPoint( 
    dtReal const & uu, dtReal const & vv, dtReal const & ww 
  ) const {
    return _tI->getValue(uu, vv, ww);
  }
  
  bool trans6SidedCube::isClosed( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return false;
      case 1:
        return false;
      case 2:
        return false;      
      default:
        dt__throw(
          isClosed(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }
  }
  
  dtReal trans6SidedCube::getMin( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 0.;
      case 1:
        return 0.;
      case 2:
        return 0.;    
      default:
        dt__throw(
          getMin(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    
  }
  
  dtReal trans6SidedCube::getMax( dtInt const & dir) const {
    switch (dir) {
      case 0:
        return 1.;
      case 1:
        return 1.;
      case 2:
        return 1.;    
      default:
        dt__throw(
          getMax(),
          << dt__eval(dir) << std::endl
          << "dir should be 0, 1 or 2."
        );
    }    
  }

  map2dTo3d * trans6SidedCube::segmentConstU( dtReal const & uu ) const {
    if ( analyticGeometry::inUVWTolerance(uu, getUMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[1]->clone();
    }
    else if ( analyticGeometry::inUVWTolerance(uu, getUMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[3]->clone();
    }
    else {
      return map3dTo3d::segmentConstU(uu);
    }
  }
    
  map2dTo3d * trans6SidedCube::segmentConstV( dtReal const & vv ) const {
    if ( analyticGeometry::inUVWTolerance(vv, getVMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[4]->clone();      
    }
    else if ( analyticGeometry::inUVWTolerance(vv, getVMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[5]->clone();      
    }
    else {
      return map3dTo3d::segmentConstV(vv);
    }
  }
    
  map2dTo3d * trans6SidedCube::segmentConstW( dtReal const & ww ) const {
    if ( analyticGeometry::inUVWTolerance(ww, getWMin()) ) {
      return _tI->getConstRefToMap2dTo3d()[2]->clone();      
    }
    else if ( analyticGeometry::inUVWTolerance(ww, getWMax()) ) {
      return _tI->getConstRefToMap2dTo3d()[0]->clone();      
    }
    else {
      return map3dTo3d::segmentConstW(ww);
    }
  }
  
  vectorHandling< map2dTo3d const * > const & 
  trans6SidedCube::constRefMap2dTo3d( void ) const {
    return _tI->getConstRefToMap2dTo3d();
  }
  
  void trans6SidedCube::reverseU( void ) {
    _tI.reset(
      new transIntCube(
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[0]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[3],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[2]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[1],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[4]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[5]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get()
      )
    );
  }

  void trans6SidedCube::reverseV( void ) {
    _tI.reset( 
      new transIntCube(    
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[0]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[1]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[2]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get(),      
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[3]
                )->ptrConstDtSurface(),
                true, 
                false
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[5],
        _tI->getConstRefToMap2dTo3d()[4]    
      )
    );    
  }

  void trans6SidedCube::reverseW( void ) {
    _tI.reset( 
      new transIntCube(
        _tI->getConstRefToMap2dTo3d()[2],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[1]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[0],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[3]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[4]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              geomSurface_surfaceReverseConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[5]
                )->ptrConstDtSurface(),
                false, 
                true
              ).result()
            ).get()
          )
        ).get()    
      )
    );    
  }  
  
  void trans6SidedCube::exchangeUV( void ) {
    _tI.reset( 
      new transIntCube(    
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[0]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[4],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[2]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
         ).get(),
        _tI->getConstRefToMap2dTo3d()[5],
        _tI->getConstRefToMap2dTo3d()[1],
        _tI->getConstRefToMap2dTo3d()[3]    
      )
    );
  }
  
  void trans6SidedCube::exchangeUW( void ) {
    _tI.reset(     
      new transIntCube(    
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[3]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[2]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[1]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[0]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[4]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[5]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
         ).get()
      )
    );    
  }
    
  void trans6SidedCube::exchangeVW( void ) {
    _tI.reset(     
      new transIntCube(    
        _tI->getConstRefToMap2dTo3d()[5],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[1]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
        ).get(),
        _tI->getConstRefToMap2dTo3d()[4],
        dt__pH(analyticSurface)(
          new analyticSurface(
            dt__pH(dtSurface)(
              bSplineSurface_exchangeSurfaceConstructOCC(
                analyticSurface::MustConstDownCast(
                  _tI->getConstRefToMap2dTo3d()[3]
                )->ptrConstDtSurface()
              ).result()
            ).get()
          )
         ).get(),
        _tI->getConstRefToMap2dTo3d()[2],   
        _tI->getConstRefToMap2dTo3d()[0]
      )
    );      
  }
}
