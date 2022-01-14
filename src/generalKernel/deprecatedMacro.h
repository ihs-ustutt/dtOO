#ifndef DEPRECATEDMACRO_H
#define	DEPRECATEDMACRO_H

#include <dtOOTypeDef.h>

#ifdef __GNUC__
#define DTDEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DTDEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DTDEPRECATED for this compiler")
#define DTDEPRECATED(func) func
#endif

#endif	/* DEPRECATEDMACRO_H */

