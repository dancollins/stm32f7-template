#include "logging.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


static logging_init_args_t log;


static logging_level_t
get_level(const char *file, const char *function)
{
    logging_level_t file_level = LOG_LEVEL_INVALID;

    /* We're searching for a matching file/function pair, and we also note a
     * file-only level if we find one. */
    for (logging_config_t *ptr = log.config; ptr->file != NULL; ptr++)
    {
        /* There's no way to verify these strings are safe, so we'll have to
         * just assume they are. The compiler's job is to keep everything const,
         * and the logging is all set at compile-time. Don't make a bad config I
         * guess! */
        if (strstr(file, ptr->file) == NULL)
            continue;

        if (ptr->function == NULL)
            file_level = ptr->level;
        else if (strstr(function, ptr->function) != NULL)
            return ptr->level;
    }

    return file_level ? file_level : log.default_level;
}


static const char *
get_filename(const char *file)
{
    /* Our __FILE__ macro might contain an absolute path - and we don't want
     * that in our print messages. We'll instead take everything to the right of
     * the right-most 'src' substring. */
    for (const char *src = strrchr(file, '\0') - strlen("src");
         src >= file;
         src--)
    {
        if (strncmp(src, "src", 3) == 0)
            return src;
    }

    return "???";
}


static const char *
get_level_str(logging_level_t level)
{
    const char *log_str[] = {
        "ERR",
        "WRN",
        "INF",
        "DBG",
        "VER",
    };

    if (level < LOG_LEVEL_ERROR || level > LOG_LEVEL_VERBOSE)
        return "XXX";

    return log_str[level - LOG_LEVEL_ERROR];
}


void
logging_init(logging_init_args_t *init_args)
{
    memcpy(&log, init_args, sizeof(log));
}


void
logging_log(logging_level_t level,
            const char *file, const char *function, int line,
            const char *format, ...)
{
    static char buffer[256];
    logging_level_t config_level;
    va_list args;
    int len;

    /* Check that logging is enabled. */
    if (level <= LOG_LEVEL_NONE)
        return;

    config_level = get_level(file, function);
    if (level > config_level)
        return;

    /* Prepend our log format. */
    len = snprintf(buffer, sizeof(buffer), "%s %lu %s:%d ",
                   get_level_str(level),
                   1234ul, get_filename(file), line);

    va_start(args, format);
    vsnprintf(buffer + len, sizeof(buffer) - len, format, args);
    va_end(args);

    log.handler(buffer);
}
