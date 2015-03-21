#include "scaPiecewiseLinear.h"

#include <logMe/logMe.h>
#include <do/coDoSet.h>
#include <do/coDoData.h>
#include <progHelper.h>

namespace dtOO {
  scaPiecewiseLinear::scaPiecewiseLinear() : scaFunction() {
    
  }
  
  scaPiecewiseLinear::scaPiecewiseLinear(std::vector< dtPoint2> const * point) : scaFunction() {
    _point.clear();
    for (int ii=0;ii<point->size();ii++) {
      _point.push_back( point->at(ii) );
    }
  }

  scaPiecewiseLinear::scaPiecewiseLinear(std::vector< dtPoint2> const & point) : scaFunction() {
    _point.clear();
    for (int ii=0;ii<point.size();ii++) {
      _point.push_back( point.at(ii) );
    }
  }
  
  scaPiecewiseLinear::scaPiecewiseLinear(const scaPiecewiseLinear& orig) : scaFunction(orig) {
    dt__FORALL(orig._point, ii,
      _point.push_back( orig._point[ii] );
    );
  }

  scaPiecewiseLinear::~scaPiecewiseLinear() {
    _point.clear();
  }
  
  scaPiecewiseLinear * scaPiecewiseLinear::clone( void ) const {
    return new scaPiecewiseLinear( *this );
  }
  
  scaPiecewiseLinear * scaPiecewiseLinear::create( void ) const {
    return new scaPiecewiseLinear();
  }  
  
  void scaPiecewiseLinear::dump(void) {
    
  }
  float scaPiecewiseLinear::getValue(float const xx) const {
  }

  std::vector< float > scaPiecewiseLinear::getVectorValue(float const xx) const {
    
  }  
  
  coDoSet* scaPiecewiseLinear::toCoDoSet(char const * const str) const {
    //
    // get min and max
    //
    float xMin = _point[0].x();
    float xMax = _point[0].x();
    float yMin = _point[0].y();
    float yMax = _point[0].y();
    for (int ii=0;ii<_point.size();ii++) {
      if (_point[ii].x() < xMin) {
        xMin = _point[ii].x();
      }
      if (_point[ii].x() > xMax) {
        xMax = _point[ii].x();
      }
      if (_point[ii].y() < yMin) {
        yMin = _point[ii].y();
      }
      if (_point[ii].y() > yMax) {
        yMax = _point[ii].y();
      }
    }

  
    float * xpl = new float[_point.size()];
    float * ypl = new float[_point.size()];
//    float rX = fabs((xMax - xMin)/100.);
//    float rY = fabs((yMax - yMin)/100.);
    coDistributedObject ** coDoVec2Objects = new coDistributedObject* [2];     
    for (int ii=0;ii<_point.size();ii++) {
      xpl[ii] = _point[ii].x();
      ypl[ii] = _point[ii].y();
//      int nPoints = getSurroundingPoints( 
//                      _point[ii].x(), 
//                      _point[ii].y(), 
//                      rX, 
//                      rY, 
//                      &xpl, 
//                      &ypl 
//                    );
    }
    char * objName = new char[strlen(str)+5];
    sprintf(objName,"%s_%d", str, 0);
    coDoVec2Objects[0] = new coDoVec2(objName, _point.size(), xpl, ypl);
    coDoVec2Objects[1] = NULL;
    


    //
    // options
    //
    //calculate world
    float interval = fabs( 0.1*(xMax-xMin) );
    xMin = xMin - interval;
    xMax = xMax + interval;
    interval = fabs( 0.1*(yMax-yMin) );
    yMin = yMin - interval;
    yMax = yMax + interval;    
    std::ostringstream outStream;
    outStream << "\0"
              << "TITLE \"" << getLabel() << "\"\n"
              << "xaxis LABEL \"" << "x" << "\"\n"
              << "yaxis LABEL \"" << "y" << "\"\n"
              << "SETS linewidth 4\n"
              << "SETS LINESTYLE 5\n"
              << "SETS SYMBOL 2\n"
              << "WORLD " << xMin << "," << yMin << "," << xMax << "," << yMax << "\n"
              << "FRAME ON\n"
              << "LEGEND ON \n"
              << "LEGEND BOX ON \n"
              << "LEGEND BOX FILL ON\n"
              << "LEGEND BOX FILL COLOR 0\n"
              << "SETS COLOR " << 0%12+2 << "\n"
//              << "S0 COLOR " << 0%12+2 << "\n"
//              << "S1 COLOR " << 1%12+2 << "\n"
              << "YAXIS TICK MAJOR " << (yMax-yMin)/5 << "\nYAXIS TICK MINOR " << (yMax-yMin)/10 << "\n"
              << "XAXIS TICK MAJOR " << (xMax-xMin)/5 << "\nXAXIS TICK MINOR " << (xMax-xMin)/10 << "\n";
    std::string plbuf = outStream.str();
    
    //
    // create set and add options
    //
    coDoSet* returnSet = new coDoSet(str, (coDistributedObject **)coDoVec2Objects);      
    returnSet->addAttribute("COMMANDS", plbuf.c_str());
    return returnSet;      
  }

  coDoVec2* scaPiecewiseLinear::toCoDoVec2(char const * const str) {

  }  

  covise::coDoSet * scaPiecewiseLinear::packToCoDoSet( char const * const str) const {
    covise::coDistributedObject ** elem = new covise::coDistributedObject*[2];
    
    elem[0] = const_cast< covise::coDistributedObject * >(
              scaFunction::packToCoDoSet(
                ("scaMuParserOneD::scaFunction::"+getLabel()).c_str()
              )->getElement(0)
            );
        
    covise::coDoVec2 * coVec = new covise::coDoVec2(
                                  ("pointCloud::point::"+getLabel()).c_str(), 
                                  _point.size()
                                );
    float * xx;
    float * yy;
    coVec->getAddresses(&xx, &yy);
    
    for (int ii=0; ii<_point.size();ii++) {
      xx[ii] = _point[ii].x();
      yy[ii] = _point[ii].y();
    }

    elem[1] = coVec;
    elem[2] = NULL;
    
    covise::coDoSet * set = new covise::coDoSet(("pointCloud::coDoSet::"+getLabel()).c_str(), elem);
    
    covise::coDistributedObject ** retElem = new covise::coDistributedObject*[2];    
    retElem[0] = set;
    retElem[1] = NULL;
    set->addAttribute("factoryType", "pointCloud");
    set->addAttribute("name", getLabel().c_str());
    return new covise::coDoSet(str, retElem);
  }
  
  void scaPiecewiseLinear::unpackFromCoDoSet( covise::coDoSet const * const set ) {
    setLabel( set->getAttribute("name") );

    covise::coDoSet const * setArr;
    dt__mustDownCast(set->getElement(0), covise::coDoSet const, setArr); 
    scaFunction::unpackFromCoDoSet(setArr);
    
    covise::coDoVec2 const * coVec;
    dt__mustDownCast(set->getElement(1), covise::coDoVec2 const, coVec);

    float * xx;
    float * yy;    
    coVec->getAddresses(&xx, &yy);
    for (int ii=0; ii<_point.size();ii++) {
      _point.push_back( dtPoint2(xx[ii], yy[ii]) );
    }   
  }  
}
