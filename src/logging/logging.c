#include "logging.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


static logging_init_args_t log;


static logging_level_t
get_level(const char *file)
{
    for (logging_config_t *ptr = log.config; ptr->file != NULL; ptr++)
    {
        /* There's no way to verify these strings are safe, so we'll have to
         * just assume they are. The compiler's job is to keep everything const,
         * and the logging is all set at compile-time. Don't make a bad config I
         * guess! */
        if (strstr(file, ptr->file) == NULL)
            return ptr->level;
    }

    return log.default_level;
}


static const char *
get_level_str(logging_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_ERROR:
        return "ERR";

    case LOG_LEVEL_WARNING:
        return "WRN";

    case LOG_LEVEL_INFO:
        return "INF";

    case LOG_LEVEL_DEBUG:
        return "DBG";

    case LOG_LEVEL_VERBOSE:
        return "VRB";

    default:
        return "XXX";
    }
}


void
logging_init(logging_init_args_t *init_args)
{
    memcpy(&log, init_args, sizeof(log));
}


void
logging_log(logging_level_t level,
            const char *file, int line,
            const char *format, ...)
{
    static char buffer[256];
    va_list args;
    int len;

    /* Check that logging is enabled. */
    if (level <= LOG_LEVEL_NONE)
        return;

    if (level > get_level(file))
        return;

    /* Prepend our log format. */
    len = snprintf(buffer, sizeof(buffer), "%s %lu %s:%d ",
                   get_level_str(level),
                   log.get_time(), file, line);

    va_start(args, format);
    vsnprintf(buffer + len, sizeof(buffer) - len, format, args);
    va_end(args);

    log.handler(buffer);
}
