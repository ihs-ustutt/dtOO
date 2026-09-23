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

#ifndef __dtLogMacros_H__
#define __dtLogMacros_H__

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

#endif // __dtLogMacros_H__
