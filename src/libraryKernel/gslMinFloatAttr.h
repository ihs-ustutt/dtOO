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

#ifndef gslMinFloatAttr_H
#define	gslMinFloatAttr_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <attributionHeaven/floatAtt.h>

namespace dtOO { 

  class gslMinFloatAttr {
    public:  
      dt__classOnlyName(gslMinFloatAttr);
      gslMinFloatAttr(
        floatAtt * attribute,
        std::vector< dtReal > const & guess,
        std::vector< dtReal > const & step,
        dtReal const & precision,
        dtInt const & maxIterations = 100
      );
      gslMinFloatAttr(
        floatAtt * attribute,
        dtPoint2 const & guess,
        dtPoint2 const & step,
        dtReal const & precision,
        dtInt const & maxIterations = 100
      );
      gslMinFloatAttr(
        floatAtt * attribute,
        dtPoint3 const & guess,
        dtPoint3 const & step,
        dtReal const & precision,
        dtInt const & maxIterations = 100
      );
     
      gslMinFloatAttr( gslMinFloatAttr const & orig );
      virtual ~gslMinFloatAttr();
      virtual gslMinFloatAttr * clone( void ) const;
 
      std::vector< dtReal > const & guess() const;
      std::vector< dtReal > const & step() const;
      dtReal const & precision() const;
      dtInt const & maxIterations() const;
      bool const & converged() const;
      std::vector< dtReal > const & result() const;
      std::string const & lastStatus() const;
      dtInt const & dimension() const;
      //
      // set result
      //
      void converged( bool const converged );
      void result( std::vector< dtReal > const result );
      void lastStatus( std::string const & lastStatus );
      bool perform();
      floatAtt const * const ptrAttribute( void ) const;
      //void setFloatAttr( dt__pH(floatAtt) attribute );
      //void setFloatAttr( floatAtt * attribute );
    private:
      dtInt const _dimension;
      std::vector< dtReal> const _guess;
      std::vector< dtReal> const _step;
      dtReal const _precision;
      dtInt const _maxIterations;
      bool _converged;
      std::vector< dtReal > _result;
      std::string _lastStatus;
      dt__pH(floatAtt) _attribute;
     
  };
  dt__H_addCloneForpVH(gslMinFloatAttr);
}
#endif	/* gslMinFloatAttr_H */
