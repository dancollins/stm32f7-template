#ifndef CONFIG_H__
#define CONFIG_H__

#include "logging/logging.h"

/*
 * Global logging configuration.
 *
 * The config is searched for the most-specific entry that matches. E.g. a
 * file/function combo takes precedence over just a file log level. Entries with
 * no file are skipped - and the NULL entry (shown below) marks the end of the
 * configuration.
 *
 * The full filepath is optional, as we just check that the file entry matches
 * the right side of the string (i.e. it must end in '.c' to match
 * successfully)!
 */
static logging_config_t logging_config[] =
{
    {"src/main.c", LOG_LEVEL_INFO},

    /* We detect the end using a NULL entry. */
    {NULL, 0}
};

#endif
