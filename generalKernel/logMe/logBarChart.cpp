#include "logBarChart.h"

#include <logMe/logMe.h>

namespace dtOO {  
  logBarChart::logBarChart(
    std::string const & title,
    float const & min, float const & max, int const & nBars 
  ) {
    _min = min;
    _max = max;
    _nBars = nBars;
    _bar.resize(nBars, 0);
    _globalMin = max;
    _globalMax = min;
    _step = (max - min) / nBars;
    _nValues = 0;
    _title = title;
  }

  logBarChart::~logBarChart() {
  }
  
  void logBarChart::operator()( float const & val ) {
    int location = (val - _min) / _step;
    
    location
    =
    std::min(
      std::max(0, location),
      _nBars-1
    );
    
    _bar[location] = _bar[location] + 1;
    
    _globalMax = std::max(_globalMax, val);
    _globalMin = std::min(_globalMin, val);
    
    _nValues++;    
  }
  
  float logBarChart::globalMin( void ) const {
    return _globalMin;  
  }
  
  float logBarChart::globalMax( void ) const {
    return _globalMax;
  }
  
  std::ostream& operator<<(std::ostream& os, const logBarChart& toLog) {
    std::vector< float > percent(toLog._bar.size(), 0.);
    std::vector< float > scale(toLog._bar.size(), 0.);
    
    float av = 0.;
    dt__forAllIndex(toLog._bar, ii) {
      percent[ii] = static_cast< float >(toLog._bar[ii]) / toLog._nValues;
      av = av + toLog._bar[ii] * .5 * (ii+1) * toLog._step;
    }
    av = av / toLog._nValues;
    
    float maxPercent = progHelper::max(percent);
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
    
    int avPos = av * 30 / (toLog._max-toLog._min);
    std::string avString(avPos, ' ');
    avString = avString+"#";
    os << logMe::dtFormat("| < %+5.2e  %-32s          |") 
      % av % avString << std::endl;         
    os << "|" << std::endl;
    dt__forAllIndex(scale, ii) {
      os << logMe::dtFormat("| < %+5.2e ") % (toLog._min + toLog._step * (ii+1));
      int nSigns = scale[ii] * 30;
      std::string barString(nSigns, '+');
      os 
      << logMe::dtFormat(" %-32s %+6.2f %% | %i ") 
           % barString.c_str()
           % (100. * percent[ii]) 
           % toLog._bar[ii];
      os << std::endl;
    }
    return os;
  }  
}