/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in
    the dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef __dtLog_H__
#define __dtLog_H__

#include "logMe/dtMacros.h"
#include <dtOOTypeDef.h>

#include <algorithm>
#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <exceptionHeaven/eGSL.h>
#include <exceptionHeaven/eGeneral.h>

#define dtLog__(severity)                                                      \
  BOOST_LOG_TRIVIAL(severity)                                                  \
    << "[" << __FILE_NAME__ << ":" << __LINE__ << ":" << __FUNCTION__ << "] "

#define dtLogInt__(level)                                                      \
  BOOST_LOG_SEV(                                                               \
    boost::log::trivial::logger::get(),                                        \
    static_cast<boost::log::trivial::severity_level>(level)                    \
  ) << "["                                                                     \
    << __FILE_NAME__ << ":" << __LINE__ << ":" << __FUNCTION__ << "] "
#define dtLog__trace dtLog__(trace)
#define dtLog__debug dtLog__(debug)
#define dtLog__info dtLog__(info)
#define dtLog__warning dtLog__(warning)
#define dtLog__error dtLog__(error)
#define dtLog__fatal dtLog__(fatal)
#define dtLog__always BOOST_LOG(dtOO::dtLog::alwaysLogger())

// macros
#define dt__quote(name) #name
#define dt__point2d(point)                                                     \
  #point " = ( " << point.x() << ", " << point.y() << ")"
#define dt__vector3d(vector)                                                   \
  #vector " = ( " << vector.x() << ", " << vector.y() << ", " << vector.z()    \
                  << ")"
#define dt__point3d(point)                                                     \
  #point " = ( " << point.x() << ", " << point.y() << ", " << point.z() << ")"
#define dt__eval(eval) #eval " = " << eval
#define dt__dumpToString(log) this << " {" << std::endl log << std::endl << "}"
#define dt__ddebug(functionname, message) dtLog__trace message;
#define dt__debug(functionname, message) dtLog__debug message;
#define dt__info(functionname, message) dtLog__info message;
#define dt__warning(functionname, message) dtLog__warning message;
#define dt__infoNoClass(functionname, message) dtLog__info message;
#define dt__debugNoClass(functionname, message) dtLog__debug message;
#define dt__warningNoClass(functionname, message) dtLog__warning message;
#define dt__quickinfo(message) dtLog__info message;
#define dt__quickdebug(message) dtLog__debug message;
#define dt__throw(functionname, message)                                       \
  throw dtOO::eGeneral(                                                        \
    std::ostringstream().flush()                                               \
    << "[ " << className() << "::" #functionname << " ]" << std::endl          \
    << "*-> file '" << __FILE__ << "'" << std::endl                            \
    << "*-> line '" << __LINE__ << "'" << std::endl                            \
    << std::endl message << std::endl                                          \
    << dtOO::dtLog::trace() << std::endl                                       \
    << "Honor thy error as a hidden intention. (Brian Eno)"                    \
  )
#define dt__throwNoClass(functionname, message)                                \
  throw dtOO::eGeneral(                                                        \
    std::ostringstream().flush()                                               \
    << "[ ::" #functionname << " ]" << std::endl                               \
    << "*-> file '" << __FILE__ << "'" << std::endl                            \
    << "*-> line '" << __LINE__ << "'" << std::endl                            \
    << std::endl message << std::endl                                          \
    << dtOO::dtLog::trace() << std::endl                                       \
    << "Honor thy error as a hidden intention. (Brian Eno)"                    \
  )
#define dt__throwUnexpected(functionname)                                      \
  dt__throw(functionname, << "Unexpected")
#define dt__throwUnexpectedNoClass(functionname)                               \
  dt__throwNoClass(functionname, << "Unexpected")
#define dt__throwIfWithMessage(cond, functionname, message)                    \
  if (cond)                                                                    \
  {                                                                            \
    dt__throw(                                                                 \
      functionname, << "condition: " #cond " is true." << std::endl message    \
    );                                                                         \
  }
#define dt__throwIf(cond, functionname)                                        \
  if (cond)                                                                    \
  {                                                                            \
    dt__throw(functionname, << "condition: " #cond " is true.");               \
  }
#define dt__throwIfNoClass(cond, functionname)                                 \
  if (cond)                                                                    \
  {                                                                            \
    dt__throwNoClass(functionname, << "condition: " #cond " is true.");        \
  }
#define dt__warnIfWithMessage(cond, functionname, message)                     \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(                                                               \
      functionname, << "condition: " #cond " is true." << std::endl message    \
    );                                                                         \
  }
#define dt__warnIfWithSolution(cond, solution, functionname)                   \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(                                                               \
      functionname, << "condition: " #cond " is true." << std::endl            \
    );                                                                         \
    solution;                                                                  \
  }
#define dt__warnIfWithMessageAndSolution(                                      \
  cond, solution, functionname, message                                        \
)                                                                              \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(                                                               \
      functionname, << "condition: " #cond " is true." << std::endl message    \
    );                                                                         \
    solution;                                                                  \
  }
#define dt__warnIf(cond, functionname)                                         \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(functionname, << "condition: " #cond " is true.");             \
  }
#define dt__solution(cond, solution)                                           \
  if (cond)                                                                    \
    solution;
#define dt__tryOcc(cmd, errorOut)                                              \
  try                                                                          \
  {                                                                            \
    OCC_CATCH_SIGNALS                                                          \
    cmd                                                                        \
  } catch (Standard_Failure & stdF)                                            \
  {                                                                            \
    throw eGeneral(                                                            \
      std::ostringstream().flush()                                             \
      << className() << "::" << std::endl                                      \
      << "*-> file '" << __FILE__ << "'" << std::endl                          \
      << "*-> line '" << __LINE__ << "'" << std::endl                          \
      << std::endl                                                             \
      << stdF.GetMessageString() << std::endl                                  \
      << std::endl errorOut << std::endl                                       \
      << dtOO::dtLog::trace() << std::endl                                     \
      << "Honor thy error as a hidden intention. (Brian Eno)"                  \
    );                                                                         \
  }
#define moab__throwIf(cond, functionname)                                      \
  if (cond)                                                                    \
  {                                                                            \
    std::string err;                                                           \
    moab::MBErrorHandler_GetLastError(err);                                    \
    dt__throw(                                                                 \
      functionname,                                                            \
      << "condition: " #cond " is true." << std::endl                          \
      << dt__eval(err)                                                         \
    );                                                                         \
  }
#define moab__throwIfWithMessage(cond, functionname, message)                  \
  if (cond)                                                                    \
  {                                                                            \
    std::string err;                                                           \
    moab::MBErrorHandler_GetLastError(err);                                    \
    dt__throw(                                                                 \
      functionname,                                                            \
      << "condition: " #cond " is true." << std::endl                          \
      << dt__eval(err) << std::endl message                                    \
    );                                                                         \
  }

namespace dtOO {

enum dtLogLevel {
  logDDEBUG = 0,
  logDEBUG,
  logINFO,
  logWARNING,
  logERROR,
  logFATAL
};

class dtLog {
  dt__classOnlyName(dtLog);

public:
  typedef ::boost::format dtFormat;

private:
  typedef boost::log::sinks::synchronous_sink<
    boost::log::sinks::text_file_backend>
    fileSink;

  dtLog(const ::std::string &filename);

public:
  dtLog(dtLog &other) = delete;
  void operator=(const dtLog &) = delete;

  static dtLog *ptr(const ::std::string &filename = ::std::string());
  static dtReal NowExTime(void);
  static std::string NowDateAndTime(void);
  static void setLogLevel(boost::log::trivial::severity_level level);
  static void setLogLevel(const ::std::string &level);
  static void setLogLevel(const int &level);
  static boost::log::sources::logger_mt &alwaysLogger();
  virtual ~dtLog();
  static std::string trace(void);
  static dtLogLevel reportingLevel(void);

  template <class T> static std::string str(std::vector<T> const &values)
  {
    std::ostringstream os;
    os << "[ ";
    dt__forAllRefAuto(values, value) os << value << " | ";
    os << " ]";
    return os.str();
  }

  template <class T0, class T1>
  static std::string str(const std::map<T0, T1> &toLog)
  {
    std::ostringstream os;
    typename std::map<T0, T1>::const_iterator cIt;
    os << "[ " << std::endl;
    for (cIt = toLog.begin(); cIt != toLog.end(); ++cIt)
    {
      os << cIt->first << " -> " << cIt->second << std::endl;
    }
    os << " ]";
    return os.str();
  }

  template <class T> static std::string str(std::list<T> const &values)
  {
    std::ostringstream os;
    os << "[ ";
    dt__forAllRefAuto(values, value) os << value << " | ";
    os << " ]";
    return os.str();
  }

  template <class T>
  static std::string
  str(std::vector<std::vector<T>> const &mat, dtInt width, dtInt precision)
  {
    if (mat.empty())
      return "";

    dtInt nU = mat.size();
    dtInt nV = mat[0].size();

    auto format = [&](T const &value) {
      std::ostringstream os;
      if constexpr (std::is_integral_v<T>)
        os << value;
      else
        os << std::showpos << std::scientific << std::setprecision(precision)
           << value;
      return os.str();
    };

    std::vector<dtInt> w(nV, width);
    for (dtInt jj = 0; jj < nV; jj++)
      for (dtInt ii = 0; ii < nU; ii++)
        w[jj] = std::max(w[jj], static_cast<dtInt>(format(mat[ii][jj]).size()));

    std::ostringstream os;
    os << "|  i   |   j>> ";
    for (dtInt jj = 0; jj < nV; jj++)
      os << "| " << std::setw(w[jj]) << jj << " ";
    os << "|\n";
    for (dtInt ii = 0; ii < nU; ii++)
    {
      os << "| " << std::setw(12) << ii << " ";
      for (dtInt jj = 0; jj < nV; jj++)
        os << "| " << std::setw(w[jj]) << format(mat[ii][jj]) << " ";
      os << "|\n";
    }
    os << std::endl;
    return os.str();
  }

  static std::string
  floatMatrixToString(std::vector<std::vector<dtReal>> const &mat)
  {
    return str(mat, 13, 6);
  }

  template <class T>
  static std::string vecToTable(
    std::vector<std::string> const &header,
    std::vector<T> const &vec,
    std::vector<std::string> addInfo = std::vector<std::string>(0, "")
  )
  {
    std::ostringstream os;
    for (int ii = 0; ii < addInfo.size(); ii++)
    {
      os << addInfo[ii] << std::endl;
    }
    for (int ii = 0; ii < header.size(); ii++)
    {
      os << boost::format("| %13s ") % header[ii];
    }
    os << std::endl;
    dtInt ii = 0;
    dtInt grouping = header.size();
    while (ii < vec.size())
    {
      for (int jj = 0; jj < grouping; jj++)
      {
        os << boost::format("| %+11.6e ") % vec[ii];
        ii++;
        if (ii == vec.size())
          break;
      }
      os << std::endl;
    }
    return os.str();
  }

  static bool report(dtInt const &level) { return level >= logLevel_; }
  static bool report(dtLogLevel const &level)
  {
    return report(static_cast<dtInt>(level));
  }

private:
  static dtLog *instance_;
  static clock_t startTime_;
  static boost::shared_ptr<fileSink> fileSink_;
  std::string fileName_;
  static dtInt logLevel_;
};

} // namespace dtOO

#endif // __dtLog_H__
