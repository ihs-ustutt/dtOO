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

#ifndef DTOCCCURVE_H
#define	DTOCCCURVE_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
#include "dtCurve.h"

class Geom_Curve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCCurve : public dtCurve {
    public:
      dt__class(dtOCCCurve, dtCurve);
      dtOCCCurve();
      dtOCCCurve( dtOCCCurveBase const & orig);
      virtual dtOCCCurve * clone( void ) const = 0;
      virtual ~dtOCCCurve();
      //
      // overload
      //
      virtual dtReal minPara ( dtInt const & dir ) const;
      virtual dtReal maxPara ( dtInt const & dir ) const;
  //    virtual dtInt getKind( void ) const;
      virtual bool closed( void ) const;
      virtual dtPoint3 point( dtReal const uu ) const;
      virtual dtVector3 firstDer( dtReal const uu) const;
      virtual dtReal l_u( dtReal const uu ) const;
      virtual dtReal u_l( dtReal const length ) const;
      virtual dtReal reparam(dtPoint3 const point) const;
      //
      // optional
      //   
      virtual dtInt continuity( void ) const;
  	  virtual std::string dumpToString( void ) const;
      /**
       * @todo Do this in external builder.
       */
      virtual void revert(void);   
      /**
       * @todo Do this in external builder.
       */
  	  virtual void translate( dtVector3 const & tt );
      dtOCCCurveBase const & OCCRef( void ) const;
      dtOCCCurveBase & OCCRef( void );
    private:
      ptrHandling< dtOCCCurveBase > _curve;
      Geom_Curve const * _ptr;
  };
  dt__H_addCloneForpVH(dtOCCCurve);
}
#endif	/* DTOCCCURVE_H */
