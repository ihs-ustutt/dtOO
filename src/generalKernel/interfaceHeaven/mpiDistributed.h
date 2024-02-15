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

#ifndef mpiDistributed_H
#define	mpiDistributed_H

#include <dtOOTypeDef.h>

#include "staticPropertiesHandler.h"
#ifdef DTOO_HAS_MPI
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#endif

namespace dtOO {
  template< typename T >
  class mpiDistributed {
  public:
    dt__classOnlyName(mpiDistributed);    
    mpiDistributed() 
      : _thisRank(staticPropertiesHandler::getInstance()->thisRank()), 
        _nRanks(staticPropertiesHandler::getInstance()->nRanks()),
        _t(staticPropertiesHandler::getInstance()->nRanks()) {            
    }

    virtual ~mpiDistributed() {
      _t.clear();
    }
    
    T & operator()( void ) {
      return _t[ _thisRank ];
    };

    std::vector< T > & global( void ) {
      return _t;  
    }

    void clear( void ) {
      _t.clear();  
      _t.resize(_nRanks);
    }
    
    static void barrier( void ) {
      if ( staticPropertiesHandler::getInstance()->mpiParallel() ) {      
#ifdef DTOO_HAS_MPI    
        ::boost::mpi::communicator().barrier();
#endif
      }
    }  
    
    void distribute( void ) {
      if ( staticPropertiesHandler::getInstance()->mpiParallel() ) {
#ifdef DTOO_HAS_MPI
        ::boost::mpi::communicator cc;
        std::vector< T > all;
        ::boost::mpi::gather< T >(cc, _t[ _thisRank ], all, 0);
        if (_thisRank == 0) _t = all;
        ::boost::mpi::broadcast< T >(cc, &(_t[ 0 ]), _nRanks, 0);
#endif
      }
    }
    
    void broadcast( dtInt const & broadCastFrom ) {
      if ( staticPropertiesHandler::getInstance()->mpiParallel() ) {      
#ifdef DTOO_HAS_MPI    
        ::boost::mpi::communicator cc;
        ::boost::mpi::broadcast< T >(cc, &(_t[ 0 ]), _nRanks, broadCastFrom);
#endif
      }
    }    
  private:
    std::vector< T > _t;
    dtInt const _thisRank;
    dtInt const _nRanks;
  };
}

#endif	/* mpiDistributed_H */
