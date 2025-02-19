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

#ifndef dtPoint3Algo_H
#define	dtPoint3Algo_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

namespace dtOO { 
  class analyticGeometry;

  class dtPoint3Algo {
    public:  
      dt__classOnlyName(dtPoint3Algo);
      // 2-dimensional constructor
      dtPoint3Algo( 
        dtPoint3 const & p3, 
        dtPoint2 const & guess,
        dtPoint2 const & step,
        dtReal const & precision,
        dtInt const & maxIterations
      );
      // 3-dimensional constructor
      dtPoint3Algo( 
        dtPoint3 const & p3, 
        dtPoint3 const & guess,
        dtPoint3 const & step,
        dtReal const & precision,
        dtInt const & maxIterations
      );
     
      virtual ~dtPoint3Algo();
      dtPoint3Algo( dtPoint3Algo const & orig );
      virtual dtPoint3Algo * clone( void ) const = 0;
      //
      // visitor overload
      //
      virtual void visit( analyticGeometry const * const aG ) = 0;
      //
      // get properties
      //
      dtInt const & dimension() const;
      dtPoint3 const & p3(void) const;
      std::vector< dtReal > const & guess() const;
      std::vector< dtReal > const & step() const;
      dtReal const & precision() const;
      dtInt const & maxIterations() const;
      bool const & converged() const;
      std::vector< dtReal > const & result() const;
      std::string const & lastStatus() const;
      //
      // set result
      //
      void converged( bool const converged );
      void result( std::vector< dtReal > const result );
      void lastStatus( std::string const & lastStatus );
    private:
      dtPoint3 const _p3;
      dtInt const _dim;
      std::vector< dtReal> const _guess;
      std::vector< dtReal> const _step;
      dtReal const _precision;
      dtInt const _maxIterations;
      bool _converged;
      std::vector< dtReal > _result;
      std::string _lastStatus;
  };
  dt__H_addCloneForpVH(dtPoint3Algo);
}
#endif	/* dtPoint3Algo_H */
