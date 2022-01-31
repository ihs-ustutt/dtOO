#include "barChart.h"

#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {  
  barChart::barChart(
    std::string const & title,
    dtReal const & min, dtReal const & max, dtInt const & nBars 
  ) {
    _min = min;
    _max = max;
    _nBars = nBars;
    _bar.resize(nBars, 0);
    _barValue.resize(nBars, 0.);
    _globalMin = max;
    _globalMax = min;
    _step = (max - min) / nBars;
    _nValues = 0;
    _title = title;
    _sum = 0.;
  }

  barChart::~barChart() {
  }
  
  void barChart::operator()( dtReal const & val ) {
    _sum = val + _sum;
    
    dtInt location = (val - _min) / _step;
    
    location
    =
    std::min(
      std::max(0, location),
      _nBars-1
    );
    
    _bar[location] = _bar[location] + 1;
    _barValue[location] = _barValue[location] + val;
    
    _globalMax = std::max(_globalMax, val);
    _globalMin = std::min(_globalMin, val);
    
    _nValues++;    
  }
  
  dtReal barChart::globalMin( void ) const {
    return _globalMin;  
  }
  
  dtReal barChart::globalMax( void ) const {
    return _globalMax;
  }
  
  dtInt barChart::nBars( void ) const {
    return _nBars;
  }
  
  dtReal barChart::barAverage( dtInt const & location ) const {
    if (_bar[location] != 0) {    
      return _barValue[location] / _bar[location];
    }

    return 0.;
  }
  
  dtReal barChart::average( void ) const {
    return _sum / _nValues;
  }
  
  std::ostream& operator<<(std::ostream& os, const barChart& toLog) {
    std::vector< dtReal > percent(toLog._bar.size(), 0.);
    std::vector< dtReal > scale(toLog._bar.size(), 0.);
    
    dtReal av = 0.;
    dt__forAllIndex(toLog._bar, ii) {
      percent[ii] = static_cast< dtReal >(toLog._bar[ii]) / toLog._nValues;
      av = av + toLog._bar[ii] * .5 * (ii+1) * toLog._step;
    }
    av = av / toLog._nValues;
    
    dtReal maxPercent = progHelper::max(percent);
    dt__forAllIndex(scale, ii) {
      scale[ii] = percent[ii] / maxPercent;
    }    
    
    os 
    << "|" << std::endl
    << "| " << toLog._title << std::endl
    << logMe::dtFormat("| nValues = %i") % toLog._nValues << std::endl    
    << logMe::dtFormat("| min       = %+5.2e, max       = %+5.2e") 
      % toLog._min % toLog._max <<  std::endl
    << logMe::dtFormat("| globalMin = %+5.2e, globalMax = %+5.2e") 
      % toLog._globalMin % toLog._globalMax << std::endl
    << logMe::dtFormat("| average   = %+5.2e") % av << std::endl      
    << "|" << std::endl;
    
    dtInt avPos = av * 30 / (toLog._max-toLog._min);
    std::string avString(avPos, ' ');
    avString = avString+"#";
    os << logMe::dtFormat("| < %+5.2e  %-32s          |") 
      % av % avString << std::endl;         
    os << "|" << std::endl;
    dt__forAllIndex(scale, ii) {
      os << logMe::dtFormat("| < %+5.2e ") % (toLog._min + toLog._step * (ii+1));
      dtInt nSigns = scale[ii] * 30;
      std::string barString(nSigns, '+');
      os 
      << logMe::dtFormat(" %-32s %+6.2f %% | %i ( %5.2e )") 
           % barString.c_str()
           % (100. * percent[ii]) 
           % toLog._bar[ii]
           % toLog.barAverage(ii);
      os << std::endl;
    }
    return os;
  }  
}
