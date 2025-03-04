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

#ifndef DEPRECATEDMACRO_H
#define DEPRECATEDMACRO_H

#include <dtOOTypeDef.h>

#ifdef __GNUC__
#define DTDEPRECATED(func) func __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DTDEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DTDEPRECATED for this compiler")
#define DTDEPRECATED(func) func
#endif

#endif /* DEPRECATEDMACRO_H */
