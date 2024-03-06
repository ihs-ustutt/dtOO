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

#include "qShapeMetric.h"

#include <gmsh.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <logMe/logMe.h>

namespace dtOO {
  qShapeMetric::qShapeMetric() {
  }

  qShapeMetric::~qShapeMetric() {
  }

  dtReal qShapeMetric::operator()( ::MElement const * const me ) {
    ::MElement * nC = const_cast< ::MElement * >(me);
    ::MPyramid * pyr = dynamic_cast< ::MPyramid * >(nC);
    ::MTetrahedron * tet = dynamic_cast< ::MTetrahedron * >(nC);
    
    if (pyr) return calculatePyramid(pyr);
    else if(tet) return calculateTetrahedron(tet);
    else dt__throwUnexpected(operator()()); 
  }

  dtReal qShapeMetric::calculatePyramid( ::MPyramid * pyr ) {
    ::MTetrahedron tet0(
      pyr->getVertex(0), 
      pyr->getVertex(2),
      pyr->getVertex(4),
      pyr->getVertex(3),
      -1
    );
    ::MTetrahedron tet1(
      pyr->getVertex(4), 
      pyr->getVertex(2),
      pyr->getVertex(0),
      pyr->getVertex(1),
      -1
    );
    ::MTetrahedron tet2(
      pyr->getVertex(4), 
      pyr->getVertex(3),
      pyr->getVertex(1),
      pyr->getVertex(2),
      -1
    );
    ::MTetrahedron tet3(
      pyr->getVertex(1), 
      pyr->getVertex(3),
      pyr->getVertex(4),
      pyr->getVertex(0),
      -1
    );  
    if( pyr->getVolume()<0. ) {
      tet0.reverse();
      tet1.reverse();
      tet2.reverse();
      tet3.reverse();
    }
    
    return
      std::min(
        std::min(
          std::min(
#if GMSH_API_VERSION_MAJOR>=4 && GMSH_API_VERSION_MINOR>=9            
            fabs(tet0.getInnerRadius()/tet0.getOuterRadius()),
            fabs(tet1.getInnerRadius()/tet1.getOuterRadius())
          ),
          fabs(tet2.getInnerRadius()/tet2.getOuterRadius())
        ),
        fabs(tet3.getInnerRadius()/tet3.getOuterRadius())
#else        
            fabs(tet0.getInnerRadius()/tet0.getCircumRadius()),
            fabs(tet1.getInnerRadius()/tet1.getCircumRadius())
          ),
          fabs(tet2.getInnerRadius()/tet2.getCircumRadius())
        ),
        fabs(tet3.getInnerRadius()/tet3.getCircumRadius())
#endif        
      ) * 3. 
      / 
      .86034;
  }  

  dtReal qShapeMetric::calculateTetrahedron( ::MTetrahedron * tet ) {
#if GMSH_API_VERSION_MAJOR>=4 && GMSH_API_VERSION_MINOR>=9        
    return 3. * fabs(tet->getInnerRadius()/tet->getOuterRadius());
#else            
    return 3. * fabs(tet->getInnerRadius()/tet->getCircumRadius());
#endif
  }    
}
