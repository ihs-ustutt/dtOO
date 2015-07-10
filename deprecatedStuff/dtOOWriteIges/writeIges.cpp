#include "writeIges.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <do/coDoSet.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <cadToolHeaven/igesWriter.h>
#include <QtXml/QDomElement>

#define SUCCESS 1;
#define FAILURE 0;

namespace dtOO {
  DTCLASSLOGMETHODI(writeIges,
          << "No member attributes.");
  
  writeIges::writeIges(int argc, char *argv[]) : covise::coModule(argc, argv, "writeIges") {
    
    _pIn_anGeometryP = addInputPort("anGeometryP", "USR_AnGeo", "_p_anGeometryDescription");
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");    
    _p_igesFilebrowser = addFileBrowserParam("igesFilebrowser", "igesFilebrowserDescription");
    _p_igesFilebrowser->setValue(".", "*.iges/*");
    
  }

  writeIges::~writeIges() {
    _aGeo.destroy();
  }
  
  int writeIges::compute(const char * port) {
    try {
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
      
      //
      // recreate analyticGeometries
      //
      const covise::coDistributedObject * inObj = _pIn_anGeometryP->getCurrentObject();
      covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
      _aGeo.destroy();
      abstractModule::recreateAnalyticGeometries(set, _aGeo);

      //
      // parse xml file
      //
      dtXmlParser parser;
      parser.openFileAndParse( _p_xmlFilebrowser->getValue() );

      dt__info(writeIges(),
              << "Writing Iges file to " << _p_igesFilebrowser->getValue() );
      igesWriter aWriter;

      //
      // init file
      //
      aWriter.InitFile( _p_igesFilebrowser->getValue() );
      
      ::QDomElement wElement;
      wElement = parser.getElement("igesWriter");
      wElement = parser.getChild("analyticGeometry", wElement);
      while ( !wElement.isNull() ) {
				aWriter.InitAnalyticGeometry( 
					_aGeo.get(
						parser.getAttributeStr("label", wElement)
					) 
				);
        
				//goto next sibling
        wElement = parser.getNextSibling("analyticGeometry", wElement);
      }

      //
      // write geometry
      //
      aWriter.writeGeometry( _p_igesFilebrowser->getValue() );

      //
      // close file
      //
      aWriter.CloseFile( _p_igesFilebrowser->getValue() );
    }
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }
  }
}
MODULE_MAIN(designTool, dtOO::writeIges)