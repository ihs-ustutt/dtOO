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

#include "dtMuParser.h"

#include "muParserFunctions.h"

namespace dtOO {
  dtMuParser::dtMuParser() : mu::Parser() {
    this->DefineFun("oneInRange", &muParserFunctions::oneInRange, false);
    this->DefineFun("YFix", &muParserFunctions::YFix, false);
    this->DefineFun("Y", &muParserFunctions::Y, false);
  }

  dtMuParser::dtMuParser(const dtMuParser& orig) {
  }

  dtMuParser::~dtMuParser() {
  }
}
