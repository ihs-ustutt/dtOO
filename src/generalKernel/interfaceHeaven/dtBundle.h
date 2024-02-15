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

#ifndef dtBundle_H
#define	dtBundle_H

#include <dtOOTypeDef.h>

#include <string>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class baseContainer;

//! Bundle to keep baseContainer and labeledVectorHandlings. 
/*! The bundle keeps baseContainer and labeledVectorHandlings as dt__pH.
 */
  class dtBundle {
    public:  
      dt__classOnlyName(dtBundle);
      dtBundle(); 
      virtual ~dtBundle();
     /*! 
       * @return Constant pointer to baseContainer.
       *
       *   Get constant pointer to baseContainer
       */
      baseContainer * const cptr_bC( void ) const;
     /*! 
       * @return Constant pointer to lvH_constValue.
       *
       *   Get constant pointer to lvH_constValue.
       */
      lvH_constValue * const cptr_cV( void ) const;
     /*! 
       * @return Constant pointer to lvH_analyticFunction.
       *
       *   Get constant pointer to lvH_analyticFunction.
       */
      lvH_analyticFunction * const cptr_aF( void ) const;
     /*! 
       * @return Constant pointer to lvH_analyticGeometry.
       *
       *   Get constant pointer to lvH_analyticGeometry.
       */
      lvH_analyticGeometry * const cptr_aG( void ) const;
     /*! 
       * @return Constant pointer to lvH_boundedVolume.
       *
       *   Get constant pointer to lvH_boundedVolume.
       */
      lvH_boundedVolume * const cptr_bV( void ) const;
     /*! 
       * @return Constant pointer to lvH_dtCase.
       *
       *   Get constant pointer to lvH_dtCase.
       */
      lvH_dtCase * const cptr_dC( void ) const;
     /*! 
       * @return Constant pointer to lvH_dtPlugin.
       *
       *   Get constant pointer to lvH_dtPlugin.
       */
      lvH_dtPlugin * const cptr_dP( void ) const;
      /*! 
       * @param keepConstValue Flag to keep constValue.
       *
       *   Clear whole bundle. If keepConstValue is true, then lvH_constValue 
       *   is kept.
       */
      void clear( bool const keepConstValue = false );
 
    private:
      dt__pH(baseContainer) _bC;
      dt__pH(lvH_constValue) _cV;
      dt__pH(lvH_analyticFunction) _aF;
      dt__pH(lvH_analyticGeometry) _aG;
      dt__pH(lvH_boundedVolume) _bV;
      dt__pH(lvH_dtCase) _dC;
      dt__pH(lvH_dtPlugin) _dP;
  };
}
#endif	/* dtBundle_H */
