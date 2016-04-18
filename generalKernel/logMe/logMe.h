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
#include "dtMacros.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  template< class T >
  std::ostream& operator<<(std::ostream& os, const std::vector< T >& toLog) {
    typename std::vector< T >::const_iterator cIt;
    os <<  "[ ";
    for (cIt = toLog.begin(); cIt!=toLog.end(); ++cIt) os << *cIt << " | ";
    os << " ]";
    return os;
  }
  template< class T0, class T1 >  
  std::ostream& operator<<(std::ostream& os, const std::map< T0, T1 >& toLog) {
    typename std::map< T0, T1 >::const_iterator cIt;
    os <<  "[ " << std::endl;
    for (cIt = toLog.begin(); cIt!=toLog.end(); ++cIt) {
      os << cIt->first << " -> " << cIt->second << std::endl;
    }
    os << " ]";
    return os;
  }    
  
  //
  // functions
  //
  class logMe {
    public:
      typedef ::boost::format dtFormat;
    public:
      dt__classOnlyName(logMe);
      static std::string initLog( std::string const & logFileName );
      static void closeLog( void );
      static std::string Backtrace(void);
            
      template< class T >
      static inline std::string vecToString( 
        std::vector< T > const & vec, int const groupIn = 0 
      ) {
        int grouping = vec.size();
        
        if (groupIn > 0) grouping = groupIn;
        
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
      
      template< class mT, class T, class rT >
      static inline std::string stringPtrVec( 
        mT const & vec, rT (T::*TMemFn)() const, int const grouping = 0
      ) {
        std::vector< rT > retVec;
        
        //for (int ii=0;ii<vec.size();ii++) {
        for(
          typename mT::const_iterator cIt = vec.begin();
          cIt != vec.end(); 
          ++cIt
        ) retVec.push_back( ((*cIt)->*TMemFn)() );
        
        return vecToString< rT >(retVec, grouping);
      }

      template< class mT, class T, class rT >
      static inline std::string stringPtrVec( 
        mT const & vec, rT (T::*TMemFn)(), int const grouping = 0
      ) {
        std::vector< rT > retVec;
        
        for(
          typename mT::iterator it = vec.begin();
          it != vec.end(); 
          ++it
        ) retVec.push_back( ((*it)->*TMemFn)() );
        
        return vecToString< rT >(retVec, grouping);
      }
      
      template< class T >
      static std::string vecToTable( 
        std::vector< std::string > const & header, 
        std::vector< T > const & vec,
        std::vector< std::string > addInfo = std::vector< std::string >(0,"")        
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
      
      static std::string floatMatrixToString( 
        std::vector< std::vector< float > > const & mat 
      );
  };
  
  //
  // dtOO::FILELog
  //
  class FILELog : public logBase<Output2FILE> {};

  //
  // macros
  //
  #define dt__quote(name) #name
  #define dt__vector2d( vector ) \
    #vector" = ( " << vector.x() << ", " << vector.y() << ")"  
  #define dt__point2d( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ")"  
  #define dt__vector3d( vector ) \
    #vector" = ( " \
    << vector.x() << ", " << vector.y() << ", " << vector.z() \
    << ")"
  #define dt__point3d( point ) \
    #point" = ( " << point.x() << ", " << point.y() << ", " << point.z() << ")"
  #define dt__eval( eval ) #eval" = " << eval
  #define dt__dumpToString( log ) \
    this << " {" << std::endl \
    log << std::endl \
		<< "}"
  #define dt__info(functionname, message) \
    if (dtOO::logINFO > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logINFO) \
        << "[ " << className() << "::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__debug(functionname, message) \
    if (dtOO::logDEBUG > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logDEBUG) \
        << "[ " << className() << "::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__warning(functionname, message) \
    if (dtOO::logWARNING > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logWARNING) \
        << "[ " << className() << "::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__infoNoClass(functionname, message) \
    if (dtOO::logINFO > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logINFO) \
        << "[ ::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__debugNoClass(functionname, message) \
    if (dtOO::logDEBUG > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logDEBUG) \
        << "[ ::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__warningNoClass(functionname, message) \
    if (dtOO::logWARNING > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get(dtOO::logWARNING) \
        << "[ ::"#functionname << " ]" << std::endl \
        message;\
    }
  #define dt__quickinfo(message) \
    if (dtOO::logINFO > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().GetNoHeader(dtOO::logDEBUG) message;\
    }
  #define dt__quickdebug(message) \
    if (dtOO::logDEBUG > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().GetNoHeader(dtOO::logDEBUG) message; \
    }
  #define dt__throw(functionname, message) \
    throw dtOO::eGeneral( \
      std::ostringstream().flush() \
        << "[ " << className() << "::"#functionname << " ]" << std::endl \
        << "*-> file '" <<  __FILE__ << "'" << std::endl \
        << "*-> line '" << __LINE__ << "'" << std::endl \
        << std::endl \
        message \
        << std::endl \
        << dtOO::logMe::Backtrace() \
        << std::endl \
        << "Honor thy error as a hidden intention. (Brian Eno)"\
    )
  #define dt__throwNoClass(functionname, message) \
    throw dtOO::eGeneral( \
      std::ostringstream().flush() \
        << "[ ::"#functionname << " ]" << std::endl \
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
  #define dt__throwUnexpectedNoClass(functionname) \
    dt__throwNoClass(functionname, << "Unexpected")
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
  #define dt__throwIfNoClass(cond, functionname) \
    if (cond) { \
      dt__throwNoClass(functionname, << "condition: "#cond" is true."); \
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
  #define dt__warnIfWithMessageAndSolution( \
    cond, solution, functionname, message \
  ) \
    if (cond) { \
      dt__warning(functionname, \
      << "condition: "#cond" is true." << std::endl \
      message); \
      solution; \
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
    dtOO::FILELog().Get(dtOO::logERROR) \
      << "|    " << className() << "::"#functionname \
      << " >> Catching instance of eGeneral from " \
      << eGWhat \
      << std::endl \
      << std::endl
  #define dt__catchNoClass(functionname, eGWhat) \
    dtOO::FILELog().Get(dtOO::logERROR) \
      << "|    " << "::"#functionname \
      << " >> Catching instance of eGeneral from " \
      << eGWhat \
      << std::endl \
      << std::endl
  #define dt__solution(cond, solution) \
    if (cond) solution;
  #define dt__makeChapter(cName) \
    if (dtOO::logINFO > dtOO::FILELog::ReportingLevel() ) {} else { \
      dtOO::FILELog().Get() \
        << std::endl \
        << "-----------------------------------------------------------------" \
        << std::endl \
        << "| " << std::endl \
        << "| "#cName << " (send from " << className() << ")" << std::endl \
        << "| " << std::endl \
        << "-----------------------------------------------------------------" ;\
    }
  #define dt__tryOcc(cmd, errorOut) \
    try { \
      OCC_CATCH_SIGNALS \
      cmd \
    } \
    catch( Standard_Failure ) { \
        throw eGeneral( \
          std::ostringstream().flush() << className() << "::" << std::endl \
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
        : m_ios( &ios ), m_sbMerk( ios.rdbuf( sb ) ) {
      }

      ~IosSwitch() {
        if( m_ios ) m_ios->rdbuf( m_sbMerk );
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
      Spreadbuf( 
        std::basic_streambuf< char >* sb1, std::basic_streambuf< char >* sb2 
      ) : m_sb1( sb1 ), m_sb2( sb2 ) {
      }
    protected:
      virtual int_type overflow( int_type c = traits_type::eof() ) {
        if( !traits_type::eq_int_type( c, traits_type::eof() ) ) {
          const char x = traits_type::to_char_type( c );
          if( 
            traits_type::eq_int_type( m_sb1->sputc( x ), traits_type::eof() )
            || 
            traits_type::eq_int_type( m_sb2->sputc( x ), traits_type::eof() ) 
          ) return traits_type::eof();
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