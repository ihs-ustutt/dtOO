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

#ifndef BVOINTERFACE_H
#define	BVOINTERFACE_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

#include <xmlHeaven/qtXmlBase.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/optionHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class bVOInterface : public optionHandling {
    public:
      dt__classOnlyName(bVOInterface);  
      bVOInterface();
      virtual ~bVOInterface();
      virtual bVOInterface * create( void ) const = 0;           
      virtual void preUpdate( void );
      virtual void postUpdate( void );
      virtual void jInit( 
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );
      virtual void jInit( jsonPrimitive const & jE, boundedVolume * attachTo );
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );
      virtual std::vector< std::string > factoryAlias( void ) const;            
      boundedVolume const & constRefBoundedVolume(void);
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
      boundedVolume * ptrBoundedVolume(void);
    private:
      jsonPrimitive _config;
      boundedVolume * _bV;
  };
  dt__I_addCloneForpVHNotImpl(bVOInterface);
}
#endif	/* BVOINTERFACE_H */
