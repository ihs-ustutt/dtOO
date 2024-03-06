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

#include "dtOMMeshDivided.h"
#include "progHelper.h"
#include <interfaceHeaven/stringPrimitive.h>

namespace dtOO {
  dtOMMeshDivided::dtOMMeshDivided() {
  }

  dtOMMeshDivided::~dtOMMeshDivided() {
  }
  
  void dtOMMeshDivided::add( dtOMMesh const & om ) {
    _part.push_back(om);
  }
  
  dtOMMesh dtOMMeshDivided::connect( void ) {
    dt__throwIf(_part.size()<1, connect());
    
    //
    // create initial mesh from last part in vector
    //
    dtOMMesh result;
    result.add( _part.back() );
    _part.pop_back();
    
    //
    // loop through _part vector
    //
    while (_part.size() > 0) {
      //
      // get an edge at boundary
      //
      omEdgeH connectEdge;
      dt__forFromToIter( 
        omEdgeI, result.edges_begin(), result.edges_end(), eIt
      ) {
        if ( result.is_boundary( *eIt ) ) {
          connectEdge = *eIt;
          break;
        }
      }
      dt__throwIf( !connectEdge.is_valid(), connect() );
      dt__throwIf( !result.is_boundary(connectEdge), connect() );

      //
      // get according halfedge at boundary
      //
      omHalfedgeH connectHalfedge;
      if ( result.is_boundary( result.halfedge_handle( connectEdge, 0 ) ) ) {
        connectHalfedge = result.halfedge_handle( connectEdge, 1 );
      }
      else {
        connectHalfedge = result.halfedge_handle( connectEdge, 0 );
      }
      dt__throwIf( !connectHalfedge.is_valid(), connect() );
      
      omVertexH omVH0 = result.from_vertex_handle( connectHalfedge );
      omVertexH omVH1 = result.to_vertex_handle( connectHalfedge );
      ::MVertex const * const mv0 = result.at( omVH0 );
      ::MVertex const * const mv1 = result.at( omVH1 );
        
      //
      // find corresponding face in part
      //
      dt__forAllIter( std::list< dtOMMesh >, _part, aPartIt ) {
        dtOMMesh const & aPart = *aPartIt;
        //
        // check if two connect vertices are part of this part
        //
        if ( aPart.contains( mv0 ) && aPart.contains( mv1 ) ) {
          omVertexH omVH0_part = aPart.at( mv0 );
          omVertexH omVH1_part = aPart.at( mv1 );
          //
          // determine corresponding halfedge in part mesh
          //
          omHalfedgeH connectHalfedge_part;
          dt__forFromToIter(
            omConstVertexIHalfedgeI, 
            aPart.cvih_begin(omVH0_part), aPart.cvih_end(omVH0_part), 
            vihIt
          ) {
            if ( aPart.from_vertex_handle(*vihIt) == omVH1_part ) {
              connectHalfedge_part = *vihIt;
            }
          }
          if ( !connectHalfedge_part.is_valid() ) {
            dt__forFromToIter(
              omConstVertexOHalfedgeI, 
              aPart.cvoh_begin(omVH0_part), aPart.cvoh_end(omVH0_part), 
              vohIt
            ) {
              if ( aPart.to_vertex_handle(*vohIt) == omVH1_part ) {
                connectHalfedge_part = *vohIt;
              }
            }
          }
          
          //
          // flip if halfedge is on boundary
          //
          if ( aPart.is_boundary( connectHalfedge_part ) ) {
            connectHalfedge_part 
            = 
            aPart.opposite_halfedge_handle( connectHalfedge_part );
          }
          dt__throwIf(!connectHalfedge_part.is_valid(), connect());
          
                    //
          // check if part is inverted
          //
          if (
            aPart.at( aPart.from_vertex_handle(connectHalfedge_part) )
            == 
            result.at( result.to_vertex_handle(connectHalfedge) )
          ) {
            dt__info( 
              connect(), 
              << "Add part inverted ( " << _part.size()-1 << " left ) "
            );
            result.add(aPart);
          }
          else {
            dt__info( 
              connect(), 
              << "Add part ( " << _part.size()-1 << " left ) "
            );
            result.addInv(aPart);
          }
          
          _part.erase( aPartIt );
          break;
        }
      }
    }
    
    return result;
  }
}
