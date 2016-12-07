#include "dtPoint3_readIBL.h"
#include "progHelper.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dtOO {
	dtPoint3_readIBL::dtPoint3_readIBL(std::string const & filename) {
		std::ifstream in(filename.c_str());
		std::vector< std::vector< double > > fields;
		if (in) {
			//
			// read file
			//
			std::string line;
			while (getline(in, line)) {
				//
				// find first "begin curve" occurence
				//
				if ( line.find("curve") == std::string::npos ) continue;
				while ( getline(in, line) ) {
					//
					// break loop if next "begin" found
					//
				  if ( line.find("begin") != std::string::npos ) break;
					::boost::algorithm::trim_all(line);
//					dt__info(dtPoint3_readIBL(), << "line = " << line);
					
          std::vector< std::string > parts;
          ::boost::split(
            parts, line, ::boost::is_any_of("\t "), ::boost::token_compress_on
          );
//					dt__info(
//						dtPoint3_readIBL(), 
//						<< "parts = " << parts << std::endl
//						<< dt__eval(parts.size())
//					);
					dt__throwIf(parts.size()!=3, dtPoint3_readIBL());
					//
					// add new point
					//
				  fields.push_back( std::vector< double >() );
					dt__forAllIter(std::vector< std::string >, parts, it) {
					  fields.back().push_back( stringPrimitive::stringToFloat(*it) );
					}
				}
			}
		}
		dt__forAllConstIter(std::vector< std::vector < double > >, fields, it) {
			_pp.push_back( dtPoint3(it->at(0), it->at(1), it->at(2)) );
		}		
		in.close();
	}

	dtPoint3_readIBL::~dtPoint3_readIBL() {
		
	}
	
	std::vector< dtPoint3 > dtPoint3_readIBL::result(void) const {
		return _pp;
	}
}
