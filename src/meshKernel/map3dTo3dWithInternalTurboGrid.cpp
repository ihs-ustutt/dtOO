#include "map3dTo3dWithInternalTurboGrid.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <interfaceHeaven/systemHandling.h>
#include <iostream>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>
#include <gmsh.h>
#include <gmsh/MVertex.h>
#include <gmsh/MElement.h>
#include "boundedVolumeFactory.h"

namespace dtOO {
  bool map3dTo3dWithInternalTurboGrid::_registrated 
  =
  boundedVolumeFactory::registrate(
    dt__tmpPtr(
      map3dTo3dWithInternalTurboGrid, new map3dTo3dWithInternalTurboGrid()
    )
  );
  
	map3dTo3dWithInternalTurboGrid::map3dTo3dWithInternalTurboGrid(
  ) : gmshBoundedVolume() {
    
	}

	map3dTo3dWithInternalTurboGrid::~map3dTo3dWithInternalTurboGrid() {
	}
  
  void map3dTo3dWithInternalTurboGrid::init( 
    ::QDomElement const & element,
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV
	) {
    //
    // init boundedVolume
    //
    gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);

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
		
		std::vector<::QDomElement> eVec = qtXmlPrimitive::getChildVector(element);
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
					dt__throw(
					  init(), 
						<< dt__eval(label) << std::endl
						<< dt__eval(m2d) << std::endl
            << dt__eval(m3d) << std::endl									
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
		//
		// set current model
		//
		::GModel::setCurrent( _gm );
    _gm->clearModel();
		
		systemHandling::createDirectory(_directory);
		std::fstream of;
		std::string ofName;

		//
		// write hub curve
		//		
		dt__pH(map1dTo3d) hub( 
			_channel->segmentPercent(dtPoint3(0.,0.,0.), dtPoint3(0.,1.,0.)) 
		);		
		ofName = "./"+_directory+"/hub.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;
		for ( dtInt jj=0; jj<_nPoints;jj++) {			
			dtReal jjF = (dtReal) jj;
			dtReal nPointsJJF = (dtReal) _nPoints;                
			dtReal percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = hub->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();		
		
		//
		// write shroud curve
		//
		dt__pH(map1dTo3d) shroud( 
			_channel->segmentPercent(dtPoint3(0.,0.,1.), dtPoint3(0.,1.,1.)) 
		);		
		ofName = "./"+_directory+"/shroud.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;
		for ( dtInt jj=0; jj<_nPoints;jj++) {			
			dt__toFloat(dtReal jjF,  jj);
			dt__toFloat(dtReal nPointsJJF,  _nPoints);          
			dtReal percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = shroud->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();					
		
		//
		// write blade cut curves
		//
		ofName = "./"+_directory+"/blade.curve";				
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;
		for ( dtInt ii=0; ii<_nInternalCuts;ii++) {			
			dt__toFloat(dtReal iiF,  ii);
			dt__toFloat(dtReal nPointsIIF,  _nInternalCuts);               
			dtReal percentII = iiF * (1. / (nPointsIIF-1.) );  		
			of << "# percentII = " << percentII << std::endl;
			for ( dtInt jj=0; jj<_nPoints;jj++) {			
				dtReal jjF = (dtReal) jj;
				dtReal nPointsJJF = (dtReal) _nPoints;                
				dtReal percentJJ = jjF * (1. / (nPointsJJF-1) );  

				dtPoint3 pp = _internal->getPointPercent(percentJJ, percentII);
				of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
			}		
		}
	  of.close();		
		
		//
		// write bladeGen.inf
		//
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
		
		//
		// call script (turboGrid)
		//
		systemHandling::commandAndWait(_script);
		
		_gm->dtReadCGNS(_meshFileName.c_str());
		
		boundedVolume::postNotify();
		
		//
		// delete turboGrid directory
		//
		if ( !optionTrue("keep_directory") ) {
			systemHandling::deleteDirectory("./"+_directory);		
		}
		
		//
		// mark as meshed
		//
		boundedVolume::setMeshed();
	}
  
	/**
	 * @todo Calculate rotation angle or number of internals.
   */
	void map3dTo3dWithInternalTurboGrid::makePreGrid(void) {
		boundedVolume::preNotify();	
	}
}
