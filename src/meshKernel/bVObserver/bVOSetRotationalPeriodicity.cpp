#include "bVOSetRotationalPeriodicity.h"

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
#include <dtTransformerHeaven/xYz_rPhiZ.h>
#include <progHelper.h>

#include <gmsh/Context.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOSetRotationalPeriodicity::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOSetRotationalPeriodicity, new bVOSetRotationalPeriodicity())
  );
  
  bVOSetRotationalPeriodicity::bVOSetRotationalPeriodicity() {
  }

  bVOSetRotationalPeriodicity::~bVOSetRotationalPeriodicity() {
    
  }

   void bVOSetRotationalPeriodicity::jInit( 
    jsonPrimitive const & jE,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    boundedVolume * attachTo
  ) {
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);

    _dtT
    =
    xYz_rPhiZ::PointerDownCast(
      jE.lookupClone< dtTransformer >( 
        "_dtT", 
        bC->constPtrTransformerContainer()->lVH()
      )
    );
  }  
  
  void bVOSetRotationalPeriodicity::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
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
		//   name="bVOSetRotationalPeriodicity" 
		//   faceMaster="name0"
    //   faceSlave="name1"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    jsonPrimitive jE;
		jE.append< std::string >(
      "_faceMaster", dtXmlParserBase::getAttributeStr("faceMaster", element)
    );
    jE.append< std::string >(
      "_faceSlave", dtXmlParserBase::getAttributeStr("faceSlave", element)
    );
    
    jE.append< dtTransformer const * >(
      "_dtT",
      bC->constPtrTransformerContainer()->get(
        dtXmlParserBase::getAttributeStr("transformer", element)
      )
    );
    bVOSetRotationalPeriodicity::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOSetRotationalPeriodicity::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    //
    // get faces
    //		
    dtGmshFace * const masterF 
    = 
    gm->getDtGmshFaceByPhysical(config().lookup< std::string >("_faceMaster"));
    dtGmshFace * const slaveF 
    = 
    gm->getDtGmshFaceByPhysical(config().lookup< std::string >("_faceSlave") );
    dt__throwIf(masterF==slaveF, preUpdate());
    //
    // get edges
    //
    std::vector< ::GEdge * > slaveEdge = slaveF->edges();
    std::vector< ::GEdge * > masterEdge = masterF->edges();
    
    bool restart = false;
    dt__forAllIndex(masterEdge, kk) {
      restart = false;
      dt__forAllIndex(masterEdge, ii) {
        dt__forAllIndex(slaveEdge, jj) {
          if ( slaveEdge[jj]->tag() == masterEdge[ii]->tag() ) {
            dt__info(
              preUpdate(), 
              << "Remove edge with tag " << slaveEdge[jj]->tag()
            );
            slaveEdge.erase(slaveEdge.begin() + jj);
            masterEdge.erase(masterEdge.begin() + ii);
            restart = true;
          }
          if (restart) break;
        }
        if (restart) break;
      }
      if (!restart) break;
    }
    
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
    std::vector< dtReal > angle;
    dt__forAllRefAuto(masterVertex, aVertex) {
      dtPoint3 aVertex_rPhiZ 
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
          dtPoint3 const vA_rPhiZ 
          = 
          (*_dtT)( dtGmshModel::extractPosition(vA), -1 );
          dtPoint3 const vB_rPhiZ
          = 
          (*_dtT)( dtGmshModel::extractPosition(vB), -1 );
          return 
            (
              fabs(
                aVertex_rPhiZ.x()-vA_rPhiZ.x()
              ) 
              +
              fabs(
                aVertex_rPhiZ.z()-vA_rPhiZ.z()
              )
            )
            < 
            (
              fabs(
                aVertex_rPhiZ.x()-vB_rPhiZ.x()
              ) 
              +
              fabs(
                aVertex_rPhiZ.z()-vB_rPhiZ.z()
              )
            );
        }
      );
      
      // mappings
      master_slave_v[ *minSlave ] = aVertex;
      slave_master_v[ aVertex ] = *minSlave;
      
      if ( aVertex->tag() == (*minSlave)->tag() ) {
        dt__info(
          preUpdate(), 
          << "Same vertices" << std::endl
          << aVertex->tag() << " <-> " << (*minSlave)->tag() 
        );
        continue;
      }
      
      angle.push_back(
        (*_dtT)( dtGmshModel::extractPosition(*minSlave), -1 ).y()
        -      
        (*_dtT)( dtGmshModel::extractPosition(aVertex), -1 ).y()
      );
      
      if ( angle.back() < 0. ) {
        angle.back() = angle.back()+2.*M_PI;
      }
      
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
        << "angle = " << angle.back()
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
    
    double ux = _dtT->rotationAxis().x();
    double uy = _dtT->rotationAxis().y();
    double uz = _dtT->rotationAxis().z();
    double ANGLE = dtLinearAlgebra::sum(angle) / angle.size();
    tfo[0*4+0] = cos (ANGLE) + ux*ux*(1.-cos(ANGLE));
    tfo[0*4+1] = ux*uy*(1.-cos(ANGLE)) - uz * sin(ANGLE);
    tfo[0*4+2] = ux*uz*(1.-cos(ANGLE)) + uy * sin(ANGLE);

    tfo[1*4+0] = ux*uy*(1.-cos(ANGLE)) + uz * sin(ANGLE);
    tfo[1*4+1] = cos(ANGLE) + uy*uy*(1.-cos(ANGLE));
    tfo[1*4+2] = uy*uz*(1.-cos(ANGLE)) - ux * sin(ANGLE);

    tfo[2*4+0] = ux*uz*(1.-cos(ANGLE)) - uy * sin(ANGLE);
    tfo[2*4+1] = uy*uz*(1.-cos(ANGLE)) + ux * sin(ANGLE);
    tfo[2*4+2] = cos(ANGLE) + uz*uz*(1.-cos(ANGLE));

    double origin[3] 
    = 
    { 
      _dtT->origin().x(), _dtT->origin().y(), _dtT->origin().z()
    };

    for (int i=0;i<3;i++) tfo[i*4+3] =  origin[i];
    for (int i=0;i<3;i++) {
      for (int j=0;j<3;j++) {
        tfo[i*4+3] -= tfo[i*4+j] * origin[j];
      }
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
            << "Distance of corresponding vertices is greater than gmsh "
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
