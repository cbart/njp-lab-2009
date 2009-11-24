#include <iostream>

#include "./debug_tools.h"

DiagStream::DiagStream(DebugLevel min_debug_lv)
    : MIN_DEBUG_LEVEL(min_debug_lv)
{
}

DiagStream& operator<<
(DiagStream& ds, std::ostream& (*message_fun)(std::ostream&))
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message_fun;
    return ds;
}

DiagStream& logErr()
{
    static DiagStream error_stream = DiagStream(ERROR_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= ERROR_DEBUG_LEVEL)
        std::cerr << "(EE) " << DIAG_PROG_NAME << " ";
    return error_stream;
}

DiagStream& logWarn()
{
    static DiagStream warning_stream = DiagStream(WARNING_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= WARNING_DEBUG_LEVEL)
        std::cerr << "(WW) " << DIAG_PROG_NAME << " ";
    return warning_stream;
}

DiagStream& logInfo()
{
    static DiagStream info_stream = DiagStream(INFO_DEBUG_LEVEL);
    if(DEBUG_LEVEL >= INFO_DEBUG_LEVEL)
        std::cerr << "(II) " << DIAG_PROG_NAME << " ";
    return info_stream;
}
