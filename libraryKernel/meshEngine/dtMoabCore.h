#ifndef DTMOABCORE_H
#define	DTMOABCORE_H

#include <moab/Core.hpp>
#include <moab/Range.hpp>
#include <logMe/dtMacros.h>

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
    virtual ~dtMoabCore();
    void addVertices( std::vector< ::MVertex const * > const & mv );
    moab::Range addElements( std::vector< ::MElement const * > const & me );
  private:
    moab::ReadUtilIface * _readUtilIface;
		std::map<long, moab::EntityHandle> _node_id_map;      
  };
}
#endif	/* DTMOABCORE_H */