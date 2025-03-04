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

#include "dtPoint3_readCSV.h"
#include "progHelper.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <fstream>
#include <interfaceHeaven/stringPrimitive.h>
#include <iostream>
#include <logMe/logMe.h>
#include <sstream>

namespace dtOO {
dtPoint3_readCSV::dtPoint3_readCSV(std::string const &filename)
{
  //
  // read file
  //
  std::ifstream in(filename.c_str());
  std::vector<std::vector<double>> fields;
  if (in)
  {
    std::string line;
    while (getline(in, line))
    {
      boost::algorithm::trim_all(line);

      std::vector<std::string> parts;
      boost::split(
        parts, line, boost::is_any_of("\t ,"), boost::token_compress_on
      );

      dt__throwIf(parts.size() != 3, dtPoint3_readCSV());

      //
      // add new point
      //
      fields.push_back(std::vector<double>());
      dt__forAllIter(std::vector<std::string>, parts, it)
      {
        fields.back().push_back(stringPrimitive::stringToFloat(*it));
      }
    }
  }
  in.close();

  //
  // fill point vector
  //
  dt__forAllConstIter(std::vector<std::vector<double>>, fields, it)
  {
    _pp.push_back(dtPoint3(it->at(0), it->at(1), it->at(2)));
  }

  dt__info(
    dtPoint3_readCSV(),
    << "Read " << _pp.size() << " points from file " << filename << "."
  );
}

dtPoint3_readCSV::~dtPoint3_readCSV() {}

std::vector<dtPoint3> dtPoint3_readCSV::result(void) const { return _pp; }
} // namespace dtOO
