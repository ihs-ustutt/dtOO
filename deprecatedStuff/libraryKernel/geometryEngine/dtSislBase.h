#ifndef DTSISLBASE_H
#define	DTSISLBASE_H

#include <vector>
#include <string>
#include <logMe/dtMacros.h>
#include <deprecatedMacro.h>

namespace dtOO { 
  class dtSislBase {
  public:
    dt__CLASSNAME(dtSislBase);   
    dtSislBase();
    dtSislBase(const dtSislBase& orig);
    virtual ~dtSislBase();
    bool allRight( int const errFlag ) const;
    bool allRight( const int errFlag, std::string const file, int const line ) const;
    double & refToMinPointDistance( void );
    double const & refToMinPointDistance( void ) const;
    double & refToGeometricResolution( void );
    double const & refToGeometricResolution( void ) const;
  protected:
    void setErrStr( void );
    void setKindStr( void );
    static int const POLYB = 1;
    static int const RATB = 2;
    static int const POLYBEZIER = 3;
    static int const RATBEZIER = 4;
  private:
    std::vector< std::string > errStr;
    double _minPointDistance;
    double _geometricResolution;
  protected:
    std::vector< std::string > curveKindStr;
    std::vector< std::string > surfKindStr;    
    std::vector< std::string > intCurveKindStr;  
    static const int DIMTHREED = 3;
    static const int DIMTWOD = 2;
  };
}
#endif	/* DTSISLBASE_H */

