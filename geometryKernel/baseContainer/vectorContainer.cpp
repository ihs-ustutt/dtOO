#include "vectorContainer.h"
#include <vector>
//#include <do/coDoData.h>
//#include <do/coDoPolygons.h>
#include <logMe/logMe.h>
#include <math.h>
//#include <do/coDoSet.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {  
  vectorContainer::vectorContainer() {
    _tipSize
    =
    staticPropertiesHandler::getInstance()->getOptionFloat("vector_render_size");    
  }

  vectorContainer::vectorContainer(const vectorContainer& orig) {
    _tipSize=orig._tipSize;
  }

  vectorContainer::~vectorContainer() {
    _pair.clear();
  }

  void vectorContainer::add(dtVector3 const * const vectorP, std::string const * const stringP ) {
    this->add(*vectorP, *stringP);  
  }

  void vectorContainer::add(dtVector3 const * const vectorP, std::string const  string ) {
    this->add(*vectorP, string);  
  }

  void vectorContainer::add(dtVector3 const vector, std::string const * const stringP ) {
    this->add(vector, *stringP);    
  }

  void vectorContainer::add(dtVector3 const vector, std::string const string ) {
    add(vector, string, dtPoint3(0,0,0));
  }

  void vectorContainer::add(dtVector3 const * const vector, std::string const string, dtPoint3 const point ) {
    this->add(*vector, string, point);
  }

  void vectorContainer::add(dtVector3 const * const vector, std::string const * const string, dtPoint3 const point ) {
    this->add(*vector, *string, point);
  }

  void vectorContainer::add(dtVector3 const * const vector, std::string const * const string, dtPoint3 const * const point ) {
    this->add(*vector, *string, *point);
  }

  void vectorContainer::add(dtVector3 const vector, std::string const * const string, dtPoint3 const point ) {
    this->add(vector, *string, point);
  }

  void vectorContainer::add(dtVector3 const vector, std::string const * const string, dtPoint3 const * const point ) {
    this->add(vector, string, *point);
  }

  void vectorContainer::add(dtVector3 const vector, std::string const string, dtPoint3 const point ) {
    if ( string != "" ) {
      for (int ii=0;ii<_pair.size();ii++) {
        if (_pair[ii].second == string) {
          dt__THROW(add(),
                  << "Try to add a vector that is already in container." << LOGDEL
                  << DTLOGEVAL(string));
        }
      }
    }
    std::pair< dtVector3, dtPoint3 > inpair( vector, point );
    _pair.push_back( std::pair < std::pair< dtVector3, dtPoint3 >, std::string >(inpair, string) );  
  }

  dtVector3 vectorContainer::get(std::string const string ) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return ( _pair[ii].first.first );
      }
    }
  }


  dtVector3 vectorContainer::get(std::string const * const stringP ) const {
    return ( this->get(*stringP) );
  }

  bool vectorContainer::has(std::string const * const stringP) const {
    return this->has(*stringP);
  }

  bool vectorContainer::has(std::string const string) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return true;
      }
    }
    return false;
  }

//	covise::coDoSet * vectorContainer::toCoDoSet(char const * const str) const {
//	  return toCoDoSet( std::string(str) );	
//	}
//	
//  covise::coDoSet * vectorContainer::toCoDoSet(string const string) const {
//    covise::coDistributedObject **objects;
//    char* objName;
//    objects = new covise::coDistributedObject* [3];
//    objects[2] = NULL;
//    objName = new char[strlen(string.c_str())+5];
//    sprintf(objName,"%s_%d", string.c_str(),0);
//    float* xCoords;
//    float* yCoords;
//    float* zCoords;
//    int* polyPolyList; 
//    int* polyCornerList; 
//    covise::coDoPolygons * onePolyObject;
//    int nPolyPoints = _pair.size() * 5 ; 
//    int nPolyCorners = _pair.size() * 6; 
//    int nPolyPolygons = _pair.size() * 2 ;  
//    onePolyObject = new covise::coDoPolygons(objName, nPolyPoints, nPolyCorners, nPolyPolygons); 
//    onePolyObject->getAddresses(&xCoords, &yCoords, &zCoords, &polyCornerList, &polyPolyList);
//    int polyCoordsCounter = 0;
//    int polyCornerCounter = 0;
//    int polyPolyCounter = 0;
//
//    covise::coDoLines * oneLineObject;
//    float* xLineCoords;
//    float* yLineCoords;
//    float* zLineCoords;
//    int* lineList; 
//    int* lineCornerList; 
//    int nLinePoints = _pair.size() * 2 ; 
//    int nLineCorners = _pair.size() * 2; 
//    int nLines = _pair.size();  
//    oneLineObject = new covise::coDoLines(objName, nLinePoints, nLineCorners, nLines); 
//    oneLineObject->getAddresses(&xLineCoords, &yLineCoords, &zLineCoords, &lineCornerList, &lineList);
//    int lineCoordsCounter = 0;
//    int lineCornerCounter = 0;
//    int lineLineCounter = 0;
//    dtPoint3 corner[5];
//    
//    for(int ii=0;ii<_pair.size();ii++){         
//      float vecLengthU = sqrt( _pair[ii].first.first.squared_length() );
//      dtVector3 normVecU = _pair[ii].first.first / (vecLengthU);
////      DTINFOWF(vectorContainer(), 
////      << DTLOGEVAL(normVecU.squared_length()) << LOGDEL
////              << DTLOGEVAL(_pair.size())
////      );
//      dtPoint3 topCorner = _pair[ii].first.second + _pair[ii].first.first;
//      dtPoint3 baseMiddle = topCorner - _tipSize * normVecU;
//      corner[0] = topCorner;
//      float ny, nx, uyx;
//      if (_pair[ii].first.first.x() == 0) {
//        nx = 1.;
//        ny = 0.;
//      }
//      else {
//        uyx = _pair[ii].first.first.y() / _pair[ii].first.first.x();
//        ny = sqrt(1 / (1 + (uyx * uyx)));
//        nx = -uyx * ny;
//      }
//
//
////      DTINFOWF(vectorContainer(),
////              << DTLOGEVAL(_pair[ii].first.first.y()) << LOGDEL
////              << DTLOGEVAL(ny) << LOGDEL
////              << DTLOGEVAL(_pair[ii].first.first.x()) << LOGDEL
////              << DTLOGEVAL(nx)
////              );
//      dtVector3 nVec(nx,ny,0.);
//      corner[1] = baseMiddle - _tipSize * nVec;
//      corner[2] = baseMiddle + _tipSize * nVec;
//      dtVector3 nVecOrtho = cross_product (_pair[ii].first.first, nVec);
//      float nVecOrthoLength = sqrt( nVecOrtho.squared_length() );
//      dtVector3 nVecOrthoNorm = nVecOrtho / (nVecOrthoLength);
//      corner[3] = baseMiddle - _tipSize * nVecOrthoNorm;
//      corner[4] = baseMiddle + _tipSize * nVecOrthoNorm;
//
//      int countPoints = 0;
//      int idC[5];
//      for (int jj=0;jj<5;jj++) {
//        idC[jj] = countPoints+polyCoordsCounter;
//        *(xCoords+countPoints+polyCoordsCounter) = corner[jj].x();
//        *(yCoords+countPoints+polyCoordsCounter) = corner[jj].y();
//        *(zCoords+countPoints+polyCoordsCounter) = corner[jj].z();
//        countPoints = countPoints + 1;
//      }      
//      polyCoordsCounter = polyCoordsCounter + 5;
//
//      polyPolyList[polyPolyCounter+0] = polyCornerCounter+0;
//      polyCornerList[polyCornerCounter+0] = idC[0];
//      polyCornerList[polyCornerCounter+1] = idC[1];
//      polyCornerList[polyCornerCounter+2] = idC[2];
//      polyPolyList[polyPolyCounter+1] = polyCornerCounter+3;
//      polyCornerList[polyCornerCounter+3] = idC[0];
//      polyCornerList[polyCornerCounter+4] = idC[3];
//      polyCornerList[polyCornerCounter+5] = idC[4];
//      polyCornerCounter = polyCornerCounter+6;
//      polyPolyCounter = polyPolyCounter+2;     
//
//
//      *(xLineCoords+lineCoordsCounter) = topCorner.x();
//      *(yLineCoords+lineCoordsCounter) = topCorner.y();
//      *(zLineCoords+lineCoordsCounter) = topCorner.z();
//      *(xLineCoords+lineCoordsCounter+1) = _pair[ii].first.second.x();
//      *(yLineCoords+lineCoordsCounter+1) = _pair[ii].first.second.y();
//      *(zLineCoords+lineCoordsCounter+1) = _pair[ii].first.second.z();
//      lineList[0+lineLineCounter] = 0+lineCornerCounter;
//      lineCornerList[0+lineCornerCounter] = lineCoordsCounter+0;
//      lineCornerList[1+lineCornerCounter] = lineCoordsCounter+1;
//      lineCoordsCounter = lineCoordsCounter+2;
//      lineLineCounter++;      
//      lineCornerCounter = lineCornerCounter + 2;
//    }
//    
//    objects[0] = onePolyObject; //poly
//    objects[1] = oneLineObject;
//    objects[0]->addAttribute("COLOR", "magenta");
//    
//    covise::coDoSet * retSet 
//		= 
//		new covise::coDoSet( 
//		  string.c_str(), (covise::coDistributedObject **)objects
//		);
//
//    delete objects[0];		
//		delete objects[1];
//		
//		delete [] objects;
//		
//		return retSet;
//  }
//
//  covise::coDoSet * vectorContainer::toCoDoSetNoLines(string const string) const {
//    covise::coDistributedObject **objects;
//    char* objName;
//    objects = new covise::coDistributedObject* [2];
//    objects[1] = NULL;
//    objName = new char[strlen(string.c_str())+5];
//    sprintf(objName,"%s_%d", string.c_str(),0);
//    float* xCoords;
//    float* yCoords;
//    float* zCoords;
//    int* polyPolyList; 
//    int* polyCornerList; 
//    covise::coDoPolygons * onePolyObject;
//    int nPolyPoints = _pair.size() * 5 ; 
//    int nPolyCorners = _pair.size() * 6; 
//    int nPolyPolygons = _pair.size() * 2 ;  
//    onePolyObject = new covise::coDoPolygons(objName, nPolyPoints, nPolyCorners, nPolyPolygons); 
//    onePolyObject->getAddresses(&xCoords, &yCoords, &zCoords, &polyCornerList, &polyPolyList);
//    int polyCoordsCounter = 0;
//    int polyCornerCounter = 0;
//    int polyPolyCounter = 0;
//    dtPoint3 corner[5];
//    for(int ii=0;ii<_pair.size();ii++){         
//      float vecLengthU = sqrt( _pair[ii].first.first.squared_length() );
//      dtVector3 normVecU = _pair[ii].first.first / (vecLengthU);
////      DTINFOWF(vectorContainer(), 
////      << DTLOGEVAL(normVecU.squared_length()) << LOGDEL
////              << DTLOGEVAL(_pair.size())
////      );
//      dtPoint3 topCorner = _pair[ii].first.second + _tipSize * normVecU; // + _pair[ii].first.first;
//      dtPoint3 baseMiddle = topCorner - _tipSize * normVecU;
//      corner[0] = topCorner;
//      float ny, nx, uyx;
//      if (_pair[ii].first.first.x() == 0) {
//        nx = 1.;
//        ny = 0.;
//      }
//      else {
//        uyx = _pair[ii].first.first.y() / _pair[ii].first.first.x();
//        ny = sqrt(1 / (1 + (uyx * uyx)));
//        nx = -uyx * ny;
//      }
//
//
////      DTINFOWF(vectorContainer(),
////              << DTLOGEVAL(_pair[ii].first.first.y()) << LOGDEL
////              << DTLOGEVAL(ny) << LOGDEL
////              << DTLOGEVAL(_pair[ii].first.first.x()) << LOGDEL
////              << DTLOGEVAL(nx)
////              );
//      dtVector3 nVec(nx,ny,0.);
//      corner[1] = baseMiddle - _tipSize * nVec;
//      corner[2] = baseMiddle + _tipSize * nVec;
//      dtVector3 nVecOrtho = cross_product (_pair[ii].first.first, nVec);
//      float nVecOrthoLength = sqrt( nVecOrtho.squared_length() );
//      dtVector3 nVecOrthoNorm = nVecOrtho / (nVecOrthoLength);
//      corner[3] = baseMiddle - _tipSize * nVecOrthoNorm;
//      corner[4] = baseMiddle + _tipSize * nVecOrthoNorm;
//
//      int countPoints = 0;
//      int idC[5];
//      for (int jj=0;jj<5;jj++) {
//        idC[jj] = countPoints+polyCoordsCounter;
//        *(xCoords+countPoints+polyCoordsCounter) = corner[jj].x();
//        *(yCoords+countPoints+polyCoordsCounter) = corner[jj].y();
//        *(zCoords+countPoints+polyCoordsCounter) = corner[jj].z();
//        countPoints = countPoints + 1;
//      }      
//      polyCoordsCounter = polyCoordsCounter + 5;
//
//      polyPolyList[polyPolyCounter+0] = polyCornerCounter+0;
//      polyCornerList[polyCornerCounter+0] = idC[0];
//      polyCornerList[polyCornerCounter+1] = idC[1];
//      polyCornerList[polyCornerCounter+2] = idC[2];
//      polyPolyList[polyPolyCounter+1] = polyCornerCounter+3;
//      polyCornerList[polyCornerCounter+3] = idC[0];
//      polyCornerList[polyCornerCounter+4] = idC[3];
//      polyCornerList[polyCornerCounter+5] = idC[4];
//      polyCornerCounter = polyCornerCounter+6;
//      polyPolyCounter = polyPolyCounter+2;     
//    }
//    
//    objects[0] = onePolyObject; //poly
//    objects[0]->addAttribute("COLOR", "magenta");
//    
//    return (new covise::coDoSet( string.c_str(), (covise::coDistributedObject **)objects) );
//  }

  bool vectorContainer::isEmpty( void ) const {
    return _pair.empty();
  }
}