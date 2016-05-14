#include "dtMuParser.h"

#include "muParserFunctions.h"

namespace dtOO {
  dtMuParser::dtMuParser() : mu::Parser() {
    this->DefineFun("oneInRange", &muParserFunctions::oneInRange, false);
  }

  dtMuParser::dtMuParser(const dtMuParser& orig) {
  }

  dtMuParser::~dtMuParser() {
  }
}
