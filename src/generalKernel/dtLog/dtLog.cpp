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

#include "dtLog.h"

#include "dtGmshMessagePrinter.h"
#include "dtOCCMessagePrinter.h"
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <cxxabi.h>
#include <dlfcn.h>
#include <dtOOVersion.h>
#include <execinfo.h>
#include <interfaceHeaven/stringPrimitive.h>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

namespace dtOO {

dtLog *dtLog::instance_ = nullptr;

clock_t dtLog::startTime_ = clock();

boost::shared_ptr<dtLog::fileSink> dtLog::fileSink_ = nullptr;

dtInt dtLog::logLevel_ = -1;

dtReal dtLog::NowExTime(void)
{
  return (dtReal(clock() - startTime_) / (dtReal CLOCKS_PER_SEC));
}

void formatLogRecord(
  boost::log::record_view const &record, boost::log::formatting_ostream &stream
)
{
  auto always = record["Always"];
  auto message = record[boost::log::expressions::smessage];
  if (always)
  {
    if (message)
    {
      const std::string continuation =
        (::boost::format("[%10.2f] ") % dtLog::NowExTime()).str();
      stream << continuation; // << message.get();
      std::string text = message.get();
      std::size_t pos = 0;
      while ((pos = text.find('\n', pos)) != std::string::npos)
      {
        text.replace(pos, 1, "\n" + continuation);
        pos += 1 + continuation.size();
      }
      stream << text;
    }
    return;
  }

  stream << ::boost::format("[%10.2f][") % dtLog::NowExTime();
  auto severity = record[boost::log::trivial::severity];
  if (severity)
  {
    switch (severity.get())
    {
    case boost::log::trivial::trace:
      stream << "T";
      break;
    case boost::log::trivial::debug:
      stream << "D";
      break;
    case boost::log::trivial::info:
      stream << "I";
      break;
    case boost::log::trivial::warning:
      stream << "W";
      break;
    case boost::log::trivial::error:
      stream << "E";
      break;
    case boost::log::trivial::fatal:
      stream << "F";
      break;
    }
  }
  stream << "]";

  if (message)
  {
    const std::string continuation = "[          ] +   ";
    std::string text = message.get();
    std::size_t pos = 0;
    while ((pos = text.find('\n', pos)) != std::string::npos)
    {
      text.replace(pos, 1, "\n" + continuation);
      pos += 1 + continuation.size();
    }
    stream << text;
  }
}

boost::log::sources::logger_mt &dtLog::alwaysLogger()
{
  static boost::log::sources::logger_mt logger;
  static auto const attribute = logger.add_attribute(
    "Always", boost::log::attributes::constant<bool>(true)
  );
  return logger;
}

std::string dtLog::NowDateAndTime(void)
{
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct);
  return buf;
}

dtLog::dtLog(const ::std::string &filename)
{
  fileName_ = filename;
  fileSink_ = logging::add_file_log(
    keywords::file_name = fileName_, keywords::format = &formatLogRecord
  );
  logging::add_common_attributes();
  setLogLevel(boost::log::trivial::info);
  dtLog__always << "*" << std::endl
                << "* (d)esign (t)ool (O)bject-(O)riented" << " : "
                << dtOO::git::versionMajor << "." << dtOO::git::versionMinor
                << "." << dtOO::git::versionCommitNo << " ( "
                << dtOO::git::versionName << " )" << std::endl
                << "*";
  // register additional message printers
  dtLog__always << "Registering OCC message printer.";
  dtOCCMessagePrinter::registerPrinter();
  dtLog__always << "Registering gmsh message printer.";
  dtGmshMessagePrinter::registerPrinter();
}

dtLog::~dtLog() {}

dtLog *dtLog::ptr(const std::string &filename)
{
  if (instance_ == nullptr)
  {
    instance_ = new dtLog(filename);
  }
  return instance_;
}

void dtLog::setLogLevel(boost::log::trivial::severity_level level)
{
  if (fileSink_ == nullptr)
  {
    dt__throw(
      setLogLevel(),
      << "File sink is not initialized. Please initialize the logger first."
    );
  }
  fileSink_->set_filter(
    expr::has_attr<bool>("Always") ||
    (expr::attr<boost::log::trivial::severity_level>("Severity") >= level)
  );
  logLevel_ = static_cast<int>(level);
}

void dtLog::setLogLevel(const ::std::string &level)
{
  if (stringPrimitive::isInt(level))
  {
    setLogLevel(stringPrimitive::stringToInt(level));
  }
  else
  {
    if (level == "trace")
      setLogLevel(boost::log::trivial::trace);
    else if (level == "debug")
      setLogLevel(boost::log::trivial::debug);
    else if (level == "info")
      setLogLevel(boost::log::trivial::info);
    else if (level == "warning")
      setLogLevel(boost::log::trivial::warning);
    else if (level == "error")
      setLogLevel(boost::log::trivial::error);
    else if (level == "fatal")
      setLogLevel(boost::log::trivial::fatal);
    else
      dt__throw(
        setLogLevel(),
        << "Unknown log level '" << level
        << "'. Valid levels are: trace, debug, info, warning, error, fatal."
      );
  }
  return;
}

void dtLog::setLogLevel(const int &level)
{
  int const corLev = std::min(std::max(level, 0), 5);
  if (level < 0 || level > 5)
  {
    dtLog__always
      << "Invalid log level '" << std::to_string(level)
      << "'. Valid levels are: 0 (trace), 1 (debug), 2 (info), 3 (warning), 4 "
         "(error), 5 (fatal). Setting log level to "
      << corLev << ".";
  }
  setLogLevel(static_cast<boost::log::trivial::severity_level>(corLev));
}

std::string dtLog::trace(void)
{
  int skip = 1;
  void *callstack[128];
  const dtInt nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
  char buf[1024];
  int nFrames = backtrace(callstack, nMaxFrames);
  char **symbols = backtrace_symbols(callstack, nFrames);

  std::ostringstream trace_buf;
  trace_buf << std::endl << "Backtrace\n";
  for (int i = skip; i < nFrames; i++)
  {
    Dl_info info;
    if (dladdr(callstack[i], &info) && info.dli_sname)
    {
      char *demangled = NULL;
      int status = -1;
      if (info.dli_sname[0] == '_')
        demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
      snprintf(
        buf,
        sizeof(buf),
        "[ %-3d ] %*p %s + %zd\n",
        i,
        int(2 + sizeof(void *) * 2),
        callstack[i],
        status == 0           ? demangled
        : info.dli_sname == 0 ? symbols[i]
                              : info.dli_sname,
        (char *)callstack[i] - (char *)info.dli_saddr
      );
      free(demangled);
    }
    else
    {
      snprintf(
        buf,
        sizeof(buf),
        "[ %-3d ] %*p %s\n",
        i,
        int(2 + sizeof(void *) * 2),
        callstack[i],
        symbols[i]
      );
    }
    trace_buf << buf;
  }
  free(symbols);
  if (nFrames == nMaxFrames)
    trace_buf << "[truncated]\n";
  trace_buf << std::endl;
  return trace_buf.str();
}

dtLogLevel dtLog::reportingLevel(void)
{
  return static_cast<dtLogLevel>(logLevel_);
}

bool dtLog::report(dtInt const &level) { return level >= logLevel_; }

bool dtLog::report(dtLogLevel const &level)
{
  return report(static_cast<dtInt>(level));
}

bool dtLog::logLevelIs(dtLogLevel const &level)
{
  return (static_cast<dtInt>(level) >= logLevel_);
}
} // namespace dtOO
