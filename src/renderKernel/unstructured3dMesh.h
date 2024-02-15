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

#ifndef UNSTRUCTURED3DMESH_H
#define	UNSTRUCTURED3DMESH_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <vector>

namespace dtOO {
  class unstructured3dMesh : public renderInterface {
    public:
      dt__class(unstructured3dMesh, renderInterface);    
      unstructured3dMesh();
      virtual ~unstructured3dMesh();
      void addPoints( std::vector< dtPoint3 > const & pp );
      void addPoint( dtPoint3 const & pp );
      void addElement( std::vector< dtInt > const & el );
      std::vector< dtPoint3 > const & refP3( void ) const;    
      twoDArrayHandling< dtInt > const & refEl( void ) const;
      dtInt getNHex( void ) const;
      dtInt getNTet( void ) const;
      dtInt getNPyr( void ) const;
      dtInt getNPri( void ) const;
    private:
      std::vector< dtPoint3 > _pp;
      twoDArrayHandling< dtInt > _el;
      dtInt _nTet;
      dtInt _nHex;
      dtInt _nPyr;
      dtInt _nPri;
  };
}
#endif	/* UNSTRUCTURED3DMESH_H */
