#ifndef IGESWRITER_H
#define	IGESWRITER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <logMe/dtMacros.h>

class SISLSurf;
class SISLCurve;

namespace dtOO {
  class analyticGeometry;
  
  /**
   * writes an IGES file
   * @author C. Brüggemann
   */
  class igesWriter {
    public:
      DTCLASSNAMEMETHOD(igesWriter);
      DTCLASSLOGMETHOD(igesWriter);      
      igesWriter();
      ~igesWriter();
      void InitFile(const char *name);	//initialise File, Start & Global Section

      void writeGeometry(const char *name);	//DE & Parameter Section
      void InitCurve(SISLCurve const * curve);	//curve in vector
      void InitSurf(SISLSurf const * surface);
      void InitAnalyticGeometry( analyticGeometry * aGeoP);
      void CloseFile(const char *name);	//Terminate Section & Close File

    private:
      std::vector < SISLCurve const * > _toWriteCurve;
      std::vector < SISLSurf const * > _toWriteSurf;
      std::vector < int > _SizeOfCurveDouble;
      std::vector < int > _SizeOfSurfDouble;
      std::vector < double > _CurveDataDouble;
      std::vector < double > _SurfDataDouble;
      std::vector < int > _CurveDataInt;
      std::vector < int > _SurfDataInt;
      int paraSequ;
      int DESequ;
  };
}
#endif	/* IGESWRITER_H */

