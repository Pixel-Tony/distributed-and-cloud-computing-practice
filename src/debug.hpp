#ifndef _LOGGING_H
#define _LOGGING_H

#ifdef DEBUG_ENABLE
#include <iostream>

#define _WRAP(a) do { a } while (0)
#define _MSG(msg, stream, after) _WRAP(before; std::stream << msg << std::endl; after;)

#define LOG(msg) _MSG(msg, cout, {})
#define ERROR(msg) _MSG(msg, cerr, {})
#define FATAL(msg) _MSG(msg, cerr, exit(-1))
#define ASSERT_FATAL(expr, msg) _WRAP(if (!expr) {, FATAL(msg), })

#define CONSTEXPR_UNLESS_DEBUG
#else
#define LOG_ERROR(msg) do {} while (0)
#define ERROR_IF(expr, msg) do {} while (0)
#define CONSTEXPR_UNLESS_DEBUG constexpr
#endif

#endif