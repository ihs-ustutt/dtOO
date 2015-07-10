#include "aGeoToXYZ.h"
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
//#include <math.h>

#define SUCCESS 1;
#define FAILURE 0;

namespace dtOO {
  aGeoToXYZ::aGeoToXYZ(int argc, char *argv[]) : coModule(argc, argv, "aGeoToXYZ") {
    _pIn_aGeoP = addInputPort("_pIn_aGeoP", "USR_AnGeo", "_pIn_aGeoPDescription");
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");    
    _p_xyzFilebrowser = addFileBrowserParam("xyzFilebrowser", "xyzFilebrowserDescription");
    _p_xyzFilebrowser->setValue(".", "*.dat/*");    
  }

  aGeoToXYZ::~aGeoToXYZ() {
  }
  
  int aGeoToXYZ::compute(char const * str) {
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      //
      // recreate scaFunctions
      //
      _aGeo.destroy();
      if ( _pIn_aGeoP->isConnected() ) {
        const covise::coDistributedObject * inObj = _pIn_aGeoP->getCurrentObject();
        covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
        abstractModule::recreateAnalyticGeometries(set, _aGeo);
      }
    
      //
      // parse xml file
      //
      dtXmlParser parser;
      parser.openFileAndParse( _p_xmlFilebrowser->getValue() );
      
      //
      // get information
      //
      ::QDomElement wElement;
      wElement = parser.getElement("aGeoToXYZ");

      //
      // open output file
      //
      std::fstream of;
      std::string ofName = _p_xyzFilebrowser->getValue();
      
      of.open(ofName.c_str(), std::ios::out | std::ios::trunc);	
      of.precision(8);
      of.fixed;        
      of << "Closed Index Arclength" << std::endl;
      
      dt__info(compute(),
              << "writing " << ofName 
              );        

        
      wElement = parser.getChild("analyticGeometry", wElement);          
      
      while ( !wElement.isNull() ) {
        
//        for (int jj=0;jj<offset;jj++) {
//          if ( parser.is("analyticGeometry", wElement) ) {
            std::string aGLabel = parser.getAttributeStr("label", wElement);
            analyticGeometry * aG = _aGeo.get(aGLabel);

            int nPointsII = parser.getAttributeInt("number_points_one", wElement);
            int nPointsJJ = parser.getAttributeInt("number_points_two", wElement);
            for ( int jj=0;jj<nPointsJJ;jj++) {

              of << "begin section ! " << jj << std::endl;
              of << "begin curve" << std::endl;
        
              float jjF = (float) jj;
              float nPointsJJF = (float) nPointsJJ;                
              float percentJJ = jjF * (1. / (nPointsJJF-1) );              
            
                
              for ( int ii=0;ii<nPointsII;ii++) {
                float iiF = (float) ii;
                float nPointsIIF = (float) nPointsII;                
                float percentII = iiF * (1. / (nPointsIIF-1) );                              

                  map2dTo3d * map;
                  dt__mustCast(aG, map2dTo3d, map);
                  dtPoint3 point = map->getPointPercent(percentII, percentJJ);
                  of << point.x() << " " << point.y() << " " << point.z() << std::endl;
              }
            }
            //goto next sibling
            wElement = parser.getNextSibling("analyticGeometry", wElement);
//        }
        
        //
        // close file
        //
        of.close();
      }
      
      
      return SUCCESS;
    }
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }    
  }
}
MODULE_MAIN(designTool, dtOO::aGeoToXYZ)