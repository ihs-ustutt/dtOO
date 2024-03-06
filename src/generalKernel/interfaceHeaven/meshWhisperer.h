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

#ifndef MESHWHISPERER_H
#define	MESHWHISPERER_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

#include <interfaceHeaven/mpiDistributed.h>
#include <map>

class GEntity;
class MVertex;
class MElement;

namespace dtOO {  
  class dtGmshModel;
  
  class meshWhisperer {
    public:
      dt__classOnlyName(meshWhisperer);    
      meshWhisperer( dtGmshModel * const gm );
      virtual ~meshWhisperer();
      void add( ::GEntity const & ge );
      void distribute( void );
      void add(::GEntity * ge );
    private:
      void add( ::MVertex * mv );
      void makeTransfinite( 
        ::MVertex const * const mv, 
        dtInt const & onWhatTag, 
        dtInt const & posU, dtInt const & posV 
      );      
      void add( ::MElement * me, ::GEntity * ge );      
      void renumberVertices( dtInt const & leader );      
      void addRenumberedVertices( void );
      void renumberElements( dtInt const & leader );      
      void addRenumberedElements( void );
    private:
      dtInt _thisRank;
      dtInt _nRanks;
      dtGmshModel * const _gm;
      mpiDistributed< std::string > _generalInfo;
      mpiDistributed< std::vector< dtReal > > _x;
      mpiDistributed< std::vector< dtReal > > _y;
      mpiDistributed< std::vector< dtReal > > _z;
      mpiDistributed< std::vector< double > > _u;
      mpiDistributed< std::vector< double > > _v;
      mpiDistributed< std::vector< dtInt > > _mvType;
      mpiDistributed< std::vector< dtInt > > _onWhatTag;
      mpiDistributed< std::vector< dtInt > > _onWhatDim;
      mpiDistributed< std::vector< dtInt > > _num;
      mpiDistributed< std::vector< dtInt > > _tFVertexPosition;
      mpiDistributed< std::vector< dtInt > > _tFVertexOnWhatTag;
      mpiDistributed< std::vector< dtInt > > _tFVertexPosU;
      mpiDistributed< std::vector< dtInt > > _tFVertexPosV;
      std::vector< ::MVertex * > _mv;
      
      mpiDistributed< std::vector< dtInt > > _meOnWhatTag;
      mpiDistributed< std::vector< dtInt > > _meOnWhatDim;      
      mpiDistributed< std::vector< dtInt > > _meNum;
      mpiDistributed< std::vector< dtInt > > _meElementStartPosition;
      mpiDistributed< std::vector< dtInt > > _meVertexNum;      
      mpiDistributed< std::vector< dtInt > > _meTypeMSH;
      std::vector< ::MElement * > _me;
  };
}
#endif	/* MESHWHISPERER_H */
