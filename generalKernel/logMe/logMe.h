#ifndef __LOGME_H__
#define __LOGME_H__

//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <boost/format.hpp>
//------------------------------------------------------------------------------
#include <exceptionHandling/eGeneral.h>
#include <exceptionHandling/eGSL.h>
//------------------------------------------------------------------------------
#include "logBase.h"
#include "Output2FILE.h"
#include <logMe/dtMacros.h>

#define LOGDEL    "\n                      |-- "
#define ENDLOGDEL "\n                      `-- "

namespace dtOO {
  //
  // functions
  //
  class logMe {
    public:
      dt__CLASSNAME(logMe);
      static void initLog( std::string const & logFileName );
      static void closeLog( void );
      static std::string Backtrace(void);  
      template< class T >
      static inline std::string vecToString( std::vector< T > const & vec ) {
        std::ostringstream os;
        os << " [";
        for (int ii=0;ii<vec.size();ii++) {
          T val = vec[ii];
          os << val << " ";
        }
        os << "] ";
        return os.str();
      }
      static inline std::string intVecToString( std::vector< int > const & vec ) {
        return vecToString< int >(vec);
      }
      static inline std::string floatVecToString( std::vector< float > const & vec ) {
        return vecToString< float >(vec);
      }     
      static std::string floatVecToString( std::vector< float > const & vec, int const grouping );
      static std::string floatVecToTable( std::vector<std::string> const & header, std::vector< float > const & vec );
      static std::string floatVecToTable( std::vector<std::string> const & addInfo, std::vector<std::string> const & header, std::vector< float > const & vec );
      static std::string floatMatrixToString( std::vector< std::vector< float > > const & mat );
  };
  
  //
  // FILELog
  //
  class FILELog : public logBase<Output2FILE> {};

  //
  // macros
  //
  #define DTLOGVEC2D( vector ) \
    #vector" = ( " << vector.x() << ", " << vector.y() << ")"  
  #define DTLOGVEC2DP( vector ) \
    #vector" = ( " << vector->x() << ", " << vector->y() << ")"  
  #define DTLOGPOI2D( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ")"  
  #define DTLOGPOI2DP( point ) \
    #point" = ( " << point->x() << ", " << point->y() << ")" 
  #define DTLOGVEC3D( vector ) \
    #vector" = ( " << vector.x() << ", " << vector.y() << ", " << vector.z() << ")"  
  #define DTLOGVEC3DP( vector ) \
    #vector" = ( " << vector->x() << ", " << vector->y() << ", " << vector->z() << ")"  
  #define DTLOGPOI3D( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ", " << point.z() << ")"  
  #define DTLOGPOI3DP( point ) \
    #point" = ( " << point->x() << ", " << point->y() << ", " << point->z() << ")"  
  #define DTLOGEVAL( eval ) \
    #eval" = " << eval
  #define DTINFOWF(functionname, message) \
      if (logINFO > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logINFO) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          message \
          << ENDLOGDEL \
          << "\n" ; \
          }
  #define DTWARNINGWF(functionname, message) \
      if (logWARNING > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logWARNING) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          message \
          << ENDLOGDEL \
          << "\n" ; \
          }
  #define DTDEBUGWF(functionname, message) \
      if (logDEBUG > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logDEBUG) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          message \
          << ENDLOGDEL \
          << "\n" ;\
          }
  #define dt__THROW(functionname, message) \
      throw eGeneral( std::ostringstream().flush() << ClassName() << "::"#functionname << LOGDEL \
          << "file '" <<  __FILE__ << "'" << LOGDEL \
          << "line '" << __LINE__ << "'" << LOGDEL \
          << LOGDEL \
          message \
          << LOGDEL \
          << dtOO::logMe::Backtrace() \
          << LOGDEL \
          << "Honor thy error as a hidden intention. (Brian Eno)")
  #define dt__THROW_IFWM(cond, functionname, message) \
      if (cond) { \
        dt__THROW(functionname, \
        << "condition: "#cond" is true." << LOGDEL \
        message); \
      }
  #define dt__THROW_IF(cond, functionname) \
      if (cond) { \
        dt__THROW(functionname, << "condition: "#cond" is true."); \
      }
  #define dt__WARN_IFWM(cond, functionname, message) \
      if (cond) { \
        DTWARNINGWF(functionname, \
        << "condition: "#cond" is true." << LOGDEL \
        message); \
      }
  #define dt__WARN_IF(cond, functionname) \
      if (cond) { \
        DTWARNINGWF(functionname, << "condition: "#cond" is true."); \
      }
  #define dt__THROWSPEC(eType, functionname, message) \
      eType( std::ostringstream().flush() << ClassName() << "::"#functionname << LOGDEL \
          << "file '" <<  __FILE__ << "'" << LOGDEL \
          << "line '" << __LINE__ << "'" << LOGDEL \
          << LOGDEL \
          message \
          << LOGDEL \
          << dtOO::logMe::Backtrace() \
          << LOGDEL \
          << "Honor thy error as a hidden intention. (Brian Eno)")
  #define DTCATCHERRORWF(functionname, eGWhat) \
      FILELog().Get(logERROR) << ClassName() << "::"#functionname << LOGDEL \
          << "Catching instance of eGeneral:" << LOGDEL \
          << eGWhat \
          << ENDLOGDEL \
          << "\n"
  #define DTLOGCHAP(chaptername) \
    if (logDEBUG > FILELog::ReportingLevel() ) {} else { \
    FILELog().Get() << LOGDEL \
          << LOGDEL \
          << "-------------------------------------------------------------------" << LOGDEL \
          << "| " << LOGDEL \
          << "| "#chaptername << " (send from " << ClassName() << ")" << LOGDEL \
          << "| " << LOGDEL \
          << "-------------------------------------------------------------------" << ENDLOGDEL \
          << "\n" ; \
          }
  #define DTFUNCTIONNOTI(functionname) \
          std::cout << "This function is not yet implemented." << std::endl
  #define DTBUFFERINIT() \
    std::ostringstream dtDebugWf_buf_stream
  #define DTBUFFER( message ) \
    dtDebugWf_buf_stream message
  #define DTDEBUGWF_BUFFER(functionname) \
    if (logDEBUG > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logDEBUG) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          << dtDebugWf_buf_stream.str() \
          << ENDLOGDEL \
          << "\n"; \
          } \
          dtDebugWf_buf_stream.clear()
  #define DTINFOWF_BUFFER(functionname) \
    if (logINFO > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logINFO) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          << dtDebugWf_buf_stream.str() \
          << ENDLOGDEL \
          << "\n"; \
          } \
          dtDebugWf_buf_stream.clear()
  #define DTWARNINGWF_BUFFER(functionname) \
    if (logWARNING > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logWARNING) << ClassName() << "::"#functionname << LOGDEL \
          << LOGDEL \
          << dtDebugWf_buf_stream.str() \
          << ENDLOGDEL \
          << "\n"; \
          } \
          dtDebugWf_buf_stream.clear()
  #define dt__THROW_BUFFER(functionname) \
    throw eGeneral( std::ostringstream().flush() << ClassName() << "::"#functionname << LOGDEL \
        << "file '" <<  __FILE__ << "'" << LOGDEL \
        << "line '" << __LINE__ << "'" << LOGDEL \
        << LOGDEL \
        << dtDebugWf_buf_stream.str() \
        << LOGDEL \
        << LOGDEL)
//        dtDebugWf_buf_stream.clear()
  #define dt__TRYOCC(cmd, errorOut) \
  try { \
    OCC_CATCH_SIGNALS \
    cmd \
  } \
  catch( Standard_Failure ) { \
      throw eGeneral( std::ostringstream().flush() << ClassName() << "::" << LOGDEL \
          << "file '" <<  __FILE__ << "'" << LOGDEL \
          << "line '" << __LINE__ << "'" << LOGDEL \
          << LOGDEL \
          << Standard_Failure::Caught() << LOGDEL \
          << Standard_Failure::Caught()->GetMessageString() << LOGDEL \
          << LOGDEL \
          errorOut \
          << LOGDEL \
          << dtOO::logMe::Backtrace() \
          << LOGDEL \
          << "Honor thy error as a hidden intention. (Brian Eno)"); \
  }
//  //
//  // can redirect std::cout output to logfile
//  //
//  class IosSwitch {
//    public:
//      IosSwitch( std::basic_ios< char >& ios, std::streambuf* sb )
//        : m_ios( &ios )
//        , m_sbMerk( ios.rdbuf( sb ) ) {
//      }
//
//      ~IosSwitch() {
//        if( m_ios ) {
//            m_ios->rdbuf( m_sbMerk );
//        }
//      }
//      std::streambuf* rdbuf() const { 
//        return m_sbMerk; 
//      }
//    private:
//      std::basic_ios< char >* m_ios;
//      std::streambuf*         m_sbMerk;
//
//      // ---  nicht kopierbar
//      IosSwitch( const IosSwitch& );
//      IosSwitch& operator=( const IosSwitch& );
//  };
// 
//  class Spreadbuf : public std::basic_streambuf< char > {
//    public:
//      Spreadbuf( std::basic_streambuf< char >* sb1, std::basic_streambuf< char >* sb2 )
//        : m_sb1( sb1 )
//        , m_sb2( sb2 ) {
//      }
//    protected:
//      virtual int_type overflow( int_type c = traits_type::eof() ) {
//        if( !traits_type::eq_int_type( c, traits_type::eof() ) ) {
//            const char x = traits_type::to_char_type( c );
//            if( traits_type::eq_int_type( m_sb1->sputc( x ), traits_type::eof() )
//                    || traits_type::eq_int_type( m_sb2->sputc( x ), traits_type::eof() ) )
//                return traits_type::eof();
//        }
//        return traits_type::not_eof( c );
//      }
//    private:
//        std::basic_streambuf< char >* m_sb1;
//        std::basic_streambuf< char >* m_sb2;
//  };
  
//  #define DTLOGCOUT( spready,  coutswitch) \
//  	Spreadbuf spready( std::cout.rdbuf(), Output2FILE::Stream().rdbuf() ); \
//		IosSwitch coutswitch( std::cout, &spready );


  }
#endif //__LOGME_H__