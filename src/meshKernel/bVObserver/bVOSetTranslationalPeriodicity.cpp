#include "bVOSetTranslationalPeriodicity.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <baseContainerHeaven/baseContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <dtTransformerHeaven/translate.h>
#include <progHelper.h>

#include <gmsh/Context.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOSetTranslationalPeriodicity::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(
      bVOSetTranslationalPeriodicity, 
      new bVOSetTranslationalPeriodicity()
    )
  );
  
  bVOSetTranslationalPeriodicity::bVOSetTranslationalPeriodicity() {
  }

  bVOSetTranslationalPeriodicity::~bVOSetTranslationalPeriodicity() {
    
  }
  
  void bVOSetTranslationalPeriodicity::bVOSetTranslationalPeriodicity::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
		boundedVolume * attachTo
  ) {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("faceMaster", element)
      ||
      !dtXmlParserBase::hasAttribute("faceSlave", element)
      ||
      !dtXmlParserBase::hasAttribute("transformer", element),
      init()
    );
    
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
		// <bVObserver 
		//   name="bVOSetTranslationalPeriodicity" 
		//   faceMaster="name0"
    //   faceSlave="name1"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceMaster = dtXmlParserBase::getAttributeStr("faceMaster", element);
    _faceSlave = dtXmlParserBase::getAttributeStr("faceSlave", element);
    
    dt__ptrAss(
      _dtT,
      translate::ConstDownCast(
        bC->constPtrTransformerContainer()->get(
          dtXmlParserBase::getAttributeStr("transformer", element)
        )
      )
    );
  }
  
  void bVOSetTranslationalPeriodicity::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    //
    // get faces
    //		
    dtGmshFace * const masterF = gm->getDtGmshFaceByPhysical(_faceMaster);
    dtGmshFace * const slaveF = gm->getDtGmshFaceByPhysical(_faceSlave);
    dt__throwIf(masterF==slaveF, preUpdate());
    //
    // get edges
    //
    std::vector< ::GEdge * > slaveEdge = slaveF->edges();
    std::vector< ::GEdge * > masterEdge = masterF->edges();
    
    //
    // create unique vectors of all vertices of slave and master face
    //
    std::vector< ::GVertex * > slaveVertex;
    dt__forAllIndex( slaveEdge, ii ) {
      slaveVertex.push_back( slaveEdge[ii]->getBeginVertex() );
      slaveVertex.push_back( slaveEdge[ii]->getEndVertex() );
    }
    progHelper::removeBastardTwins( slaveVertex );
    std::vector< ::GVertex * > masterVertex;
    dt__forAllIndex( masterEdge, ii ) {
      masterVertex.push_back( masterEdge[ii]->getBeginVertex() );
      masterVertex.push_back( masterEdge[ii]->getEndVertex() );      
    }
    progHelper::removeBastardTwins( masterVertex );
    
    //
    // find mapping between master and slave periodical vertices
    //
    std::map< ::GVertex *, ::GVertex * > master_slave_v;
    std::map< ::GVertex *, ::GVertex * > slave_master_v;
    std::vector< dtReal > distance;
    dt__forAllRefAuto(masterVertex, aVertex) {
      dtPoint3 aVertex_t
      = 
      (*_dtT)( dtGmshModel::extractPosition(aVertex), -1 );
      
      auto minSlave
      =
      std::min_element(
        slaveVertex.cbegin(), 
        slaveVertex.cend(), 
        [=]( 
          const ::GVertex * vA, 
          const ::GVertex * vB 
        ) {  
          dtPoint3 const vA_t 
          = 
          (*_dtT)( dtGmshModel::extractPosition(vA), -1 );
          dtPoint3 const vB_t
          = 
          (*_dtT)( dtGmshModel::extractPosition(vB), -1 );
          return 
            (
              fabs(
                aVertex_t.x()-vA_t.x()
              ) 
              +
              fabs(
                aVertex_t.y()-vA_t.y()
              )                  
              +
              fabs(
                aVertex_t.z()-vA_t.z()
              )
            )
            < 
            (
              fabs(
                aVertex_t.x()-vB_t.x()
              ) 
              +
              fabs(
                aVertex_t.y()-vB_t.y()
              )                  
              +
              fabs(
                aVertex_t.z()-vB_t.z()
              )
            );
        }
      );
      
      // mappings
      master_slave_v[ *minSlave ] = aVertex;
      slave_master_v[ aVertex ] = *minSlave;
      
      distance.push_back(
        dtLinearAlgebra::distance(
          (*_dtT)( dtGmshModel::extractPosition(*minSlave), -1 ),
          dtGmshModel::extractPosition(aVertex)
        )
      );
           
      //
      // output
      //
      dt__info(
        preUpdate(),
        << "slave:  tag = " << (*minSlave)->tag() << std::endl
        << "master: tag = " << aVertex->tag() << std::endl
        << "_dtT[-1](slave ) = ( " 
        << (*_dtT)( dtGmshModel::extractPosition(*minSlave), -1 ) 
        << " )" << std::endl
        << "_dtT[-1](master) = ( " 
        << (*_dtT)( dtGmshModel::extractPosition(aVertex), -1 ) 
        << " )" << std::endl
        << "distance = " << distance.back()
      );      
    }
    
    
    //
    // set master edge on slave edges
    //
    dt__forAllIndex( slaveEdge, ii ) {
      dtInt masterTag 
      = 
      gm->getDtGmshEdgeTagByFromTo(
        master_slave_v[ slaveEdge[ii]->getBeginVertex() ]->tag(),
        master_slave_v[ slaveEdge[ii]->getEndVertex() ]->tag()
      );
      
      dt__info(
        preUpdate(),
        << "slaveEdge  : tag = " << slaveEdge[ii]->tag() << std::endl
        << "masterEdge : tag = " << masterTag << std::endl
      );
      slaveEdge[ii]->setMeshMaster( 
        (::GEdge *) gm->getDtGmshEdgeByTag(masterTag), masterTag 
      );
    }
    
    //
    // create affine transformation of periodicity
    //
    std::vector<double> tfo(16);
    
    tfo[0*4+0] = 1;
    tfo[0*4+1] = 0;
    tfo[0*4+2] = 0;

    tfo[1*4+0] = 0;
    tfo[1*4+1] = 1;
    tfo[1*4+2] = 0;

    tfo[2*4+0] = 0;
    tfo[2*4+1] = 0;
    tfo[2*4+2] = 1;

    double origin[3] = { 0., 0., 0. };
    double translation[3] 
    = 
    { 
      _dtT->translationAxis3d().x(), 
      _dtT->translationAxis3d().y(), 
      _dtT->translationAxis3d().z() 
    };

    dtInt idx = 0;
    for (int i = 0; i < 3; i++,idx++) {
      dtInt tIdx = i*4+3;
      tfo[tIdx] = origin[i] + translation[i];
      for (int j = 0; j < 3; j++,idx++) tfo[tIdx] -= tfo[idx] * origin[j];
    }
    for (int i=0;i<4;i++) tfo[12+i] = 0;
    tfo[15] = 1;
    
    double scale = 1.;
    double minDistance = std::numeric_limits< double >::max();
    double maxDistance = std::numeric_limits< double >::min();
    dt__forAllIndex(masterVertex, ii) {
      ::SPoint3 xyzOri(
        masterVertex[ii]->x(), masterVertex[ii]->y(), masterVertex[ii]->z()
      );
      ::SPoint3 xyzSlv( 
        slave_master_v[ masterVertex[ii] ]->x(), 
        slave_master_v[ masterVertex[ii] ]->y(),
        slave_master_v[ masterVertex[ii] ]->z()
      );      
      ::SPoint3 xyzTfo(0,0,0);
      dtInt idx = 0;
      for (int i=0;i<3;i++) {
        for (int j=0;j<3;j++) xyzTfo[i] += xyzOri[j] * tfo[idx++];
        xyzTfo[i] += tfo[idx++];
      }
      
      //
      // get min and max distance of rotating vertices
      //
      minDistance = std::min( xyzSlv.distance(xyzTfo), minDistance );
      maxDistance = std::max( xyzSlv.distance(xyzTfo), maxDistance );
      
      //
      // hack to handle very high precision in gmsh
      //      
      if (
        xyzSlv.distance(xyzTfo) 
        > 
        CTX::instance()->geom.tolerance * CTX::instance()->lc
      ) {
        
        double cScale 
        = 
        xyzSlv.distance(xyzTfo) 
        / 
        ( CTX::instance()->geom.tolerance * CTX::instance()->lc );
        
        scale = std::max( scale, cScale );
        dt__warning(
          preUpdate(), 
            << "Distance of corresponding vertices is bigger than gmsh "
              "internal tolerance." 
            << std::endl
            << logMe::dtFormat("distance = %.16e, cScale = %.16e") 
              % xyzSlv.distance(xyzTfo) % cScale
            << std::endl
            << "Use force option for this bVOObserver."
        );
      }
    }
    
    SBoundingBox3d bbox = gm->bounds();
    double lc = bbox.empty() ? 1. : norm(SVector3(bbox.max(), bbox.min()));  
    dt__info(
      preUpdate(),
      << "lc = " << lc << std::endl
      << "minDistance = " << minDistance << std::endl
      << "maxDistance = " << maxDistance << std::endl
      << "==> tolerance = minDistance / lc = " << minDistance/lc << std::endl
      << "==> tolerance = maxDistance / lc = " << maxDistance/lc
    );
    
    double const gTol = CTX::instance()->geom.tolerance;
    if ( optionHandling::optionTrue("force") ) {
      CTX::instance()->geom.tolerance 
      = 
      1.1*scale * CTX::instance()->geom.tolerance;
      dt__info(
        preUpdate(),
        << "Use force option: Geom.Tolerance = " 
        << CTX::instance()->geom.tolerance
      );      
    }
    
    //
    // set master face and affine transformation
    //
    slaveF->setMeshMaster(masterF, tfo); 
    
    CTX::instance()->geom.tolerance = gTol;
  }
}
