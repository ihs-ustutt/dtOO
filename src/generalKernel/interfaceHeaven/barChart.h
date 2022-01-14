#ifndef barChart_H
#define	barChart_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
  class barChart {
    public:
      dt__classOnlyName(barChart);    
      barChart(
        std::string const & title,
        dtReal const & min, dtReal const & max, int const & nBars             
      );
      virtual ~barChart();
      void operator()( dtReal const & val );
      dtReal globalMin( void ) const;
      dtReal globalMax( void ) const;
      int nBars( void ) const;
      dtReal barAverage( int const & location ) const;
      dtReal average( void ) const;
    private:
      friend std::ostream& operator<<(std::ostream& os, const barChart& toLog);    
    private:
      dtReal _min;
      dtReal _max;
      int _nBars;
      dtReal _globalMin;
      dtReal _globalMax;
      std::vector< int > _bar;
      std::vector< dtReal > _barValue;
      dtReal _step;
      int _nValues;
      std::string _title;
      dtReal _sum;
  };
  
  std::ostream& operator<<(std::ostream& os, const barChart& toLog);
}

#endif	/* barChart_H */

