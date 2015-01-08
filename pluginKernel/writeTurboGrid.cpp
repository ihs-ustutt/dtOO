#include "writeTurboGrid.h"

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
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
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtPlugin * > const * const pL
	) {
	  dtPlugin::init(element, cV, aF, aG, bV, pL);
		
		_dir = dtXmlParserBase::getAttributeStr("directory", element);
		_hub.reset( aG->get(dtXmlParserBase::getAttributeStr("hub", element))->clone() );
		_shroud.reset( aG->get(dtXmlParserBase::getAttributeStr("shroud", element))->clone() );
		_blade.reset( aG->get(dtXmlParserBase::getAttributeStr("blade", element))->clone() );
		_nBladeCuts = 3;
	}
		
  void writeTurboGrid::apply(void) {
		systemHandling::createDirectory(_dir);
//		for (int ii=0; ii<_aG.size(); ii++) {
		int nPoints = 100;
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

		for ( int jj=0; jj<nPoints;jj++) {			
			float jjF = (float) jj;
			float nPointsJJF = (float) nPoints;                
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

		for ( int jj=0; jj<nPoints;jj++) {			
			float jjF = (float) jj;
			float nPointsJJF = (float) nPoints;                
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
			float iiF = (float) ii;
			float nPointsIIF = (float) _nBladeCuts;                
			float percentII = iiF * (1. / (nPointsIIF-1.) );  		
			of << "# percentII = " << percentII << std::endl;
			for ( int jj=0; jj<nPoints;jj++) {			
				float jjF = (float) jj;
				float nPointsJJF = (float) nPoints;                
				float percentJJ = jjF * (1. / (nPointsJJF-1) );  

				dtPoint3 pp = blade->getPointPercent(percentJJ, percentII);
				of << pp.x() << " " << pp.y() << " " << pp.z() << std::endl;
			}		
		}
	  of.close();					
  }
}


