#ifndef GSLERROR_H
#define GSLERROR_H

#include <dtOOTypeDef.h>

#include <gsl/gsl_errno.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

  namespace ROOT {
    namespace Math {
      class GSLError {
        public:
          dt__classOnlyName(GSLError);
          GSLError() {
            //gsl_set_error_handler(&GSLError::Handler);
            gsl_set_error_handler(&GSLError::Handler); // JLK : turn off message errors from GSL
            // set a new handler for GSL
          }
          static void Handler(const char * reason, const char * file, int line, int gsl_errno)  {
             dt__throwSpec(
               dtOO::eGSL, 
               Handler(), 
               << boost::format( "Error %d in %s at %d : %s" ) % gsl_errno % file % line % reason 
             );
           }
        };

     }
  }
  static ROOT::Math::GSLError gGSLError;

#endif /* GSLERROR_H */