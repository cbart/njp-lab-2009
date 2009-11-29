/** Debug tools.                                           *
 * author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl>  *
 *     id: cb277617@students.mimuw.edu.pl                  */

#ifndef _DEBUG_TOOLS_H_
#define _DEBUG_TOOLS_H_

#include <iostream>

typedef unsigned long DebugLevel;

#ifdef DEBUGLEVEL
    const DebugLevel DEBUG_LEVEL = DEBUGLEVEL;
#else
    const DebugLevel DEBUG_LEVEL = 0;
#endif

const std::string DIAG_PROG_NAME = "DEBUG";

#ifdef ERRORLEVEL
    const DebugLevel ERROR_DEBUG_LEVEL = ERRORLEVEL;
#else
    const DebugLevel ERROR_DEBUG_LEVEL = 2;
#endif

#ifdef WARNINGLEVEL
    const DebugLevel WARNING_DEBUG_LEVEL = WARNINGLEVEL;
#else
    const DebugLevel WARNING_DEBUG_LEVEL = 2;
#endif

#ifdef INFOLEVEL
    const DebugLevel INFO_DEBUG_LEVEL = INFOLEVEL;
#else
    const DebugLevel INFO_DEBUG_LEVEL = 2;
#endif

/** A tool for managing diagnostic info. */
class DiagStream
{

    private:

        /** Minimal level of debugging with which
        * this object will print the output. */
        const DebugLevel MIN_DEBUG_LEVEL;

    public:

        /** Creates new diagnostic stream with defined
         * `min_debug_lv` (minimal debug level). */
        DiagStream(DebugLevel min_debug_lv)
            : MIN_DEBUG_LEVEL(min_debug_lv)
        {
        }

        /** 'Send to stream' operator designed for all kinds
         * of messages. */
        template<typename T>
            friend DiagStream& operator<<(DiagStream& ds, const T& message);

        /** 'Send to stream' operator designed for operations
         * like std::flush, std::endl, etc... */
        friend DiagStream& operator<<
            (DiagStream& ds, std::ostream& (*message_fun)(std::ostream&));

};

template<typename T>
inline static DiagStream& operator<<(DiagStream& ds, const T& message)
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message;
    return ds;
}

inline DiagStream& operator<<
(DiagStream& ds, std::ostream& (*message_fun)(std::ostream&))
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message_fun;
    return ds;
}

/** Returns stream for error logging. */
inline DiagStream& logErr()
{
    static DiagStream error_stream = DiagStream(ERROR_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= ERROR_DEBUG_LEVEL)
        std::cerr << "(EE) " << DIAG_PROG_NAME << " ";
    return error_stream;
}

/** Returns stream for warnings logging. */
inline DiagStream& logWarn()
{
    static DiagStream warning_stream = DiagStream(WARNING_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= WARNING_DEBUG_LEVEL)
        std::cerr << "(WW) " << DIAG_PROG_NAME << " ";
    return warning_stream;
}

/** Returns stream for information logging. */
inline DiagStream& logInfo()
{
    static DiagStream info_stream = DiagStream(INFO_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= INFO_DEBUG_LEVEL)
        std::cerr << "(II) " << DIAG_PROG_NAME << " ";
    return info_stream;
}

#endif
