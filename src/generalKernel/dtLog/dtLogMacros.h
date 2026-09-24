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

/*!
 * @file dtLogMacros.h
 *
 * This header provides convenience macros for the dtOO logging system.
 * The macros are based on Boost.Log and provide a compact interface for
 * writing messages with different severity levels.
 *
 * The logging macros automatically add source file, line number, and
 * function name information to log messages. Additional macros provide
 * formatted output for geometric objects and expressions, conditional
 * warnings, exception handling, and error reporting.
 *
 */

#ifndef __dtLogMacros_H__
#define __dtLogMacros_H__

/*!
 * @def dtLog__
 * @brief Write a log message with source location information.
 *
 * The specified severity level is passed to Boost.Log. The source file,
 * line number, and function name are automatically added to the message.
 *
 * @param severity Boost.Log severity level.
 */
#define dtLog__(severity)                                                      \
  BOOST_LOG_TRIVIAL(severity)                                                  \
    << "[" << __FILE_NAME__ << ":" << __LINE__ << ":" << __FUNCTION__ << "] "

/*!
 * @def dtLogInt__
 * @brief Write a log message using an integer severity level.
 *
 * The integer level is converted to a Boost.Log severity level. The source
 * file, line number, and function name are automatically added to the message.
 *
 * @param level Integer severity level.
 */
#define dtLogInt__(level)                                                      \
  BOOST_LOG_SEV(                                                               \
    boost::log::trivial::logger::get(),                                        \
    static_cast<boost::log::trivial::severity_level>(level)                    \
  ) << "["                                                                     \
    << __FILE_NAME__ << ":" << __LINE__ << ":" << __FUNCTION__ << "] "

/*!
 * @def dtLog__trace
 * @brief Write a trace-level log message.
 */
#define dtLog__trace dtLog__(trace)

/*!
 * @def dtLog__debug
 * @brief Write a debug-level log message.
 */
#define dtLog__debug dtLog__(debug)

/*!
 * @def dtLog__info
 * @brief Write an informational log message.
 */
#define dtLog__info dtLog__(info)

/*!
 * @def dtLog__warning
 * @brief Write a warning-level log message.
 */
#define dtLog__warning dtLog__(warning)

/*!
 * @def dtLog__error
 * @brief Write an error-level log message.
 */
#define dtLog__error dtLog__(error)

/*!
 * @def dtLog__fatal
 * @brief Write a fatal-level log message.
 */
#define dtLog__fatal dtLog__(fatal)

/*!
 * @def dtLog__always
 * @brief Write a log message independently of the configured log level.
 *
 * Messages written using this macro are always passed to the logging sinks,
 * independently of the configured log level.
 */
#define dtLog__always BOOST_LOG(dtOO::dtLog::alwaysLogger())

// macros

/*!
 * @def dt__quote
 * @brief Convert a preprocessor argument to a string literal.
 *
 * @param name Expression or identifier to be converted to a string.
 */
#define dt__quote(name) #name

/*!
 * @def dt__point2d
 * @brief Format a two-dimensional point for logging.
 *
 * The point is represented by its x and y coordinates.
 *
 * @param point Two-dimensional point providing x() and y() methods.
 */
#define dt__point2d(point)                                                     \
  #point " = ( " << point.x() << ", " << point.y() << ")"

/*!
 * @def dt__vector3d
 * @brief Format a three-dimensional vector for logging.
 *
 * The vector is represented by its x, y, and z components.
 *
 * @param vector Three-dimensional vector providing x(), y(), and z() methods.
 */
#define dt__vector3d(vector)                                                   \
  #vector " = ( " << vector.x() << ", " << vector.y() << ", " << vector.z()    \
                  << ")"

/*!
 * @def dt__point3d
 * @brief Format a three-dimensional point for logging.
 *
 * The point is represented by its x, y, and z coordinates.
 *
 * @param point Three-dimensional point providing x(), y(), and z() methods.
 */
#define dt__point3d(point)                                                     \
  #point " = ( " << point.x() << ", " << point.y() << ", " << point.z() << ")"

/*!
 * @def dt__eval
 * @brief Format an expression together with its evaluated value.
 *
 * For example, @c dt__eval(value) produces output similar to
 * @c "value = 42".
 *
 * @param eval Expression to be displayed and evaluated.
 */
#define dt__eval(eval) #eval " = " << eval

/*!
 * @def dt__dumpToString
 * @brief Format the current object together with additional stream content.
 *
 * The macro uses the current object through @c this and encloses the
 * additional stream content in braces.
 *
 * @param log Stream expression containing additional information.
 */
#define dt__dumpToString(log) this << " {" << std::endl log << std::endl << "}"

/*!
 * @def dt__ddebug
 * @brief Write a trace-level log message.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the message.
 */
#define dt__ddebug(functionname, message) dtLog__trace message;

/*!
 * @def dt__debug
 * @brief Write a debug-level log message.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the message.
 */
#define dt__debug(functionname, message) dtLog__debug message;

/*!
 * @def dt__info
 * @brief Write an informational log message.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the message.
 */
#define dt__info(functionname, message) dtLog__info message;

/*!
 * @def dt__warning
 * @brief Write a warning log message.
 *
 * A warning reports a potentially problematic condition without interrupting
 * the normal program flow.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the warning message.
 */
#define dt__warning(functionname, message) dtLog__warning message;

/*!
 * @def dt__infoNoClass
 * @brief Write an informational log message without class context.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the message.
 */
#define dt__infoNoClass(functionname, message) dtLog__info message;

/*!
 * @def dt__debugNoClass
 * @brief Write a debug-level log message without class context.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the message.
 */
#define dt__debugNoClass(functionname, message) dtLog__debug message;

/*!
 * @def dt__warningNoClass
 * @brief Write a warning log message without class context.
 *
 * @param functionname Function name associated with the message.
 * @param message Stream expression containing the warning message.
 */
#define dt__warningNoClass(functionname, message) dtLog__warning message;

/*!
 * @def dt__quickinfo
 * @brief Write an informational log message without function context.
 *
 * @param message Stream expression containing the message.
 */
#define dt__quickinfo(message) dtLog__info message;

/*!
 * @def dt__quickdebug
 * @brief Write a debug-level log message without function context.
 *
 * @param message Stream expression containing the message.
 */
#define dt__quickdebug(message) dtLog__debug message;

/*!
 * @def dt__throw
 * @brief Throw a general dtOO exception.
 *
 * Constructs and throws a @c dtOO::eGeneral exception containing the class
 * name, function name, source file, source line, supplied message, and a
 * stack trace.
 *
 * @param functionname Function name associated with the error.
 * @param message Stream expression containing the exception message.
 */
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

/*!
 * @def dt__throwNoClass
 * @brief Throw a general dtOO exception without a class name.
 *
 * Constructs and throws a @c dtOO::eGeneral exception containing the function
 * name, source file, source line, supplied message, and a stack trace.
 *
 * @param functionname Function name associated with the error.
 * @param message Stream expression containing the exception message.
 */
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

/*!
 * @def dt__throwUnexpected
 * @brief Throw a general dtOO exception for an unexpected condition.
 *
 * @param functionname Function name associated with the error.
 */
#define dt__throwUnexpected(functionname)                                      \
  dt__throw(functionname, << "Unexpected")

/*!
 * @def dt__throwIfWithMessage
 * @brief Throw a general dtOO exception if a condition is true.
 *
 * The condition and an additional message are included in the exception.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the error.
 * @param message Additional stream expression containing the error message.
 */
#define dt__throwIfWithMessage(cond, functionname, message)                    \
  if (cond)                                                                    \
  {                                                                            \
    dt__throw(                                                                 \
      functionname, << "condition: " #cond " is true." << std::endl message    \
    );                                                                         \
  }

/*!
 * @def dt__throwIf
 * @brief Throw a general dtOO exception if a condition is true.
 *
 * The condition itself is included in the generated exception message.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the error.
 */
#define dt__throwIf(cond, functionname)                                        \
  if (cond)                                                                    \
  {                                                                            \
    dt__throw(functionname, << "condition: " #cond " is true.");               \
  }

/*!
 * @def dt__throwIfNoClass
 * @brief Throw a general dtOO exception if a condition is true without class
 * context.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the error.
 */
#define dt__throwIfNoClass(cond, functionname)                                 \
  if (cond)                                                                    \
  {                                                                            \
    dt__throwNoClass(functionname, << "condition: " #cond " is true.");        \
  }

/*!
 * @def dt__warnIfWithMessage
 * @brief Report a warning if a condition is true.
 *
 * The condition and an additional message are included in the warning.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the warning.
 * @param message Additional stream expression containing the warning message.
 */
#define dt__warnIfWithMessage(cond, functionname, message)                     \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(                                                               \
      functionname, << "condition: " #cond " is true." << std::endl message    \
    );                                                                         \
  }

/*!
 * @def dt__warnIfWithSolution
 * @brief Report a warning and execute a solution if a condition is true.
 *
 * @param cond Condition to test.
 * @param solution Statement executed when the condition is true.
 * @param functionname Function name associated with the warning.
 */
#define dt__warnIfWithSolution(cond, solution, functionname)                   \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(                                                               \
      functionname, << "condition: " #cond " is true." << std::endl            \
    );                                                                         \
    solution;                                                                  \
  }

/*!
 * @def dt__warnIfWithMessageAndSolution
 * @brief Report a warning and execute a solution if a condition is true.
 *
 * The condition and an additional message are reported before the solution
 * is executed.
 *
 * @param cond Condition to test.
 * @param solution Statement executed when the condition is true.
 * @param functionname Function name associated with the warning.
 * @param message Additional stream expression containing the warning message.
 */
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

/*!
 * @def dt__warnIf
 * @brief Report a warning if a condition is true.
 *
 * The condition itself is included in the generated warning message.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the warning.
 */
#define dt__warnIf(cond, functionname)                                         \
  if (cond)                                                                    \
  {                                                                            \
    dt__warning(functionname, << "condition: " #cond " is true.");             \
  }

/*!
 * @def dt__solution
 * @brief Execute a solution if a condition is true.
 *
 * @param cond Condition to test.
 * @param solution Statement executed when the condition is true.
 */
#define dt__solution(cond, solution)                                           \
  if (cond)                                                                    \
    solution;

/*!
 * @def dt__tryOcc
 * @brief Execute an OpenCASCADE command and convert exceptions to dtOO errors.
 *
 * The macro catches @c Standard_Failure exceptions raised by OpenCASCADE and
 * converts them into @c eGeneral exceptions. The OpenCASCADE error message,
 * source location, additional error information, and a stack trace are
 * included in the generated exception.
 *
 * @param cmd OpenCASCADE command to execute.
 * @param errorOut Additional error information.
 */
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

/*!
 * @def moab__throwIf
 * @brief Throw a dtOO exception if a MOAB error condition is detected.
 *
 * The last MOAB error message is retrieved and included in the exception.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the error.
 */
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

/*!
 * @def moab__throwIfWithMessage
 * @brief Throw a dtOO exception if a MOAB error condition is detected.
 *
 * The last MOAB error message and additional user-provided information are
 * included in the exception.
 *
 * @param cond Condition to test.
 * @param functionname Function name associated with the error.
 * @param message Additional stream expression containing error information.
 */
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
