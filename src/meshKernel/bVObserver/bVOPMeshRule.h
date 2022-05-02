#ifndef bVOPMeshRule_H
#define	bVOPMeshRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/labeledVectorHandling.h>

class GEntity;

namespace dtOO {
  class dtMeshOperator;
  class dtGmshVertex;
  class dtGmshRegion;
  
  class bVOPMeshRule : public bVOInterface {
    public:
      dt__class(bVOPMeshRule, bVOInterface);
      dt__classSelfCreate(bVOPMeshRule);
      bVOPMeshRule();
      virtual ~bVOPMeshRule();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      static bool isMeshed(dtGmshVertex const * const gent);
      static bool isMeshed(dtGmshRegion const * const gent);
      template < typename T >
      static bool isMeshed(T const * const gent);
      static bool isOwnMaster(::GEntity const * const gent);
      template < typename T >
      static std::map< ::GEntity *, dtInt > prepareDistribution(
        std::list< T > list, std::vector< dtReal > weight
      );      
    private:
      labeledVectorHandling< dtMeshOperator * > _meshOperator;
      std::vector< std::string > _rule1D;
      std::vector< std::string > _rule2D;
      std::vector< std::string > _rule3D;
      std::vector< std::string > _only;
      std::vector< dtReal > _edgeWeight;
      std::vector< dtReal > _faceWeight;
      std::vector< dtReal > _regionWeight;
      static bool _registrated;
  };
}
#endif	/* bVOPMeshRule_H */

