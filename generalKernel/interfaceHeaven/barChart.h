#ifndef barChart_H
#define	barChart_H

#include <logMe/dtMacros.h>

namespace dtOO {
  
  
  class barChart {
  public:
    dt__classOnlyName(barChart);    
    barChart(
      std::string const & title,
      float const & min, float const & max, int const & nBars             
    );
    virtual ~barChart();
    void operator()( float const & val );
    float globalMin( void ) const;
    float globalMax( void ) const;
    int nBars( void ) const;
    float barAverage( int const & location ) const;
  private:
    friend std::ostream& operator<<(std::ostream& os, const barChart& toLog);    
  private:
    float _min;
    float _max;
    int _nBars;
    float _globalMin;
    float _globalMax;
    std::vector< int > _bar;
    std::vector< float > _barValue;
    float _step;
    int _nValues;
    std::string _title;
  };
  
  std::ostream& operator<<(std::ostream& os, const barChart& toLog);
}

#endif	/* barChart_H */

