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

#ifndef lVHOInterface_H
#define	lVHOInterface_H

#include <mainConceptFwd.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class lVHOSubject;
  class labelHandling;
  class jsonPrimitive;

  class lVHOInterface {
    public:
      dt__classOnlyName(lVHOInterface);
      lVHOInterface( jsonPrimitive const & config, lVHOSubject * subj=NULL );
      virtual ~lVHOInterface();
      virtual void observeSet( labelHandling * lH );
      virtual void observe( labelHandling * lH );
      virtual bool typeObserves( lVHOSubject const & subj) const;
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
      virtual lVHOSubject & subject( void );
      virtual lVHOSubject const & subject( void ) const;
    private:
      dt__pH(jsonPrimitive) _config;
      lVHOSubject * _subj;
  };
}

#endif /* lVHOInterface_H */
