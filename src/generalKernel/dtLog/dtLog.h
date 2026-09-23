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

#include "dtLogMacros.h"

namespace dtOO {
/**
 * @brief The dtLogLevel enum
 *
 * This enumeration defines the severity levels for logging messages.
 * It is used to categorize log messages based on their importance or
 * severity. The levels are ordered from least severe (logDDEBUG) to
 * most severe (logFATAL).
 */
enum dtLogLevel {
  logDDEBUG = 0,
  logDEBUG,
  logINFO,
  logWARNING,
  logERROR,
  logFATAL
};

/**
 * @brief The dtLog class
 *
 * This class is a singleton that provides logging functionality using the
 * Boost.Log library. It allows logging messages with different severity levels
 * and provides utility functions for formatting and outputting various data
 * structures.
 */
class dtLog {
  dt__classOnlyName(dtLog);

private:
  /**
   * @brief dtLog
   * Private constructor to enforce singleton pattern.
   * @param filename Optional filename for log output.
   */
  dtLog(const ::std::string &filename);

public:
  dtLog(dtLog &other) = delete;
  void operator=(const dtLog &) = delete;
  virtual ~dtLog();
  /**
   * @brief ptr
   * Static method to get the singleton instance of dtLog.
   * @param filename Optional filename for log output.
   * @return Pointer to the singleton instance of dtLog.
   */
  static dtLog *ptr(const ::std::string &filename = ::std::string());
  /**
   * @brief NowExTime
   * Static method to get the elapsed time since the start of the program.
   * @return Elapsed time in seconds as a dtReal.
   */
  static dtReal NowExTime(void);
  /**
   * @brief NowDateAndTime
   * Static method to get the current date and time as a string.
   * @return Current date and time in string format.
   */
  static std::string NowDateAndTime(void);
  /**
   * @brief setLogLevel
   * Static method to set the logging severity level.
   * @param level Logging severity level (boost::log::trivial::severity_level).
   */
  static void setLogLevel(boost::log::trivial::severity_level level);
  /**
   * @brief setLogLevel
   * Static method to set the logging severity level using a string.
   * @param level Logging severity level as a string (e.g., "info", "debug").
   */
  static void setLogLevel(const ::std::string &level);
  /**
   * @brief setLogLevel
   * Static method to set the logging severity level using an integer.
   * @param level Logging severity level as an integer (0-5).
   */
  static void setLogLevel(const int &level);
  /**
   * @brief formatLogRecord
   * Static method to format log records for output.
   * @param record Log record to format.
   * @param stream Output stream for formatted log record.
   */
  static bool report(dtInt const &level);
  /**
   * @brief formatLogRecord
   * Static method to format log records for output.
   * @param record Log record to format.
   * @param stream Output stream for formatted log record.
   */
  static bool report(dtLogLevel const &level);
  /**
   * @brief alwaysLogger
   * Static method to get a logger that always logs messages regardless of the
   * severity level.
   * @return Reference to a logger that always logs messages.
   */
  static boost::log::sources::logger_mt &alwaysLogger();
  /**
   * @brief trace
   * Static method to get a string representation of the current call stack.
   * @return String representation of the call stack.
   */
  static std::string trace(void);
  /**
   * @brief reportingLevel
   * Static method to get the current logging severity level.
   * @return Current logging severity level as dtLogLevel.
   */
  static dtLogLevel reportingLevel(void);
  /**
   * @brief str
   * Template method to convert a vector of values to a string representation.
   * @param values Vector of values to convert.
   * @return String representation of the vector.
   */
  template <class T> static std::string str(std::vector<T> const &values)
  {
    std::ostringstream os;
    os << "[ ";
    dt__forAllConstRefAuto(values, value)
      os << value << " | ";
    os << " ]";
    return os.str();
  }
  /**
   * @brief str
   * Template method to convert a map of key-value pairs to a string
   * representation.
   * @param toLog Map of key-value pairs to convert.
   * @return String representation of the map.
   */
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
  /**
   * @brief str
   * Template method to convert a list of values to a string representation.
   * @param values List of values to convert.
   * @return String representation of the list.
   */
  template <class T> static std::string str(std::list<T> const &values)
  {
    std::ostringstream os;
    os << "[ ";
    dt__forAllIterAuto(values, valueIt) os << *valueIt << " | ";
    os << " ]";
    return os.str();
  }
  /**
   * @brief str
   * Template method to convert a vector of vectors (matrix) to a string
   * representation with specified width and precision.
   * @param mat Vector of vectors (matrix) to convert.
   * @param width Width for formatting each element.
   * @param precision Precision for formatting floating-point values.
   * @return String representation of the matrix.
   */
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
  /**
   * @brief vecToTable
   * Template method to convert a vector of values to a formatted table string
   * representation with headers and optional additional information.
   * @param header Vector of strings representing the table headers.
   * @param vec Vector of values to convert into a table format.
   * @param addInfo Optional vector of strings for additional information to be
   * printed before the table.
   * @return String representation of the formatted table.
   */
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

public:
  typedef ::boost::format dtFormat;

private:
  static dtLog *instance_;
  static clock_t startTime_;
  typedef boost::log::sinks::synchronous_sink<
    boost::log::sinks::text_file_backend>
    fileSink;
  static boost::shared_ptr<fileSink> fileSink_;
  std::string fileName_;
  static dtInt logLevel_;
};

} // namespace dtOO

#endif // __dtLog_H__
