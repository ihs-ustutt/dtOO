#include "boundedVolume.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <stdexcept>
#include <bVObserver/bVOInterfaceFactory.h>
#include <bVObserver/bVOInterface.h>
#include <dtXmlParserDecorator/qtXmlPrimitive.h>


namespace dtOO {  
  boundedVolume::boundedVolume() { 
		_meshed = false;
  }

  boundedVolume::~boundedVolume() {
    _map2dTo3d.destroy();
    _map1dTo3d.destroy();
    _vertexP.destroy();    
  }

  void boundedVolume::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // clear maps
    //
    _vId.clear();
    _eId.clear();
    _fId.clear();
    _rId.clear();
    
    _vertexP.destroy();
    _map1dTo3d.destroy();
    _map2dTo3d.destroy();
    
    //
    // set label of boundenVolume
    //
    labelHandling::setLabel( qtXmlPrimitive::getAttributeStr("label", element ) );
    
    //
    // set options
    //
    optionHandling::init( &element );
    
    //
    // check for bVObservers
    //
    if ( qtXmlPrimitive::hasChild("bVObserver", element) ) {      
      std::vector< QDomElement > transElement 
			= 
			qtXmlPrimitive::getChildVector("bVObserver", element);
      
      for (int ii=0; ii<transElement.size(); ii++) {
        bVOInterface * bVI
        =
        bVOInterfaceFactory::create(
					qtXmlPrimitive::getAttributeStr("name", transElement[ii])
				);
        bVI->init(transElement[ii], bC, cV, aF, aG, bV, this);
        this->attachBVObserver( bVI );
      }

    }
  }

  void boundedVolume::makePreGrid(void) {
    makeGrid();
  }
  
  void boundedVolume::addId(std::string const typeStr, std::string const nameStr) {
    if (typeStr == "vertex") {
      int id = _vId.size();
      _vId.insert(std::pair<std::string, int>(nameStr, id) );
      _vertexP.push_back( NULL );
    }
    else if (typeStr == "edge") {
      int id = _eId.size();
      _eId.insert(std::pair<std::string, int>(nameStr, id) );
      _map1dTo3d.push_back( NULL );
    }
    else if (typeStr == "face") {
      int id = _fId.size();
      _fId.insert(std::pair<std::string, int>(nameStr, id) );
      _map2dTo3d.push_back( NULL );
    }
    else if (typeStr == "region") {
      int id = _rId.size();
      _rId.insert(std::pair<std::string, int>(nameStr, id) );
      _map3dTo3d.push_back(NULL);
    }
    else {
      dt__THROW(addId(),
              << "Unknown type." << LOGDEL
              << DTLOGEVAL(typeStr) );
    }
  }
  
  int boundedVolume::strToId(std::string const typeStr, std::string const nameStr) const {
    if (typeStr == "vertex") {
      return _vId.at(nameStr);
    }
    else if (typeStr == "edge") {
      return _eId.at(nameStr);
    }
    else if (typeStr == "face") {
      return _fId.at(nameStr);
    }
    else if (typeStr == "region") {
      return _rId.at(nameStr);
    }
    else {
      dt__THROW(strToId(),
              << "Unknown type." << LOGDEL
              << DTLOGEVAL(typeStr) );
    }    
  }

  bool boundedVolume::hasStrToId(std::string const typeStr, std::string const nameStr) const {
    try {
      if (typeStr == "vertex") {
        int const tmp = _vId.at(nameStr);
      }
      else if (typeStr == "edge") {
        int const tmp = _eId.at(nameStr);
      }
      else if (typeStr == "face") {
        int const tmp = _fId.at(nameStr);
      }
      else if (typeStr == "region") {
        int const tmp = _rId.at(nameStr);
      }
    }
    catch ( const std::out_of_range & oor ) {
      return false;
    }
    return true;    
  }

  
  int boundedVolume::vStrToId(std::string const nameStr) const {
    return strToId("vertex", nameStr);
  }
  
  int boundedVolume::eStrToId(std::string const nameStr) const {
    return strToId("edge", nameStr);    
  }
  
  int boundedVolume::fStrToId(std::string const nameStr) const {
    return strToId("face", nameStr);
  }
  
  int boundedVolume::rStrToId(std::string const nameStr) const {
    return strToId("region", nameStr);
  }

  map3dTo3d const * boundedVolume::getPtrToMap3dTo3d( int const id ) const{
    return _map3dTo3d[id];
  }
  
  map2dTo3d const * boundedVolume::getPtrToMap2dTo3d( int const id ) const{
    return _map2dTo3d[id];
  }

  map1dTo3d const * boundedVolume::getPtrToMap1dTo3d( int const id ) const {
    return _map1dTo3d[id];
  }

  dtPoint3 const * boundedVolume::getPtrToVertex( int const id ) const {
    return _vertexP[id];
  }    

  vectorHandling< map2dTo3d * > & boundedVolume::getRefToMap2dTo3dHandling( void ) {
    return _map2dTo3d;
  }

  vectorHandling< map2dTo3d * > const & boundedVolume::getConstRefToMap2dTo3dHandling( void ) const {
    return _map2dTo3d;
  }  
  
  vectorHandling< map1dTo3d * > & boundedVolume::getRefToMap1dTo3dHandling( void ) {
    return _map1dTo3d;
  }

  vectorHandling< map3dTo3d * > & boundedVolume::getRefToMap3dTo3dHandling( void ) {
    return _map3dTo3d;
  }  
  
  vectorHandling< dtPoint3 * > & boundedVolume::getRefToVertexHandling( void ) {
    return _vertexP;
  }
	
  bool boundedVolume::isMeshed( void ) const {
		return _meshed;
	}
	
  void boundedVolume::setMeshed( void ) {
		_meshed = true;
	}
	
	std::vector< std::string > boundedVolume::getMeshTags( void ) const {
		return std::vector< std::string >(0);
	}	
}


