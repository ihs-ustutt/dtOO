#include "map3dTo3dCompoundVolume.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <dtGmshRegion.h>
#include <dtGmshModel.h>
#include <dtGmshFace.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <gmsh/Context.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/Gmsh.h>
#include <gmsh/OpenFile.h>
#include <gmsh/GModel.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MHexahedron.h>

namespace dtOO {
	map3dTo3dCompoundVolume::map3dTo3dCompoundVolume() {
	  GmshInitialize();
	}

	map3dTo3dCompoundVolume::~map3dTo3dCompoundVolume() {
		GmshFinalize();
	}
	
  void map3dTo3dCompoundVolume::init( 
		QDomElement const & element,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,
		vectorHandling< boundedVolume * > const * const depBVolP
	) {
    //
    // init cardinalDirVolume
    //
    boundedVolume::init(element, cValP, sFunP, depAGeoP, depBVolP);
		
    //
    // region
    //
    addId("region", "dtVolume");
		
    QDomElement wElement = getChild("analyticGeometry", element);
    std::string label = getAttributeStr("label", wElement);
    analyticGeometry const * aG = depAGeoP->get(label);
		dt__PTRASS(
			map3dTo3d const * mm3d,
			map3dTo3d::ConstDownCast(aG)
		);
    boundedVolume::getRefToMap3dTo3dHandling()[rStrToId("dtVolume")]
		= 
		mm3d->clone();
		
//		analyticGeometryPointerCompound< map3dTo3d > * aGC;
//		= 
//		new analyticGeometryPointerCompound< map3dTo3d >(aG);
		
		DTINFOWF(
		  init(),
			<< DTLOGEVAL(map3dTo3d::ConstDownCast(aG)) << LOGDEL
			<< DTLOGEVAL( mm3d->isCompound() ) << LOGDEL
			<< DTLOGEVAL( mm3d->compoundInternal().size() )
		);
		
		vectorHandling< analyticGeometry const * > cI = mm3d->compoundInternal();
		
		_gm.reset( new dtGmshModel(getLabel()) );
		
		
		dt__FORALL(cI, ii,
		  _gm->addRegionToGmshModel(map3dTo3d::ConstSecureCast(cI[ii]));
		  _gm->getDtGmshRegionByTag(_gm->getNumRegions())->meshTransfinite();
		);
		
//	  _gm->writeGEO( (getLabel()+".geo").c_str() );	
	}
	
  void map3dTo3dCompoundVolume::makeGrid(void) {
    //
    // set options
    //      
    optionGroup oG = getOptionGroup("gmsh");      
    for (int ii=0;ii<oG.size();ii++) {
      GmshSetOption(oG[ii].first[0], oG[ii].first[1], oG[ii].second);
    }
		
    //
    // set a bounding box, necessary to set CTX::instance()->lc to prevent
    // very small elements
    //
    SetBoundingBox();
//		CTX::instance()->lc = 10.;
		
//		_gm->writeGEO( (getLabel()+".geo").c_str() );
		
//		_gm->mesh(3);
    _gm->meshVertex();
		
    for (int ii=1; ii<=_gm->getNumRegions(); ii++) {
		  _gm->meshRegion( ii );
			_gm->writeMSH( (getLabel()+"_building.msh").c_str() );
		}
		_gm->writeMSH( (getLabel()+".msh").c_str() );
		
	}
  
	void map3dTo3dCompoundVolume::makePreGrid(void) {
		
	}
  
	vectorHandling< renderInterface * > map3dTo3dCompoundVolume::getRender( void ) const {
		unstructured3dMesh * um = new unstructured3dMesh();
		
//    std::vector< unsigned > numElements(5,0);
//    /*
//     * cast away constness by macro, because getNumMeshElements 
//     * is not a const routine
//     */
//    _gm->getNumMeshElements( &(numElements[0]) );
//    int nElemTot = numElements[0] + numElements[1] + numElements[2] + numElements[3] + numElements[4];

//    DTINFOWF(getRender(),
//      << "tetrahedra = " << numElements[0] << LOGDEL
//      << "hexahedra = " << numElements[1] << LOGDEL
//      << "prisms = " << numElements[2] << LOGDEL
//      << "pyramids = " << numElements[3] << LOGDEL
//      << "polyhedra = " << numElements[4] << LOGDEL
//    //  << "quadrangle = " << nElemQuad
//    );
    
      for( int ii=0; ii<_gm->getNumMeshVertices(); ii++ ) {
        MVertex const * const mv = _gm->getMeshVertexByTag(ii+1);
				um->addPoint( 
				  dtPoint3(
				    static_cast< float >(mv->x()), 
					  static_cast< float >(mv->y()), 
						static_cast< float >(mv->z())
				  )
				);
      }
		
	  vectorHandling< renderInterface * > rV;
		rV.push_back(um);
		
		return rV;
	}	
}
