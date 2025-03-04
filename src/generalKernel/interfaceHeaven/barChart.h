/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef barChart_H
#define barChart_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
class barChart {
public:
  dt__classOnlyName(barChart);
  barChart(
    std::string const &title,
    dtReal const &min,
    dtReal const &max,
    dtInt const &nBars
  );
  virtual ~barChart();
  void operator()(dtReal const &val);
  dtReal globalMin(void) const;
  dtReal globalMax(void) const;
  dtInt nBars(void) const;
  dtReal barAverage(dtInt const &location) const;
  dtReal average(void) const;

private:
  friend std::ostream &operator<<(std::ostream &os, const barChart &toLog);

private:
  dtReal _min;
  dtReal _max;
  dtInt _nBars;
  dtReal _globalMin;
  dtReal _globalMax;
  std::vector<dtInt> _bar;
  std::vector<dtReal> _barValue;
  dtReal _step;
  dtInt _nValues;
  std::string _title;
  dtReal _sum;
};

std::ostream &operator<<(std::ostream &os, const barChart &toLog);
} // namespace dtOO

#endif /* barChart_H */
