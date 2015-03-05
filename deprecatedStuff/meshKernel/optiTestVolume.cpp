#include "optiTestVolume.h"

#include <analyticFunctionHeaven/constValue.h>
#include <logMe/logMe.h>

namespace dtOO {
  DTCLASSLOGMETHODI(optiTestVolume,
          << "No member attributes.");
  
  optiTestVolume::optiTestVolume() {
    _val.resize(_nVal);
    _valCValP.resize(_nVal); 
  }

  optiTestVolume::optiTestVolume(const optiTestVolume& orig) {
  }

  optiTestVolume::~optiTestVolume() {
    _val.clear();
    _valCValP.clear();
  }
  
  void optiTestVolume::init(
                 QDomElement const & element,
                 vectorHandling< constValue * > const * const cValP,
                 vectorHandling< scaFunction * > const * const sFunP,
                 vectorHandling< analyticGeometry * > const * const depAGeoP 
               ) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, cValP, sFunP, depAGeoP);    
    //
    // set label of boundenVolume
    //
    labelHandling::setName( getAttributeStr("label", element ) );

    //
    // set constValues float
    //
    QDomElement wElement = getChild("constValue", element);
    while ( !wElement.isNull() ) {
      //
      // get label of constValue
      //
      std::string label = getAttributeStr("label", wElement);
      //
      // get attribute
      //
      std::string attribute = getAttributeStr("attribute", wElement);  
      //
      // get surface
      //      
      constValue * cV = cValP->get(label);
      if (!cV) {
        dt__THROW(init,
                DTLOGEVAL(cV) << LOGDEL
                << "constValue " << DTLOGEVAL(label) 
                << " not in vector _cValP.");
      }
      if (attribute == "dtX") {
        _val[dtX] = cV->getValue();
        _valCValP[dtX] = cV;             
      }
      else if (attribute == "dtY") {
        _val[dtY] = cV->getValue();
        _valCValP[dtY] = cV;         
      }
      //
      // go to next element
      //
      wElement = getNextSibling("constValue", wElement);
    }    

  }

  float optiTestVolume::characterizeMe(void) {
    float xx = _valCValP[dtX]->getValue();
    float yy = _valCValP[dtY]->getValue();
    
    float val = (xx - 2.) * (xx - 2.) + yy * yy + 2. + 4.;
    
    DTINFOWF(characterizeMe(),
            << DTLOGEVAL(xx) << LOGDEL
            << DTLOGEVAL(yy) << LOGDEL
            << DTLOGEVAL(val) );
    return val;
  }

  float optiTestVolume::characterizeFailedDesign(void) {
    return 100.;
  }
  
  void optiTestVolume::doCycle( void ) {
    //
    //recursive call
    //
    optimizationCyclingDecorator::doCycle();
    
    //
    // do my work
    //
    //nothing
  }
  void optiTestVolume::writeGrid( void ) {
    DTINFOWF(writeGrid(),
            << "Writing abstract, senseless grid ;-)");
  }

  covise::coDoSet * optiTestVolume::toCoDoSet(char const * const str) const {
    
  }
  
  covise::coDoSet * optiTestVolume::toCoDoUnstructuredGrid(char const * const str) const {
    return NULL;
  }
}