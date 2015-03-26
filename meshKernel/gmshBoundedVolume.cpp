#include "gmshBoundedVolume.h"

#include "dtXmlParserDecorator/dtXmlParserBase.h"
#include <unstructured3dSurfaceMesh.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <iostream>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <gmsh/Gmsh.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>

namespace dtOO {
	gmshBoundedVolume::gmshBoundedVolume() {
	  GmshInitialize();		
	}

	gmshBoundedVolume::~gmshBoundedVolume() {
	  GmshFinalize();		
	}

	void gmshBoundedVolume::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
		//
		// get compound and put pieces as regions to gmsh model
		//
		_gm.reset( new dtGmshModel() );

    ::GModel::setCurrent(_gm.get());		
	}	
	
	vectorHandling< renderInterface * > gmshBoundedVolume::getRender( void ) const {
		if (mustExtRender()) return vectorHandling< renderInterface * >(0);
		
		::GModel::setCurrent(_gm.get());
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = _gm->toUnstructured3dMesh();//_vertices, _elements);
		return rV;
	}	
	
	vectorHandling< renderInterface * > gmshBoundedVolume::getExtRender( void ) const {
		::GModel::setCurrent(_gm.get());
		
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
							std::vector< ::MVertex const * > vertices;
							_gm->getMeshVerticesForPhysicalGroup(ii, pNum, vertices);
							if (vertices.empty()) {
								vertices = gf->getMeshVertices();
							}
							rV.push_back(
							  dtGmshModel::toAdequateSurfaceRenderInterface(vertices, elTwoD)
							);
						}
						else if (gr) {
							std::vector< ::MElement const * > elThreeD;
							for (int jj=0;jj<gr->getNumMeshElements(); jj++) {
								elThreeD.push_back( gr->getMeshElement(jj) );	
							}							
							std::vector< ::MVertex const * > vertices;
							_gm->getMeshVerticesForPhysicalGroup(ii, pNum, vertices);
							rV.push_back(
							  dtGmshModel::toUnstructured3dMesh(vertices, elThreeD)
							);							
						}
						
					}
				}
			}
		}
		return rV;
	}		
	
  std::vector< std::string > gmshBoundedVolume::getMeshTags( void ) const {
		std::vector< std::string > tags;
    for (int ii=0; ii<4; ii++) {
			dt__forAllIndex(_physLabels[ii], jj) tags.push_back(_physLabels[ii][jj]);
		}		
		return tags;		
	}
	
	dtGmshFace const * gmshBoundedVolume::getFace( std::string const & tag ) const {
		::GModel::setCurrent(_gm.get());
		
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
	
	dtGmshRegion const * gmshBoundedVolume::getRegion( std::string const & tag ) const {
		::GModel::setCurrent(_gm.get());
		
		dt__forAllIndex(_physLabels[3], jj) {
			if (_physLabels[3][jj] == tag) {
				int pNum = _gm->getPhysicalNumber(3, tag);
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
		return _gm.get();
	}
	
	void gmshBoundedVolume::updatePhysicals( void ) {
		::GModel::setCurrent(_gm.get());
		
		dtGmshModel::intGEntityVMap groups;		
		_physLabels.resize(4);
		for (int ii=0; ii<4; ii++) {
			_physLabels[ii].clear();
			_gm->getPhysicalGroups(ii, groups);
			dt__forAllIter(dtGmshModel::intGEntityVMap, groups, it) {
				int num = it->first;
				std::string pName = _gm->getPhysicalName(ii, num);
				if (pName != "") _physLabels[ii].push_back(pName);
			}
		}
	}
}
