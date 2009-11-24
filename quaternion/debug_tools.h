#ifndef _DEBUG_TOOLS_H_
#define _DEBUG_TOOLS_H_

#include <iostream>

typedef unsigned long DebugLevel;
typedef std::string String;

extern const DebugLevel DEBUG_LEVEL;
extern const String DIAG_PROG_NAME;

extern const DebugLevel ERROR_DEBUG_LEVEL;
extern const DebugLevel WARNING_DEBUG_LEVEL;
extern const DebugLevel INFO_DEBUG_LEVEL;

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
        DiagStream(DebugLevel min_debug_lv);

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
DiagStream& operator<<(DiagStream& ds, const T& message)
{
    if(DEBUG_LEVEL >= ds.MIN_DEBUG_LEVEL)
        std::cerr << message;
    return ds;
}

DiagStream& operator<<
(DiagStream& ds, std::ostream&(*message_fun)(std::ostream&));

/** Returns stream for error logging. */
DiagStream& logErr();

/** Returns stream for warnings logging. */
DiagStream& logWarn();

/** Returns stream for information logging. */
DiagStream& logInfo();

#endif
