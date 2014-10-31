#include "writeXy.h"

#define SUCCESS 1;
#define FAILURE 0;

#include <functionHeaven/scaFunction.h>
#include <functionHeaven/scaCurve2dOneD.h>
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <math.h>

namespace dtOO {
  writeXy::writeXy(int argc, char *argv[]) : coModule(argc, argv, "writeXy") {
    _pIn_sFunP = addInputPort("_pIn_sFunP", "USR_ScaFun", "_pIn_sFunPDescription");
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");    
    _p_xyFilebrowser = addFileBrowserParam("xyFilebrowser", "xyFilebrowserDescription");
    _p_xyFilebrowser->setValue(".", "*.dat/*");    
  }

  writeXy::~writeXy() {
  }
  
  int writeXy::compute(char const * str) {
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      //
      // recreate scaFunctions
      //
      _sFun.destroy();
      if ( _pIn_sFunP->isConnected() ) {
        const covise::coDistributedObject * inObj = _pIn_sFunP->getCurrentObject();
        covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
        abstractModule::recreateScaFunctions(set, _sFun);
      }
    
      //
      // parse xml file
      //
      dtXmlParser parser;
      parser.openFileAndParse( _p_xmlFilebrowser->getValue() );
      
      //
      // get information
      //
      QDomElement wElement;
      wElement = parser.getElement("xyWriter");
      int offset = 1;
      if ( parser.hasAttribute("scaFunction_offset", wElement) ) {
        offset = parser.getAttributeInt("scaFunction_offset", wElement);
      }
      std::string filename = parser.getAttributeStr("filename", wElement);
      int filenameCounter = 0;
      bool rotateFirstToXAndNormalize = parser.getAttributeBool("rotate_first_to_x_and_normalize", wElement);
      
      wElement = parser.getChild("scaFunction", wElement);          
      
      while ( !wElement.isNull() ) {

        //
        // open output file
        //
        std::fstream of;
        of.open(
          (filename+parser.intToStringLZ(filenameCounter, 2)+".dat").c_str(), 
          std::ios::out | std::ios::trunc
        );	
        filenameCounter++;
        of.precision(8);
        of.fixed;        

        DTINFOWF(compute(),
                << "writing " <<  filename+parser.intToStringLZ(filenameCounter, 2)+".dat" 
                );        
        
        for (int jj=0;jj<offset;jj++) {
          if ( parser.is("scaFunction", wElement) ) {
            std::string sFLabel = parser.getAttributeStr("label", wElement);
            dt__PTRASS(
							scaOneD const * const sF,
							scaOneD::ConstDownCast( _sFun.get(sFLabel) )
					  );

            int nPoints = parser.getAttributeInt("number_points", wElement);

            //
            // reduce loop
            //
            int iiLoop = nPoints;
            if ( parser.getAttributeBool("ignore_last", wElement) ) {
              iiLoop--;
            }         

            if ( !rotateFirstToXAndNormalize ) {
              for ( int ii=0;ii<iiLoop;ii++) {
                float iiF = (float) ii;
                float nPointsF = (float) nPoints;

                float percent = iiF * (1. / (nPointsF-1) );              
                if ( parser.getAttributeBool("revert", wElement) ) {
                  percent = 1. - percent;
                }     
                std::vector< float > pp = sF->getVectorValuePercent(percent);
                of << pp[0] << " " << pp[1] << std::endl;
              }
              
              scaSISLSplineTwoD const * sSs;
              dt__CANDOWNCAST(sF, scaSISLSplineTwoD const, sSs);
              if (sSs) {
                
                std::fstream ofcp;
                ofcp.open(
                  (filename+parser.intToStringLZ(filenameCounter-1, 2)+"_controlPoints.dat").c_str(), 
                  std::ios::out | std::ios::trunc
                );	
                ofcp.precision(8);
                
                std::vector< dtPoint2 > cP = sSs->getControlPoints();
                dt__FORALL(cP, ii,
                  ofcp << cP[ii].x() << " " << cP[ii].y() << std::endl;
                );
                ofcp.close();
              }
            }
            else {
              std::vector< float > ppNorm;
//                if ( parser.getAttributeBool("revert", wElement) ) {
                  ppNorm = sF->getVectorValuePercent(1.);
//                }     
//                else {
//                  ppNorm = sF->getVectorValuePercent(0.);
//                }
              float ppNormLength = sqrt(ppNorm[0] * ppNorm[0] + ppNorm[1] * ppNorm[1]);
              
              float alpha = atan(ppNorm[1]/ppNorm[0]);
              if (alpha < 0.) {
                alpha = -(M_PI + alpha);
              }
              std::fstream ofTwo;
              ofTwo.open(
                (filename+parser.intToStringLZ(filenameCounter-1, 2)+"_rotInfo.dat").c_str(),
                std::ios::out | std::ios::trunc
              );
              ofTwo.precision(8);
              ofTwo.fixed;
              ofTwo << "#ppNormLength" << std::endl;
              ofTwo << ppNormLength << std::endl;
              ofTwo << "#rotAngle (rad)" << std::endl;
              ofTwo << alpha << std::endl;
              ofTwo << "#rotAngle (deg)" << std::endl;
              ofTwo << alpha*180./M_PI << std::endl;    
              ofTwo.close();
              
                for ( int ii=0;ii<iiLoop;ii++) {
                  float iiF = (float) ii;
                  float nPointsF = (float) nPoints;

                  float percent = iiF * (1. / (nPointsF-1) );              
                  if ( parser.getAttributeBool("revert", wElement) ) {
                    percent = 1. - percent;
                  }     
                  std::vector< float > pp = sF->getVectorValuePercent(percent);
                  std::vector< float > tmp(2,0.);
                  tmp[0] = cos(alpha) * pp[0] - sin(alpha) * pp[1];
                  tmp[1] = sin(alpha) * pp[0] + cos(alpha) * pp[1];
                  pp[0] = tmp[0];
                  pp[1] = tmp[1];
                  
                  pp[0] = pp[0] / ppNormLength;
                  pp[1] = pp[1] / ppNormLength;
                  of << pp[0] << " " << -pp[1] << std::endl;
                }
            }


          }
          //goto next sibling
          wElement = parser.getNextSibling("scaFunction", wElement);
        }
        
        of.close();
      }
      
      
      return SUCCESS;
    }
    catch (eGeneral & eGenRef) {
      DTCATCHERRORWF(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }    
  }
}
MODULE_MAIN(designTool, dtOO::writeXy)