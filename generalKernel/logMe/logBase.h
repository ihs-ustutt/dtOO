#ifndef LOGBASE_H
#define	LOGBASE_H

#include <sstream>
#include <string>

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
    public:
      static TLogLevel& ReportingLevel(void);
      static std::string ToString(TLogLevel level);
      static TLogLevel FromString(const std::string& level);
    protected:
      std::ostringstream os;
    private:
      logBase(const logBase&);
      logBase& operator =(const logBase&);
      TLogLevel myLogLevel;
  };

  //
  // methods
  //
  template < typename T > 
  logBase< T >::logBase() {
    
  }

  template < typename T > 
  std::ostringstream& logBase< T >::Get(TLogLevel level) {
    os 
    << "*--> " << NowTime()
    << " < " << ToString(level) << " >"
    << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t')
    << std::endl;
    
    myLogLevel = level;
    return os;
  }

  template < typename T > 
  logBase< T >::~logBase() {
    os << std::endl;
    T::Output(os.str(), myLogLevel);
  }

  template < typename T > 
  TLogLevel& logBase< T >::ReportingLevel(void) {
    static TLogLevel reportingLevel = logDEBUG;
    return reportingLevel;
  }

  template < typename T > 
  std::string logBase< T >::ToString(TLogLevel level) {
    static const char* const buffer[] = {"ERROR  ", "WARNING", "INFO   ", "DEBUG  "};
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

