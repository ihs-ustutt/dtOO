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

#ifndef DTCURVE_H
#define	DTCURVE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
    class dtCurve {
    public:
      dt__classOnlyName(dtCurve);     
      //
      // overload
      //
      dtCurve();
      virtual ~dtCurve();
     	virtual dtCurve * clone( void ) const = 0;
      virtual dtReal minPara ( dtInt const & dir ) const = 0;
      virtual dtReal maxPara ( dtInt const & dir ) const = 0;
      virtual dtInt order( void ) const = 0;
      virtual bool closed( void ) const = 0;
      virtual dtPoint3 point( dtReal const uu ) const = 0;
      virtual dtVector3 firstDer( dtReal const uu) const = 0;
      virtual dtInt nControlPoints( void ) const = 0;
      virtual dtPoint3 controlPoint( dtInt const nPoint ) const = 0;
      virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point ) = 0;    
      virtual dtReal l_u( dtReal const uu ) const = 0;
      virtual dtReal u_l( dtReal const length ) const = 0;
      virtual dtReal reparam(dtPoint3 const point) const;
      virtual void revert( void ) = 0;
      virtual void translate( dtVector3 const & tt ) = 0;      
      //
      // optional overload
      //
      virtual dtInt continuity( void ) const;
      void dump(void);
      virtual std::string dumpToString(void) const;
      //
      // 
      //
      dtReal getUMin ( void ) const;
      dtReal getUMax ( void ) const;      
      dtReal u_uPercent(dtReal const percent) const;
      dtReal uPercent_u(dtReal const uu) const;
      dtPoint3 pointPercent( dtReal const percent ) const;
      dtVector3 firstDerPercent( dtReal const percent) const;    
      dtReal length(void) const;
      dtReal l_uPercent(dtReal const percent) const;
      dtReal lPercent_u(dtReal const uu) const;
      dtReal lPercent_uPercent(dtReal const uuPercent) const;
      dtReal u_lPercent( dtReal const percent ) const;
      dtReal uPercent_lPercent( dtReal const percent ) const;
      dtReal uPercent_l( dtReal const percent ) const;      
    };
    dt__H_addCloneForpVH(dtCurve);        
}
#endif	/* DTCURVE_H */
