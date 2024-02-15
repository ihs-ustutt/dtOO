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

#include "dtPointD_readCSV.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dtOO {
	dtPointD_readCSV::dtPointD_readCSV(std::string const & filename) {
		std::ifstream in(filename.c_str());
		std::vector< std::vector< double > > fields;
		if (in) {
			std::string line;
      while ( getline(in, line) ) {
        boost::algorithm::trim_all(line);
        
        //
        // remove comments
        //
        if (stringPrimitive::stringContains("#", line) ) continue;

        std::vector< std::string > parts;
        boost::split(
          parts, line, boost::is_any_of("\t ,"), boost::token_compress_on
        );

        //
        // add new point
        //
        fields.push_back( std::vector< double >() );
        dt__forAllIter(std::vector< std::string >, parts, it) {
          fields.back().push_back( stringPrimitive::stringToFloat(*it) );
        }
      }
		}
		dt__forAllConstIter(std::vector< std::vector < double > >, fields, it) {
			_pp.push_back( dtPointD(it->size(), it->begin(), it->end()) );
		}		
		in.close();
	}

	dtPointD_readCSV::~dtPointD_readCSV() {
		
	}
	
	std::vector< dtPointD > dtPointD_readCSV::result(void) const {
		return _pp;
	}
}
