#ifndef LOGBASE_H
#define	LOGBASE_H

#include <sstream>
#include <string>
#include <boost/format.hpp>

namespace dtOO {
  std::string NowTime();
  std::string NowDateAndTime();
  
  enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG};
  
  template < typename T > 
  class logBase {
    public:
      logBase();
      virtual ~logBase();
      std::ostringstream& Get(TLogLevel level = logINFO);
      std::ostringstream& GetNoHeader( TLogLevel level = logINFO );
    public:
      static TLogLevel& ReportingLevel(void);
      static std::string ToString(TLogLevel level);
      static TLogLevel FromString(const std::string& level);
    protected:
      std::ostringstream os;
    private:
      static clock_t startTime;
    private:
      logBase(const logBase&);
      logBase& operator =(const logBase&);
      TLogLevel myLogLevel;
  };

  //
  // methods
  //
  template < typename T > 
  clock_t logBase< T >::startTime = clock();
  
  template < typename T > 
  logBase< T >::logBase() {
    
  }

  template < typename T > 
  std::ostringstream& logBase< T >::Get(TLogLevel level) {
    os 
    << std::endl
    << std::endl
    << ::boost::format("[ %12d ][ " ) % (clock() - startTime)
    << ToString(level) << " ]";

    myLogLevel = level;
    return os;
  }

  template < typename T > 
  std::ostringstream& logBase< T >::GetNoHeader( TLogLevel level ) {   
    os << std::endl;    

    myLogLevel = level;    
    return os;
  }  

  template < typename T > 
  logBase< T >::~logBase() {
//    os << std::endl;
    T::Output(os.str(), myLogLevel);
  }

  template < typename T > 
  TLogLevel& logBase< T >::ReportingLevel(void) {
    static TLogLevel reportingLevel = logDEBUG;
    return reportingLevel;
  }

  template < typename T > 
  std::string logBase< T >::ToString(TLogLevel level) {
    static const char* const buffer[] = {"E", "W", "I", "D"};
    return buffer[level];
  }

  template < typename T > 
  TLogLevel logBase< T >::FromString(const std::string& level) {
    if (level == "DEBUG")
      return logDEBUG;
    if (level == "INFO")
      return logINFO;
    if (level == "WARNING")
      return logWARNING;
    if (level == "ERROR")
      return logERROR;
    logBase< T >().Get(logWARNING) 
    << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
  }
}
#endif	/* LOGBASE_H */
