#ifndef LOGGING_H__
#define LOGGING_H__


typedef enum logging_level_t
{
    LOG_LEVEL_INVALID,
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE,
} logging_level_t;


typedef struct logging_config_t
{
    const char *file;
    const char *function;
    const logging_level_t level;
} logging_config_t;


typedef struct logging_init_args_t
{
    logging_config_t *config;
    void (*handler)(const char *message);
    logging_level_t default_level;
} logging_init_args_t;


#define LOG_ERROR(...) do {                             \
        logging_log(LOG_LEVEL_ERROR,                    \
                    __FILE__, __FUNCTION__, __LINE__,   \
                    __VA_ARGS__);                       \
    } while(0)

#define LOG_WARNING(...) do {                           \
        logging_log(LOG_LEVEL_WARNING,                  \
                    __FILE__, __FUNCTION__, __LINE__,   \
                    __VA_ARGS__);                       \
    } while(0)

#define LOG_INFO(...) do {                              \
        logging_log(LOG_LEVEL_INFO,                     \
                    __FILE__, __FUNCTION__, __LINE__,   \
                    __VA_ARGS__);                       \
    } while(0)

#define LOG_DEBUG(...) do {                             \
        logging_log(LOG_LEVEL_DEBUG,                    \
                    __FILE__, __FUNCTION__, __LINE__,   \
                    __VA_ARGS__);                       \
    } while(0)

#define LOG_VERBOSE(...) do {                           \
        logging_log(LOG_LEVEL_VERBOSE,                  \
                    __FILE__, __FUNCTION__, __LINE__,   \
                    __VA_ARGS__);                       \
    } while(0)


extern void
logging_init(logging_init_args_t *init_args);


extern void
logging_log(logging_level_t level,
            const char *file, const char *function, int line,
            const char *format, ...);


#endif
