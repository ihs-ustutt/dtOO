#ifndef DTMOABCORE_H
#define	DTMOABCORE_H

#include <moab/Core.hpp>
#include <moab/Range.hpp>
#include <logMe/dtMacros.h>
#include "dtOMVertexField.h"
#include "dtOMEdgeField.h"

class MElement;
class MVertex;

namespace moab {
  class ReadUtilIface;  
}

namespace dtOO {
  class dtGmshFace;  
  
  class dtMoabCore : public moab::Core {
  public:
    dt__CLASSNAME(dtMoabCore);    
    dtMoabCore();
    dtMoabCore( dtOMMesh const & om );
    virtual ~dtMoabCore();
    void addVertices( std::vector< ::MVertex const * > const & mv );
    moab::Range addElements( std::vector< ::MElement const * > const & me );
    void addVertexField( dtOMVertexField< bool > const & field );
	  void addVertexField( dtOMVertexField< int > const & field );
    void addVertexField( dtOMVertexField< float > const & fF );
    void addVertexField( dtOMVertexField< dtVector3 > const & vF );
    void addEdgeField( dtOMEdgeField< float > const & eF );    
  private:
    moab::ReadUtilIface * _readUtilIface;
		std::map<long, moab::EntityHandle> _node_id_map;
    std::map<omEdgeH, moab::EntityHandle> _edge_id_map;
    dt__pVH(::MElement) _edgeElement;
  };
}
#endif	/* DTMOABCORE_H */