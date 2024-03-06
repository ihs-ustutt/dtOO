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

#ifndef constValue_H
#define	constValue_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class constValue : public labelHandling {
    public:
      dt__class(constValue, labelHandling);
      constValue( constValue const & orig );
      constValue( std::string const & label, dtReal const & val );
      virtual ~constValue();
      static constValue * create(
        std::string const & type, 
        std::string const & label, 
        std::string const & valueStr,
        bool const loadable
      );
      virtual constValue * clone( void ) const = 0;    
      virtual void dump(void) const = 0;
      virtual dtReal getValue(void) const;
      virtual std::string getValueStr(void) const;
      virtual dtReal getValuePercent(void) const;
      dtReal operator()( void ) const;
      virtual void setValue(dtReal const toSet);
      virtual void setRange(dtReal const min, dtReal const max);
      virtual void writeToElement( 
        ::QDomDocument & doc, ::QDomElement & element
      ) = 0;
      virtual dtReal getMin(void) const;
      virtual dtReal getMax(void) const;
      virtual bool loadable( void ) const;
      virtual void resolveConstraint( 
        lvH_constValue const * const cVArr
      );
      void setState(std::string const & state);
      std::string getState( void ) const;
      void freeState( void );
      bool inState( void ) const;
    private:
      dtReal _value;
      dtReal _min;
      dtReal _max;
      bool _loadable;
      std::string _state;
  };
  dt__H_addCloneForpVH(constValue);
}
#endif	/* constValue_H */
