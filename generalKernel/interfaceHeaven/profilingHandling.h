#ifndef PROFILINGHANDLING_H
#define	PROFILINGHANDLING_H

#ifdef DTOO_USE_GOOGLEPROFTOOLS
#include <google/profiler.h>

#define dtOO__STARTPROFILE() \
  ProfilerStart("./profiler.gperf")

#define dtOO__ENDPROFILE() \
  ProfilerStop()
#else
#define dtOO__STARTPROFILE()
#define dtOO__ENDPROFILE()
#endif

#endif	/* PROFILINGHANDLING_H */

