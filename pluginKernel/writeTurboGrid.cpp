#include "writeTurboGrid.h"
#include <dtXmlParserDecorator/dtXmlParser.h>

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>

#include <interfaceHeaven/systemHandling.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {  
  writeTurboGrid::writeTurboGrid() { 
  }

  writeTurboGrid::~writeTurboGrid() {
  }

	void writeTurboGrid::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtPlugin * > const * const pL
	) {
	  dtPlugin::init(element, bC, cV, aF, aG, bV, pL);
		
		_dir = dtXmlParserBase::getAttributeStr("directory", element);
		_scr = dtXmlParserBase::getAttributeStr("run_script", element);
		_nBladeCuts 
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_blade_cuts", element, cV, aF);
		_nBlades
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_blades", element, cV, aF);		
		_nPoints
		= 
		dtXmlParserBase::getAttributeIntMuParse("number_points", element, cV, aF);		
		
		std::vector<QDomElement> eVec = dtXmlParserBase::getChildVector(element);
		for ( auto &ii : eVec ) {
		  //
			// handle analyticGeometries
			//
		  if (dtXmlParserBase::is("analyticGeometry", ii)) {
				std::string pos = dtXmlParserBase::getAttributeStr("position", ii);
				std::string label = dtXmlParserBase::getAttributeStr("label", ii);
				if (pos == "hub") {
					_hub.reset(aG->get(label)->clone());
				}
				else if (pos == "shroud") {
					_shroud.reset(aG->get(label)->clone());
				}
				else if (pos == "blade") {
					_blade.reset(aG->get(label)->clone());
				}
				else {
					dt__THROW(
					  init(), 
						<< DTLOGEVAL(pos) << LOGDEL
						<< "Unknown position"
					);
				}
			}
		  //
			// handle vector
			//		
			else if (dtXmlParserBase::is("Vector_3", ii)) {
				_vv = dtXmlParserBase::getDtVector3(&ii, bC, cV, aF, aG);
			}
		}
		
//		_hub.reset( aG->get(dtXmlParserBase::getAttributeStr("hub", element))->clone() );
//		_shroud.reset( aG->get(dtXmlParserBase::getAttributeStr("shroud", element))->clone() );
//		_blade.reset( aG->get(dtXmlParserBase::getAttributeStr("blade", element))->clone() );
//		_nBladeCuts = 3;
	}
		
  void writeTurboGrid::apply(void) {
		systemHandling::createDirectory(_dir);
//		for (int ii=0; ii<_aG.size(); ii++) {
//		int nPoints = 100;
		std::fstream of;
		std::string ofName;

		ofName = "./"+_dir+"/hub.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;

		dt__PTRASS(
			map1dTo3d const * const hub, 
			map1dTo3d::ConstDownCast(_hub.get())
		);

		for ( int jj=0; jj<_nPoints;jj++) {			
			float jjF = (float) jj;
			float nPointsJJF = (float) _nPoints;                
			float percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = hub->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();		
		
		ofName = "./"+_dir+"/shroud.curve";			
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;

		dt__PTRASS(
			map1dTo3d const * const shroud, 
			map1dTo3d::ConstDownCast(_shroud.get())
		);

		for ( int jj=0; jj<_nPoints;jj++) {			
			dt__TOFLOAT(float jjF,  jj);
			dt__TOFLOAT(float nPointsJJF,  _nPoints);          
			float percentJJ = jjF * (1. / (nPointsJJF-1) );  

      dtPoint3 pp = shroud->getPointPercent(percentJJ);
      of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
		}
	  of.close();					
		
		ofName = "./"+_dir+"/blade.curve";				
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		of.precision(8);
		of.fixed;

		dt__PTRASS(
			map2dTo3d const * const blade, 
			map2dTo3d::ConstDownCast(_blade.get())
		);
		
		for ( int ii=0; ii<_nBladeCuts;ii++) {			
			dt__TOFLOAT(float iiF,  ii);
			dt__TOFLOAT(float nPointsIIF,  _nBladeCuts);               
			float percentII = iiF * (1. / (nPointsIIF-1.) );  		
			of << "# percentII = " << percentII << std::endl;
			for ( int jj=0; jj<_nPoints;jj++) {			
				float jjF = (float) jj;
				float nPointsJJF = (float) _nPoints;                
				float percentJJ = jjF * (1. / (nPointsJJF-1) );  

				dtPoint3 pp = blade->getPointPercent(percentJJ, percentII);
				of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
			}		
		}
	  of.close();		

		ofName = "./"+_dir+"/bladeGen.inf";				
		of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
		
    of << "!======  CFX-BladeGen Export  ========" << std::endl;
    of << "Axis of Rotation: " << dtLinearAlgebra::directionString(_vv) << std::endl;
    of << "Number of Blade Sets: " << _nBlades << std::endl;
    of << "Geometry Units: MM" << std::endl;
    of << "Base Units: M" << std::endl;
    of << "Hub Data File: hub.curve" << std::endl;
    of << "Shroud Data File: shroud.curve" << std::endl;
    of << "Profile Data File: blade.curve" << std::endl;
		if (!blade->isClosedU()) of << "Blade 0 TE: CutOffEnd" << std::endl;
		
		of.close();
		
		systemHandling::commandAndWait(_scr);
  }
}


