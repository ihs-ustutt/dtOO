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

#include "dtGmshModel.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include <gmsh/HighOrder.h>
#include <gmsh/GEdge.h>
#include <gmsh/GFace.h>
#include <gmsh/GVertex.h>
#include <gmsh/GRegion.h>

#include "dtGmshRegion.h"
#include "dtGmshFace.h"
#include "dtGmshEdge.h"
#include "dtGmshVertex.h"
#include "discrete3dPoints.h"
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/multipleBoundedSurface.h>
#include <analyticGeometryHeaven/multipleBoundedVolume.h>
#include <interfaceHeaven/ptrHandling.h>
#include <unstructured3dMesh.h>
#include <unstructured3dSurfaceMesh.h>
#include "dtMeshGEdge.h"
#include "dtMeshGFace.h"
#include "dtMeshGRegion.h"
#include <logMe/dtMacros.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MPrism.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <cgnslib.h>

#include <boost/assign.hpp>
#include <boost/regex.hpp>

#define __caCThis const_cast< dtGmshModel * >(this)

#define __cgnsCheck(cmd) \
  if (cmd) dt__throw(__cgnsCheck(), << dt__eval(cg_get_error()))

namespace dtOO {
    std::map< std::string, dtInt > dtGmshModel::_facePositionStr
    =
    ::boost::assign::map_list_of
      ("SOUTH", 0)("south", 0)("0", 0)("S", 0)("s", 0)
      ("NORTH", 1)("north", 1)("1", 1)("N", 1)("n", 1)
      ("FRONT", 2)("front", 2)("2", 2)("F", 2)("f", 2)
      ("BACK", 3)("back", 3)("3", 3)("B", 3)("b", 3)
      ("WEST", 4)("west", 4)("4", 4)("W", 4)("w", 4)  
      ("EAST", 5)("east", 5)("5", 5)("E", 5)("e", 5)
      ("6", 6)
      ("7", 7)
      ("8", 8)
      ("9", 9)
      ("10", 10);
    std::map< int, std::vector< std::string > > dtGmshModel::_positionStrFace
    =
    ::boost::assign::map_list_of
      (
        0, 
        ::boost::assign::list_of
          ("SOUTH")("south")
          ("0")
          ("S")("s").convert_to_container< std::vector< std::string > >()
      )
      (
        1, 
        ::boost::assign::list_of
          ("NORTH")("north")
          ("1")
          ("N")("n").convert_to_container< std::vector< std::string > >()
      )
      (
        2, 
        ::boost::assign::list_of
          ("FRONT")("front")
          ("2")
          ("F")("f").convert_to_container< std::vector< std::string > >()
      )    
      (
        3, 
        ::boost::assign::list_of
          ("BACK")("back")
          ("3")
          ("B")("b").convert_to_container< std::vector< std::string > >()
      )    
      (
        4, 
        ::boost::assign::list_of
          ("WEST")("west")
          ("4")
          ("W")("w").convert_to_container< std::vector< std::string > >()
      )       
      (
        5, 
        ::boost::assign::list_of
          ("EAST")("east")
          ("5")
          ("E")("e").convert_to_container< std::vector< std::string > >()
      )
      (
        6, 
        ::boost::assign::list_of
          ("6").convert_to_container< std::vector< std::string > >()
      )
      (
        7, 
        ::boost::assign::list_of
          ("7").convert_to_container< std::vector< std::string > >()
      )
      (
        8, 
        ::boost::assign::list_of
          ("8").convert_to_container< std::vector< std::string > >()
      )
      (
        9, 
        ::boost::assign::list_of
          ("9").convert_to_container< std::vector< std::string > >()
      )
      (
        10, 
        ::boost::assign::list_of
          ("10").convert_to_container< std::vector< std::string > >()
      )
      (
        11, 
        ::boost::assign::list_of
          ("11").convert_to_container< std::vector< std::string > >()
      );
      
    std::map< std::string, dtInt > dtGmshModel::_edgePositionStr
    =
    ::boost::assign::map_list_of
      ("0", 0)
      ("1", 1)
      ("2", 2)
      ("3", 3);     
    std::map< int, std::vector< std::string > > dtGmshModel::_positionStrEdge
    =
    ::boost::assign::map_list_of
      (
        0, 
        ::boost::assign::list_of
          ("0").convert_to_container< std::vector< std::string > >()
      )
      (
        1, 
        ::boost::assign::list_of
          ("1").convert_to_container< std::vector< std::string > >()
      )
      (
        2, 
        ::boost::assign::list_of
          ("2").convert_to_container< std::vector< std::string > >()
      )    
      (
        3, 
        ::boost::assign::list_of
          ("3").convert_to_container< std::vector< std::string > >()
      );    
    std::map< std::string, dtInt > dtGmshModel::_vertexPositionStr
    =
    ::boost::assign::map_list_of
      ("0", 0)("START", 0)("start", 0)("S", 0)("s", 0)
      ("1", 1)("END", 1)("end", 1)("E", 1)("e", 1);
    std::map< int, std::vector< std::string > > dtGmshModel::_positionStrVertex
    =
    ::boost::assign::map_list_of
      (
        0, 
        ::boost::assign::list_of
          ("0")
          ("START")("start")
          ("S")("s").convert_to_container< std::vector< std::string > >()
      )
      (
        1, 
        ::boost::assign::list_of
          ("1")
          ("END")("end")
          ("E")("e").convert_to_container< std::vector< std::string > >()
      );    
  dtGmshModel::dtGmshModel(std::string name) : GModel(name){
    _debug = "";
    _fmTwin = NULL;
  }

  dtGmshModel::dtGmshModel(
    std::string name, ::FieldManager * fm
  ) : GModel(name){
    _debug = "";
    
    //
    // temporarly store field
    //
    _fmTwin = GModel::_fields;
    
    //
    // set given field to model
    //
    GModel::_fields = fm;
  }
  
  dtGmshModel::~dtGmshModel() {
    //
    // if initialized with field, change to old one before destruction
    //
    if (_fmTwin) GModel::_fields = _fmTwin;
  }
  
  void dtGmshModel::addIfVertexToGmshModel( 
    dtPoint3 const & vertex, dtInt * const tag 
  ) {
		*tag = this->getMaxVertexTag()+1;
    dtGmshVertex * gv = new dtGmshVertex(this, *tag);
		gv->setPosition(vertex); 
		
		int tTag = alreadyInModel(gv);
		
		if (tTag) {
			delete gv;
			*tag = tTag;
		}
		else {
			this->add(gv);
		}
  }	

  void dtGmshModel::addIfEdgeToGmshModel( 
    map1dTo3d const * const edge, 
    dtInt * const tag, 
    dtInt const from, 
    dtInt const to 
  ) {
		*tag = this->getMaxEdgeTag()+1;
    
    if ( edge->degenerated() ) {
		  dt__info(
        addIfEdgeToGmshModel(),
        << "Try to add a degenerated edge." << std::endl
        << "boundingBoxValue = " << edge->boundingBoxValue()
      );      
      *tag = 0;
      
      return;
    }
    
		dtGmshEdge * ge = new dtGmshEdge(
			this, *tag, ::GModel::getVertexByTag(from), ::GModel::getVertexByTag(to)
		);
		ge->setMap1dTo3d( edge );
		
		int tTag = alreadyInModel(ge);
		if (tTag) {
			delete ge;
			*tag = tTag;
			if ( ::GModel::getEdgeByTag(*tag)->getBeginVertex()->tag() != from ) {
				*tag = -tTag;
			}
		}
		else {
			this->add(ge);
		}
  }
	
  void dtGmshModel::addIfEdgeToGmshModel(
    map1dTo3d const * const edge, dtInt * const tag
  ) {
		*tag = this->getMaxEdgeTag()+1;		
		
		std::vector< dtInt > vId(2);
		addIfVertexToGmshModel(edge->getPointPercent(0.), &(vId[0]) );
		addIfVertexToGmshModel(edge->getPointPercent(1.), &(vId[1]) );

    addIfEdgeToGmshModel(edge, tag, vId[0], vId[1]);
	}
	
  dtInt dtGmshModel::addIfEdgeToGmshModel( map1dTo3d const * const edge ) {
    dtInt tag;
    addIfEdgeToGmshModel(edge, &tag);
    return tag;
  }

  void dtGmshModel::addIfFaceToGmshModel( 
    map2dTo3d const * const face, dtInt * const tag,
		std::list< ::GEdge * > const & edges, std::vector< dtInt > const & ori
  ) {
		*tag = this->getMaxFaceTag()+1;	
		
    if ( face->degenerated() ) {
      dt__info(
        addIfFaceToGmshModel(),
        << "Try to add a degenerated face." << std::endl
        << "boundingBoxValue = " << face->boundingBoxValue()
      );
      *tag = 0;
      return;
    }
    
		dtGmshFace * gf = new dtGmshFace(this, *tag, edges, ori);
		gf->setMap2dTo3d(face);
		
		int tTag = alreadyInModel(gf);
		if (tTag) {
			delete gf;
			*tag = tTag;
		}
		else {
			this->add(gf);
		}
  }	
	 
  void dtGmshModel::addIfFaceToGmshModel( 
    map2dTo3d const * const face, dtInt * const tag,
		int const & eId0, dtInt const & eId1, dtInt const & eId2, dtInt const & eId3
  ) {
		std::list< ::GEdge * > ge;
		ge.push_back( this->getEdgeByTag(abs(eId0)) );
		ge.push_back( this->getEdgeByTag(abs(eId1)) );
		ge.push_back( this->getEdgeByTag(abs(eId2)) );
		ge.push_back( this->getEdgeByTag(abs(eId3)) );
		std::vector<int> ori(4);
		ori[0] = ( eId0 < 0 ? -1 : 1);
		ori[1] = ( eId1 < 0 ? -1 : 1);
		ori[2] = ( eId2 < 0 ? -1 : 1);
		ori[3] = ( eId3 < 0 ? -1 : 1);  
		
		addIfFaceToGmshModel(face, tag, ge, ori);
  }
	
  void dtGmshModel::addIfFaceToGmshModel(
    map2dTo3d const * const face, dtInt * const tag
  ) {
		*tag = this->getMaxFaceTag()+1;		
		std::list< ::GEdge * > edges;//(4);
    std::vector< dtInt > ori;
    dtInt tmpEId = 0;
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentConstVPercent(0)), &(tmpEId) 
    );
    if (tmpEId!=0) {
      edges.push_back( getDtGmshEdgeByTag(tmpEId) );
      ori.push_back(1);
    }
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentConstUPercent(1)), &(tmpEId) 
    );
    if (tmpEId!=0) {
      edges.push_back( getDtGmshEdgeByTag(tmpEId) );
      ori.push_back(1);
    }
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentConstVPercent(1)), &(tmpEId) 
    );
    if (tmpEId!=0) {
      edges.push_back( getDtGmshEdgeByTag(tmpEId) );
      ori.push_back(-1);
    }
		addIfEdgeToGmshModel(
      dt__tmpPtr(map1dTo3d, face->segmentConstUPercent(0)), &(tmpEId) 
    );
    if (tmpEId!=0) {
      edges.push_back( getDtGmshEdgeByTag(tmpEId) );
      ori.push_back(-1);
    }
		addIfFaceToGmshModel(face, tag, edges, ori);  	
	}

  dtInt dtGmshModel::addIfFaceToGmshModel( map2dTo3d const * const face ) {
    dtInt tag;
    addIfFaceToGmshModel(face, &tag);
    return tag;
  }

  void dtGmshModel::addIfRegionToGmshModel(
    map3dTo3d const * const region, dtInt * const tag,
		std::list< ::GFace * > const & faces, std::vector< dtInt > const & ori    
  ) {
		*tag = this->getMaxRegionTag()+1;	
		
		dtGmshRegion * gr = new dtGmshRegion(this, *tag, faces, ori);

    //
    // customGmsh calls this function with NULL pointer for region
    // so we have to check here for this case, otherwise we will get a segfault
    //
    if (region != NULL) {
      dt__throwIfWithMessage(
        region->degenerated(), 
        addIfRegionToGmshModel(),
        << "Try to add a degenerated region." << std::endl
        << "boundingBoxValue = " << region->boundingBoxValue()
      );
    }
		int tTag = alreadyInModel(gr);
		if (tTag) {
			delete gr;
			*tag = tTag;
		}
		else {
			this->add(gr);
		}    
  }

  void dtGmshModel::addIfRegionToGmshModel( 
    map3dTo3d const * const region, dtInt * const tag,
		int const & fId0, dtInt const & fId1, 
    dtInt const & fId2, dtInt const & fId3, 
    dtInt const & fId4, dtInt const & fId5
  ) {
		std::list< ::GFace * > gf;
		gf.push_back( this->getFaceByTag(abs(fId0)) );
		gf.push_back( this->getFaceByTag(abs(fId1)) );
		gf.push_back( this->getFaceByTag(abs(fId2)) );
		gf.push_back( this->getFaceByTag(abs(fId3)) );
    gf.push_back( this->getFaceByTag(abs(fId4)) );
    gf.push_back( this->getFaceByTag(abs(fId5)) );
		std::vector<int> ori(6);
		ori[0] = ( fId0 < 0 ? -1 : 1);
		ori[1] = ( fId1 < 0 ? -1 : 1);
		ori[2] = ( fId2 < 0 ? -1 : 1);
		ori[3] = ( fId3 < 0 ? -1 : 1);  
    ori[4] = ( fId4 < 0 ? -1 : 1);  
    ori[5] = ( fId5 < 0 ? -1 : 1);  
		
		addIfRegionToGmshModel(region, tag, gf, ori);
  }
    
  void dtGmshModel::addIfRegionToGmshModel(
    map3dTo3d const * const region, dtInt * const tag
  ) {
		*tag = this->getMaxRegionTag()+1;		
		std::list< ::GFace * > faces;//(4);
    std::vector< dtInt > ori;
    dtInt tmpFId = 0;
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstWPercent(0)), &(tmpFId)
		);
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }    
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstWPercent(1)), &(tmpFId)
		);
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }        
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstVPercent(0)), &(tmpFId)
		);
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }        
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstVPercent(1)), &(tmpFId)
		);
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }        
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstUPercent(0)), &(tmpFId)
		);
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }        
		addIfFaceToGmshModel(
		  dt__tmpPtr(map2dTo3d, region->segmentConstUPercent(1)), &(tmpFId)
		);    
    if (tmpFId!=0) {
      faces.push_back( getDtGmshFaceByTag(tmpFId) );
      ori.push_back(1);
    }    
    
  	addIfRegionToGmshModel( region, tag, faces, ori );    
  }  

  dtInt dtGmshModel::addIfRegionToGmshModel(
    map3dTo3d const * const region, std::vector< int > const & faceIds
  ) {
    std::list< ::GFace * > faces;
    std::vector< int > ori;
    dt__forAllRefAuto(faceIds, faceId) {
      faces.push_back( this->getDtGmshFaceByTag(faceId) );
      if (faceId<0) {
        ori.push_back(-1);
      }
      else {
        ori.push_back(1);
      }
    }
    dtInt rId;
    addIfRegionToGmshModel(region, &rId, faces, ori);
    return rId;
  }

  dtInt dtGmshModel::addIfRegionToGmshModel( map3dTo3d const * const region ) {
    dtInt rId;
    addIfRegionToGmshModel(region, &rId);
    return rId;
  }

  void dtGmshModel::addIfToGmshModel(
    analyticGeometry const * const aG, dtInt * const tag
  ) {
    if ( map1dTo3d::ConstDownCast(aG) ) {
      addIfEdgeToGmshModel(map1dTo3d::ConstSecureCast(aG), tag);
    }
    else if ( map2dTo3d::ConstDownCast(aG) ) {
      addIfFaceToGmshModel(map2dTo3d::ConstSecureCast(aG), tag);
    }    
    else if ( map3dTo3d::ConstDownCast(aG) ) {
      addIfRegionToGmshModel(map3dTo3d::ConstSecureCast(aG), tag);
    }
    else if ( multipleBoundedSurface::ConstDownCast(aG) ) {
      multipleBoundedSurface const * const mbs 
      = 
      multipleBoundedSurface::ConstSecureCast(aG);

      //
      // extract edges and create orientation vector
      //
      std::list< ::GEdge * > edges;
      std::vector< dtInt > ori;
      dt__pVH(analyticGeometry) const & bounds = mbs->boundsVectorConstRef();
      dt__forAllIndex(bounds, ii) {
        dtInt tmpTag;
        addIfToGmshModel( &(bounds[ii]), &(tmpTag) );
        if (tmpTag != 0) {
          edges.push_back( getDtGmshEdgeByTag(tmpTag) );
          if (tmpTag<0) {
            ori.push_back(-1);
          }
          else {
            ori.push_back(1);
          }
        }
        else {
          dt__warning(
            addIfToGmshModel(), 
            << "Degenerated bound (edge) [ ii = " << ii << " ]."
          );
        }
      }

      //
      // add surface with multiple bounded edges
      // currently it has to be a map2dTo3d
      //
      addIfFaceToGmshModel(
        map2dTo3d::MustConstDownCast( mbs->surfaceConstPtr() ), tag, edges, ori
      );
    }
    else if ( multipleBoundedVolume::ConstDownCast(aG) ) {
      multipleBoundedVolume const * const mbv 
      = 
      multipleBoundedVolume::ConstSecureCast(aG);

      //
      // extract faces and orientation vector
      //
      std::list< ::GFace * > faces;
      std::vector< dtInt > ori;
      dt__pVH(analyticGeometry) const & bounds = mbv->boundsVectorConstRef();
      dt__forAllIndex(bounds, ii) {
        dtInt tmpTag;
        addIfToGmshModel( &(bounds[ii]) , &(tmpTag) );
        if (tmpTag != 0) {
          faces.push_back( getDtGmshFaceByTag(tmpTag) );
          if (tmpTag<0) {
            ori.push_back(-1);
          }
          else {
            ori.push_back(1);
          }
        }
        else {
          dt__warning(
            addIfToGmshModel(), 
            << "Degenerated bound (face) [ ii = " << ii << " ]."
          );
        }
      }

      //
      // add region with multiple bounded edges
      // currently it has to be a map2dTo3d
      //
      addIfRegionToGmshModel(
        map3dTo3d::MustConstDownCast( mbv->regionConstPtr() ), tag, faces, ori
      );
    }
    else dt__throwUnexpected(addIfToGmshModel());
  }
        
  dtGmshRegion * dtGmshModel::getDtGmshRegionByTag( dtInt const tag ) const {
    ::GRegion * region = ::GModel::getRegionByTag(abs(tag));
    dt__ptrAss(dtGmshRegion * gRegion, dtGmshRegion::DownCast(region));
    
    return gRegion;    
  }
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByTag( dtInt const tag ) const {
    ::GFace * face = ::GModel::getFaceByTag(abs(tag));
    dt__ptrAss(dtGmshFace * gFace, dtGmshFace::DownCast(face));
    
    return gFace;    
  }
	
  ::GEntity * dtGmshModel::getGEntityByPhysical( 
    std::string const & physical 
  ) const {
    dt__throwIf( 
      stringPrimitive::isWildcard( physical), getGEntityByPhysical() 
    );
    dt__forAllRefAuto( dtEdges(), aEdge ) {
      if ( matchWildCardPhysical( physical, aEdge ) ) return aEdge;
    }    
    dt__forAllRefAuto( dtFaces(), aFace ) {
      if ( matchWildCardPhysical( physical, aFace ) ) return aFace;
    }
    dt__forAllRefAuto( dtRegions(), aRegion ) {
      if ( matchWildCardPhysical( physical, aRegion ) ) return aRegion;
    }      
    dt__throw(
      getGEntityByPhysical(), << "entity[ " << physical << " ] not found."
    );
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByPhysical( 
    std::string const & physical 
  ) const {
    if ( 
      stringPrimitive::stringContains("->", physical) 
      || 
      stringPrimitive::isWildcard(physical) 
    ) {
      dt__forAllRefAuto( dtEdges(), aEdge ) {
        if ( matchWildCardPhysical( physical, aEdge ) ) {
          return aEdge;
        }
      }
      dt__throw(
        getDtGmshEdgeByPhysical(), << "edge[ " << physical << " ] not found."
      );
    }
    else {
      dtInt pN = __caCThis->getPhysicalNumber(1, physical);  
      intGEntityVMap gE_pN;
      __caCThis->getPhysicalGroups(1, gE_pN);

      dt__throwIfWithMessage(
        gE_pN[pN].size()!=1, 
        getDtGmshEdgeByPhysical(),
        << "edge[ " << physical << " ] not found."
      );

      return cast2DtGmshEdge(gE_pN[pN][0]);    
    }
  }	
  
  dtGmshFace * dtGmshModel::getDtGmshFaceByPhysical( 
    std::string const & physical 
  ) const {
    if ( 
      stringPrimitive::stringContains("->", physical) 
      || 
      stringPrimitive::isWildcard(physical) 
    ) {
      dt__forAllRefAuto( dtFaces(), aFace ) {
        if ( matchWildCardPhysical( physical, aFace ) ) {
          return aFace;
        }
      }
      dt__throw(
        getDtGmshFaceByPhysical(), << "face[ " << physical << " ] not found."
      );
    }
    else {
      dtInt pN = __caCThis->getPhysicalNumber(2, physical);  
      intGEntityVMap gE_pN;
      __caCThis->getPhysicalGroups(2, gE_pN);

      dt__throwIfWithMessage(
        gE_pN[pN].size()!=1, 
        getDtGmshFaceByPhysical(),
        << "face[ " << physical << " ] not found."
      );

      return cast2DtGmshFace(gE_pN[pN][0]);    
    }
  }	

  std::list< dtGmshFace * > dtGmshModel::getDtGmshFaceListByPhysical( 
    std::string const & physical 
  ) const {
    std::list< dtGmshFace * > faceL;
    if ( 
      stringPrimitive::stringContains("->", physical) 
      || 
      stringPrimitive::isWildcard(physical) 
    ) {
      dt__forAllRefAuto( dtFaces(), aFace ) {
        if ( matchWildCardPhysical( physical, aFace ) ) {
          faceL.push_back( aFace );
        }
      }
    }
    else {
      dtInt pN = __caCThis->getPhysicalNumber(2, physical);  
      intGEntityVMap gE_pN;
      __caCThis->getPhysicalGroups(2, gE_pN);

      faceL = cast2DtGmshFace( progHelper::vector2List(gE_pN[pN]) );    
    }
    
    return faceL;
  }
  
  dtGmshRegion * dtGmshModel::getDtGmshRegionByPhysical( 
    std::string const & physical 
  ) const {
    if ( 
      stringPrimitive::stringContains("->", physical) 
      || 
      stringPrimitive::isWildcard(physical) 
    ) {
      dt__forAllRefAuto( dtRegions(), aReg ) {
        if ( matchWildCardPhysical( physical, aReg ) ) {
          return aReg;
        }
      }
      dt__throw(
        getDtGmshRegionByPhysical(), << "region[ " << physical << " ] not found."
      );
    }
    else {
      dtInt pN = __caCThis->getPhysicalNumber(3, physical);  
      intGEntityVMap gE_pN;
      __caCThis->getPhysicalGroups(3, gE_pN);

      dt__throwIfWithMessage(
        gE_pN[pN].size()!=1, 
        getDtGmshRegionByPhysical(),
        << "region[ " << physical << " ] not found."
      );

      return cast2DtGmshRegion(gE_pN[pN][0]);    
    }
  }	  
  
  std::list< dtGmshRegion * > dtGmshModel::getDtGmshRegionListByPhysical( 
    std::string const & physical 
  ) const {
    std::list< dtGmshRegion * > regionL;
    if ( 
      stringPrimitive::stringContains("->", physical) 
      || 
      stringPrimitive::isWildcard(physical) 
    ) {
      dt__forAllRefAuto( dtRegions(), aRegion ) {
        if ( matchWildCardPhysical( physical, aRegion ) ) {
          regionL.push_back( aRegion );
        }
      }
    }
    else {
      dtInt pN = __caCThis->getPhysicalNumber(3, physical);  
      intGEntityVMap gE_pN;
      __caCThis->getPhysicalGroups(3, gE_pN);

      regionL = cast2DtGmshRegion( progHelper::vector2List(gE_pN[pN]) );    
    }
    
    return regionL;
  }  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByTag( dtInt const tag ) const {
    ::GEdge * edge = ::GModel::getEdgeByTag(abs(tag));
    dt__ptrAss(dtGmshEdge * gEdge, dtGmshEdge::DownCast(edge));
    
    return gEdge;    
  }

  dtInt dtGmshModel::getDtGmshEdgeTagByFromTo( 
    dtInt const from, dtInt const to 
  ) const {
    dtGmshVertex * gv = getDtGmshVertexByTag(from);
    
    dt__forAllRefAuto(gv->edges(), edge) {
      if ( edge->getBeginVertex()->tag() == to ) {
        return -(edge->tag());
      }
      else if ( edge->getEndVertex()->tag() == to ) {
        return edge->tag();
      }
    }
    dt__throwUnexpected(getDtGmshEdgeTagByFromTo());
  }
  
  dtGmshEdge * dtGmshModel::getDtGmshEdgeByFromTo( 
    dtInt const from, dtInt const to 
  ) const {
    return getDtGmshEdgeByTag( abs(getDtGmshEdgeTagByFromTo(from, to)) );
  }  

  dtGmshVertex * dtGmshModel::getDtGmshVertexByTag( dtInt const tag ) const {
    ::GVertex * vertex = ::GModel::getVertexByTag(tag);
   
    dt__ptrAss( dtGmshVertex * gVertex, dtGmshVertex::DownCast(vertex) );
    
    return gVertex;    
  }  

  std::list< dtInt > dtGmshModel::getDtGmshEdgeTagListByFromToPhysical( 
    std::string const & from, std::string const & to 
  ) const {
    std::list< dtInt > retList;
    dt__forAllRefAuto( cast2DtGmshEdge( this->edges() ), edge) {
      ::GVertex const * const v0 = edge->getBeginVertex();
      dt__forAllRefAuto( v0->faces(), fromFace ) {
        if ( matchWildCardPhysical( from, fromFace ) ) {
          ::GVertex const * const v1 = edge->getEndVertex();
          dt__forAllRefAuto( v1->faces(), toFace ) {
            if ( matchWildCardPhysical( to, toFace ) ) {
              retList.push_back( edge->tag() );
            }
          }
        }
        else if ( matchWildCardPhysical( to, fromFace ) ) {
          ::GVertex const * const v1 = edge->getEndVertex();
          dt__forAllRefAuto( v1->faces(), toFace ) {
            if ( matchWildCardPhysical( from, toFace ) ) {
              retList.push_back( -edge->tag() );
            }
          }
        }
      }
    }
    progHelper::removeBastardTwins( retList );
    return retList;
  }

  dtGmshRegion * dtGmshModel::cast2DtGmshRegion( ::GEntity * gr ){
    assert( gr!=NULL );
    return dtGmshRegion::MustDownCast(gr);
  }
  
  dtGmshFace * dtGmshModel::cast2DtGmshFace( ::GEntity * gf ){   
    assert( gf!=NULL );
    return dtGmshFace::MustDownCast(gf);
  }
  
  dtGmshEdge * dtGmshModel::cast2DtGmshEdge( ::GEntity * ge ) {
    assert( ge!=NULL );
    return dtGmshEdge::MustDownCast(ge);
  }
  
  ::GModel * dtGmshModel::cast2GModel( dtGmshModel * gm ) {
    assert( gm!=NULL );
    return dynamic_cast< ::GModel * >(gm);
  }
 
  ::GEntity * dtGmshModel::cast2GEntity( dtGmshRegion * gr ) {
    assert( gr!=NULL );
    return dynamic_cast< ::GEntity * >(gr);
  }

  ::GEntity * dtGmshModel::cast2GEntity( dtGmshFace * gf ) {
    assert( gf!=NULL );
    return dynamic_cast< ::GEntity * >(gf);
  }

  ::GEntity * dtGmshModel::cast2GEntity( dtGmshEdge * ge ) {
    assert( ge!=NULL );
    return dynamic_cast< ::GEntity * >(ge);
  }

  ::GEntity * dtGmshModel::cast2GEntity( dtGmshVertex * gv ) {
    assert( gv!=NULL );
    return dynamic_cast< ::GEntity * >(gv);
  }

  std::list< dtGmshEdge * > dtGmshModel::cast2DtGmshEdge( 
    std::list< ::GEdge * > edges 
  ) {
		std::list< dtGmshEdge * > retEdges;
		dt__forAllIter(std::list< ::GEdge * >, edges, it) {
			retEdges.push_back( cast2DtGmshEdge(*it) );
		}
		return retEdges;
	}
	
  std::vector< dtGmshEdge * > dtGmshModel::cast2DtGmshEdge( 
    std::vector< ::GEdge * > edges 
  ) {
		std::vector< dtGmshEdge * > ret;
		dt__forAllRefAuto(edges, anEdge) ret.push_back( cast2DtGmshEdge(anEdge) );
    
		return ret;
	}
  
  std::list< dtGmshFace * > dtGmshModel::cast2DtGmshFace( 
    std::list< ::GFace * > faces 
  ) {
		std::list< dtGmshFace * > ret;
		dt__forAllRefAuto(faces, aFace) ret.push_back( cast2DtGmshFace(aFace) );
		return ret;
	}

  std::vector< dtGmshFace * > dtGmshModel::cast2DtGmshFace( 
    std::vector< ::GFace * > faces 
  ) {
		std::vector< dtGmshFace * > ret;
		dt__forAllRefAuto(faces, aFace) ret.push_back( cast2DtGmshFace(aFace) );
		return ret;
	}  

  std::list< dtGmshFace * > dtGmshModel::cast2DtGmshFace( 
    std::list< ::GEntity * > faces 
  ) {
		std::list< dtGmshFace * > ret;
		dt__forAllRefAuto(faces, aFace) ret.push_back( cast2DtGmshFace(aFace) );
		return ret;
	}
  
  std::list< dtGmshRegion * > dtGmshModel::cast2DtGmshRegion( 
    std::list< ::GRegion * > regions 
  ) {
		std::list< dtGmshRegion * > ret;
		dt__forAllIter(std::list< ::GRegion * >, regions, it) {
			ret.push_back( cast2DtGmshRegion(*it) );
		}
		return ret;
	}	  
	
  std::list< dtGmshRegion * > dtGmshModel::cast2DtGmshRegion( 
    std::list< ::GEntity * > regions 
  ) {
		std::list< dtGmshRegion * > ret;
		dt__forAllIter(std::list< ::GEntity * >, regions, it) {
			ret.push_back( cast2DtGmshRegion(*it) );
		}
		return ret;
	}	  
  
  dtGmshVertex * dtGmshModel::cast2DtGmshVertex( ::GEntity * gv ) {
    assert( gv!=NULL );
    return dtGmshVertex::MustDownCast(gv);
  }

  std::list< dtGmshVertex * > dtGmshModel::cast2DtGmshVertex( 
    std::list< ::GVertex * > vertices 
  ) {
		std::list< dtGmshVertex * > retVertices;
		dt__forAllIter(std::list< ::GVertex * >, vertices, it) {
			retVertices.push_back( cast2DtGmshVertex(*it) );
		}
		return retVertices;
	}
  
  std::vector< dtGmshVertex * > dtGmshModel::cast2DtGmshVertex( 
    std::vector< ::GVertex * > vertices 
  ) {
		std::vector< dtGmshVertex * > ret;
		dt__forAllRefAuto(vertices, aVert) ret.push_back( cast2DtGmshVertex(aVert) );
    
		return ret;
	}
  
  dtPoint3 dtGmshModel::extractPosition( ::GVertex const * const gv ) {
    return cast2DtGmshVertex(
			const_cast< ::GVertex * >(gv)
		)->cast2DtPoint3();
  }

  dtPoint3 dtGmshModel::extractPosition( ::MVertex const * const mv ) {
    return dtPoint3(
			const_cast< ::MVertex * >(mv)->x(), 
			const_cast< ::MVertex * >(mv)->y(), 
			const_cast< ::MVertex * >(mv)->z()
		);
  }
	
	void dtGmshModel::setPosition( ::MVertex * mv, dtPoint3 const & pp ) {
		mv->setXYZ(pp.x(), pp.y(), pp.z());
	}
  
  void dtGmshModel::add( ::GEntity * ge) {
    if ( dtGmshVertex::DownCast(ge) ) {
      GModel::add( dtGmshVertex::SecureCast(ge) );
    }
    else if ( dtGmshEdge::DownCast(ge) ) {
      GModel::add( dtGmshEdge::SecureCast(ge) );
    }
    else if ( dtGmshFace::DownCast(ge) ) {
      GModel::add( dtGmshFace::SecureCast(ge) );
    }
    else if ( dtGmshRegion::DownCast(ge) ) {
      GModel::add( dtGmshRegion::SecureCast(ge) );
    }    
    else dt__throwUnexpected(add);
  }
	
  void dtGmshModel::meshEdgeTransfiniteFromTo(
    dtInt const from, dtInt const to, 
    dtInt const type, dtReal const coeff, 
    dtInt const nEl) {
    
    dtInt eTag = getDtGmshEdgeTagByFromTo(from, to);
    dtGmshEdge * dtge = getDtGmshEdgeByTag( abs(eTag) );
    
    if (eTag > 0) {
      dtge->meshTransfiniteWNElements(type, coeff, nEl);
    }
    else {
      dtge->meshTransfiniteWNElements(type, 1./coeff, nEl);
    }
  }
  
  void dtGmshModel::prepareToMesh( void ) {
    // Change any high order elements back into first order ones    
    // Initialize pseudo random mesh generator with the same seed
    srand(1);
    SetOrder1(this);    
  }

  dtInt dtGmshModel::alreadyInModel( ::GVertex const * const gv ) const {
		for (
      ::GModel::viter vIt = __caCThis->firstVertex(); 
      vIt != __caCThis->lastVertex(); 
      ++vIt
    ) {
			if ( dtGmshVertex::isEqual(gv, *vIt) ) {
				return (*vIt)->tag();
			}
		}
		return 0;
	}

  dtInt dtGmshModel::alreadyInModel( ::GEdge const * const ge ) const {
		for (
      ::GModel::eiter eIt = __caCThis->firstEdge(); 
      eIt != __caCThis->lastEdge(); 
      ++eIt
    ) {
			if ( dtGmshEdge::isEqual(ge, *eIt) ) {
				return (*eIt)->tag();
			}
		}
		return 0;
	}
	
  dtInt dtGmshModel::alreadyInModel( ::GFace const * const gf ) const {
		for (
      ::GModel::fiter fIt = __caCThis->firstFace(); 
      fIt != __caCThis->lastFace(); 
      ++fIt
    ) {
			if ( dtGmshFace::isEqual(gf, *fIt) ) {
				return (*fIt)->tag();
			}
		}
		return 0;
	}

  dtInt dtGmshModel::alreadyInModel( ::GRegion const * const gr ) const {
		for (
      ::GModel::riter rIt = __caCThis->firstRegion(); 
      rIt != __caCThis->lastRegion(); 
      ++rIt
    ) {
			if ( dtGmshRegion::isEqual(gr, *rIt) ) {
				return (*rIt)->tag();
			}
		}
		return 0;
	}	  
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( void ) const {
		::GModel::setCurrent(__caCThis);
			
		//
		// get all entities
		//
		std::vector< ::GEntity * > ent;
		this->getEntities(ent);
		vectorHandling< dtPoint3 > pp(__caCThis->getNumMeshVertices());		
		std::map< int, dtInt > vLoc_num;
		
		int counter = 0;
		for( dtInt ii=0; ii<ent.size(); ii++ ) {
      for( dtInt jj=0; jj<ent[ii]->getNumMeshVertices(); jj++ ) {
				::MVertex const * const mv = ent[ii]->getMeshVertex(jj);				
				pp[counter]
				=
				dtPoint3(
					static_cast< dtReal >(mv->x()), 
					static_cast< dtReal >(mv->y()), 
					static_cast< dtReal >(mv->z())
				); 				
				vLoc_num[mv->getNum()] = counter;
				counter++;
			}
		}	
    
		//
		// create unstructured mesh and set points
		//
		unstructured3dMesh * um = new unstructured3dMesh();		
		um->addPoints(pp);
		
		for( dtInt ii=0; ii<ent.size(); ii++ ) {
			::GVertex * gv = dynamic_cast< ::GVertex * >(ent[ii]);
			::GEdge * ge = dynamic_cast< ::GEdge * >(ent[ii]);
			::GFace * gf = dynamic_cast< ::GFace * >(ent[ii]);
			::GRegion * gr = dynamic_cast< ::GRegion * >(ent[ii]);

      if (gv) {
				unsigned nEl = gv->getNumMeshElements();
				int nElTot = nEl;
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GVertex>:" << std::endl
					<< "points = " << nElTot << std::endl
					<< dt__eval(nElTot)
				);		
			}		
      else if (ge) {
				unsigned nEl = ge->getNumMeshElements();
				int nElTot = nEl;
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GEdge>:" << std::endl
					<< "lines = " << nElTot << std::endl
					<< dt__eval(nElTot)
				);		
			}					
      else if (gf) {
				std::vector< unsigned > nEl(3,0);
				gf->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2];
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GFace>:" << std::endl
					<< "triangles = " << nEl[0] << std::endl
					<< "quadrangles = " << nEl[1] << std::endl
					<< "polygons = " << nEl[2] << std::endl
					<< dt__eval(nElTot)
				);		
			}			
      else if (gr) {
				std::vector< unsigned > nEl(5,0);
				ent[ii]->getNumMeshElements(&(nEl[0]));
				int nElTot = nEl[0] + nEl[1] + nEl[2] + nEl[3] + nEl[4];
				dt__info(toCoDoUnstructuredGrid(),
					<< "GEntity[" << ii << "]<GRegion>:" << std::endl
					<< "tetrahedra = " << nEl[0] << std::endl
					<< "hexahedra = " << nEl[1] << std::endl
					<< "prisms = " << nEl[2] << std::endl
					<< "pyramids = " << nEl[3] << std::endl
					<< "polyhedra = " << nEl[4] << std::endl
					<< dt__eval(nElTot)
				);		
			}
		}
		//
		// write only 3d elements
		//
    for (
			::GModel::riter r_it = __caCThis->firstRegion(); 
			r_it != __caCThis->lastRegion(); 
			++r_it
		) {
      ::GRegion * gr = *r_it;
			unsigned nElemTot = gr->getNumMeshElements();
      for( unsigned ii=0; ii<nElemTot; ii++ ) {
        ::MElement * me = gr->getMeshElement(ii);
        ::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
        ::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
        //
        // tetrahedron
        //
        if ( mtet ) {
				  vectorHandling< dtInt > vertsIndex(4);					
          std::vector< ::MVertex * > verts;
          mtet->getVertices(verts);        
          vertsIndex[0] = vLoc_num[verts[0]->getNum()];
          vertsIndex[1] = vLoc_num[verts[1]->getNum()];
          vertsIndex[2] = vLoc_num[verts[2]->getNum()];
          vertsIndex[3] = vLoc_num[verts[3]->getNum()];
					um->addElement(vertsIndex);
        }     
        //
        // hexahedron
        //
        else if ( mhex ) {
					vectorHandling< dtInt > vertsIndex(8);
          std::vector< ::MVertex * > verts;
					mhex->getVertices(verts); 
          vertsIndex[0] = vLoc_num[verts[4]->getNum()];
          vertsIndex[1] = vLoc_num[verts[5]->getNum()];
          vertsIndex[2] = vLoc_num[verts[1]->getNum()];
          vertsIndex[3] = vLoc_num[verts[0]->getNum()];
          vertsIndex[4] = vLoc_num[verts[7]->getNum()];
          vertsIndex[5] = vLoc_num[verts[6]->getNum()];
          vertsIndex[6] = vLoc_num[verts[2]->getNum()];
          vertsIndex[7] = vLoc_num[verts[3]->getNum()];          
					um->addElement(vertsIndex);
        }  
      }
    }
		
		return um;
	}
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( 
	  std::vector< ::MElement const * > const & elements
	) {
		std::vector< ::MVertex const * > vertices;
		for( dtInt ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			std::vector< ::MVertex * > verts;
			me->getVertices(verts);
			for( dtInt ii=0; ii<verts.size(); ii++ ) vertices.push_back(verts[ii]);			
		}		
		progHelper::removeBastardTwins(vertices);
		return dtGmshModel::toUnstructured3dMesh(vertices, elements);
	}	
	
  unstructured3dMesh * dtGmshModel::toUnstructured3dMesh( 
	  std::vector< ::MVertex const * > const & vertices, 
    std::vector< ::MElement const * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, dtInt > vLoc_num;
		for( dtInt ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< dtReal >(mv->x()), 
				static_cast< dtReal >(mv->y()), 
				static_cast< dtReal >(mv->z())
			); 				
			vLoc_num[mv->getNum()] = ii;
		}

		unstructured3dMesh * um = new unstructured3dMesh();		
		um->addPoints(pp);
		
		for( dtInt ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			::MTetrahedron * mtet = dynamic_cast< ::MTetrahedron * >(me);
			::MPyramid * mpyr = dynamic_cast< ::MPyramid * >(me);
			::MPrism * mpri = dynamic_cast< ::MPrism * >(me);			
			::MHexahedron * mhex = dynamic_cast< ::MHexahedron * >(me);
			//
			// tetrahedron
			//
			if ( mtet ) {
				vectorHandling< dtInt > vertsIndex(4);					
				std::vector< ::MVertex * > verts;
				mtet->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}     
			//
			// pyramid
			//
			else if ( mpyr ) {
				vectorHandling< dtInt > vertsIndex(5);
				std::vector< ::MVertex * > verts;
				mpyr->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				vertsIndex[4] = vLoc_num[verts[4]->getNum()];
				um->addElement(vertsIndex);
			}  
			//
			// prism
			//
			else if ( mpri ) {
				vectorHandling< dtInt > vertsIndex(6);
				std::vector< ::MVertex * > verts;
				mpri->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				vertsIndex[4] = vLoc_num[verts[4]->getNum()];
				vertsIndex[5] = vLoc_num[verts[5]->getNum()];
				um->addElement(vertsIndex);
			}  			
			//
			// hexahedron
			//
			else if ( mhex ) {
				vectorHandling< dtInt > vertsIndex(8);
				std::vector< ::MVertex * > verts;
				mhex->getVertices(verts); 
				vertsIndex[0] = vLoc_num[verts[4]->getNum()];
				vertsIndex[1] = vLoc_num[verts[5]->getNum()];
				vertsIndex[2] = vLoc_num[verts[1]->getNum()];
				vertsIndex[3] = vLoc_num[verts[0]->getNum()];
				vertsIndex[4] = vLoc_num[verts[7]->getNum()];
				vertsIndex[5] = vLoc_num[verts[6]->getNum()];
				vertsIndex[6] = vLoc_num[verts[2]->getNum()];
				vertsIndex[7] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}  
		}		
		
		return um;
	}	
	
  unstructured3dSurfaceMesh * dtGmshModel::toUnstructured3dSurfaceMesh( 
	  std::vector< ::MVertex const * > const & vertices, 
		std::vector< ::MElement const * > const & elements
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		std::map< int, dtInt > vLoc_num;
		for( dtInt ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< dtReal >(mv->x()), 
				static_cast< dtReal >(mv->y()), 
				static_cast< dtReal >(mv->z())
			); 			
			vLoc_num[mv->getNum()] = ii;
		}

		unstructured3dSurfaceMesh * um = new unstructured3dSurfaceMesh();		
		um->addPoints(pp);
		
		for( dtInt ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			::MQuadrangle * mquad = dynamic_cast< ::MQuadrangle * >(me);
			::MTriangle * mtri = dynamic_cast< ::MTriangle * >(me);
			
			//
			// quadrangle
			//
			if ( mquad ) {
				vectorHandling< dtInt > vertsIndex(4);					
				std::vector< ::MVertex * > verts;
				mquad->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				vertsIndex[3] = vLoc_num[verts[3]->getNum()];
				um->addElement(vertsIndex);
			}    
			else if ( mtri ) {
				vectorHandling< dtInt > vertsIndex(3);					
				std::vector< ::MVertex * > verts;
				mtri->getVertices(verts);        
				vertsIndex[0] = vLoc_num[verts[0]->getNum()];
				vertsIndex[1] = vLoc_num[verts[1]->getNum()];
				vertsIndex[2] = vLoc_num[verts[2]->getNum()];
				um->addElement(vertsIndex);
			}     				
		}		
		
		return um;		
	}	

  discrete3dPoints * dtGmshModel::toDiscrete3dPoints( 
	  std::vector< ::MVertex const * > const & vertices
	) {
		std::vector< dtPoint3 > pp(vertices.size());
		for( dtInt ii=0; ii<vertices.size(); ii++ ) {
			::MVertex const * const mv = vertices[ii];				
			pp[ii]
			=
			dtPoint3(
				static_cast< dtReal >(mv->x()), 
				static_cast< dtReal >(mv->y()), 
				static_cast< dtReal >(mv->z())
			); 			
		}
		
		return new discrete3dPoints(pp);
	}	
	
  renderInterface * dtGmshModel::toAdequateSurfaceRenderInterface( 
		std::vector< ::MElement const * > const & elements
	) {
		std::vector< ::MVertex const * > vertices;
		for( dtInt ii=0; ii<elements.size(); ii++ ) {
			::MElement * me = const_cast< ::MElement * >(elements[ii]);
			std::vector< ::MVertex * > verts;
			me->getVertices(verts);
			for( dtInt ii=0; ii<verts.size(); ii++ ) vertices.push_back(verts[ii]);			
		}		
    progHelper::removeBastardTwins(vertices);		
    
		return dtGmshModel::toAdequateSurfaceRenderInterface(vertices, elements);
	}		
	
  renderInterface * dtGmshModel::toAdequateSurfaceRenderInterface( 
	  std::vector< ::MVertex const * > const & vertices, 
		std::vector< ::MElement const * > const & elements
	) {
		if (elements.size() != 0) {
			return toUnstructured3dSurfaceMesh(vertices, elements);
		}
		else return toDiscrete3dPoints(vertices);
	}		
	
	void dtGmshModel::dtReadCGNS(const std::string &name) {		
			std::vector< dtGmshFace * >  faces;
			std::vector< dtGmshRegion * >  regions;
							
			typedef std::map<int, ::MVertex*> vertexMap_t;
			typedef std::map<int, ::MElement*> elementMap_t;		
			
			int vNum = 0;	
			vertexMap_t vertexMap;
			int minVertex = 1;
			int maxVertex = 0;		

			int eNum = 0;
			elementMap_t elementMap;			
			int minElement = 1;
			int maxElement = 0;
				
			int fNum = 0;
			int rNum = 0;

			//
			// open cgns file
			//
			int index_file;
			__cgnsCheck(cg_open(name.c_str(), CG_MODE_READ, &index_file));

			//
			// get number of bases
			//
			int nBases;
			__cgnsCheck(cg_nbases(index_file, &nBases));
			dt__info(dtReadCGNS(), << "Found " << nBases << " base(s).");
			dt__throwIf(nBases > 1, dtReadCGNS());
			int index_base = 1;
			
			//
			// number of zones
			//
			int nZones;
			__cgnsCheck(cg_nzones(index_file, index_base, &nZones));
			dt__info(dtReadCGNS(), << "Found " << nZones << " zone(s).");

			for (int index_zone = 1; index_zone <= nZones; index_zone++) {
			  dt__info(dtReadCGNS(), << "Reading zone " << index_zone);
				
				ZoneType_t zoneType;
				__cgnsCheck(cg_zone_type(index_file, index_base, index_zone, &zoneType));

				dt__throwIf(zoneType==ZoneTypeNull, dtReadCGNS() );
				dt__throwIf(zoneType==ZoneTypeUserDefined, dtReadCGNS() );
				dt__throwIf(zoneType==Structured, dtReadCGNS() );
				if ( zoneType == Unstructured ) {
					dt__info(dtReadCGNS(), << "Unstructured zone detected.");
					//
					// read zone info
					//				
					cgsize_t zoneSizes[3];				
					char zoneName[35];
					__cgnsCheck(
            cg_zone_read(
              index_file, index_base, index_zone, zoneName, zoneSizes
            )
          );
					int nNodes = static_cast< dtInt >(zoneSizes[0]);
					int nCells = static_cast< dtInt >(zoneSizes[1]);
					dt__info(
					  dtReadCGNS(), 
						<< dt__eval(zoneName) << std::endl
						<< dt__eval(zoneSizes[0]) << std::endl
						<< dt__eval(zoneSizes[1]) << std::endl
						<< dt__eval(zoneSizes[2]) << std::endl
						<< dt__eval(nNodes) << std::endl
						<< dt__eval(nCells)
					);
					
					//
					// read coordinates
					//				
					int nCoords;
					__cgnsCheck(cg_ncoords(index_file, index_base, index_zone, &nCoords));		
					DataType_t dataType;
					char coordName[35];
					void* coord;
					twoDArrayHandling< double > nodes(nNodes, nCoords);

					for ( dtInt iCoord = 0; iCoord < nCoords; iCoord++ ) {
						__cgnsCheck(
							cg_coord_info(
								index_file, index_base, index_zone, 
								iCoord+1, &dataType, coordName
							)
						);
						dt__info(
							dtReadCGNS(),
							<< "Reading coordinate " << iCoord+1 << " : " << coordName
						);

						cgsize_t irmin  = 1;
						cgsize_t irmax = zoneSizes[0];
						switch(dataType) {
							case RealSingle:
    						dt__info(dtReadCGNS(), << "Type is float");
								coord = new dtReal[nNodes];
								__cgnsCheck(
									cg_coord_read(
										index_file, index_base, index_zone, 
										coordName, dataType, &irmin, &irmax, coord
									)
								);
								for (int iNode = 0; iNode < nNodes; iNode++) {
									nodes[iNode][iCoord] = (double)((dtReal*)coord)[iNode];
								}
								delete [] (dtReal*)coord;
								break;
							case RealDouble:
								dt__info(dtReadCGNS(), << "Type is double");
								coord = new double[nNodes];
								__cgnsCheck(
									cg_coord_read(
										index_file, index_base, index_zone, 
										coordName, dataType, &irmin, &irmax, coord
									)
								);
								for (int iNode = 0; iNode < nNodes; iNode++) {
									nodes[iNode][iCoord] = ((double*) coord)[iNode];
								}
								delete [] (double*)coord;
								break;
						}
					}	

					//
					// create vertices
					//		
					for (int iNode = 0; iNode < nNodes; iNode++) {
						vNum++;
						::MVertex* mv 
            = 
            new ::MVertex(
              nodes[iNode][0], nodes[iNode][1], nodes[iNode][2], 0, vNum
            );
						minVertex = std::min(minVertex, vNum);
						maxVertex = std::max(maxVertex, vNum);
						vertexMap[vNum] = mv;
					}
	
			  //
			  // read sections
				//
				int nSec;
				__cgnsCheck( cg_nsections(index_file, index_base, index_zone, &nSec) );
				dt__info(dtReadCGNS(), << "Found " << nSec << " sections.");
				for (int index_section = 1; index_section <= nSec; index_section++) {
					char secName[30];
					ElementType_t elementType;
					cgsize_t bounds[2];
					int nBoundary;
					int parentFlag = 0;
					__cgnsCheck(
						cg_section_read(
							index_file, index_base, index_zone, index_section,
							secName, &elementType, 
              &(bounds[0]), &(bounds[1]), &nBoundary, &parentFlag
						)
					);
					dt__info(
					  dtReadCGNS(), 
						<< dt__eval(secName) << std::endl
						<< dt__eval(bounds[0]) << std::endl
						<< dt__eval(bounds[1]) << std::endl
						<< dt__eval(nBoundary) << std::endl
						<< dt__eval(parentFlag) 
					);					
           
					//
					// read element data
					//
					cgsize_t elementDataSize;				
					__cgnsCheck( 
						cg_ElementDataSize(
							index_file, index_base, index_zone, index_section,
							&elementDataSize
						)
					);
					std::vector< cgsize_t > elementData(elementDataSize);					
					__cgnsCheck(
						cg_elements_read(
							index_file, index_base, index_zone, index_section,
							&(elementData[0]), NULL
						)
					);

					dt__info(
						dtReadCGNS(), 
						<< dt__eval(elementDataSize) << std::endl
						<< dt__eval(elementType) << std::endl
            << dt__eval(ElementTypeName[elementType])
					);
					int nElements;
					int tmpC = 0;
					switch(elementType) {
						/*        gmsh                              cgns                   
						 *         v                                 v                        
						 *         ^                                 ^                         
						 *         |                                 |                      
						 *   3-----------2                     3-----------2                  
						 *   |     |     |                     |     |     |                
						 *   |     |     |           >>>       |     |     |               
						 *   |     +---- | --> u     >>>       |     +---- | --> u              
						 *   |           |           >>>       |           |                   
						 *   |           |                     |           |              
						 *   0-----------1                     0-----------1           
						 */									
						case QUAD_4:
							fNum++;
							faces.push_back( new dtGmshFace(this, fNum) );
							nElements = elementDataSize/4;
							//
							// create elements
							//
							tmpC = 0;
							faces.back()->addPhysicalEntity(
							  setPhysicalName(secName, 2, 0)
							);
							this->add( faces.back() );
							for (int iEl = bounds[0]; iEl<=bounds[1]; iEl++) {
								eNum++;								
								int counter = (tmpC)*4;
								tmpC++;
								std::vector< ::MVertex * > locVertices(4, NULL);
								locVertices[0] = vertexMap[elementData[counter+0]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+2]];
								locVertices[3] = vertexMap[elementData[counter+3]];
								::MQuadrangle * me = new ::MQuadrangle(locVertices, eNum, 0);
								minElement = std::min(minElement, eNum);
								maxElement = std::max(maxElement, eNum);
								elementMap[eNum] = me;
								faces.back()->addQuadrangle(me);
							}
							break;
						/*         gmsh                           cgns
						 *          v                              v
						 *   3----------2                   7----------6
						 *   |\     ^   |\                  |\     ^   |\
						 *   | \    |   | \                 | \    |   | \
						 *   |  \   |   |  \                |  \   |   |  \
						 *   |   7------+---6        >>>    |   8------+---5
						 *   |   |  +-- |-- | -> u   >>>    |   |  +-- |-- | -> u
						 *   0---+---\--1   |        >>>    3---+---\--2   |
						 *    \  |    \  \  |                \  |    \  \  |
						 *     \ |     \  \ |                 \ |     \  \ |
						 *      \|      w  \|                  \|      w  \|
						 *       4----------5                   4----------1
						 */													
						case HEXA_8:
							rNum++;
							regions.push_back( new dtGmshRegion(this, rNum) );
							regions.back()->addPhysicalEntity(
							  setPhysicalName(secName, 3, 0)
							);							
							this->add( regions.back() );
							nElements = elementDataSize/4;							
							nElements = elementDataSize/8;
							//
							// create elements
							//
							tmpC = 0;
							for (int iEl = bounds[0]; iEl<= bounds[1]; iEl++) {						
								eNum++;								
								int counter = (tmpC)*8;
								tmpC++;
								std::vector< ::MVertex * > locVertices(8, NULL);
								locVertices[0] = vertexMap[elementData[counter+2]];
								locVertices[1] = vertexMap[elementData[counter+1]];
								locVertices[2] = vertexMap[elementData[counter+5]];
								locVertices[3] = vertexMap[elementData[counter+6]];
								locVertices[4] = vertexMap[elementData[counter+3]];
								locVertices[5] = vertexMap[elementData[counter+0]];
								locVertices[6] = vertexMap[elementData[counter+4]];
								locVertices[7] = vertexMap[elementData[counter+7]];
								::MHexahedron * me = new ::MHexahedron(locVertices, eNum, 0);
								minElement = std::min(minElement, eNum);
								maxElement = std::max(maxElement, eNum);
								elementMap[eNum] = me;
								regions.back()->addHexahedron(me);
							}
							break;
						default:
							dt__throw(dtReadCGNS(), << ElementTypeName[elementType]);
					}
				}				
			}
		}
		
		__cgnsCheck( cg_close(index_file) );
		
		dt__throwIf( vertexMap.size() != vNum, dtReadCGNS() );
		dt__throwIf( elementMap.size() != eNum, dtReadCGNS() );
		
		dt__forAllIter(vertexMap_t, vertexMap, it) {
			::MVertex * mv = it->second;
			mv->setEntity(regions.back());
			regions.back()->addMeshVertex(mv);
		}
	}	

	void dtGmshModel::clearModel( void ) {
	  this->destroy(true);
		this->deleteMesh();
	}	

	int dtGmshModel::getMaxVertexTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::viter v_it=GModel::vertices.begin(); 
      v_it!=GModel::vertices.end(); 
      ++v_it 
    ) if ( (*v_it)->tag() > maxTag ) maxTag = (*v_it)->tag();
		return maxTag;
	}
	
	int dtGmshModel::getMaxEdgeTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::eiter e_it= GModel::edges.begin(); 
      e_it!=GModel::edges.end(); 
      ++e_it 
    ) if ( (*e_it)->tag() > maxTag ) maxTag = (*e_it)->tag();
		return maxTag;
	}
	
	int dtGmshModel::getMaxFaceTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::fiter f_it = GModel::faces.begin(); 
      f_it!=GModel::faces.end(); 
      ++f_it 
    ) if ( (*f_it)->tag() > maxTag ) maxTag = (*f_it)->tag();
		return maxTag;
	}	
	
	int dtGmshModel::getMaxRegionTag( void ) const {
		int maxTag = 0;
		for( 
      ::GModel::riter r_it = GModel::regions.begin(); 
      r_it!=GModel::regions.end(); 
      ++r_it 
    ) if ( (*r_it)->tag() > maxTag ) maxTag = (*r_it)->tag();
		return maxTag;
	}		
	
  std::list< ::GVertex * > dtGmshModel::vertices( void ) const {
		std::list< ::GVertex * > ll;
    for( 
      ::GModel::viter it = GModel::vertices.begin(); 
      it!=GModel::vertices.end(); 
      ++it ) ll.push_back( *it );
		
    return ll;
	}
	
  std::list< ::GEdge * > dtGmshModel::edges( void ) const {
		std::list< ::GEdge * > ll;
    for( 
      ::GModel::eiter it = GModel::edges.begin(); 
      it!=GModel::edges.end(); 
      ++it 
    ) ll.push_back( *it );
    
		return ll;		
	}
		
	std::list< ::GFace * > dtGmshModel::faces( void ) const {
		std::list< ::GFace * > faceL;
    for( 
      ::GModel::fiter f_it = GModel::faces.begin(); 
      f_it!=GModel::faces.end(); 
      ++f_it 
    ) faceL.push_back( *f_it );

		return faceL;
	}
	
	std::list< ::GRegion * > dtGmshModel::regions( void ) const {
		std::list< ::GRegion * > regionL;
    for( 
      ::GModel::riter r_it = GModel::regions.begin(); 
      r_it!=GModel::regions.end(); 
      ++r_it 
    ) regionL.push_back( *r_it );

		return regionL;
	}

  std::list< dtGmshVertex * > dtGmshModel::dtVertices( void ) const {
    return cast2DtGmshVertex( vertices() );
	}
	
  std::list< dtGmshEdge * > dtGmshModel::dtEdges( void ) const {
    return cast2DtGmshEdge( edges() );
	}
		
	std::list< dtGmshFace * > dtGmshModel::dtFaces( void ) const {
    return cast2DtGmshFace( faces() );    
	}
	
	std::list< dtGmshRegion * > dtGmshModel::dtRegions( void ) const {
    return cast2DtGmshRegion( regions() );
	}
  
  void dtGmshModel::untagPhysical( ::GEntity * const ge ) {
		dt__throwIfWithMessage(
      ge->physicals.size()>1, 
      untagPhysical(),
      << "physicalName = " << getPhysicalNames(ge->dim(), ge->physicals)
      << std::endl
      << "ge->physicals = [ " << ge->physicals << " ]"
    );
		
    if ( ge->physicals.empty() ) return;
    
    dtInt pNum = ge->physicals[0];
		ge->physicals.clear();
    std::vector< ::GEntity * > ent;
    this->getEntities(ent, ge->dim());
    bool found = false;
    dt__forAllRefAuto(ent, anEnt) {
      if ( 
        std::find( anEnt->physicals.begin(), anEnt->physicals.end(), pNum )
        !=
        anEnt->physicals.end()
      ) {
        found = true;
      }
    }
    if (!found) {
      this->removePhysicalName( this->getPhysicalName(ge->dim(), pNum) );
    }
	}
	
  std::string dtGmshModel::getPhysicalString( 
    ::GEntity const * const ge 
  ) const {
		std::vector< dtInt > pInt 
    = 
    const_cast< ::GEntity * >(ge)->getPhysicalEntities();
    
    if(pInt.size() == 0) return std::string("");
    
    dt__throwIfWithMessage(
      pInt.size()!=1, 
      getPhysicalString(),
      << dt__eval(pInt) << std::endl
      << dt__eval( getPhysicalNames(ge->dim(), pInt) )
    );
    
		return ::GModel::getPhysicalName(ge->dim(), pInt[0]);
	}
	
  std::vector< std::string > dtGmshModel::getPhysicalNames(
    dtInt const & dim, std::vector< dtInt > const & number
  ) const {
    std::vector< std::string > names;
    dt__forAllRefAuto(number, aNumber) {
      names.push_back( 
        ::GModel::getPhysicalName(dim, aNumber)
      );
    }
    return names;
  }

  void dtGmshModel::tagPhysical(
    ::GEntity * const ge, std::string const & pName
  ) {		
    ge->addPhysicalEntity( GModel::setPhysicalName(pName, ge->dim(), 0) );
	}
	  
  dtInt dtGmshModel::getPhysicalNumber(
    const dtInt &dim, const std::string &name
  ) const {
    return 
      const_cast< dtGmshModel * >(
        this
      )->GModel::getPhysicalNumber(dim, name);
  }
  
  void dtGmshModel::removeEmptyPhysicals( void ) {
    logContainer< dtGmshModel > logC(logDEBUG, "removeEmptyPhysicals()");
    
    std::map< std::pair< int, dtInt >, bool > keepName;
    dt__forAllRefAuto( GModel::getPhysicalNames(), aPair ) {
      keepName[ aPair.first ] = false;
    }
    dtGmshModel::intGEntityVMap ge_number;
    //
    // for each dimension
    //
    dt__forFromToIndex(0, 4, dim) {
      logC() << "dim = " << dim << std::endl;
      GModel::getPhysicalGroups(dim, ge_number);     
      //
      // for each physical group
      //
      dt__forAllConstIter(dtGmshModel::intGEntityVMap, ge_number, nIt) {
        logC() << "  id = " << nIt->first << " / " 
          << getPhysicalName(dim, nIt->first) << " has " 
          << nIt->second.size() << " GEntities"
          << std::endl;
        if (nIt->second.empty()) {
          logC() << "    --> delete" << std::endl;
          GModel::removePhysicalGroup(dim, nIt->first);
        }
        else keepName[ std::pair< int, dtInt >(dim, nIt->first) ] = true;
      }
    }
    dt__forAllRefAuto( keepName, aPair ) {
      if ( _physicalNames.find(aPair.first) == _physicalNames.end() ) continue;
      logC() << "Check " << _physicalNames.at(aPair.first) << std::endl;
      if ( !aPair.second ) {
        _physicalNames.erase(
          _physicalNames.find( aPair.first )
        );
        logC() << "  --> erase" << std::endl;
      }
    }    
  }
  
  std::list< std::string > dtGmshModel::getFullPhysicalList( 
    ::GEntity const * const ent 
  ) const {
    std::list< std::string > fullList;

    //
    // region
    //
    if ( dtGmshRegion::ConstDownCast( ent ) ) {
      dtGmshRegion const * const reg = dtGmshRegion::ConstDownCast( ent );
      //
      // add label
      //
      fullList.push_back( reg->getPhysicalString() );
      
      //
      // add number in vector
      //
      dtInt cc = 0;
      dt__forAllRefAuto( dtRegions(), aReg ) {
        if (aReg == reg) {
          fullList.push_back( "R_"+stringPrimitive::intToString(cc) );
        }
        cc = cc + 1;
      }      
    }
    //
    // face
    //
    else if ( dtGmshFace::ConstDownCast( ent ) ) {
      dtGmshFace const * const face = dtGmshFace::ConstDownCast( ent );
      //
      // add label
      //
      fullList.push_back( face->getPhysicalString() );
      std::string faceStr = face->getPhysicalString();
      //
      // region iterate
      //
      dt__forAllRefAuto( face->dtRegions(), aReg ) {
        //
        // region prefix iterate
        //
        dt__forAllRefAuto( getFullPhysicalList(aReg), aPrefix ) {
          fullList.push_back( aPrefix + "->" + faceStr );
          //
          // get position index
          //
          std::list< dtGmshFace * > fL = aReg->dtFaces();
          dtInt faceI = std::distance(
            fL.begin(),
            std::find( fL.begin(), fL.end(), face )
          );
          
          //
          // add position alias
          //
          fullList.push_back( 
            aPrefix + "->F_" + stringPrimitive::intToString(faceI) 
          );          
          if ( _positionStrFace.find(faceI) == _positionStrFace.end() ) {
            continue;
          }
          dt__forAllRefAuto(_positionStrFace[faceI], posStr) {
            fullList.push_back( aPrefix + "->" + posStr );
          }        
        }
      }
    }    
    else if ( dtGmshEdge::ConstDownCast( ent ) ) {
      dtGmshEdge const * const edge = dtGmshEdge::ConstDownCast( ent );
      //
      // add label
      //
      fullList.push_back( edge->getPhysicalString() );
      std::string edgeStr = edge->getPhysicalString();       
      //
      // region iterate
      //
      dt__forAllRefAuto( edge->dtFaces(), aFace ) {
        //
        // region prefix iterate
        //
        dt__forAllRefAuto( getFullPhysicalList(aFace), aPrefix ) {
          fullList.push_back( aPrefix + "->" + edgeStr );
          //
          // get position index
          //
          std::list< dtGmshEdge * > eL = aFace->dtEdges();
          dtInt edgeI = std::distance(
            eL.begin(),
            std::find( eL.begin(), eL.end(), edge )
          );
          //
          // add position alias
          //
          dt__forAllRefAuto(_positionStrEdge[edgeI], posStr) {
            fullList.push_back( aPrefix + "->" + posStr );
          }        
        }
      }
    }
    else if ( dtGmshVertex::ConstDownCast( ent ) ) {
      dtGmshVertex const * const vertex = dtGmshVertex::ConstDownCast( ent );
      //
      // add label
      //
      fullList.push_back( vertex->getPhysicalString() );
      std::string vertexStr = vertex->getPhysicalString();       
      //
      // edge iterate
      //
      dt__forAllRefAuto( vertex->dtEdges(), aEdge ) {
        //
        // edge prefix iterate
        //
        dt__forAllRefAuto( getFullPhysicalList(aEdge), aPrefix ) {
          fullList.push_back( aPrefix + "->" + vertexStr );
          //
          // get position index
          //
          std::list< dtGmshVertex * > vL = aEdge->dtVertices();
          dtInt vertexI = std::distance(
            vL.begin(),
            std::find( vL.begin(), vL.end(), vertex )
          );
          //
          // add position alias
          //
          dt__forAllRefAuto(_positionStrVertex[vertexI], posStr) {
            fullList.push_back( aPrefix + "->" + posStr );
          }        
        }
      }
    }      
    
    return fullList;
  }
        
  std::list< ::GEntity * > dtGmshModel::getGEntityListByWildCardPhysical( 
    std::string wildStr
  ) const {
//    ::boost::smatch what;
    std::vector< std::string > physV;

//    if ( !wildStr.empty() ) {
//      std::string::const_iterator st = wildStr.begin();
//      std::string::const_iterator en = wildStr.end();       
//      // example: REGION->FACE->EDGE->VERTEX
//      // what(0): what.str(0) : "REGION"
//      // what(0): what.str(1) : ""
//      // what(0): what.str(2) : "REGION"
//      // what(1): what.str(0) : "->FACE"
//      // what(1): what.str(1) : "->"
//      // what(1): what.str(2) : "FACE"      
//      // what(2): what.str(0) : "->EDGE"
//      // what(2): what.str(1) : "->"
//      // what(2): what.str(2) : "EDGE"         
//      // what(3): what.str(0) : "->VERTEX"
//      // what(3): what.str(1) : "->"
//      // what(3): what.str(2) : "VERTEX"               
//      //
//      while ( 
//        ::boost::regex_search(
//          st, en, what, ::boost::regex("([-][>])?+([^->]+)")
//        )
//      ) {
//        physV.push_back( what.str(2) );
//        st = what[0].second;
//      }
//    }
//    else physV.push_back("");
    
//    if ( (physV.size() == 1) && ( ge->dim()!=3 ) ) {
//      dt__forFromToIndex(0, (4-ge->dim()-1), ii) {
//        physV.insert( physV.begin(), std::string("*") );
//      }
//    }

    //
    // check input
    //
    dt__throwIf( physV.size()==0, matchWildCardPhysical() );

    //
    // arrays
    //
    std::list< dtGmshRegion * > region;
    std::list< dtGmshFace * > face;
    std::list< dtGmshEdge * > edge;
    std::list< dtGmshVertex * > vertex;
    
    //
    // region str
    //
    if ( physV.size() > 0 ) {
      //
      // pick region
      //
      if ( !stringPrimitive::isWildcard( physV[0] ) ) {
        region = getDtGmshRegionListByPhysical( physV[0] );
      }
      else {
        dt__forAllRefAuto(dtRegions(), aReg) {
          if ( 
            stringPrimitive::matchWildcard( 
              physV[0], aReg->getPhysicalString() 
            )
          ) {
            region.push_back( aReg );
          }
        }
      }
      //
      // face str
      //
      if ( physV.size() > 1 ) {
        dt__forAllRefAuto(region, aReg) {
          //
          // pick face
          //
          if ( _facePositionStr.find(physV[1]) != _facePositionStr.end() ) {
            face.push_back(
              progHelper::list2Vector( aReg->dtFaces() )[
                _facePositionStr[ physV[1] ]
              ]                  
            );
          }
          else {
            dt__forAllRefAuto(aReg->dtFaces(), aFace) {
              if ( 
                stringPrimitive::matchWildcard( 
                  physV[1], aFace->getPhysicalString() 
                )
              ) {
                face.push_back( aFace );
              }
            }            
          }
        }
        //
        // edge str
        //
        if ( physV.size() > 2 ) {
          dt__forAllRefAuto(face, aFace) {
            //
            // pick edge
            //
            if ( _edgePositionStr.find(physV[2]) != _edgePositionStr.end() ) {
              edge.push_back(
                progHelper::list2Vector( aFace->dtEdges() )[
                  _edgePositionStr[ physV[2] ]
                ]
              );
            }
            else {
              dt__forAllRefAuto(aFace->dtEdges(), aEdge) {
                if ( 
                  stringPrimitive::matchWildcard( 
                    physV[2], aEdge->getPhysicalString() 
                  )
                ) {
                  edge.push_back( aEdge );
                }
              }            
            }          
          }
          if ( physV.size() > 3 ) {
            dt__forAllRefAuto(edge, aEdge) {
              vertex.push_back(
                progHelper::list2Vector( aEdge->dtVertices() )[
                  _vertexPositionStr[ physV[3] ]
                ]
              );
            }
          }            
        }
      }   
    }
    
    std::list< ::GEntity * > ent;
    if ( physV.size() == 4 ) {
      dt__forAllRefAuto(vertex, aRef) {
        ent.push_back( dynamic_cast< ::GEntity * >(aRef) );
      }
    }
    else if ( physV.size() == 3 ) {
      dt__forAllRefAuto(edge, aRef) {
        ent.push_back( dynamic_cast< ::GEntity * >(aRef) );
      }      
    }    
    else if ( physV.size() == 2 ) {
      dt__forAllRefAuto(face, aRef) {
        ent.push_back( dynamic_cast< ::GEntity * >(aRef) );
      }      
    }        
    else if ( physV.size() == 1 ) {
      dt__forAllRefAuto(region, aRef) {
        ent.push_back( dynamic_cast< ::GEntity * >(aRef) );
      }      
    }
    progHelper::removeBastardTwins( ent );
    
    return ent;
  }
  
  bool dtGmshModel::matchWildCardPhysical( 
    std::string wildStr, ::GEntity const * const ge 
  ) const {
    std::vector< std::string > physV;

    if ( !wildStr.empty() ) {
      // example: REGION->FACE->EDGE->VERTEX
      // what(0): what.str(0) : "REGION"
      // what(0): what.str(1) : ""
      // what(0): what.str(2) : "REGION"
      // what(1): what.str(0) : "->FACE"
      // what(1): what.str(1) : "->"
      // what(1): what.str(2) : "FACE"      
      // what(2): what.str(0) : "->EDGE"
      // what(2): what.str(1) : "->"
      // what(2): what.str(2) : "EDGE"         
      // what(3): what.str(0) : "->VERTEX"
      // what(3): what.str(1) : "->"
      // what(3): what.str(2) : "VERTEX"               
      //
      std::string::const_iterator st = wildStr.begin();
      std::string::const_iterator en = wildStr.end();
      ::boost::smatch what;
      while ( 
        ::boost::regex_search(
          st, en, what, ::boost::regex("([-][>])?+([^->]+)")
        )
      ) {
        physV.push_back( what.str(2) );
        st = what[0].second;
      }
    }
    else physV.push_back("");
    
    if ( (physV.size() == 1) && ( ge->dim()!=3 ) ) {
      dt__forFromToIndex(0, (4-ge->dim()-1), ii) {
        physV.insert( physV.begin(), std::string("*") );
      }
    }
    std::string wildStrWork = physV[0];
    dt__forFromToIndex(1, physV.size(), ii) {
      wildStrWork = wildStrWork+"->"+physV[ii];
    }
   
    dt__forAllRefAuto( getFullPhysicalList(ge), aPhysical) {
      if ( !stringPrimitive::isWildcard(wildStr) ) {
        if ( wildStr ==  aPhysical ) {
          dt__debug(
            matchWildCardPhysical(), 
            << "Direct match with wildStr = " << wildStr
          );
          return true;
        }
      }
      if ( stringPrimitive::matchWildcard(wildStrWork, aPhysical) ) {
        dt__debug(
          matchWildCardPhysical(), 
          << "Match with wildStrWork = " << wildStrWork
          << " == " << aPhysical
        );
        return true;
      }
    }
    return false;
  }
  
  void dtGmshModel::setDebug( std::string const debug ) {
    _debug = debug;
  }
  
  std::map< ::GEntity *, ::GEntity * > dtGmshModel::createTwin( 
    dtInt const dim0Tag, dtInt const dim1Tag, dtInt const dim2Tag, dtInt const dim3Tag 
  ) const {
    std::map< dtGmshVertex *, dtGmshVertex * > dim0 = createVertexTwins(dim0Tag);
    std::map< dtGmshEdge *, dtGmshEdge * > dim1 = createEdgeTwins(dim1Tag);
    std::map< dtGmshFace *, dtGmshFace * > dim2 = createFaceTwins(dim2Tag);
    std::map< dtGmshRegion *, dtGmshRegion * > dim3 = createRegionTwins(dim3Tag);
    
    updateAssociations( dim0, dim1, dim2, dim3 );
    
    std::map< ::GEntity *, ::GEntity * > retMap;
    dt__forAllRefAuto(dim0, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim1, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim2, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    dt__forAllRefAuto(dim3, aPair) {
      retMap[ (::GEntity *) aPair.first ] = (::GEntity *) aPair.second;
    }
    
    return retMap;
  }
 
  std::map< dtGmshVertex *, dtGmshVertex * > 
  dtGmshModel::createVertexTwins( dtInt startTag ) const {
    std::map< dtGmshVertex *, dtGmshVertex * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshVertex(vertices()), aGV ) {
      //
      // clone
      //
      startTag++;
      dtGmshVertex * twin = new dtGmshVertex( NULL, startTag );
      twin->setPosition( dtGmshModel::extractPosition( aGV ) );
      newOld[ aGV ] = twin;
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGV->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }
  
  std::map< dtGmshEdge *, dtGmshEdge * >
  dtGmshModel::createEdgeTwins( dtInt startTag ) const {
    std::map< dtGmshEdge *, dtGmshEdge * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshEdge(edges()), aGE ) {
      //
      // clone
      //
      startTag++;
      dtGmshEdge * twin = new dtGmshEdge( NULL, startTag );
      newOld[ aGE ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGE->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  std::map< dtGmshFace *, dtGmshFace * >
  dtGmshModel::createFaceTwins( dtInt startTag ) const {
    std::map< dtGmshFace *, dtGmshFace * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace(faces()), aGF ) {
      //
      // clone
      //
      startTag++;      
      dtGmshFace * twin = new dtGmshFace( NULL, startTag );
      newOld[ aGF ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGF->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  std::map< dtGmshRegion *, dtGmshRegion * >
  dtGmshModel::createRegionTwins( dtInt startTag ) const {
    std::map< dtGmshRegion *, dtGmshRegion * > newOld;
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshRegion(regions()), aGR ) {
      //
      // clone
      //
      startTag++; 
      dtGmshRegion * twin = new dtGmshRegion( NULL, startTag );
      newOld[ aGR ] = twin;
      
//      //
//      // copy mesh vertices
//      //
//      dt__forAllRefAuto( aGR->mesh_vertices, aMV ) {
//        twin->mesh_vertices.push_back( 
//          new ::MVertex(aMV->x(), aMV->y(), aMV->z(), twin) 
//        );
//      }
    }
    
    return newOld;
  }

  void dtGmshModel::updateAssociations( 
    std::map< dtGmshVertex *, dtGmshVertex * > & dim0, 
    std::map< dtGmshEdge *, dtGmshEdge * > & dim1,
    std::map< dtGmshFace *, dtGmshFace * > & dim2,
    std::map< dtGmshRegion *, dtGmshRegion * > & dim3
  ) const {
    //
    // vertex associations
    //
    dt__info(updateAssociations(), << "Vertex associations.");
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshVertex(vertices()), aGV ) {
      dtGmshVertex * clone = dim0[ aGV ];
      // update edge associations
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshEdge(aGV->edges()), aGE 
      ) {
        clone->addGEntity( dim1[ aGE ] );
      }
    }

    //
    // edge associations
    //
    dt__info(updateAssociations(), << "Edge associations.");
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshEdge(edges()), aGE ) {
      dtGmshEdge * clone = dim1[ aGE ];
      // update begin and end vertex
      clone->setBeginVertex( 
        dim0[ dtGmshVertex::DownCast( aGE->getBeginVertex()) ] 
      );
      clone->setEndVertex( 
        dim0[ dtGmshVertex::DownCast( aGE->getEndVertex()) ] 
      );      
      // update vertices
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshVertex(aGE->vertices()), aGV 
      ) {
        clone->addVertex( dim0[ aGV ] );
      }
      // update faces
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshFace(aGE->faces()), aGF
      ) {
        clone->addFace( dim2[ aGF ] );
      }      
    }

    //
    // face associations
    //
    dt__info(updateAssociations(), << "Face associations.");
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace(faces()), aGF ) {
      dtGmshFace * clone = dim2[ aGF ];
      // update edges
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshEdge(aGF->edges()), aGE
      ) {
        clone->addEdge( dim1[ aGE ], aGF->edgeOrientation( aGE ) );
      }
      // update regions
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshRegion(aGF->regions()), aGR
      ) {
        clone->addRegion( dim3[ aGR ] );
      }      
    }    

    //
    // region associations
    //
    dt__info(updateAssociations(), << "Region associations.");
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshRegion(regions()), aGR ) {
      dtGmshRegion * clone = dim3[ aGR ];
      // update faces
      dt__forAllRefAuto( 
        dtGmshModel::cast2DtGmshFace(aGR->faces()), aGF
      ) {
        clone->addFace( dim2[ aGF ], aGR->faceOrientation( aGF ) );
      }
    }        
  }
}
