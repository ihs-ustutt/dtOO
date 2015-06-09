#ifndef __LOGME_H__
#define __LOGME_H__

//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <boost/format.hpp>
//------------------------------------------------------------------------------
#include <exceptionHeaven/eGeneral.h>
#include <exceptionHeaven/eGSL.h>
//------------------------------------------------------------------------------
#include "logBase.h"
#include "Output2FILE.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  template< class T >
  std::ostream& operator<<(std::ostream& os, const std::vector< T >& toLog) {
    typename std::vector< T >::const_iterator cIt;
    os <<  "[ ";
    for (cIt = toLog.begin(); cIt!=toLog.end(); ++cIt) {  
      os << *cIt << " | ";
    }
    os << " ]";
    return os;
  }  
  
  //
  // functions
  //
  class logMe {
    public:
      typedef boost::format dtFormat;
    public:
      dt__classOnlyName(logMe);
      static std::string initLog( std::string const & logFileName );
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
      template< class T >
      static inline std::string vecToString( std::vector< T > const & vec, int const grouping ) {
        std::ostringstream os;
        os << "[" << std::endl;
        int ii = 0;
        while( ii < vec.size() ) {
          for (int jj=0;jj<grouping;jj++) {
            os << vec[ii] << " ";
            ii++;
            if (ii == vec.size()) break;
          }
          os << std::endl;
        }
        os << "]";
        return os.str();
      }
      template< class T >
      static std::string vecToTable( std::vector< std::string > const & header, std::vector< T > const & vec ) {
        std::ostringstream os;
        for (int ii=0; ii<header.size(); ii++) {
         os << boost::format("| %13s ") % header[ii];
        }
        os << std::endl;
        int ii = 0;
        int grouping = header.size();
        while( ii < vec.size() ) {
          for (int jj=0;jj<grouping;jj++) {
            os << boost::format("| %+11.6e ") % vec[ii];
            ii++;
            if (ii == vec.size()) break;
          }
          os << std::endl;
        }
        return os.str();
      }
      template< class T >
      static std::string vecToTable( 
        std::vector< std::string > const & addInfo, 
        std::vector< std::string > const & header, 
        std::vector< T > const & vec 
      ) {
        std::ostringstream os;
        for (int ii=0; ii<addInfo.size(); ii++) {		
          os << addInfo[ii] << std::endl;
        }
        for (int ii=0; ii<header.size(); ii++) {
         os << boost::format("| %13s ") % header[ii];
        }
        os << std::endl;
        int ii = 0;
        int grouping = header.size();
        while( ii < vec.size() ) {
          for (int jj=0;jj<grouping;jj++) {
            os << boost::format("| %+11.6e ") % vec[ii];
            ii++;
            if (ii == vec.size()) break;
          }
          os << std::endl;
        }
        return os.str();
      } 	
      template< class T0, class T1 >
      static std::string mapToTable( 
        std::map< T0, T1 > const & mm 
      ) {
        std::map< T0, T1 > mmNC = (std::map< T0, T1 >) mm; 
        std::ostringstream os;
        typename std::map< T0, T1 >::const_iterator cIt;
        for (cIt=mm.begin(); cIt != mm.end(); ++cIt) {
          os << cIt->first << " -> " << cIt->second << std::endl;
        }
        return os.str();
      }       
      static inline std::string intVecToString( 
        std::vector< int > const & vec 
      ) {
        return vecToString< int >(vec);
      }
      static inline std::string floatVecToString( 
        std::vector< float > const & vec 
      ) {
        return vecToString< float >(vec);
      }
      static inline std::string stringVecToString( 
        std::vector< std::string > const & vec 
      ) {
        return vecToString< std::string >(vec);
      }
      static inline std::string stringVecToString( 
        std::vector< std::string > const & vec, int const grouping 
      ) {
        return vecToString< std::string >(vec, grouping);
      }
      static std::string floatVecToString( 
        std::vector< float > const & vec, int const grouping 
      ) {
        return vecToString< float >(vec, grouping);
      }
      static std::string floatVecToTable( 
        std::vector<std::string> const & header, 
        std::vector< float > const & vec 
      ) {
        return vecToTable< float >(header, vec);
      }
      static std::string floatVecToTable( 
        std::string const & header, std::vector< float > const & vec 
      ) {
        return floatVecToTable(std::vector< std::string >(1, header), vec);
      }
      static std::string stringVecToTable( 
        std::vector< std::string > const & header, 
        std::vector< std::string > const & vec 
      ) {
        return vecToTable< std::string >(header, vec);
      }            
      static std::string floatVecToTable( 
        std::vector<std::string> const & addInfo, 
        std::vector<std::string> const & header, 
        std::vector< float > const & vec 
      ) {
        return vecToTable< float >(addInfo, header, vec);
      }            
      static std::string floatMatrixToString( 
        std::vector< std::vector< float > > const & mat 
      );
  };
  
  //
  // FILELog
  //
  class FILELog : public logBase<Output2FILE> {};

  //
  // macros
  //
  #define dt__vector2d( vector ) \
    #vector" = ( " << vector.x() << ", " << vector.y() << ")"  
  #define dt__point2d( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ")"  
  #define dt__vector3d( vector ) \
    #vector" = ( " << vector.x() << ", " << vector.y() << ", " << vector.z() << ")"  
  #define dt__point3d( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ", " << point.z() << ")"  
  #define dt__eval( eval ) #eval" = " << eval
  #define dt__info(functionname, message) \
    if (logINFO > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logINFO) \
        << "|    " << className() << "::"#functionname << std::endl \
        message \
        << std::endl \
        << std::endl; \
    }
  #define dt__warning(functionname, message) \
    if (logWARNING > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logWARNING) \
        << "|    " << className() << "::"#functionname << std::endl \
        message \
        << std::endl \
        << std::endl; \
    }
  #define dt__debug(functionname, message) \
    if (logDEBUG > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get(logDEBUG) \
        << "|    " << className() << "::"#functionname << std::endl \
        message \
        << std::endl \
        << std::endl ;\
    }
  #define dt__throw(functionname, message) \
    throw dtOO::eGeneral( \
      std::ostringstream().flush() \
        << className() << "::"#functionname << std::endl \
        << "*-> file '" <<  __FILE__ << "'" << std::endl \
        << "*-> line '" << __LINE__ << "'" << std::endl \
        << std::endl \
        message \
        << std::endl \
        << dtOO::logMe::Backtrace() \
        << std::endl \
        << "Honor thy error as a hidden intention. (Brian Eno)"\
    )
  #define dt__throwUnexpected(functionname) \
    dt__throw(functionname, << "Unexpected")
  #define dt__throwIfWithMessage(cond, functionname, message) \
    if (cond) { \
      dt__throw(functionname, \
      << "condition: "#cond" is true." << std::endl \
      message); \
    }
  #define dt__throwIf(cond, functionname) \
    if (cond) { \
      dt__throw(functionname, << "condition: "#cond" is true."); \
    }
  #define dt__warnIfWithMessage(cond, functionname, message) \
    if (cond) { \
      dt__warning(functionname, \
      << "condition: "#cond" is true." << std::endl \
      message); \
    }
  #define dt__warnIfWithSolution(cond, solution, functionname) \
    if (cond) { \
      solution; \
      dt__warning(functionname, \
      << "condition: "#cond" is true." << std::endl); \
    }
  #define dt__warnIfWithMessageAndSolution(cond, solution, functionname, message) \
    if (cond) { \
      solution; \
      dt__warning(functionname, \
      << "condition: "#cond" is true." << std::endl \
      message); \
    }
  #define dt__warnIf(cond, functionname) \
    if (cond) { \
      dt__warning(functionname, << "condition: "#cond" is true."); \
    }
  #define dt__throwSpec(eType, functionname, message) \
    eType( \
      std::ostringstream().flush() \
        << className() << "::"#functionname << std::endl \
        << "*-> file '" <<  __FILE__ << "'" << std::endl \
        << "*-> line '" << __LINE__ << "'" << std::endl \
        << std::endl \
        message \
        << std::endl \
        << dtOO::logMe::Backtrace() \
        << std::endl \
        << "Honor thy error as a hidden intention. (Brian Eno)"\
    )
  #define dt__catch(functionname, eGWhat) \
    FILELog().Get(logERROR) \
      << "|    " << className() << "::"#functionname \
      << " >> Catching instance of eGeneral from " \
      << eGWhat \
      << std::endl \
      << std::endl
  #define dt__solution(cond, solution) \
    if (cond) solution;
  #define dt__makeChapter(chaptername) \
    if (logINFO > FILELog::ReportingLevel() ) {} else { \
      FILELog().Get() \
        << "-------------------------------------------------------------------" << std::endl \
        << "| " << std::endl \
        << "| "#chaptername << " (send from " << className() << ")" << std::endl \
        << "| " << std::endl \
        << "-------------------------------------------------------------------" << std::endl \
        << std::endl; \
    }
  #define dt__functionNotImplemented(functionname) \
    std::cout << "This function is not yet implemented." << std::endl
  #define dt__tryOcc(cmd, errorOut) \
    try { \
      OCC_CATCH_SIGNALS \
      cmd \
    } \
    catch( Standard_Failure ) { \
        throw eGeneral( std::ostringstream().flush() << className() << "::" << std::endl \
            << "*-> file '" <<  __FILE__ << "'" << std::endl \
            << "*-> line '" << __LINE__ << "'" << std::endl \
            << std::endl \
            << Standard_Failure::Caught() << std::endl \
            << Standard_Failure::Caught()->GetMessageString() << std::endl \
            << std::endl \
            errorOut \
            << std::endl \
            << dtOO::logMe::Backtrace() \
            << std::endl \
            << "Honor thy error as a hidden intention. (Brian Eno)"); \
    }
  #define moab__throwIf(cond, functionname) \
    if (cond) { \
      std::string err; \
      moab::MBErrorHandler_GetLastError(err); \
      dt__throw( \
        functionname, \
        << "condition: "#cond" is true." << std::endl \
        << dt__eval(err) ); \
    }  
  #define meshkit__catch(functionname) \
    catch( MeshKit::Error & merr ) { \
      dt__throw(functionname, << merr.what() ); \
    }
  //
  // can redirect std::cout output to logfile
  //
  class IosSwitch {
    public:
      IosSwitch( std::basic_ios< char >& ios, std::streambuf* sb )
        : m_ios( &ios )
        , m_sbMerk( ios.rdbuf( sb ) ) {
      }

      ~IosSwitch() {
        if( m_ios ) {
            m_ios->rdbuf( m_sbMerk );
        }
      }
      std::streambuf* rdbuf() const { 
        return m_sbMerk; 
      }
    private:
      std::basic_ios< char >* m_ios;
      std::streambuf*         m_sbMerk;

      // ---  nicht kopierbar
      IosSwitch( const IosSwitch& );
      IosSwitch& operator=( const IosSwitch& );
  };
 
  class Spreadbuf : public std::basic_streambuf< char > {
    public:
      Spreadbuf( std::basic_streambuf< char >* sb1, std::basic_streambuf< char >* sb2 )
        : m_sb1( sb1 )
        , m_sb2( sb2 ) {
      }
    protected:
      virtual int_type overflow( int_type c = traits_type::eof() ) {
        if( !traits_type::eq_int_type( c, traits_type::eof() ) ) {
            const char x = traits_type::to_char_type( c );
            if( traits_type::eq_int_type( m_sb1->sputc( x ), traits_type::eof() )
                    || traits_type::eq_int_type( m_sb2->sputc( x ), traits_type::eof() ) )
                return traits_type::eof();
        }
        return traits_type::not_eof( c );
      }
    private:
        std::basic_streambuf< char >* m_sb1;
        std::basic_streambuf< char >* m_sb2;
  };
  
  #define dt__pipeCout( spready,  coutswitch ) \
  	Spreadbuf spready( std::cout.rdbuf(), Output2FILE::Stream().rdbuf() ); \
		IosSwitch coutswitch( std::cout, &spready );
  }
#endif //__LOGME_H__