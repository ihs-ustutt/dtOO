#include "map3dTo3dWithInternalTurboGrid.h"
#include "dtXmlParserDecorator/dtXmlParserBase.h"
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <unstructured3dMesh.h>
#include <discrete3dPoints.h>

#include <moab/Core.hpp>
#include <moab/CN.hpp>
#include <iostream>

namespace dtOO {
	map3dTo3dWithInternalTurboGrid::map3dTo3dWithInternalTurboGrid() {
  _mb.reset( new moab::Core() );
	}

	map3dTo3dWithInternalTurboGrid::~map3dTo3dWithInternalTurboGrid() {
	}
	
  void map3dTo3dWithInternalTurboGrid::init( 
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

		_meshFileName = getOption("mesh_file");		
		_directory = qtXmlPrimitive::getAttributeStr("directory", element);
		_script = qtXmlPrimitive::getAttributeStr("run_script", element);
		_nInternalCuts
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_internal_cuts", element, cV, aF);
		_nInternals
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_internals", element, cV, aF);		
		_nPoints
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_points", element, cV, aF);
		
		std::vector<QDomElement> eVec = qtXmlPrimitive::getChildVector(element);
		for ( auto &ii : eVec ) {
		  //
			// handle analyticGeometries
			//
		  if (qtXmlPrimitive::is("analyticGeometry", ii)) {
//				std::string pos = dtXmlParserBase::getAttributeStr("position", ii);
				std::string label = qtXmlPrimitive::getAttributeStr("label", ii);
				map3dTo3d const * m3d = map3dTo3d::ConstDownCast( aG->get(label) );
				map2dTo3d const * m2d = map2dTo3d::ConstDownCast( aG->get(label) );
				
				if (m3d) {
					_channel.reset( m3d->clone() );
				}
				else if (m2d) {
					_internal.reset( m2d->clone() );
				}
				else {
					dt__THROW(
					  init(), 
						<< DTLOGEVAL(label) << LOGDEL
						<< DTLOGEVAL(m2d) << LOGDEL
            << DTLOGEVAL(m3d) << LOGDEL									
						<< "Unexpected type of part."
					);
				}
			}
		  //
			// handle vector
			//		
			else if (qtXmlPrimitive::is("Vector_3", ii)) {
				_vv = dtXmlParserBase::getDtVector3(&ii, bC, cV, aF, aG);
			}
		}		
		
	}
	
  void map3dTo3dWithInternalTurboGrid::makeGrid(void) {
		dt__THROW_IF(_mb == NULL, makeGrid());

		// Load the mesh from vtk file
		moab::ErrorCode rval = _mb->load_mesh(_meshFileName.c_str());
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
	}
  
	/**
	 * @todo Calculate rotation angle or number of internals.
   */
	void map3dTo3dWithInternalTurboGrid::makePreGrid(void) {
		boundedVolume::notify();	
		
		systemHandling::createDirectory(_directory);
		std::fstream of;
		std::string ofName;

		ofName = "./"+_directory+"/hub.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;
		dt__pH(map1dTo3d) hub( 
			_channel->segmentPercent(dtPoint3(0.,0.,0.), dtPoint3(0.,1.,0.)) 
		);

		for ( int jj=0; jj<_nPoints;jj++) {			
			float jjF = (float) jj;
			float nPointsJJF = (float) _nPoints;                
			float percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = hub->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();		
		
		ofName = "./"+_directory+"/shroud.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;

		dt__pH(map1dTo3d) shroud( 
			_channel->segmentPercent(dtPoint3(0.,0.,1.), dtPoint3(0.,1.,1.)) 
		);

		for ( int jj=0; jj<_nPoints;jj++) {			
			dt__TOFLOAT(float jjF,  jj);
			dt__TOFLOAT(float nPointsJJF,  _nPoints);          
			float percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = shroud->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();					
		
		ofName = "./"+_directory+"/blade.curve";				
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;

//		dt__PTRASS(
//			map2dTo3d const * const blade, 
//			map2dTo3d::ConstDownCast(_blade.get())
//		);
		
		for ( int ii=0; ii<_nInternalCuts;ii++) {			
			dt__TOFLOAT(float iiF,  ii);
			dt__TOFLOAT(float nPointsIIF,  _nInternalCuts);               
			float percentII = iiF * (1. / (nPointsIIF-1.) );  		
			of << "# percentII = " << percentII << std::endl;
			for ( int jj=0; jj<_nPoints;jj++) {			
				float jjF = (float) jj;
				float nPointsJJF = (float) _nPoints;                
				float percentJJ = jjF * (1. / (nPointsJJF-1) );  

				dtPoint3 pp = _internal->getPointPercent(percentJJ, percentII);
				of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
			}		
		}
	  of.close();		
		
		ofName = "./"+_directory+"/bladeGen.inf";				
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		
    of << "!======  CFX-BladeGen Export  ========" << std::endl;
    of << "Axis of Rotation: " << dtLinearAlgebra::directionString(_vv) << std::endl;
    of << "Number of Blade Sets: " << _nInternals << std::endl;
    of << "Geometry Units: MM" << std::endl;
    of << "Base Units: M" << std::endl;
    of << "Hub Data File: hub.curve" << std::endl;
    of << "Shroud Data File: shroud.curve" << std::endl;
    of << "Profile Data File: blade.curve" << std::endl;
		if (!_internal->isClosedU()) of << "Blade 0 TE: CutOffEnd" << std::endl;
		
		of.close();
		
		systemHandling::commandAndWait(_script);
	}
  
	vectorHandling< renderInterface * > map3dTo3dWithInternalTurboGrid::getRender( void ) const {
		vectorHandling< renderInterface * > rV(1);
		
		moab::ErrorCode rval;
		
		moab::Range verts;
		rval = _mb->get_entities_by_type(0, moab::MBVERTEX, verts);
		
		std::vector<double> xx(verts.size());
		std::vector<double> yy(verts.size());
		std::vector<double> zz(verts.size());
		rval = _mb->get_coords(verts, &xx[0], &yy[0], &zz[0]);
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
		
		unstructured3dMesh * um = new unstructured3dMesh();
		rV[0] = um;
		um->addPoints(
		  vectorHandling<dtPoint3>(dtLinearAlgebra::toDtPoint3Vector(xx, yy, zz))
		);
		
	  moab::Range hex;
    rval = _mb->get_entities_by_type(0, moab::MBHEX, hex);
		dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());
		
		for (moab::Range::iterator it = hex.begin(); it != hex.end(); it++) {
			moab::EntityHandle const * conn;
			int nNodes;
			rval = _mb->get_connectivity(*it, conn, nNodes);
			dt__THROW_IF(rval != moab::MB_SUCCESS, makeGrid());

//   		DTINFOWF(
//				makeGrid(), 
//				<< DTLOGEVAL(nNodes) << LOGDEL
//				<< DTLOGEVAL(conn) << LOGDEL
//			  << moab::CN::EntityTypeName(_mb->type_from_handle(*it)) << " " 
//				<< _mb->id_from_handle(*it) << " hex connectivity is: "
//			);
			vectorHandling< int > tmp(nNodes);
      for (int i = 0; i < nNodes; i++) tmp[i] = conn[i]-1;
			um->addElement(tmp);
    }
	
		
		return rV;
	}	
}