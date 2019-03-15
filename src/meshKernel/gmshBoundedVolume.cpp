#include "gmshBoundedVolume.h"

#include <unstructured3dSurfaceMesh.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>
#include <discrete3dVector.h>
#include <solid3dLine.h>

#include <iostream>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <gmsh/Gmsh.h>
#include <gmsh/Context.h>
#include <gmsh/OpenFile.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include <gmsh/Field.h>

namespace dtOO {
	gmshBoundedVolume::gmshBoundedVolume() : boundedVolume() {
	  GmshInitialize();		
	}

	gmshBoundedVolume::~gmshBoundedVolume() {
    //
    // gmsh deletes also the models
    //
	  GmshFinalize();		
	}
      
	void gmshBoundedVolume::init( 
		::QDomElement const & element,
		baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
		//
		// get compound and put pieces as regions to gmsh model
		//
		_gm = new dtGmshModel( getLabel() );

    ::GModel::setCurrent( _gm );		
    
    if (optionHandling::debugTrue()) {
      _gm->setDebug( getLabel()+"_debug.msh" );
    };
	}	
	
	vectorHandling< renderInterface * > gmshBoundedVolume::getRender( 
    void 
  ) const {
		if (mustExtRender()) return vectorHandling< renderInterface * >(0);

		//
		// update
		//
		updatePhysicals();		
		
		::GModel::setCurrent( _gm );
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = _gm->toUnstructured3dMesh();//_vertices, _elements);
		return rV;
	}	
	
	vectorHandling< renderInterface * > gmshBoundedVolume::getExtRender( 
    void 
  ) const {
		//
		// update
		//
		updatePhysicals();
		
		::GModel::setCurrent( _gm );
		
		vectorHandling< renderInterface * > rV;
		std::string toRender = extRenderWhat();

    for (int ii=0; ii<4; ii++) {
			dt__forAllIndex(_physLabels[ii], jj) {
			  if (_physLabels[ii][jj] == toRender) {
					int pNum = _gm->getPhysicalNumber(ii, toRender);
					dtGmshModel::intGEntityVMap groups;
					_gm->getPhysicalGroups(ii, groups);
					int physNum = _gm->getPhysicalNumber(ii, toRender);
					std::vector< ::GEntity * > & geV = groups[physNum];
					
					dt__forAllIter(std::vector< ::GEntity * >, geV, it) {
						::GEntity * ge = *it;
						dtGmshFace * gf = dtGmshFace::DownCast(ge);
						dtGmshRegion * gr = dtGmshRegion::DownCast(ge);

					  if (gf) {
							std::vector< ::MElement const * > elTwoD;
							for (int jj=0;jj<gf->getNumMeshElements(); jj++) {
								elTwoD.push_back( gf->getMeshElement(jj) );	
							}
//							std::vector< ::MVertex const * > vertices;
//							_gm->getMeshVerticesForPhysicalGroup(ii, pNum, vertices);
//							if (vertices.empty()) {
//								vertices = gf->getMeshVertices();
//							}
							rV.push_back(
							  dtGmshModel::toAdequateSurfaceRenderInterface(elTwoD)
							);
						}
						else if (gr) {
							std::vector< ::MElement const * > elThreeD;
							for (int jj=0;jj<gr->getNumMeshElements(); jj++) {
								elThreeD.push_back( gr->getMeshElement(jj) );	
							}							
//							std::vector< ::MVertex const * > vertices;
//							_gm->getMeshVerticesForPhysicalGroup(ii, pNum, vertices);
							rV.push_back(
							  dtGmshModel::toUnstructured3dMesh(elThreeD)
							);							
						}
						
					}
				}
			}
		}
    
    if (toRender == "gmsh") {
      dt__forAllRefAuto(_gm->dtEdges(), anEdge) {
        vectorHandling< dtPoint3 > ppV(5);
        vectorHandling< dtPoint3 > vpV(3);
        vectorHandling< dtVector3 > vvV(3);
        ppV[0] = anEdge->getMap1dTo3d()->getPointPercent(0.0);
        ppV[1] = anEdge->getMap1dTo3d()->getPointPercent(0.1);
        ppV[2] = anEdge->getMap1dTo3d()->getPointPercent(0.5);
        ppV[3] = anEdge->getMap1dTo3d()->getPointPercent(0.9);
        ppV[4] = anEdge->getMap1dTo3d()->getPointPercent(1.0);
        
        vvV[0] = anEdge->getMap1dTo3d()->firstDerUPercent(0.1);
        vpV[0] = anEdge->getMap1dTo3d()->getPointPercent(0.1);
        vvV[1] = anEdge->getMap1dTo3d()->firstDerUPercent(0.9);        
        vpV[1] = anEdge->getMap1dTo3d()->getPointPercent(0.9);
        vvV[2] 
        = 
        dtGmshModel::extractPosition(anEdge->getEndVertex())
        -
        dtGmshModel::extractPosition(anEdge->getBeginVertex());
        vpV[2] = anEdge->getMap1dTo3d()->getPointPercent(0.5);
        
        rV.push_back( new solid3dLine( ppV ) );
        rV.push_back( new discrete3dVector( vvV, vpV ) );
      }
    }
		return rV;
	}		
	
  std::vector< std::string > gmshBoundedVolume::getMeshTags( void ) const {
		//
		// update
		//
		updatePhysicals();
		
		std::vector< std::string > tags;
    for (int ii=0; ii<5; ii++) {
			dt__forAllIndex(_physLabels[ii], jj) tags.push_back(_physLabels[ii][jj]);
		}		
		return tags;		
	}
	
	dtGmshFace * gmshBoundedVolume::getFace( std::string const & tag ) const {
		//
		// update
		//
		updatePhysicals();
		
		::GModel::setCurrent( _gm );
		
		dt__forAllIndex(_physLabels[2], jj) {
			if (_physLabels[2][jj] == tag) {
				int pNum = _gm->getPhysicalNumber(2, tag);
				dtGmshModel::intGEntityVMap groups;
				_gm->getPhysicalGroups(2, groups);
				std::vector< ::GEntity * > & geV
				=
				groups[_gm->getPhysicalNumber(2, tag)];

				dt__forAllIter(std::vector< ::GEntity * >, geV, it) {
					::GEntity * ge = *it;
					dtGmshFace * gf = dtGmshFace::DownCast(ge);
					if (gf) {
						return gf;
					}
				}
			}
	  }
	}
	
	dtGmshRegion * gmshBoundedVolume::getRegion( std::string const & tag ) const {
		//
		// update
		//
		updatePhysicals();
		
		::GModel::setCurrent( _gm );
		
		dt__forAllIndex(_physLabels[3], jj) {
			if (_physLabels[3][jj] == tag) {
//				int pNum = _gm->getPhysicalNumber(3, tag);
				dtGmshModel::intGEntityVMap groups;
				_gm->getPhysicalGroups(3, groups);
				std::vector< ::GEntity * > & geV
				=
				groups[_gm->getPhysicalNumber(3, tag)];

				dt__forAllIter(std::vector< ::GEntity * >, geV, it) {
					::GEntity * ge = *it;
					dtGmshRegion * gr = dtGmshRegion::DownCast(ge);
					if (gr) {
						return gr;
					}
				}
			}
	  }
	}
	
  dtGmshModel * gmshBoundedVolume::getModel( void ) const {
		//
		// update
		//
		updatePhysicals();
		
		return  _gm ;
	}
	
	void gmshBoundedVolume::updatePhysicals( void ) const {
		::GModel::setCurrent( _gm );
    _physLabels.clear();
		_physLabels.resize(5);
    dt__forFromToIndex(0, 4, dim) {
      dtGmshModel::intGEntityVMap map;
      _gm->getPhysicalGroups(dim, map);
      dt__forAllRefAuto(map, aPair) {
        _physLabels[dim].push_back( _gm->getPhysicalName(dim, aPair.first) );
      }
		}
    
    _physLabels[4].push_back("gmsh");
	}

	void gmshBoundedVolume::makePreGrid(void) {    
		//
		// set current model
		//
		::GModel::setCurrent( _gm );
		
    //
    // parse gmsh file if option exists
    //
    if ( hasOption("gmshMeshFile") ) {
      dt__info(
        makePreGrid(),
        << "ParseFile( " << getOption("gmshMeshFile") << ", true, true );"
      );
      ParseFile( getOption("gmshMeshFile"), true, true );
    }

    if ( hasOption("gmshMeshStr") ) {
      dt__info(
        makePreGrid(),
        << "ParseString( " << getOption("gmshMeshStr") << " );"
      );
      ParseString( getOption("gmshMeshStr") );
    }

    
    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      dt__info(
        makePreGrid(),
        << "GmshSetOption(" 
        << oG[ii].first[0] 
        << ", " 
        << oG[ii].first[1] 
        << ", " 
        << oG[ii].second << ");"
      );
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }
		
    //
    // update bounding box
    //
    updateBoundingBox();
    
    //
		// destroy old mesh
		//
		_gm->deleteMesh();
		
		//
		// reset vertex and element numbering
		//
		_gm->setMaxVertexNumber(0); 
		_gm->setMaxElementNumber(0);
    
		bVOSubject::preNotify();
    
    //
    // set pre meshed
    //
    setPreMeshed();
	}  

  void gmshBoundedVolume::makeGrid(void) {
    if ( !isPreMeshed() ) makePreGrid();
    
    //
		// set current model
		//
		::GModel::setCurrent( _gm );
    
//    //
//    // meshing
//    //
//    if ( !optionHandling::optionTrue("defer_mesh_0") ) _gm->meshPhysical(0);
//    if ( !optionHandling::optionTrue("defer_mesh_1") ) _gm->meshPhysical(1);
//    if ( !optionHandling::optionTrue("defer_mesh_2") ) _gm->meshPhysical(2);
//    if ( !optionHandling::optionTrue("defer_mesh_3") ) _gm->meshPhysical(3);

    //
    // notify observers
    //
    boundedVolume::postNotify();
  }  

  void gmshBoundedVolume::updateBoundingBox( void ) {
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();
    
    dtPoint3 bbMin(
      CTX::instance()->min[0], CTX::instance()->min[1], CTX::instance()->min[2]
    );
    dtPoint3 bbMax(
      CTX::instance()->max[0], CTX::instance()->max[1], CTX::instance()->max[2]
    );    
    dt__info(makePreGrid(),
      << "Gmsh boundingBox:" << std::endl
      << logMe::dtFormat("min: (%d, %d, %d)")
        % CTX::instance()->min[0] 
        % CTX::instance()->min[1] 
        % CTX::instance()->min[2]
      << std::endl
      << logMe::dtFormat("max: (%d, %d, %d)")
        % CTX::instance()->max[0] 
        % CTX::instance()->max[1] 
        % CTX::instance()->max[2]
      << std::endl
      << "Gmsh fieldManager:" << std::endl
      << "size: " << _gm->getFields()->size()
      << std::endl
      << "meshRand * lcMin / (modelSize * machEpsilon) = "
      << 
        CTX::instance()->mesh.randFactor * CTX::instance()->mesh.lcMin
        /
        (
          dtLinearAlgebra::length( bbMax - bbMin) 
          * 
          std::numeric_limits<float>::epsilon()
        )
      << std::endl
      << "lc = " << CTX::instance()->lc
    );    
  }
}
