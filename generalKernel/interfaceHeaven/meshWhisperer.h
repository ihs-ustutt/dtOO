#ifndef MESHWHISPERER_H
#define	MESHWHISPERER_H

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
        int const & onWhatTag, 
        int const & posU, int const & posV 
      );      
      void add( ::MElement * me, ::GEntity * ge );      
      void renumberVertices( int const & leader );      
      void addRenumberedVertices( void );
      void renumberElements( int const & leader );      
      void addRenumberedElements( void );
    private:
      int _thisRank;
      int _nRanks;
      dtGmshModel * const _gm;
      mpiDistributed< std::string > _generalInfo;
      mpiDistributed< std::vector< float > > _x;
      mpiDistributed< std::vector< float > > _y;
      mpiDistributed< std::vector< float > > _z;
      mpiDistributed< std::vector< double > > _u;
      mpiDistributed< std::vector< double > > _v;
      mpiDistributed< std::vector< int > > _mvType;
      mpiDistributed< std::vector< int > > _onWhatTag;
      mpiDistributed< std::vector< int > > _onWhatDim;
      mpiDistributed< std::vector< int > > _num;
      mpiDistributed< std::vector< int > > _tFVertexPosition;
      mpiDistributed< std::vector< int > > _tFVertexOnWhatTag;
      mpiDistributed< std::vector< int > > _tFVertexPosU;
      mpiDistributed< std::vector< int > > _tFVertexPosV;
      std::vector< ::MVertex * > _mv;
      
      mpiDistributed< std::vector< int > > _meOnWhatTag;
      mpiDistributed< std::vector< int > > _meOnWhatDim;      
      mpiDistributed< std::vector< int > > _meNum;
      mpiDistributed< std::vector< int > > _meElementStartPosition;
      mpiDistributed< std::vector< int > > _meVertexNum;      
      mpiDistributed< std::vector< int > > _meTypeMSH;
      std::vector< ::MElement * > _me;
  };
}
#endif	/* MESHWHISPERER_H */

