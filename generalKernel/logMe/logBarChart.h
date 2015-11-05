#ifndef LOGBARCHART_H
#define	LOGBARCHART_H

#include <logMe/dtMacros.h>

namespace dtOO {
  
  
  class logBarChart {
  public:
    dt__classOnlyName(logBarChart);    
    logBarChart(
      std::string const & title,
      float const & min, float const & max, int const & nBars             
    );
    virtual ~logBarChart();
    void operator()( float const & val );
    float globalMin( void ) const;
    float globalMax( void ) const;
  private:
    friend std::ostream& operator<<(std::ostream& os, const logBarChart& toLog);    
  private:
    float _min;
    float _max;
    int _nBars;
    float _globalMin;
    float _globalMax;
    std::vector< int > _bar;
    float _step;
    int _nValues;
    std::string _title;
  };
  
  std::ostream& operator<<(std::ostream& os, const logBarChart& toLog);
}

#endif	/* LOGBARCHART_H */

