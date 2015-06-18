#ifndef __CUTILITY_LOG_H__
#define __CUTILITY_LOG_H__

#include "log.h"

#ifndef P_DEBUG
#define P_DEBUG DEBUG
#endif

#ifndef P_TRACE
#define P_TRACE TRACE
#endif

#ifndef P_NOTICE
#define P_NOTICE NOTICE
#endif

#ifndef P_WARNING
#define P_WARNING WARNING
#endif

#ifndef P_FATAL
#define P_FATAL FATAL
#endif

#ifndef TRY
#define TRY try
#endif

#ifndef CATCH_ALL
#define CATCH_ALL(logger, fmt, args...) \
    catch (std::exception &e)\
    {\
        logger("std::exception catched[%s], " fmt, e.what(), ##args);\
    }\
    catch (...)\
    {\
        logger("unknown exception catched, " fmt, ##args);\
    }
#endif

#ifndef FATAL_CATCH
#define FATAL_CATCH(fmt, args...) CATCH_ALL(P_FATAL, fmt, ##args)
#endif

#ifndef WARNING_CATCH
#define WARNING_CATCH(fmt, args...) CATCH_ALL(P_WARNING, fmt, ##args)
#endif

#ifndef NOTICE_CATCH
#define NOTICE_CATCH(fmt, args...) CATCH_ALL(P_NOTICE, fmt, ##args)
#endif

#ifndef TRACE_CATCH
#define TRACE_CATCH(fmt, args...) CATCH_ALL(P_TRACE, fmt, ##args)
#endif

#ifndef DEBUG_CATCH
#define DEBUG_CATCH(fmt, args...) CATCH_ALL(P_DEBUG, fmt, ##args)
#endif

#endif
