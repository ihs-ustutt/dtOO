#ifndef STLWRITER_H
#define	STLWRITER_H

#include <logMe/dtMacros.h>
#include <vector>
//class SISLSurf;

//#include <geometryEngine/dtSislSurf.h>

namespace dtOO {
    
  class dtSislSurf;
  
  class stlWriter {
  public:
      DTCLASSNAMEMETHOD(stlWriter);
      DTCLASSLOGMETHOD(stlWriter); 
      stlWriter();
      stlWriter(const stlWriter& orig);
      virtual ~stlWriter();
      
//      void initFile(const char * filename);
      void writeStlFile( const char * filename, std::vector<dtSislSurf *> surface );
      void writeSurf(dtSislSurf const * surface);
//      void closeFile(const char * filename);
  private:
//      std::vector < SISLSurf const * > _toWriteSurf;

  };

}

#endif	/* STLWRITER_H */

