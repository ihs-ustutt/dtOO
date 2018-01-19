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
